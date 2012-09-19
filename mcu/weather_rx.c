/* Asynchronous receiver code for 433Mhz pulses from no-brand weather modules.
 *
 * Receiver sends high-side pulses with data (mirrors transmitter module which sees data on low side of pulse)
 *
 * Data encoding is a 1-3ms pulse for 0, 3-6ms low pulse for 1.
 * Approx 9ms low gap between messages
 * High edges betweeb pulses are no more than 15ms
 *
 * Data is received as a preamble message (ignored), then 3 5 byte messages.
 * Gaps between each message.
 *
 *
 * Copyright (c) 2012, Angus Gratton. Licensed under the Modified BSD License.
 */
#include "weather_rx.h"
#include "config.h"

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "macro_helpers.h"


#ifndef RX_PIN
#error  You should define a pin named RX_PIN in config.h, with form D,2
#endif
#ifndef RX_INT
#error  You should define an PCINT0-7 register named RX_INT in config.h, corresponding to RX_PIN. It should be in bank 0
#endif

// constants
#define MSG_LEN 36 // 36 bit raw messages, but first 4 bytes are always 5 (1010) so we discard
#define MSG_REPEAT 5
#define MINIMUM_PULSE 1
#define ZERO_THRES 2
#define MSG_GAP 6
#define NUM_CHANNELS 3

// internal state tracking
static volatile weather_data last_data[NUM_CHANNELS];
static volatile bool last_data_is_fresh[NUM_CHANNELS];
static volatile enum { prelude, reading, repeating } state;
static volatile uint32_t raw_msg;
static volatile uint8_t bits_received;
static volatile uint8_t confirmed_repeats;

// timer convenience functions

inline static void timer_off() {
  TCCR0B = 0;
}

inline static void timer_start() { // start or restart the timer
  TCNT0 = 0;
  TCCR0B = _BV(CS00)|_BV(CS02); // prescale F_CPU/1024
}

inline static uint16_t get_timer_ms() { // ms elapsed since timer turned on
  return ((uint32_t)TCNT0*1000)/(F_CPU/1024);
}


// API


void init_weather_rx() {
  // enable pin change interrupt bank 0
  PCMSK0 |= _BV(RX_INT);
  PCICR |= _BV(PCIE0);
  // set up timer
  TIMSK0 |= _BV(TOIE0); // overflow interrupt enable
  timer_off();
}

bool get_latest_weather(uint8_t channel, weather_data *data) {
  if(channel >= NUM_CHANNELS)
    return false; // channel range 0-2 only
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    memcpy(data, (void*)&last_data[channel], sizeof(weather_data));
  }
  bool fresh = last_data_is_fresh[channel];
  last_data_is_fresh[channel] = false;
  return fresh;
}


// interrupt handlers & main implementation

ISR(TIMER0_OVF_vect) { // timer overflow means something has gone wrong, abort!
  bits_received = 0;
  raw_msg = 0L;
  state = prelude;
  timer_off();
}

// forward declarations for message reader interrupt methods
static void rising_edge();
static void falling_edge();
static void received_data_pulse(bool one);
static void received_message_end();
static void full_data_received();

ISR(PCINT0_vect) { // pin change vector means we have a pin edge
  if(READ(RX_PIN))
    rising_edge();
  else
    falling_edge();
}

static void rising_edge() {
  timer_start();
}

static void falling_edge() {
  uint16_t pulse_width = get_timer_ms();
  timer_start();
  if(pulse_width < MINIMUM_PULSE)
    return; // noise/prelude pulse (maybe don't need to track these?
  else if(pulse_width >= MSG_GAP) {
    received_message_end();
  }
  else {
    received_data_pulse(pulse_width > ZERO_THRES);
  }
}


static void received_data_pulse(bool one) {
  switch(state) {
  case prelude:
    break; // there is a pattern in the prelude state, but it's not consistent
  case reading:
    if(bits_received < MSG_LEN) {
      raw_msg = (raw_msg << 1) | (one?1:0);
      bits_received++;
    }
    break;
  case repeating:
    if(bits_received == MSG_LEN)
      break;
    if(bits_received++ < 4) // first 4 bytes are always 1010 and are discarded so packet fits in a 32-bit int
      break;
    bool last_bit = ( raw_msg & ( 1L << (MSG_LEN-bits_received) ) ) ? true : false;
    if(last_bit != one) { // repeat read doesn't match, reset!
      state = reading;
      bits_received = 0;
      raw_msg = 0L;
      return;
    }
  }
}

static void received_message_end() {
  switch(state) {
  case prelude:
    raw_msg = 0L;
    state = reading;
    break;
  case reading:
    if(bits_received == MSG_LEN) { // got a full message, wait for repeats to confirm
      state = repeating;
      confirmed_repeats = 0;
    }
    break;
  case repeating:
    if(bits_received == MSG_LEN) { // passed a full data packet that matched the original
      confirmed_repeats++;
      if(confirmed_repeats == MSG_REPEAT) { // got enough matching repeats
        full_data_received();
        state = prelude;
      }
    } else { // got a short/invalid repeated packet, go back to reading
      state = reading;
    }
    break;
  }
  bits_received = 0;
}

// constants for slicing up the 32-bit weather data packet

// bottom 8 bits are humidity
#define HUMIDITY_SHIFT 0
// second 8 bits are temp, as deci-degrees celsisu
#define TEMP_SHIFT 8
// bit is set if button is pressed forcing data to transmit, rather than timed transmit
#define BUTTON_MASK  0x00400000L
// channel 0-2 marked 1-3 on transmitter units
#define CHANNEL_MASK 0x00300000L
#define CHANNEL_SHIFT 20
// this bit is always set on all my transmitters
// (could indicate celsius, data is always sent in C regardless. Doesn't seem to be battery good status. unused feature?)
#define MYSTERY_BIT  0x00800000L
// OTOH this bytes seems to change to a random value any time a transmitter resets, doesn't change after that...
// Doesn't seem to be battery related.
#define MYSTERY_MASK 0xFF000000L

static void full_data_received()
{
  /* // uncomment this if you'd like to see some debug output
  char buf[60];
  snprintf(buf, 60, "%08lx ch=%d b=%d h=%u t=%u rest=%08lx\r\n",
           raw_msg,
           (bool)((raw_msg&CHANNEL_MASK)>>CHANNEL_SHIFT),
           (bool)(raw_msg&BUTTON_MASK),
           (uint8_t)(raw_msg>>HUMIDITY_SHIFT),
           (uint8_t)(raw_msg>>TEMP_SHIFT),
           raw_msg&0xFFFF0000L&~(BUTTON_MASK|CHANNEL_MASK)
           );
  uart_putstr(buf);
  */
  uint8_t channel = (raw_msg&CHANNEL_MASK)>>CHANNEL_SHIFT;
  if(channel == 3)
    return;
  volatile weather_data *d = &last_data[channel];
  d->button = raw_msg&BUTTON_MASK;
  d->channel = channel;
  uint8_t temp = raw_msg>>TEMP_SHIFT;
  d->temp_integral = temp/10;
  d->temp_decimal = temp%10;
  d->humidity = raw_msg>>HUMIDITY_SHIFT;
  last_data_is_fresh[channel] = true;
  timer_off();
}

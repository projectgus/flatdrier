/*
 * Asynchronous receiver code for 433Mhz pulses from no-brand weather modules.
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
#define MSG_LEN 5
#define MSG_REPEAT 3
#define MINIMUM_PULSE 1
#define ZERO_THRES 2
#define MSG_GAP 6
#define NUM_CHANNELS 3

// internal state tracking
static volatile weather_data last_data[NUM_CHANNELS];
static volatile bool last_data_is_fresh[NUM_CHANNELS];
static volatile enum { prelude, reading, repeating } state;
static volatile uint8_t msg_buf[MSG_LEN*MSG_REPEAT];
static volatile uint8_t bytes_received;
static volatile uint8_t bits_received;
static volatile uint8_t confirmed_repeats;

void uart_putchar(char c);

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
  uart_putchar('i');
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
  bytes_received = 0;
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
    if(one && bytes_received < MSG_LEN)
      msg_buf[bytes_received] |= _BV(7-bits_received);
    break;
  case repeating:
    if(bytes_received == MSG_LEN)
      break;
    bool last_bit = msg_buf[bytes_received] & _BV(7-bits_received);
    if(last_bit != one) { // repeat read doesn't match, reset!
      uart_putchar('E');
      uart_putchar('0'+confirmed_repeats);
      state = reading;
      bytes_received = 0;
      bits_received = 0;
      return;
    }
  }

  // increment bit counter
  if(bytes_received < MSG_LEN && (state == reading || state == repeating)) {
    bits_received++;
    if(bits_received == 8) {
      bytes_received++;
      bits_received = 0;
    }
  }
}

static void received_message_end() {
  switch(state) {
  case prelude:
    memset(&msg_buf,0,sizeof(msg_buf));
    state = reading;
    break;
  case reading:
    if(bytes_received >= MSG_LEN-1) { // got a full message, wait for repeats to confirm
      // TODO: why is this MSG_LEN-1 not MSG_LEN?
      state = repeating;
      confirmed_repeats = 0;
    }
    break;
  case repeating:
    if(bytes_received >= MSG_LEN-1) { // got a full data packet same TODO
      confirmed_repeats++;
      if(confirmed_repeats == MSG_REPEAT) { // got all matching repeats
        full_data_received();
        state = prelude;
      }
    } else { // got a short/invalid repeated packet, go back to reading
      state = reading;
    }
    break;
  }
  bytes_received = 0;
  bits_received = 0;
}

static void full_data_received()
{
  // copying this data out is pretty hacky, but it's easier than writing
  // a struct to represent such oddly laid out data
  uint8_t channel = msg_buf[1]&3;
  if(channel == 3)
    return;
  volatile weather_data *d = &last_data[channel];
  d->celsius = msg_buf[1]&(1<<3);
  d->button = msg_buf[1]&(1<<2);
  d->channel = channel;
  d->temp = (msg_buf[2]<<4)|((msg_buf[3]&0xF0)>>4);
  d->humidity = ((msg_buf[3]&0x0F)<<4)|((msg_buf[4]&0xF0)>>4);
  last_data_is_fresh[channel] = true;
  timer_off();
}

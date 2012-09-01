/*
 * Asynchronous receiver code for 433Mhz pulses from no-brand weather modules
 *
 * Data encoding is a 1-3ms low pulse for 0, 3-6ms low pulse for 1.
 * Approx 9ms low gap between messages
 * High edges betweeb pulses are no more than 15ms
 *
 * Data is received as a preamble message (ignored), then 3 5 byte messages.
 * Gaps between each message.
 *
 *
 */
#include "weather_rx.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "macro_helpers.h"


#ifndef RX_PIN
#error  You should define a pin named RX_PIN in config.h, with form D,2
#endif
#ifndef RX_INT
#error  You should define an PCINT0-7 register named RX_INT in config.h, ie PCINT0
#endif

#define MSG_LEN 5
#define MSG_REPEAT 3

// internal state tracking
enum { idle, prelude, reading, repeating } state;
static volatile int buf[MSG_LEN*MSG_REPEAT];
static volatile int bytes_received;

// timer convenience functions

inline static void timer_off() {
  TCCR0B = 0;
}

inline static void timer_start() { // start or restart the timer
  TCNT0 = 0;
  TCCR0B = _BV(CS00)|_BV(CS02); // prescale F_CPU/1024
}

inline static uint16_t get_timer_ms() { // ms elapsed since timer turned on
  return ((uint16_t)TCNT0*1000)/(F_CPU/1024);
}

void init_weather_rx() {
  // enable pin change interrupt bank 0
  PCMSK0 |= _BV(RX_INT);
  PCICR |= _BV(PCIE0);
  // set up timer
  TIMSK0 |= _BV(TOIE0); // overflow interrupt enable
  timer_off();
}

bool get_latest_weather(uint8_t channel, weather_data *data) {
  return false;
}

ISR(TIMER0_OVF_vect) { // timer overflow means something has gone wrong, abort!
  bytes_received = 0;
  state = idle;
  timer_off();
}

static void rising_edge() {

}

static void falling_edge() {

}

#define INT_VECT RX_INT##_vect
ISR(INT_VECT) { // pin change vector means we have a pin edge
  if(READ(RX_PIN))
    rising_edge();
  else
    falling_edge();
}

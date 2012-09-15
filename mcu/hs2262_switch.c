/*
 * Simple routines for transmitting on/off signals to remote control powerpoint switches
 * via 433Mhz transmitter module.
 *
 *
 * Copyright (c) 2012, Angus Gratton. Licensed under the Modified BSD License.
 */

#include "hs2262_switch.h"
#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "macro_helpers.h"

#ifndef HS2262_TX_PIN
#error  You should define a pin named HS2262_TX_PIN in config.h, with form D,2
#endif

void init_hs2262_switch() {
  OUTPUT(HS2262_TX_PIN);
  LOW(HS2262_TX_PIN);
}

const uint8_t BIT_ONOFF = 8;
const uint8_t CHANNEL_SHIFT = 1;
const uint8_t CHANNEL_MASK = 7;
const uint8_t OUTPUT_LENGTH = 12; // check this
const uint8_t TX_REPEATS = 16;
#define PERIOD_SHORT_US 230
#define PERIOD_LONG_US (PERIOD_SHORT_US*3)

void send_hs2262_switch(uint8_t channel, bool on) {
  channel--;
  uint16_t output = ((~channel & CHANNEL_MASK)<<CHANNEL_SHIFT) | (on ? _BV(BIT_ONOFF) : 0);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { // disable interrupts for timing, and because RX interrupts will get v. confused!
    for(uint8_t rep = 0; rep < TX_REPEATS; rep++) {
      for(uint8_t bit = 0; bit < OUTPUT_LENGTH; bit++) {
        const bool one = output & _BV(bit);
        for(uint8_t dup = 0; dup < 2; dup++) { // each bit pattern is the same thing twice
          HIGH(HS2262_TX_PIN);
          if(one) _delay_us(PERIOD_LONG_US); else _delay_us(PERIOD_SHORT_US);
          LOW(HS2262_TX_PIN);
          if(one) _delay_us(PERIOD_SHORT_US); else _delay_us(PERIOD_LONG_US);
        }
      }
      // message termination signal
      HIGH(HS2262_TX_PIN);
      _delay_us(PERIOD_SHORT_US);
      LOW(HS2262_TX_PIN);
      _delay_us(PERIOD_SHORT_US*31);
    }
  }
}



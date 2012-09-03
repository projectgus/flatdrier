#ifndef _HS2262_SWITCH_H
#define _HS2262_SWITCH_H
/*
 * Simple routines for transmitting on/off signals to remote control powerpoint switches
 * via 433Mhz transmitter module.
 *
 * Specifically I use this with "Powertran A0342" remote control power
 * points and a Jaycar ZW3100 transmitter, but will probably work with
 * other switches that use HS2262 transmitter & PT2272 receiver ICs.
 *
 * The remote control power point I have came with a grey remote with
 * 4 on buttons, 4 off buttons, and a switch to set the channel range
 * 1-4 or 5-8.
 *
 * Bit pattern was determined by reading the "BlokkerSwitch"
 * implementation of the Arduino RemoteSwitch library, which is a good
 * choice if you're using the Arduino environment:
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1216065789
 * Big thanks to Randy Simons for that library.
 */
#include <stdbool.h>
#include <stdint.h>

void init_hs2262_switch();
void send_hs2262_switch(uint8_t channel, bool on);

#endif

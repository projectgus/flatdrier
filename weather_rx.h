#ifndef _WEATHER_RX_H
#define _WEATHER_RX_H
/*
 * Asynchronous receiver code for 433Mhz pulses from no-brand weather modules
 *
 * My transmitter PCBs have "ver2" in top-right, use 433Mhz module
 * "SL-TX583 WWW.HH.COM"
 *
 * Assumes macros RX_PIN, RX_INT (for pin interrupt) set in config.h
 *
 * Uses Timer 0
 *
 */
#include <stdint.h>
#include <stdbool.h>

#include "config.h"


/* Sets up timer 0 & pin edge interrupt for receiving weather data
 *
 */
void init_weather_rx();

struct weather_data {
  uint8_t humidity_integral;
  uint8_t humidity_decimal;
  uint8_t temp_integral;
  uint8_t temp_decimal;
  uint8_t channel;
  bool button;
  bool celsius;
}; // happens to be the same as dht11_data, just with a channel number
typedef struct weather_data weather_data;

/*
 * Read the latest weather data, returns true if data has changed since
 * last time weather data was read
 */
bool get_latest_weather(uint8_t channel, weather_data *data);

#endif

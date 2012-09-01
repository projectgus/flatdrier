#ifndef _DHT11_H
#define _DHT11_H
/* Synchronous DHT-11 reader
 *
 */
#include "config.h"
#include "macro_helpers.h"

struct dht11_data {
  uint8_t humidity_integral;
  uint8_t humidity_decimal;
  uint8_t temp_integral;
  uint8_t temp_decimal;
};
typedef struct dht11_data dht11_data;

enum dht11_status {
  success = 0,
  bad_ack = -1,
  bad_post_ack = -2,
  bad_bit_prelude = -3,
  bad_bit_data_pause = -4,
  bad_checksum = -5
};
typedef enum dht11_status dht11_status;

void init_dht11();
dht11_status read_dht11(dht11_data *data);

#endif

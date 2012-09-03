/*
 *
 */
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "dht11.h"
#include "weather_rx.h"
#include "hs2262_switch.h"

static void init_uart(void) {
#include <util/setbaud.h> // BAUD is defined in config.h
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  #if USE_2X
  UCSR0A |= _BV(U2X0);
  #else
  UCSR0A &= ~_BV(U2X0);
  #endif
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

char uart_getchar() {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}

void uart_putstr(char *c) {
  for(; *c; c++)
    uart_putchar(*c);
}

// Send the latest sample to the client
void uart_send_sample() {
  const int maxlen = 40;
  char buf[maxlen];

  // local data
  dht11_data data;
  dht11_status status = read_dht11(&data);

  if(status != success) {
    snprintf(buf, maxlen, "FAIL %d\r\n", status);
  }
  else {
    snprintf(buf, maxlen, "%d.%d,%d.%d\r\n",
             data.humidity_integral, data.humidity_decimal,
             data.temp_integral, data.temp_decimal);
  }
  uart_putstr(buf);

  for(int i = 0; i <3; i++) {
    weather_data data;
    bool stat = get_latest_weather(i, &data);
    snprintf(buf, maxlen, "%d/%d - %d,%d,%d,%d\r\n",
             i, stat, data.temp, data.humidity,
             data.button, data.celsius);
    uart_putstr(buf);
  }
}

int main() {
  init_uart();
  init_dht11();
  init_weather_rx();
  init_hs2262_switch();
  sei();

  while(1) {
    switch(uart_getchar()) {
    case 'r': // read latest sample
    case 'R':
      uart_send_sample();
      break;
    case 'o': // power point on
    case 'O':
      send_hs2262_switch(HS2262_CHANNEL, true);
      break;
    case 'f': // power point off
    case 'F':
      send_hs2262_switch(HS2262_CHANNEL, false);
      break;
    default: // ignored
      break;
    }
  }

  return 0;
}


/*
 *
 */
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "dht11.h"

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

// Send the latest sample to the client
void uart_send_sample() {
  const int maxlen = 40;
  char buf[maxlen];
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
  for(char *c = buf; *c; c++)
    uart_putchar(*c);
}

int main() {
  init_uart();
  init_dht11();

  while(1) {
    switch(uart_getchar()) {
    case 'r': // read latest sample
    case 'R':
      uart_send_sample();
    default: // ignored
      break;
    }
  }

  return 0;
}


#ifndef _CONFIG_H
#define _CONFIG_H

#define F_CPU 16000000UL

#define BAUD 9600

// pin & interrupt definitions
#define  RX_PIN B,2 // arduino pin 10
#define  RX_INT PCINT2

#define DHT_PIN B,3 // arduino pin 11

#define  HS2262_TX_PIN B,4 // arduino pin 12
#define HS2262_CHANNEL 1

#endif

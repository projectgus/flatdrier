MCU ?= atmega168
TARGET = flatdrier
SRCS = main.c dht11.c weather_rx.c hs2262_switch.c

all: $(TARGET).hex

CFLAGS = -std=c99 -mmcu=$(MCU) -Wall -Wextra -Werror -Os
DEPFLAGS = -MMD -MP -MF$(@:%.o=%.d)

OBJS =$(SRCS:%.c=%.o)

%.o: %.c
	avr-gcc $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(TARGET).elf: $(OBJS)
	avr-gcc -mmcu=$(MCU) -o $@ $(OBJS)

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -R .eeprom -O ihex $<  $@

upload: $(TARGET).hex
	avrdude -c arduino -P /dev/ttyUSB0 -p $(MCU) -b 19200 -U flash:w:$<

clean:
	rm $(TARGET).elf $(TARGET).hex *.o

-include $(SRCS:%.c=%.d)

.PHONY: all clean

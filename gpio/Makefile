CC ?= $(CROSS_COMPIPLE)gcc
CFLAGS ?= -g -Wall -Werror
TARGET ?= gpio
all: gpio

# BUILDING THE LCD_DISPLAY FILE
lcd_display: gpio.c gpio.h
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

# CLEAN TARGET
clean:
	-rm gpio

.PHONY: clean

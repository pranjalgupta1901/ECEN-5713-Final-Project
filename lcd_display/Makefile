CC ?= $(CROSS_COMPIPLE)gcc
CFLAGS ?= -g -Wall -Werror
TARGET ?= lcd_display
all: lcd_display

# BUILDING THE LCD_DISPLAY FILE
lcd_display: lcd_display.c lcd_display.h
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

# CLEAN TARGET
clean:
	-rm lcd_display

.PHONY: clean

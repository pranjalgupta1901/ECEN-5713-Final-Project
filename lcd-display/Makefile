CC ?= $(CROSS_COMPIPLE)gcc
CFLAGS ?= -g -Wall -Werror
TARGET ?= lcd-display
all: lcd-display

# BUILDING THE LCD_DISPLAY FILE
lcd_display: lcd_display.c
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

# CLEAN TARGET
clean:
	-rm lcd_display

.PHONY: clean
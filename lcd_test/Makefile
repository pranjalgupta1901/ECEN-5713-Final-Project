CC ?= $(CROSS_COMPIPLE)gcc
CFLAGS ?= -g -Wall -Werror
TARGET ?= lcd_test
all: lcd_test

# BUILDING THE LCD_TEST FILE
lcd_test: lcd_test.c 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

# CLEAN TARGET
clean:
	-rm lcd_test

.PHONY: clean    


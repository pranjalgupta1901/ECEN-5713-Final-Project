# Cross-compiler (if any)
CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)gcc

# Compiler flags
CFLAGS ?= -g -Wall -Werror

# Target executable
TARGET ?= aesdsocket

# Directories
SRC_DIR := .
LCD_DISPLAY_DIR := ../lcd_display
GPIO_DIR := ../gpio

# Source files
SRC := $(wildcard $(SRC_DIR)/*.c)
SRC += $(wildcard $(LCD_DISPLAY_DIR)/*.c)
SRC += $(wildcard $(GPIO_DIR)/*.c)
# Include directories
INCLUDES := -I$(LCD_DISPLAY_DIR) -I$(GPIO_DIR)

# Linker flags
LDFLAGS :=

# Main target
all: $(TARGET)

# Build the lcd_test executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)

.PHONY: 
	clean


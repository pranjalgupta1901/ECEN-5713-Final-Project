#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "lcd_display.h"

#define LCD_ENABLE_BIT 0x04
#define LCD_SLAVE_ADDR 0x27
#define LCD_CLEAR 0x01
#define LCD_HOME 0X02
#define LCD_ENTRY_MODE 0X06
#define LCD_DISPLAY_CONTROL 0x0C
#define LCD_FUNCTION_SET_8BIT 0x38
#define LCD_FUNCTION_SET_4BIT 0x28
#define LCD_BACKLIGHT 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_DISPLAYCONTROL 0x08
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYON 0x04
#define LCD_CMD 0
#define LCD_DATA 1

int file = -1; // Initialize file descriptor with an invalid value
int row = 0;
int column = 0;

static void send_byte(uint8_t val, uint8_t mode);
static void lcd_toggle_enable(uint8_t val);

static void lcd_toggle_enable(uint8_t val) {
    uint8_t val_high = val | LCD_ENABLE_BIT;
    uint8_t val_low = val & ~LCD_ENABLE_BIT;
    usleep(1000); // Wait for enable pulse width
    write(file, &val_high, 1);
    usleep(1000); // Wait for enable pulse width
    write(file, &val_low, 1); // Clear enable bit
    usleep(666); // Wait for enable pulse width
}

void open_i2c_file() {
    if (file == -1) {
        char *i2c_dev_filename = "/dev/i2c-1";
        file = open(i2c_dev_filename, O_RDWR);
        if (file < 0) {
            perror("Failed to open the i2c-1 bus");
            exit(-1);
        }
        if (ioctl(file, I2C_SLAVE, LCD_SLAVE_ADDR) < 0) {
            perror("Failed to set the address of the device to address");
            exit(-1);
        }
    }
}

static void send_byte(uint8_t val, uint8_t mode) {
    uint8_t buf = mode | (val & 0xF0) | LCD_BACKLIGHT; 
    write(file, &buf, 1); 
    lcd_toggle_enable(buf); 
    buf = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT; 
    write(file, &buf, 1); 
    lcd_toggle_enable(buf);
}

void close_i2c_file() {
    if (file != -1) {
        close(file);
        file = -1; // Reset file descriptor after closing
    }
}

void lcd_clear(void) {
    send_byte(LCD_CLEAR, LCD_CMD); // Clear display
    send_byte(LCD_HOME, LCD_CMD); // Return Home

    usleep(2000); // Wait for LCD to complete clear operation
    row = 0;
    column = 0;
}

void lcd_on(void) {
    send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_CMD);
}

void lcd_init(void) {
    open_i2c_file();

    // Sending initialization commands
    send_byte(0x03, LCD_CMD);
    send_byte(0x03, LCD_CMD);
    send_byte(0x03, LCD_CMD);
    send_byte(0x02, LCD_CMD);

    // Initialize display configuration
    send_byte(LCD_ENTRY_MODE, LCD_CMD);
    send_byte(LCD_FUNCTION_SET_4BIT, LCD_CMD);
    lcd_on();
    lcd_clear();
}

void lcdputch(unsigned char c) {
    // Check if current column exceeds the display width
    if (column == 20) {
        // If yes, move to the next row
        if (row < 3) {
            lcdgotoxy(row + 1, 0);
        } else {
            // If at the last row, move back to the first row
            lcdgotoxy(0, 0);
        }
    }
    // Send character data to display
    send_byte(c, LCD_DATA);
    column++;
}

void lcdputs(char *str) {
    // Iterate through each character in the string
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        // Send each character to the display
        lcdputch(str[i]);
    }
}

void lcdgotoxy(uint8_t set_row, uint8_t set_column) {
    // Set the current row and column positions
    row = set_row;
    column = set_column;

    // Calculate the DDRAM address based on row and column
    uint8_t row_offsets[] = { 0x80, 0xC0, 0x94, 0xD4 };
    send_byte(row_offsets[row] + column, LCD_CMD);
}

void lcd_move_and_write(int x, int y, char *data) {
    // Move cursor to the specified position
    lcdgotoxy(x, y); 
    // Write data at the cursor position
    lcdputs(data); 
}

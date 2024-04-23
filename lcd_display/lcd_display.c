#include "lcd_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "lcd_display.h"

#define LCD_SLAVE_ADDR 0x27

int row = 0;
int column = 0;
int file;

void lcd_init(void) {
    open_i2c_file();
	send_data_or_command(0x03);
	send_data_or_command(0x03);
	send_data_or_command(0x03);
	send_data_or_command(0x02);
    	send_data_or_command(0x06); // Cursor move direction increment with display shift off
    	send_data_or_command(0x28); 
    	send_data_or_command(0x0C); 
    	send_data_or_command(0x01); 
    	send_data_or_command(0x02); // Returning to home
}

void send_data_or_command(unsigned char ch) {
    open_i2c_file();
    write(file, &ch, 1);
    usleep(100); // Delay after sending data or command
    close_i2c_file();
}

void open_i2c_file() {
    char *i2c_dev_filename = "/dev/i2c-1";
    file = open(i2c_dev_filename, O_RDWR);
    if (file < 0) {
        perror("Failed to open the i2c-1 bus");
        exit(1);
    }
    if (ioctl(file, I2C_SLAVE, LCD_SLAVE_ADDR) < 0) {
        perror("Failed to set the address of the device to address");
        exit(1);
    }
}

void close_i2c_file() {
    close(file);
}

void lcd_clear(void) {
    send_data_or_command(0x01); // Clearing command
}

void lcdputs(const char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        lcdputch(str[i]);
    }
}

void lcdputch(unsigned char c) {
    if (column == 20) {
        if (row == 0 || row == 1) {
            lcdgotoxy(row + 1, 0);
        } else if (row == 2) {
            lcdgotoxy(row + 1, 0);
        } else if (row == 3) {
            lcdgotoxy(0, 0);
        }
        column = 0;
    }
    send_data_or_command(c);
    column++;
}

void lcdgotoaddr(uint8_t address) {
    send_data_or_command(0x80 | (address));
}

void lcdgotoxy(uint8_t set_row, uint8_t set_column) {
    row = set_row;
    column = set_column;
    if (set_row == 0)
        lcdgotoaddr(0 + set_column);
    else if (set_row == 1)
        lcdgotoaddr(0x40 + set_column);
    else if (set_row == 2)
        lcdgotoaddr(0x14 + set_column);
    else if (set_row == 3)
        lcdgotoaddr(0x54 + set_column);
}



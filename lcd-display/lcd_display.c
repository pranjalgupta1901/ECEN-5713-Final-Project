#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define LCD_SLAVE_ADDR 0x11

int file;
int row = 0;
int column = 0;

void lcd_init(void);
void send_data_or_command(unsigned char ch);
void delay(void);
void lcdputch(unsigned char c);
void lcd_clear(void);
void lcdgotoaddr(uint8_t address);
void lcdgotoxy(uint8_t set_row, uint8_t set_column);
void lcdputs(const char *str);

int main() {
    // Create I2C bus
    // Always adapter 1 on RPi
    char *i2c_dev_filename = "/dev/i2c-1";
    file = open(i2c_dev_filename, O_RDWR);
    if(file < 0) {
        perror("Failed to open the i2c-1 bus");
        exit(1);
    }

    
    int addr = 0x27;

    // Set the address of the device to address
    if(ioctl(file, I2C_SLAVE, LCD_SLAVE_ADDR) < 0) {
        perror("Failed to set the address of the device to address");
        exit(1);
    }

    lcd_init();   
    lcdputs("Hello, LCD!");
    return 0;
}

void lcd_init(void) {
    send_data_or_command(0x01); // clearing display
    send_data_or_command(0x38); // setting 8 bit operation and also selecting two line display and 5x8 dot character font
    send_data_or_command(0x0F);   // turning on display and cursor with cursor blinking on
    send_data_or_command(0x06);  // Cursor move direction increment with display shift off
    send_data_or_command(0x14);  // Cursor moving and shifting to the right
    send_data_or_command(0x02); // returning to home
    send_data_or_command(0x80);
}

void send_data_or_command(unsigned char ch) {
    write(file, &ch, 1);
    delay();
}

void delay(void) {
    usleep(3000);
}

void lcdputch(unsigned char c) {
    if(column == 20) {
        if(row == 0 || row == 1) {
            lcdgotoxy(row + 1, 0);
        } else if(row == 2) {
            lcdgotoxy(row + 1, 0);
        } else if(row == 3) {
            lcdgotoxy(0,0);
        }
        column = 0;
    }
    send_data_or_command(c);
    column++;
}

void lcd_clear(void) {
    send_data_or_command(1); // clearing command
}

void lcdgotoaddr(uint8_t address) {
    send_data_or_command(0x80 | (address));
}

void lcdgotoxy(uint8_t set_row, uint8_t set_column) {
    row = set_row;
    column = set_column;
    if(set_row == 0)
        lcdgotoaddr(0 + set_column);
    else if(set_row == 1)
        lcdgotoaddr(0x40 + set_column);
    else if(set_row == 2)
        lcdgotoaddr(0x14 + set_column);
    else if(set_row == 3)
        lcdgotoaddr(0x54 + set_column);
}

void lcdputs(const char *str) {
    size_t len = strlen(str);
    for(size_t i = 0; i < len; i++) {
        lcdputch(str[i]);
    }
}

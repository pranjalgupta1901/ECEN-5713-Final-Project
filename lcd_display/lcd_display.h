#include <stdint.h>

void lcd_init(void);
void send_data_or_command(unsigned char ch);
void delay(void);
void lcdputch(unsigned char c);
void lcd_clear(void);
void lcdgotoaddr(uint8_t address);
void lcdgotoxy(uint8_t set_row, uint8_t set_column);
void lcdputs(const char *str);
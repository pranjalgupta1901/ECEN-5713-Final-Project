#include <stdint.h>

void lcd_init(void);
void lcd_clear(void);
void lcdgotoxy(uint8_t set_row, uint8_t set_column);
void lcdputch(unsigned char c);
void lcdputs(char *str);
void lcd_move_and_write(int x, int y, char *data);
void close_i2c_file() ;

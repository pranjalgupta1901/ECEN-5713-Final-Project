#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../lcd_display/lcd_display.h"

 int main() {
    // Initialize LCD
     lcd_init();
    lcd_move_and_write(1,5, "hello world");

   close_i2c_file();  
    return 0;
}


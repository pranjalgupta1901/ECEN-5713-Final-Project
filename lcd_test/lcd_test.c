#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../lcd_display/lcd_display.h"

int main() {
    // Initialize LCD
    lcd_init();

    // Clear LCD
    lcd_clear();

    // Display a message
    lcdputs("Hello, World!");

    // Delay to observe the display
    sleep(5);

    // Clear LCD
    lcd_clear();

    // Display another message
    lcdputs("Testing LCD driver!");

    // Delay to observe the display
    sleep(5);

    // Clear LCD
    lcd_clear();

    // You can add more tests or functionality here

    return 0;
}


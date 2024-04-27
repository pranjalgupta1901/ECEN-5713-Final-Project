#include <stdint.h>


#define DIRECTION_HIGH '1'
#define DIRECTION_LOW '0'

int export_gpio(const char *gpio_pin);
int read_gpio_state(const char *gpio_pin);
int gpio_set_direction(const char *gpio_pin, const char *direction);


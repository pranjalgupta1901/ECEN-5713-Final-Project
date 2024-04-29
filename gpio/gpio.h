#include <stdint.h>


#define DIRECTION_OUT "out"
#define DIRECTION_IN "in"

int export_gpio(const char *gpio_pin);
int read_gpio_state(const char *gpio_pin);
int gpio_set_direction(const char *gpio_pin, const char *direction);


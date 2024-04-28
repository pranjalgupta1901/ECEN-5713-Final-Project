#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"
#include <sys/stat.h>

int export_gpio(const char *gpio_pin) {
    int fd, len;
    char buf[100];

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open export for writing");
        return -1;
    }
	
	// using snprintf instead of sprintf to prevent buffer overflow
    len = snprintf(buf, sizeof(buf), "%s", gpio_pin);
    if (write(fd, buf, len) < 0) {
        perror("Failed to export GPIO");
        close(fd);
        return -1;
    }

    close(fd);
    
    struct stat check_file_status;
    
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%s", gpio_pin);
    if(stat(buf, &check_file_status) != 0){
    	perror("Failed to export, verified by using stat ");
    	return -1;
    }
    
    return 0;
}

// first this gpiofile should be exported and direction should be set before calling this fucntion
int read_gpio_state(const char *gpio_pin){

    int fd = 0;
    char buf[100];
    char value;
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%s/value", gpio_pin);
	
    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open value for reading");
        return -1;
    }

    if (read(fd, &value, 1) < 0) {
        perror("Failed to read value");
        close(fd);
        return -1;
    }

    close(fd);
    
    if(value == '0')
    	return 0;
    else
    	return 1;
    	
    return 0;
}


int gpio_set_direction(const char *gpio_pin, const char *direction){
    int fd = 0, len;
    char buf[100];
    
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%s/direction", gpio_pin);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open file for writing");
        return -1;
    }

    len = snprintf(buf, sizeof(buf), "%s", direction);
    if (write(fd, buf, len) < 0) {
        perror("Failed to write direction ");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;

}







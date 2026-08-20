#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "inputs.h"

int offset_x = 0;
int offset_y = 0;

static inline bool button_down(uint pin)
{
    return !gpio_get(pin);
}

void inputs_init(void)
{
    for (int i = 0; i < 4; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_up(i);   // buttons wired to GND
    }
}

void handle_input(void)
{
    if (button_down(BTN_UP))    
    {
        offset_y += 2;
        printf("idle: %d %d %d %d\n", gpio_get(0), gpio_get(1), gpio_get(2), gpio_get(3));
    }
    if (button_down(BTN_DOWN))  
    {
        offset_y -= 2;
        printf("idle: %d %d %d %d\n", gpio_get(0), gpio_get(1), gpio_get(2), gpio_get(3));
    }
    if (button_down(BTN_LEFT))  
    {
        offset_x -= 2;
        printf("idle: %d %d %d %d\n", gpio_get(0), gpio_get(1), gpio_get(2), gpio_get(3));
    }
    if (button_down(BTN_RIGHT)) 
    {
        offset_x += 2;
        printf("idle: %d %d %d %d\n", gpio_get(0), gpio_get(1), gpio_get(2), gpio_get(3));
    }
}
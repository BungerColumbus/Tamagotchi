#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "inputs.h"
#include "entity.h"

int global_x = 0;
int global_y = 0;

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

void handle_input(Entity *e)
{
    printf("global_x: %d, global_y: %d\n", global_x, global_y);
    if (button_down(BTN_UP)) e -> pos.y += 2;
    if (button_down(BTN_DOWN)) e -> pos.y -= 2;
    if (button_down(BTN_LEFT)) e -> pos.x -= 2;
    if (button_down(BTN_RIGHT)) e -> pos.x += 2;
}
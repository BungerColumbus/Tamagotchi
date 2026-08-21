#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "images.h"
#include "entity.h"
#include "hw_config.h"
#include "display_config.h"
#include "sd_commands.h"
#include "inputs.h"
#include "rooms.h"
#include "ff.h"
#include "f_util.h"


Vec2 current_room = {0, 0};
Vec2 camera = {0, 0};


Entity player = { .pos = {0, 0}, .sprite = active_character, .w = 32, .h = 32 };
Entity background = { .pos = {0, 0}, .sprite = active_background, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT };

void update(void)
{
    handle_input(&player);
    // Drawing some sprites on the buffer
    draw_sprite_to_buffer(active_background, background.w, background.h, background.pos.x, background.pos.y);

    draw_sprite_to_buffer(active_character, player.w, player.h, player.pos.x, player.pos.y);

    // e is the edge crossed by the entity player, if the edge is not NONE we call change_room
    Edge e = edge_crossed(&player);
    if (e != EDGE_NONE) change_room(e, active_background, current_room);

    
    // This is supposed to be like 60fps? more or less lol (actually less)
    sleep_ms(16);
        
    // Displaying the buffer!!!
    display_buffer();
}

// The name shall be BachaRO
int main()
{
    // LED for visual status. When LED is on => everything is fine, when LED is off => something went wrong (mostly just SD card problems)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    stdio_init_all();

    // Waiting for USB connection
    for (int i = 0; i < 30; i++)
    {
        if (stdio_usb_connected())
            break;
        sleep_ms(100);
    }
    // Testing if we can write to our txt file
    write_to_txtfile("read_from_file.txt", "This message will be read by the Pico\n");
    // Testing if we can read from our txt file
    read_from_txtfile("read_from_file.txt");
    // Reading BMP files to arrays, they are going to be array for RGB565 images, lovely for our ST7735 display :)
    read_bmp_to_array(room_at(0,0)->room_name, active_background);
    read_bmp_to_array(bmp_character, active_character);

    // Initializing the display
    st7735_init();
    inputs_init();

    // This is like the update void in Unity
    while (true)
    {
        update();
        tight_loop_contents();
    }
    return 0;
}

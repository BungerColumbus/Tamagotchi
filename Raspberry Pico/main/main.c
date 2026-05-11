#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "images.h"
#include "hw_config.h"
#include "display_config.h"
#include "sd_commands.h"
#include "ff.h"
#include "f_util.h"

void update(void)
{
    display_image(background, 0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
    sleep_ms(100);
}

int main()
{
    // LED for visual status
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    stdio_init_all();

    // Initialize USB serial
    for (int i = 0; i < 30; i++)
    {
        if (stdio_usb_connected())
            break;
        sleep_ms(100);
    }

    write_to_txtfile("read_from_file.txt", "This message will be read by the Pico\n");
    read_from_txtfile("read_from_file.txt");
    read_bmp_to_array("background.bmp", background);

    st7735_init();

    while (true)
    {
        update();
        tight_loop_contents();
    }
    return 0;
}

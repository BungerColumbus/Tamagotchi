#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "images.h"
#include "display_config.h"

static void st7735_write_command(uint8_t cmd)
{
    gpio_put(PIN_DC, 0);               // The byte is an instruction (e.g., “write pixels,” “set column range”).
    gpio_put(PIN_CS, 0);               // The display listens to SPI.
    spi_write_blocking(spi0, &cmd, 1); // Wait for buffer to be emptied
    gpio_put(PIN_CS, 1);               // The display doesn't listen to SPI.
}

// Send the bytes that command expects.
static void st7735_write_data(const uint8_t *data, size_t len)
{
    gpio_put(PIN_DC, 1);                 // The following bytes are data related to that command.
    gpio_put(PIN_CS, 0);                 // The display listens to SPI.
    spi_write_blocking(spi0, data, len); // Wait for buffer to be emptied
    gpio_put(PIN_CS, 1);                 // The display doesn't listen to SPI.
}

// Doing all the bs needed to setup this up
void st7735_init(void)
{
    // SPI0 init
    spi_init(spi0, 10000000); // 10 MHz I think are good enough
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    // CS/DC/RST/BL pins
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    gpio_init(PIN_DC);
    gpio_set_dir(PIN_DC, GPIO_OUT);

    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_init(PIN_BL);
    gpio_set_dir(PIN_BL, GPIO_OUT);
    gpio_put(PIN_BL, 1); // Turn back-light on

    // Reset
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(10);

    st7735_write_command(ST7735_SWRESET);
    sleep_ms(150);
    st7735_write_command(ST7735_SLPOUT);
    sleep_ms(150);

    // Colour mode: 16-bit colour
    st7735_write_command(ST7735_COLMOD);
    uint8_t colmod = 0x55; // 16-bit
    st7735_write_data(&colmod, 1);

    // Memory access control (orientation)
    st7735_write_command(ST7735_MADCTL);
    uint8_t madctl_data = 0x28;
    st7735_write_data(&madctl_data, 1);
    // Lowkey, this took a while to properly finish, but hopefully I won't have to touch the MADCTL AGAIN 0_0

    // Turn on display
    st7735_write_command(ST7735_DISPON);
    sleep_ms(100);
}

// Setting the address for rows and columns and telling display... YOU WILL GET DATA!!!
// I promise this is the last setup... unless?
void st7735_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Setting the address for the rows
    st7735_write_command(ST7735_CASET);
    uint8_t caset_data[] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    st7735_write_data(caset_data, sizeof(caset_data));

    // Setting the address for the columns
    st7735_write_command(ST7735_RASET);
    uint8_t raset_data[] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
    st7735_write_data(raset_data, sizeof(raset_data));

    // The next data will be related to pixels only, this is what this command does
    st7735_write_command(ST7735_RAMWR);
}

// Displaying the buffer image we created
void display_buffer(void)
{
    // Uses 160 for width, 128 for height
    st7735_set_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    uint8_t *p = (uint8_t *)screen_buffer;
    size_t bytes = SCREEN_WIDTH * SCREEN_HEIGHT * 2; // 160 * 128 * 2 = 40,960 bytes

    st7735_write_data(p, bytes);
}

// Putting an image of the buffer. The reason I use this is simple
// It manages to achieve something somewhat similar to what layers in drawing apps achieve
// And it also manages to get rid of the "transparent" (0x0000) pixels
// This is a bit slower... BUT this will only run in the background, our display_buffer is what actually acheives the display
void draw_sprite_to_buffer(const uint16_t *sprite, int sprite_w, int sprite_h, int start_x, int start_y)
{
    for (int y = 0; y < sprite_h; y++)
    {
        for (int x = 0; x < sprite_w; x++)
        {
            // Taking specific pixel from sprite
            uint16_t pixel = sprite[y * sprite_w + x];

            // Skip drawing the so-called "transparent" pixel
            if (pixel == 0x0000)
            {
                continue;
            }

            // Calculate where this pixel lands on the main screen buffer
            int screen_x = start_x + x;
            int screen_y = start_y + y;

            // Boundary check using uniform landscape dimensions
            if (screen_x >= 0 && screen_x < SCREEN_WIDTH && screen_y >= 0 && screen_y < SCREEN_HEIGHT)
            {
                // Putting the pixels in the right location of the screen buffer.
                screen_buffer[screen_y * SCREEN_WIDTH + screen_x] = pixel;
            }
        }
    }
}
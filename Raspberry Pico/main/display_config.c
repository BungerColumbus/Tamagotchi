#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "images.h"
#include "display_config.h"

// ST7735 commands
#define ST7735_SWRESET 0x01 // Software Reset (reset the display)
#define ST7735_SLPOUT 0x11  // Sleep Out (exit sleep mode)
#define ST7735_COLMOD 0x3A  // Colour Mode (use to set the colour mode)
#define ST7735_MADCTL 0x36  // Memory Access Data Control (rotation, mirroring, RGB/BGR order)
#define ST7735_DISPON 0x29  // Display On
#define ST7735_CASET 0x2A   // Column Address Set (x coordinate)
#define ST7735_RASET 0x2B   // Row Address Set (y coordinate)
#define ST7735_RAMWR 0x2C   // RAM Write (you use this to tell the SPI that the following bytes are data for pixels)

// SPI (Serial Peripheral Interface) is the way the Pico communicates with the display

// pin definitions
#define PIN_CS 5   // Chip Select
#define PIN_DC 4   // GPIO
#define PIN_RST 6  // Reset
#define PIN_MOSI 3 // Master Out Slave in, The data from the master goes to the slave
#define PIN_SCK 2  // Serial Clock (to synchronize the devices)
#define PIN_BL 0   // It's the LED pin

// display size
#define TFT_WIDTH 128
#define TFT_HEIGHT 160
// tell the display what to do.

static void st7735_write_command(uint8_t cmd)
{
    gpio_put(PIN_DC, 0);               // the byte is an instruction (e.g., “write pixels,” “set column range”).
    gpio_put(PIN_CS, 0);               // the display listens to SPI.
    spi_write_blocking(spi0, &cmd, 1); // wait for buffer to be emptied
    gpio_put(PIN_CS, 1);               // the display doesn't listen to SPI.
}
// send the bytes that command expects.
static void st7735_write_data(const uint8_t *data, size_t len)
{
    gpio_put(PIN_DC, 1);                 // the following bytes are data related to that command.
    gpio_put(PIN_CS, 0);                 // the display listens to SPI.
    spi_write_blocking(spi0, data, len); // wait for buffer to be emptied
    gpio_put(PIN_CS, 1);                 // the display doesn't listen to SPI.
}

void st7735_init(void)
{
    // SPI0 init
    spi_init(spi0, 10000000); // 10 MHz initial; increase as wiring allows
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
    gpio_put(PIN_BL, 1); // turn back-light on
    // reset
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(10);

    st7735_write_command(ST7735_SWRESET);
    sleep_ms(150);
    st7735_write_command(ST7735_SLPOUT);
    sleep_ms(150);

    // colour mode: 16-bit colour
    st7735_write_command(ST7735_COLMOD);
    uint8_t colmod = 0x55; // 16-bit
    st7735_write_data(&colmod, 1);

    // memory access control (orientation)
    st7735_write_command(ST7735_MADCTL);
    uint8_t madctl_data = 0x68;
    st7735_write_data(&madctl_data, 1);

    // turn on display
    st7735_write_command(ST7735_DISPON);
    sleep_ms(100);
}

void st7735_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Setting the address for the rows
    st7735_write_command(ST7735_RASET);
    uint8_t caset_data[] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    st7735_write_data(caset_data, sizeof(caset_data));

    // Setting the address for the columns
    st7735_write_command(ST7735_CASET);
    uint8_t raset_data[] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
    st7735_write_data(raset_data, sizeof(raset_data));

    // I wanted to have one here as example, now all the pixels will be rotated the correct way

    // The next data will be related to pixels only, this is what this command does
    st7735_write_command(ST7735_RAMWR);
}

void display_image(const uint16_t *image)
{
    st7735_set_window(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
    // send image data as bytes (RGB565)
    // convert uint16_t array to uint8_t buffer if necessary
    uint8_t *p = (uint8_t *)image;
    size_t bytes = TFT_WIDTH * TFT_HEIGHT * 2;
    st7735_write_data(p, bytes);
}
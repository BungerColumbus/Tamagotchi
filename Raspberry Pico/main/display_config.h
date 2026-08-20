#ifndef ST7735_H
#define ST7735_H

#include <stdint.h>

// ST7735 commands
#define ST7735_SWRESET 0x01 // Software Reset
#define ST7735_SLPOUT 0x11  // Sleep Out
#define ST7735_COLMOD 0x3A  // Colour Mode

#define ST7735_MADCTL 0x36 // Memory Access Data Control
/*
• 0x08 = Mirror X
• 0x10 = Mirror Y
• 0x20 = Swap X/Y (rotate 90°)
• 0x40 = Refresh direction
• 0x80 = RGB/BGR order
*/

#define ST7735_DISPON 0x29 // Display On
#define ST7735_CASET 0x2A  // Column Address Set
#define ST7735_RASET 0x2B  // Row Address Set
#define ST7735_RAMWR 0x2C  // RAM Write

#define X_OFFSET 1
#define Y_OFFSET 2

// The pins of the screen!
#define PIN_CS 13
#define PIN_DC 8
#define PIN_RST 9
#define PIN_MOSI 11
#define PIN_SCK 10
#define PIN_BL 12

/**
 * @brief Initializes the ST7735 display with default settings.
 */
void st7735_init(void);

/**
 * @brief Sets the address window for pixel streaming.
 *
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 */
void st7735_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @brief Sends a full-screen image buffer to the display.
 *
 * @param image Pointer to a uint16_t array (RGB565 format)
 */
void display_buffer(void);

/**
 * @brief Puts the pixels on the screen buffer array, avoids transparency, achieves layering
 *
 * @param sprite Pointer to a uint16_t array (RGB565 format)
 * @param width Sprite width
 * @param height Sprite height
 * @param start_x Where we start putting out sprite pixels on the buffer (horiontally)
 * @param start_y Where we start putting out sprite pixels on the buffer (vertically)
 */
void draw_sprite_to_buffer(const uint16_t *sprite, int width, int height, int start_x, int start_y);

#endif // ST7735_H
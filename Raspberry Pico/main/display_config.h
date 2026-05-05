#ifndef ST7735_H
#define ST7735_H

#include <stdint.h>

// --- ST7735 Commands ---
#define ST7735_SWRESET 0x01 // Software Reset
#define ST7735_SLPOUT 0x11  // Sleep Out
#define ST7735_COLMOD 0x3A  // Colour Mode
#define ST7735_MADCTL 0x36  // Memory Access Data Control
#define ST7735_DISPON 0x29  // Display On
#define ST7735_CASET 0x2A   // Column Address Set
#define ST7735_RASET 0x2B   // Row Address Set
#define ST7735_RAMWR 0x2C   // RAM Write

// --- Pin Definitions ---
// Note: These are defined here so other files in your project
// can reference them if needed.
#define PIN_CS 5
#define PIN_DC 4
#define PIN_RST 6
#define PIN_MOSI 3
#define PIN_SCK 2
#define PIN_BL 0

// --- Display Size ---
#define TFT_WIDTH 128
#define TFT_HEIGHT 160

// --- Public Function Prototypes ---

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
void display_image(const uint16_t *image);

#endif // ST7735_H
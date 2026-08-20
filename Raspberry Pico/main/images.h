#ifndef _IMAGES_H
#define _IMAGES_H

#include <stdint.h>



// Example: 128x160 image (RGB565)
#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 160



extern char bmp_mainroom[];
extern char bmp_leftroom[];
extern char bmp_character[];

extern uint16_t screen_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
extern uint16_t active_background[SCREEN_WIDTH * SCREEN_HEIGHT];
extern uint16_t active_character[32 * 32];

#endif // IMAGES_H
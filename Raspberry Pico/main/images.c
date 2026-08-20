#include <stdint.h>
#include "images.h"


char bmp_mainroom[] = "mainroom.bmp";
char bmp_leftroom[] = "leftroom.bmp";
char bmp_character[] = "character.bmp";
// The screen buffer that is what we actually display on the screen
uint16_t screen_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
// The active images which are what we will put on the screen buffer
uint16_t active_background[SCREEN_WIDTH * SCREEN_HEIGHT];
uint16_t active_character[32 * 32];
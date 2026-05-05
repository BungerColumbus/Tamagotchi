#include <stdint.h>
#include "images.h"
// Example: 128x160 image (RGB565)
#define IMAGE_WIDTH 128
#define IMAGE_HEIGHT 160

const uint16_t background[IMAGE_WIDTH * IMAGE_HEIGHT];
const uint16_t character[32 * 32];
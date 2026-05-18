#ifndef SD_COMMANDS_H
#define SD_COMMANDS_H

#include "ff.h"

void mount_sd_card(void);

void write_to_txtfile(char *filename, char *text);

void read_from_txtfile(char *filename);

void read_bmp_to_array(char *filename, uint16_t *dest_array);

// Note: FATFS fs and FRESULT fr are managed internally in the .c file.
// If you need to access the last result code elsewhere, you could
// declare 'extern FRESULT fr;' here.

#endif // SD_COMMANDS_H
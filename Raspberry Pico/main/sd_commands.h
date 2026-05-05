#ifndef SD_COMMANDS_H
#define SD_COMMANDS_H

#include "ff.h"

// --- Public Function Prototypes ---

/**
 * @brief Initializes and mounts the SD card filesystem.
 * This function will panic and halt if mounting fails.
 */
void mount_sd_card(void);

/**
 * @brief Opens "test.txt", appends a line of text, and unmounts the card.
 */
void write_to_txtfile(char *filename, char *text);

void read_from_txtfile(char *filename);

// Note: FATFS fs and FRESULT fr are managed internally in the .c file.
// If you need to access the last result code elsewhere, you could
// declare 'extern FRESULT fr;' here.

#endif // SD_COMMANDS_H
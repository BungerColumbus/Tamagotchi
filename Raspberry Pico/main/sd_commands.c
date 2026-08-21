#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hw_config.h"
#include "images.h"
#include "ff.h"
#include "f_util.h"

// They are required, this how FATFS stuff works
FATFS fs;
FRESULT fr;

// Mounting the SD card, it flushes before doing so, it also gives an error message if it fails :P
void mount_sd_card(void)
{
    puts("Mounting SD card...");
    fflush(stdout);

    fr = f_mount(&fs, "", 1);
    if (FR_OK != fr)
    {
        gpio_put(25, 0);
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }
}

// Un-mounting the SD card (and flushing after)
void unmount_sd_card(void)
{

    // Unmount the SD card
    f_unmount("");

    puts("Unmounted SD card! Safe to take out!");
    fflush(stdout);
}

// FIL fil; fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE); fr = f_close(&fil);
// These are the only commands that matter, the other ones are to know if stuff fails
// Also mounting and unmounting SD card
void write_to_txtfile(char *filename, char *text)
{
    mount_sd_card();

    // Open a file and write to it
    FIL fil;
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
    {
        gpio_put(25, 0);
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }
    if (f_printf(&fil, text) < 0)
    {
        gpio_put(25, 0);
        printf("f_printf failed\n");
    }

    // Close the file
    fr = f_close(&fil);
    if (FR_OK != fr)
    {
        gpio_put(25, 0);
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    unmount_sd_card();
}

// Same as in write except this time we use fr = f_open(&fil, filename, FA_READ); (WOW, so unique)
void read_from_txtfile(char *filename)
{
    mount_sd_card();

    // Open a file and read from it
    FIL fil;
    fr = f_open(&fil, filename, FA_READ);
    if (FR_OK != fr)
    {
        gpio_put(25, 0);
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }

    char buffer[100];
    if (f_gets(buffer, sizeof(buffer), &fil) == NULL)
    {
        gpio_put(25, 0);
        printf("f_gets failed\n");
    }
    else
    {
        sleep_ms(1000);
        printf("Read from file: %s\n", buffer);
    }

    // Close the file
    fr = f_close(&fil);
    if (FR_OK != fr)
    {
        gpio_put(25, 0);
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    unmount_sd_card();
}

// This is the more complex one
/*
    We have a BMP file that is 32 bit (this is what I used on my SD card)
    The parameters are the name of the file we want to read and
    in what array will we put the conversion of the BMP

    https://en.wikipedia.org/wiki/BMP_file_format
    This has all the information one needs in order to understand how to achieve this conversion
*/
void read_bmp_to_array(const char *filename, uint16_t *dest_array)
{

    // We mount the sd as we did for every other function until now
    mount_sd_card();

    // We locally set the file object and the bytes read variables.
    FIL fil;
    UINT br;

    // We have a buffer for the header of the BMP file (the ones in windows and saved by Aseprite are 122 bytes long)
    unsigned char header[122];

    // Attempt to read from the BMP file
    fr = f_open(&fil, filename, FA_READ);
    if (fr != FR_OK)
    {
        gpio_put(25, 0);
        printf("f_open(%s) error: %d\n", filename, fr);
        unmount_sd_card();
        return;
    }

    // Read the 122-byte header
    f_read(&fil, header, 122, &br);

    // Validate if it is truly BMP (Check for 'BM' signature)
    if (header[0] != 'B' || header[1] != 'M')
    {
        gpio_put(25, 0);
        printf("Not a valid BMP file\n");
        f_close(&fil);
        unmount_sd_card();
        return;
    }

    // Extracting the headers as safely as possible (It's a bit of an overkill what I did here)
    uint32_t dataOffset = header[0x0A] | (header[0x0B] << 8) | (header[0x0C] << 16) | (header[0x0D] << 24);
    int32_t width = *(int32_t *)&header[0x12]; // or use safe read macro
    int32_t height = *(int32_t *)&header[0x16];
    uint16_t bitsPerPixel = header[0x1C] | (header[0x1D] << 8); // Offset 0x1C in DIB header

    // Calculate bytes per pixel and padded row size
    uint8_t bytesPerPixel = bitsPerPixel / 8;
    uint32_t unpaddedRowSize = width * bytesPerPixel;
    uint32_t paddedRowSize = (unpaddedRowSize + 3) & ~3; // Round up to multiple of 4

    // My attempt at avoiding memory leaks start here, should learn how to use rust smh
    unsigned char *rowBuffer = malloc(paddedRowSize);
    if (rowBuffer == NULL)
    {
        printf("Failed to allocate row buffer (%d bytes)\n", paddedRowSize);
        f_close(&fil);
        unmount_sd_card();
        return;
    }

    // A bunch of stuff for debugging (was never too useful, might delete later)
    printf("BMP Debug: width=%d, height=%d, bitsPerPixel=%d, dataOffset=%lu\n",
           width, height, bitsPerPixel, dataOffset);
    printf("Calculated: bytesPerPixel=%d, paddedRowSize=%lu\n",
           bytesPerPixel, paddedRowSize);
    printf("First pixel raw: [%02X %02X %02X %02X]\n",
           rowBuffer[0], rowBuffer[1], rowBuffer[2], rowBuffer[3]);

    // Seek to pixel data
    f_lseek(&fil, dataOffset);

    // Read rows bottom-to-top (BMP stores images upside-down)
    for (int y = 0; y < height; y++)
    {
        fr = f_read(&fil, rowBuffer, paddedRowSize, &br);
        if (fr != FR_OK || br < paddedRowSize)
            break;

        for (int x = 0; x < width; x++)
        {
            uint8_t b, g, r;

            if (bitsPerPixel == 32)
            {
                // BGRA format
                r = rowBuffer[x * 4];
                g = rowBuffer[x * 4 + 1];
                b = rowBuffer[x * 4 + 2];
                // Alpha is going to be 0x0000 (the blackest of blacks), we will consider that colour the transparent colour we need
            }
            else if (bitsPerPixel == 24)
            {
                // BGR format, no alpha
                r = rowBuffer[x * 3];
                g = rowBuffer[x * 3 + 1];
                b = rowBuffer[x * 3 + 2];
            }
            else
            {
                printf("Unsupported bit depth: %d\n", bitsPerPixel);
                free(rowBuffer);
                f_close(&fil);
                unmount_sd_card();
                return;
            }

            // Convert RGB888 to RGB565
            // I also thought it was a bit stupid how easy it is to convert, but apparently,
            // One needs to only read the first 5 bits from the colour RED in RGB888 in order to obtain the RED of RGB565
            uint16_t rgb565 = (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
            rgb565 = (rgb565 >> 8) | (rgb565 << 8);

            // Store it in our destination array
            dest_array[y * width + x] = rgb565;
        }
    }

    // Here is where my attempt at avoiding memory leaks ends, is it good? I think it works (no errors until now).
    free(rowBuffer);
    rowBuffer = NULL;

    // We did it! If everything went fine, We just loaded the BMP file and converted it properly :P
    f_close(&fil);
    unmount_sd_card();
    printf("BMP loaded successfully: %s\n", filename);
}
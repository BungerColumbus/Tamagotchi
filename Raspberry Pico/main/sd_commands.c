#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hw_config.h"
#include "ff.h"
#include "f_util.h"

FATFS fs;
FRESULT fr;

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

void unmount_sd_card(void)
{

    // Unmount the SD card
    f_unmount("");

    puts("Unmounted SD card! Safe to take out!");
    fflush(stdout);
}

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
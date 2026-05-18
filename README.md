I have switched from Arduino Nano to Raspberry Pi Pico 2.

(I also used https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico.git as a repo in order to make my life easier to connecting the SD card)
It can load BMP images properly from the SD card to the display.
It can also ignore transparent pixels and achieve layering.
This is done through a screen_buffer on which we properly insert our images first. The screen_buffer is what actually gets loaded to the display.

We run this at 60FPS. For the moment I haven't tested how many things I can put on the screen buffer before displaying it and for the screen_buffer to still be good
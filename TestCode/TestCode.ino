#include "bitmap.h"
#include <avr/pgmspace.h>
#include <TFT.h>
#include <SPI.h>

const int button1 = 2;  // the number of the pushbutton pin
const int button2 = 3;  // the number of the pushbutton pin
const int button3 = 4;  // the number of the pushbutton pin
const int button4 = 5;  // the number of the pushbutton pin
const int ledPin = 7;  // the number of the LED pin

int x = 0;
int y = 0;
uint8_t mask = 0x03;

#define cs 10
#define dc 9
#define rst 8

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

uint16_t TRANSPARENT = 0xffff;

const uint16_t palette[4] PROGMEM = {
  0x31A6, 0x7BCF, 0x3D6D, 0xDEB6 //black, grey, green, bej
};

TFT TFTScreen = TFT(cs, dc, rst);

void loadBigImage(int x, int y, const uint16_t *image, int w, int h) {
  // set TFT cursor to top-left corner of the image
  TFTScreen.setAddrWindow(x, y, x + w-1, y + h-1);  // x0,y0,x1,y1

  for (int i = 0; i < w * h; i++) {
    uint16_t color = pgm_read_word(&(image[i]));
    TFTScreen.pushColor(color);                   
  }
}

void loadSmallImage(int x, int y, const uint16_t *image, uint16_t transparent, int length) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
      uint16_t color = pgm_read_word(&(image[i * length + j]));
      if (color != transparent) {
        TFTScreen.drawPixel(x + j, y + i, color);
      }
    }
  }
}

void loadXbitColorImage(int x, int y, const uint8_t *image, int length) {
  uint16_t color;
  for (int i = 0; i < length * 2; i++) {      // 8*2 = 16 pixels high
    for (int j = 0; j < length * 2; j++) {    // 8*2 = 16 pixels wide
      uint8_t byte = pgm_read_byte(&image[(i / 4) * length + (j / 4)]);
      uint8_t pix = (byte >> (2 * (3 - (j % 4)))) & 0x03;

      if (pix == 3) color = 0x001F;
      else if (pix == 2) color = 0x0000;
      else if (pix == 1) color = 0xFFFF;
      else color = 0x07E0;

      TFTScreen.drawPixel(x + j, y + i, color);
    }
  }
}



void setup() {

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  // initialize the library
  TFTScreen.begin();
  TFTScreen.setRotation(3);
  
  // IF YOU WANT TO RENDER ANYTHING ON THE DISPLAY DO IT AFTER THIS LINE
  // clear the screen with a black background
  TFTScreen.fillScreen(0xffff);
  //loadBigImage(64, 0, castle, 64, 64);
  //loadBigImage(0, 0, castle, 64, 64);
  //loadBigImage(0, 64, castle, 64, 64);
  //loadBigImage(64, 64, castle, 64, 64);
  loadXbitColorImage(x, y, smallMemoryUsage, 8);


}



void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(button1); // left
  buttonState2 = digitalRead(button2); // right
  buttonState3 = digitalRead(button3); // down
  buttonState4 = digitalRead(button4); // up

  if(buttonState1 == LOW) {
    x+=16;
    TFTScreen.fillScreen(0xffff);
    loadXbitColorImage(x, y, smallMemoryUsage, 8);
  }
  if(buttonState2 == LOW) {
    x-=16;
    TFTScreen.fillScreen(0xffff);
    loadXbitColorImage(x, y, smallMemoryUsage, 8);
  }
  if(buttonState3 == LOW) {
    y-=16;
    TFTScreen.fillScreen(0xffff);
    loadXbitColorImage(x, y, smallMemoryUsage, 8);
  }
  if(buttonState4 == LOW) {
    y+=16;
    TFTScreen.fillScreen(0xffff);
    loadXbitColorImage(x, y, smallMemoryUsage, 4);
  }

  delay(100); // debounce & slow down movement a little
}

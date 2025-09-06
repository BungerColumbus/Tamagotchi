#include "bitmap.h"
#include <avr/pgmspace.h>
#include <TFT.h>
#include <SPI.h>

const int button1 = 2;  // the number of the pushbutton pin
const int button2 = 3;  // the number of the pushbutton pin
const int button3 = 4;  // the number of the pushbutton pin
const int button4 = 5;  // the number of the pushbutton pin
const int ledPin = 7;  // the number of the LED pin

#define cs 10
#define dc 9
#define rst 8

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

const uint16_t palette[4] PROGMEM = {
  0x31A6, 0x7BCF, 0x3D6D, 0xDEB6 //black, grey, green, bej
};

TFT TFTscreen = TFT(cs, dc, rst);

void loadImage(uint16_t image[16 * 16]) {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      uint16_t color = pgm_read_word(&(image[i * 16 + j]));
      TFTscreen.drawPixel(j, i, color);
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

  //initialize the library
  TFTscreen.begin();

  TFTscreen.setRotation(3);
  // clear the screen with a black background
  
}



void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(button1); // left
  buttonState2 = digitalRead(button2); // right
  buttonState3 = digitalRead(button3); // down
  buttonState4 = digitalRead(button4); // up

  if(buttonState1 == HIGH) {
    loadImage(testSprite);
  }

  delay(100); // debounce & slow down movement a little
}

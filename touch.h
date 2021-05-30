// Touch screen configuration

#ifndef TOUCH_H
#define TOUCH_H  

#include <XPT2046_Touchscreen.h>      // XP2046 touch screen
#include "Adafruit_GFX.h"             // Graphics libraries
#include "Adafruit_ILI9341.h"         // TFT display
#include <Fonts/FreeSans9pt7b.h>      // used font5
#include <Fonts/FreeSans12pt7b.h>     // used font
#include "TouchEvent.h"               
#include "D4ProjectTest.h"
#include <Arduino.h>

extern Adafruit_ILI9341        tft;                       // TFT display
extern XPT2046_Touchscreen     touch;                     // Touch screen
extern TouchEvent              tevent;                    // init TouchEvent with pointer to the touch screen driver

// Color definitions
// 16 bit values, (MSB) 5 bits of red, 6 bits of green, 5 bits of blue (LSB)
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define DARK_GREEN  0x01E0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF

// Function prototypes
void onSwipe(uint8_t);
void onClick(TS_Point);
void onDblClick(TS_Point);
void onDraw(TS_Point);
void onTouch(TS_Point);
void onUntouch(TS_Point);
void onLongClick(TS_Point);
void touch_enable();  
#endif

// Touch screen configuration

#include <SPI.h>                      // Base SPI bus
#include "D4ProjectTest.h"      
#include "touch.h"
#include "uiButton.h"

extern  int                     screenNumber;   
extern  uiButton                button;

//swipe event
void onSwipe(uint8_t dir) {
  switch(dir) {
    case 0: 
      Serial.println(F("Swipe Left"));
      if (screenNumber <SCREEN_MAX) { //right to left switch to next screen
        screenNumber++;
      } else {
        screenNumber = 0;
      }
      break;
      
    case 1: 
      Serial.println(F("Swipe Right"));
      if (screenNumber > 0) { //left to right switch to previous screen
        screenNumber--;
      } else {
        screenNumber = SCREEN_MAX;
      }
      break;
  }
  button.clearAll();
  draw_screen(screenNumber);
}

void onClick(TS_Point p) {
    button.click(p.x,p.y);
//  if (screenNumber > 0) { //on any screen except 0 show the click position
//    tft.setFont(&FreeSans9pt7b);
//    tft.fillRect(0,40,240,70,ILI9341_WHITE);
//    tft.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
//    tft.setCursor(10,60);
//    tft.println("Clicked");
//    tft.println(p.x);
//    tft.println(p.y);
//  }
}

void onDblClick(TS_Point p) {
//  if (screenNumber > 0) { //on any screen except 0 show double click position
//    tft.fillRect(0,40,240,70,ILI9341_WHITE);
//    tft.setFont(&FreeSans9pt7b);
//    tft.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
//    tft.setCursor(10,60);
//    tft.println("Doubleclick");
//    tft.println(p.x);
//    tft.println(p.y);
//  } else {
//    tevent.setDrawMode(false);
//  }
}

void onDraw(TS_Point p) {
  //draw a line from the last position to the current position
//  if (draw) tft.drawLine(last.x,last.y,p.x,p.y,ILI9341_BLACK);
//  last = p;
}

void onTouch(TS_Point p) {
//  last = p;
//  draw = true;
}

void onUntouch(TS_Point p) {
//  draw = false;
}

void onLongClick(TS_Point p) {
//  if (screenNumber > 0) { //on any screen except 0 show double click position
//    tft.setFont(&FreeSans9pt7b);
//    tft.fillRect(0,40,240,70,ILI9341_WHITE);
//    tft.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
//   tft.setCursor(10,60);
//    tft.println("Long Click");
//    tft.println(p.x);
//    tft.println(p.y);
//  }
}

// Acviate all the touch callbacks.
void touch_enable() {
  //init TouchEvent instance
  tevent.setResolution(tft.width(),tft.height());
  tevent.setDblClick(300);
  tevent.registerOnTouchSwipe(onSwipe);
  tevent.registerOnTouchClick(onClick);
  tevent.registerOnTouchDblClick(onDblClick);
  tevent.registerOnTouchLong(onLongClick);
  tevent.registerOnTouchDraw(onDraw);
  tevent.registerOnTouchDown(onTouch);
  tevent.registerOnTouchUp(onUntouch);
}

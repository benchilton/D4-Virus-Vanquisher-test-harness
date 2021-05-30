// User interface button class

#include "uiButton.h"
#include "touch.h"


// Constructor
uiButton::uiButton() {
    buttonCount=0;                                  // There are no buttons
}

// Destructor
uiButton::~uiButton() {
}

// Clear all buttons (used on screen changes)
// Does not remove the button from the screen - since this is done by the page move 
// Do we need a remove a specific button function ?
void uiButton::clearAll() {
  buttonCount=0;
};


// Draw a button on the screen. Doesn't provide any clickability or similar, its just a button on the screen
int uiButton::create(unsigned int xp, unsigned int yp, unsigned int xs, unsigned int ys, const __FlashStringHelper * textP, unsigned int pressed, func_t call) {

  if (buttonCount >= BUTTON_MAX) {
    return 0;
  }
      
  // Store the button info
  button[buttonCount].xMin      = xp;
  button[buttonCount].yMin      = yp;
  button[buttonCount].xMax      = xp+xs;
  button[buttonCount].yMax      = yp+ys;
  button[buttonCount].callback  = call;   // Pointer to function when button pressed
  strcpy_P(button[buttonCount].label, (char *) textP);

  Serial.printf("Create button %d %s at %d,%d %d,%d\n\r",buttonCount,button[buttonCount].label, button[buttonCount].xMin, button[buttonCount].yMin, button[buttonCount].xMax, button[buttonCount].yMax);    
  draw(buttonCount,pressed);
  
  buttonCount++;
  return buttonCount;   // Button created, return its ID
}


// Redraw the specified button ID
void uiButton::draw(unsigned int buttonid, unsigned int pressed){

  uint16_t  boxcol1, boxcol2, fillcol;
  if (pressed) { 
    fillcol = DARK_GREEN;
  } else {
    fillcol = GREEN;
  }

  tft.setFont(&FreeSans9pt7b);
  tft.setTextColor(ILI9341_BLACK,ILI9341_GREEN);
  tft.fillRoundRect(button[buttonid].xMin, button[buttonid].yMin, button[buttonid].xMax-button[buttonid].xMin, button[buttonid].yMax-button[buttonid].yMin, BUTTON_ROUND_RADIUS, fillcol);
  tft.setCursor(button[buttonid].xMin+10,button[buttonid].yMin+25);
  tft.print(button[buttonid].label);
}

// A click action was detected, see which button was clicked
void uiButton::click(unsigned int xpress, unsigned int ypress) {

  int check = 0;
  int done  = 0;

  Serial.println(F("Click checking"));
  Serial.printf("There are %d buttons, press at %d,%d\n\r",buttonCount,xpress,ypress);    

  do {
    Serial.printf("Button %d\n\r",check);  
    
    if (xpress >= button[check].xMin && 
        xpress <= button[check].xMax && 
        ypress >= button[check].yMin && 
        ypress <= button[check].yMax) {
    
      // Depress the button
      Serial.println(F("Invoking button callback"));

      draw(check,BUTTON_PRESSED);
      button[check].callback();        // Run the call-back
      for (int lp = 0 ; lp < 25 ; lp++) {
        yield();
        delay(10);    
      }
      draw(check,BUTTON_NOT_PRESSED);
      done=1;
      // Wait a bit
      // unclick the button
    }   
  } while ((++check < buttonCount) && !done);
}

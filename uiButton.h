#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <Arduino.h>

#define BUTTON_MAX            6               // Maximum number of buttons
#define BUTTON_NOT_PRESSED    0               // Is the button pressed
#define BUTTON_PRESSED        1               // The button is pressed
#define BUTTON_LINE_WIDTH     3               // How many pixels wide the button line is
#define BUTTON_ROUND_RADIUS   5               // How rounded the corners are on buttons

// Type definitions
typedef void (*func_t)();                     // pointer to function with no args and void return

// The info about the buttons.
struct buttonInfo {
  unsigned int  xMin;
  unsigned int  yMin;
  unsigned int  xMax;
  unsigned int  yMax;
  char          label[20];
  func_t        callback;   // Pointer to function when button pressed
};

// A class for UI buttons and their interaction

class uiButton {
  public:
            uiButton();                                     // Constructor
            ~uiButton();                                    // Destructor    
    int     create(unsigned int, unsigned int, unsigned int, unsigned int, const __FlashStringHelper *, unsigned int , func_t);// Create a new button
    void    clearAll();                                     // Remove all buttons
    void    click(unsigned int, unsigned int);              // A button was pressed
    void    draw(unsigned int, unsigned int);
  
  private:
    int           buttonCount = 0;
    buttonInfo    button[BUTTON_MAX];                       // The button info
};

#endif

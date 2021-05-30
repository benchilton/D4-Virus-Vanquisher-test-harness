
#ifndef D4PROJECTEST_H
#define D4PROJECTEST_H  

#include <Arduino.h>                  // Base Arduino header
#include <WiFi.h>                     // Wifi
#include <driver/adc.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>

// ******************************************************
// Always building for prod unless the following is set 
// #define ENV_DEV 1
// ******************************************************

//ESP32 Voltage defines:

#define OP_VOLTAGE  3.3                                 // ESP32 runs of 3.3v         

// Speed of Sound in m/us
#define SPEED_OF_SOUND    0.000343

//ADC Max and Minimum values:
#define ADC_MIN           0
#define ADC_MAX           4095

//Battery Measurement resistor values:

#define  VM_R_TOP         331700                                // 330K Ohm on board, actual value measured given. Both 10% tolerance
#define  VM_R_BOTTOM      119700                                // 120K Ohm on board, actual value measured given
//y = mx + c
#define  VM_OFFSET        1.01                                  // Calibration Factor found by comparing measured value against theoretical value
#define  VM_RATIO         3.77                                  // Voltage Divider ratio of the above 2 values

// SPI pins
#define SPI_MOSI          23                                    // VSPI MOSI
#define SPI_CLK           18                                    // VSPI Clock (CLK)
#define SPI_MISO          19                                    // VSPI_MISO
  
//TFT pins 
#define TFT_CS             5                                    // Diplay chip select
#define TFT_DC            26                                    // Display data/command
#define TFT_RST           25                                    // Display reset
#define TFT_LED           27                                    // Display background LED

// Touch pins - also uses SPI pins above
#define TOUCH_CS          17                                    // touch screen chip select
#define TOUCH_IRQ         16                                    // touch screen interrupt

#define DC_INPUT_VOLTAGE   35                                   // Battery voltage at input pin - ADC 1/7
#define FORCE_SENSE        36                                   // Force sensing resistors      - ADC 1/0
#define DISTANCE_SENSE     22                                   // Ultrasonic distance sensor
#define DIAG_LED           33                                   // LED on the board not on ESP32.

#define PIEZO_SENSE        32                                   // Piezo 1 buzzer used for input sensing and output

#define PIEZO_BUZZ_1       14                                   // Piezo 2 Pin 1 Used for output only
#define PIEZO_BUZZ_2       12                                   // Piezo 2 Pin 2, Drive this pin 180 degrees out of phase of PIEZO_BUZZ_1

// Screen configuration
#define TFT_SCREEN_X      320                                   // Screen horizontal pixels
#define TFT_SCREEN_Y      240                                   // Screen vertical pixels
#define SCREEN_MAX        1                                     // How many screen pages (0 to n)
#define SCREEN_HEADER     50                                    // How far down from the heading
#define SCREEN_LINE       25                                    // Size of a line on the screen
#define SCREEN_COLUMN1    55                                    // Size of a column on the screen
#define SCREEN_COLUMN2    110                                   // Size of a column on the screen
#define SCREEN_COLUMN3    170                                   // Size of a column on the screen


// Network configuration
#define WIFI_SSID               "Universe"                       // Wireless details
#define WIFI_PASSWORD           "SpikyCactus5!"

#define HOSTNAME                "Vanquisher-Test"          // My device name

// Standard button sizes
#define BUTTON_HEIGHT   40

// Function prototypes
void draw_screen(uint8_t);
void update_screen(uint8_t);
void screenLine(unsigned int, unsigned int);
void drawButton(unsigned int, unsigned int, unsigned int, unsigned int, const __FlashStringHelper*);
void wakeupReason(void);  

// Callback functions for buttons
void        playSound();
void        flashLED();
uint16_t    readFSR();
uint16_t    readPiezo();
float       readBatteryLevel();
float       readUltrasound();

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);//for mapping floats as map() only supports int

void initialisePins();  //Sets all the pins to input/output respectivly

#endif

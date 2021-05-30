// Test Harness for D4 Project Test - Virus Vanquisher
// Select board as : ESP32 Dev Module

// Requires File, Preferences, Additional Boards manager set to include https://dl.espressif.com/dl/package_esp32_index.json

// TFT dispplay is ILI9341 wit the Adafruit TFT drivers
// Touch screen is XPT2046 with the TouchEvent library
// ESP32 documentation https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/

// Detail which libraries to download from library manager in Arduino

//Dont use any of the ADC1 Pins as those are needed for the WiFi,
//Dont use GPIO 17-21 as those are SPI bus pins needed for the internal flash memory
//Note that the Devkitc V4 doesnt have any inbult LED.

#include "D4ProjectTest.h"       // For this application
#include "ota.h"                      // over the air functions
#include "touch.h"                    // Touch modules
#include "uiButton.h"                 // User Interface Buttons

// Constants
const char* ssid        PROGMEM = WIFI_SSID;
const char* password    PROGMEM = WIFI_PASSWORD;

// Creat objects
Adafruit_ILI9341        tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);           // TFT display
XPT2046_Touchscreen     touch(TOUCH_CS, TOUCH_IRQ);                                // Touch screen
TouchEvent              tevent(touch);                                             // init TouchEvent with pointer to the touch screen driver
WiFiClient              espClient;                                                 // So the ESP32 can connect WiFi
class uiButton          button;                                                    // The buttons for the UI

// TFT and touch global variables
int       screenNumber = 0;       // Which screen are we on

// Variables to display on the TFT.

float   batteryVoltage               = 0.0;            // Input / Battery Voltage
float   fsrReading                   = 0.0;            // Input from the Force Sensitive resistors

// Set up the system
void setup() {
  Serial.begin(115200);

  initialisePins();
          
  Serial.println(F("Code booting"));

  //Configure the TFT display and touch screen, so we have IO first
  tft.begin();
  touch.begin();
  tft.setRotation(3);             // 0-3 for the different rotations
  touch.setRotation(3);           // Needs to match the line above
  Serial.print(F("tftx =")); Serial.print(tft.width()); Serial.print(F(" tfty =")); Serial.print(tft.height());    //Log the display characteristics

  tft.setFont(&FreeSans9pt7b);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  tft.setCursor(1,20);
  tft.print(F("MAC addr  : "));
  tft.println(WiFi.macAddress());
  tft.print(F("WiFi SSID : "));
  tft.println(ssid);
    
   // Set up WiFi
  Serial.println();
  Serial.print(F("MAC Address "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
 /*
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    tft.println(F("Failed! Rebooting..."));
    Serial.println(F("Connection failed! Rebooting..."));
    delay(5000);
    ESP.restart();
  }
  OTA_Setup();                            // Configure the Over the Air (OTA) firmware updates
*/
  Serial.print(F(""));
  Serial.print(F("WiFi connected, IP address: "));
  Serial.println(WiFi.localIP());

  tft.println(F("Connected !"));
  tft.print(F("IP address "));
  tft.println(WiFi.localIP());

  // See how much RAM we have left
  // Heap is only available to Malloc
  tft.print(F("Free heap : "));        tft.print(ESP.getFreeHeap());    
  Serial.print(F("Free heap (bytes)         : "));    Serial.println(ESP.getFreeHeap());    
  Serial.print(F("Minimum free heap (bytes) : "));    Serial.println(ESP.getMinFreeHeap());    
  Serial.print(F("Heap size (bytes)         : "));    Serial.println(ESP.getHeapSize());    
  Serial.print(F("Max alloc heap (bytes)    : "));    Serial.println(ESP.getMaxAllocHeap());    
  
  touch_enable();           // Enable touch screen

  // Reduce power consumption by disabling things we don't need
  // adc_power_off();
  // esp_bluedroid_disable();
  // esp_bt_controller_disable();
  setCpuFrequencyMhz(80);                 // Drop CPU from 240MHz to 80MHz speed to save some power
  
  //  esp_wifi_stop();
  // esp_sleep_enable_timer_wakeup(3600ul * 1000000ul); // uS_TO_S_FACTOR 1000000
  // esp_light_sleep_start();     // esp_deep_sleep_start(); 
  // Serial.print(F("Low power enabled"));
  
  // Wait a little before continuing
  for (int delaylp = 0 ; delaylp < 30 ; delaylp++ ) {
    yield();
    delay(100);
  } 
}


// Run the system
void loop() {
  Serial.println(F("Main loop started"));
  wakeupReason();            // Print the wakeup reason for ESP32
   
  draw_screen(screenNumber);        // Draw the screen
  
  while (1) {
    tevent.pollTouchScreen();       // poll for touch events
    ArduinoOTA.handle();
    yield();

    //Serial.print("Battery Voltage: ");
    //Serial.println(readBatteryLevel());
    //Serial.print("FSR Reading: ");
    //Serial.println(readFSR());
    //Serial.print("Ultrasound distance: ");
    //Serial.println(readUltrasound());
    
    update_screen(screenNumber);//If we are on the output screen then update the data we want




    //esp_sleep_enable_timer_wakeup(SLEEP_TIME); // ESP32 wakes up every 5 seconds
    //esp_wifi_stop();   // Required before entering sleep
    //Serial.println("Going to light-sleep now");
    //Serial.flush(); 
    //esp_light_sleep_start();
    //Serial.println(F("Woke up from sleep"));
    
    // We woke up from a sleep
    //esp_wifi_start();               // Required after waking up from sleep
    //wakeupReason();                 // Print the wakeup reason for ESP32   
  }
}

// Draw the screen depending on which page we are currently on
void draw_screen(uint8_t nr) {

  tft.fillScreen(ILI9341_BLACK);  

  // Draw the page title
  tft.setFont(&FreeSans12pt7b);
  tft.fillRect(1,25,TFT_SCREEN_X,3,ILI9341_GREEN);
  tft.setTextColor(ILI9341_GREEN,ILI9341_BLACK);
  tft.setCursor(1,20);
  switch (nr) {
    case 0:      
      tft.print(F("Outputs"));
      screenLine(1,0);  
      tft.printf("Test Piezos");
      screenLine(3,0);  
      tft.printf("Test LED");
      button.create(SCREEN_COLUMN3, 45,  110, BUTTON_HEIGHT,F("Play Sound"),BUTTON_NOT_PRESSED ,&playSound);
      button.create(SCREEN_COLUMN3, 95, 110, BUTTON_HEIGHT,F("Flash LED"),BUTTON_NOT_PRESSED ,&flashLED);

      break;

    case 1:
      tft.print(F("Inputs"));
      screenLine(1,0);                  tft.print("Battery Voltage");  
      //screenLine(1,SCREEN_COLUMN3);     tft.printf("%-5.2fV",readBatteryLevel());   
    
      screenLine(2,0);                  tft.print("Force Sensing");
      //screenLine(2,SCREEN_COLUMN3);     tft.printf("%4d",readFSR());
      
      screenLine(3,0);                  tft.print("Ultrasound Distance");
      //screenLine(3,SCREEN_COLUMN3);     tft.printf("%f",readUltrasound());

      screenLine(4,0);                  tft.print("Piezo Bump");
      //screenLine(4,SCREEN_COLUMN3);     tft.printf("%4d",readPiezo());
      break;
    default:
    //We should never get here but if we do the return to see if we can fix it.
      tft.print(F("Error !"));
      break;
  }
}
//A function that updates the screen for data that we want to continously update when that screen is shown
void update_screen(uint8_t nr) {
  switch(nr) {
    case 0://Currently screen 1 has only inputs so we arent interested in continously updating anythi
      return;
    case 1://Screen 1 has to FSR readings and the battery level readings so we want to update those
     //Update the values found
     screenLine(2,SCREEN_COLUMN3);     tft.printf("%4d",readFSR());
     screenLine(1,SCREEN_COLUMN3);     tft.printf("%-5.2fV",readBatteryLevel());
     screenLine(3,SCREEN_COLUMN3);     tft.printf("%5.2fcm",readUltrasound());
     screenLine(4,SCREEN_COLUMN3);     tft.printf("%4d",readPiezo());
     delay(500);
     tft.fillRect(SCREEN_COLUMN3, (SCREEN_HEADER), 110, 130, BLACK);//To clear the section of the screen that shows the data for the new data.
     break;
    default:
      Serial.println(F("Unexpected Screen Number"));
      return;
  }  
}

// Draw a line of information on the screen
void screenLine(unsigned int line, unsigned int column) {
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(column,(SCREEN_HEADER +(SCREEN_LINE * line)));
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
}

//Output a sound to the Piezo Buzzer
//4.6kHz is the Piezos resonant frequency
void playSound() {

  pinMode(PIEZO_SENSE,OUTPUT);//Ensure the sense piezo is set as an output before trying to drive it.
  
  Serial.println(F("Playing a sound"));
  //tone(PIEZO_SENSE,4600,500); //tone doesnt appear to be defined?
  /*
  resonant frequency of the Piezo is 4.6kHz +- 500Hz, therefor the delay will need to be 1/2*1/4.6kHz = 108us.
  Taken around 10us from delay to account for the delay from the instructions inbetween.
  */
  for(int j = 0; j < 10; j++) {
      for(int i = 0; i < 1000; i++) {
        digitalWrite(PIEZO_SENSE, HIGH);
    
        digitalWrite(PIEZO_BUZZ_1, HIGH);
        digitalWrite(PIEZO_BUZZ_2, LOW);
        
        delayMicroseconds(98);
        digitalWrite(PIEZO_SENSE, LOW);
    
        digitalWrite(PIEZO_BUZZ_1, LOW);
        digitalWrite(PIEZO_BUZZ_2, HIGH);
        
        delayMicroseconds(98);
        yield();
    }
  }
}

void flashLED() {
  Serial.println(F("Flashing the LED"));
  for(int i = 0; i < 10; i++) {
    digitalWrite(DIAG_LED, HIGH);
    delay(100);
    digitalWrite(DIAG_LED, LOW);
    delay(100);
  }
  digitalWrite(DIAG_LED, HIGH);//Turn LED Off again
}

//Read from the pin the supply voltage reading is on and return it.
float readBatteryLevel() {
  int adc;
  float inVoltage, result;
  Serial.println(F("Reading battery voltage"));
  adc = analogRead(DC_INPUT_VOLTAGE);
  inVoltage = mapFloat((float) adc,(float) ADC_MIN,(float) ADC_MAX,0.0,OP_VOLTAGE);//Map the 0-4095 adc reading to a 0-3.3v range
  result = VM_RATIO * (inVoltage) + VM_OFFSET;
  Serial.print(F("ADC result: "));
  Serial.print(adc);
  Serial.print(F(" | Involtage: "));
  Serial.print(inVoltage);
  Serial.print(F(" | Result: "));
  Serial.println(result);
  
  return result; //Calculate the supply voltage by reversing from the voltage divider
}

uint16_t readFSR() {
  Serial.println(F("Reading FSRs"));
  return analogRead(FORCE_SENSE);
}

//Read from the ultrasound sensor and return 
float readUltrasound() {

  float distance = 0.0;
  unsigned long deltaTime;
  uint8_t level;
  
  Serial.println(F("Reading from Ultrasound"));
  
  digitalWrite(DISTANCE_SENSE,HIGH);
  pinMode(DISTANCE_SENSE,OUTPUT);
  delayMicroseconds(3);//2us minimum, 5us typical pulser width, 3us chosen to account for intruction either side
  digitalWrite(DISTANCE_SENSE,LOW);
  pinMode(DISTANCE_SENSE,INPUT);
  delayMicroseconds(500);//We have to wait 750us for the Sonar to be sent. (From datasheet), has to be ready for low to high transition

  deltaTime = pulseIn(DISTANCE_SENSE, HIGH, 20000);//Returns how long the Distance sense goes high for, 20000us is the 20ms timeout the ultrasound module has

  distance = 100 * (deltaTime * 0.5) * SPEED_OF_SOUND;//t is the time for 2*d so we do t/2 * speed of sound given in m/us

  return distance;
}

//Read from Piezo
uint16_t readPiezo() {
  uint16_t adcVal;
  Serial.println(F("Reading from Piezo 1"));
  pinMode(PIEZO_SENSE,INPUT);//Set the piezo to be an input

  adcVal = analogRead(PIEZO_SENSE);

  if(adcVal > 250) {//If we hit the board with enough force then get the other piezo to buzz, test program to prove it works
      for(int i = 0; i < 10000; i++) {
        digitalWrite(PIEZO_BUZZ_1, HIGH);
        digitalWrite(PIEZO_BUZZ_2, LOW);
        
        delayMicroseconds(98);

        digitalWrite(PIEZO_BUZZ_1, LOW);
        digitalWrite(PIEZO_BUZZ_2, HIGH);
        
        delayMicroseconds(98);
        yield();
    }
  }

  return adcVal;
}

// Display the reason for waking up from sleep
void wakeupReason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO"); 
      break;
      
    case ESP_SLEEP_WAKEUP_EXT1: 
      Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
      break;
    
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer"); 
      break;
      
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Wakeup caused by touchpad"); 
      break;
    
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("Wakeup caused by ULP program");
      break;
    
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); 
      break;
  }
}

//Code for this function map taken from:
//https://forum.arduino.cc/index.php?topic=3922.0
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return ((x - in_min) * (out_max - out_min)) * (1 / ((in_max - in_min) + out_min));
}

void initialisePins() {
  
  pinMode(DC_INPUT_VOLTAGE,INPUT);        // Battery voltage at input pin - ADC 1/7
  pinMode(FORCE_SENSE,INPUT);             // Force sensing resistors
  pinMode(DISTANCE_SENSE,INPUT);          // Ultrasonic distance sensor
  
  pinMode(PIEZO_SENSE,OUTPUT);                  // PIEZO_SENSE buzzer, can also be used as "missed target" sensor
  
  pinMode(TFT_LED, OUTPUT);               // TFT backlight, must be on or you can't see the display
  pinMode(DIAG_LED, OUTPUT);              // Set the LED to an Output. Note its active low triggered.
  digitalWrite(DIAG_LED, HIGH);          // Drive the LED High to turn off as its active low

  pinMode(PIEZO_BUZZ_1, OUTPUT);
  pinMode(PIEZO_BUZZ_2, OUTPUT);  
  
  digitalWrite(TFT_LED, HIGH);
}

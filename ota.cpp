// Over the Air updates for ESP32
// There is a printf in here for logging, but no logging library implementation because its hard. Taking the overhead here ..

#include "ota.h"
#include "touch.h"


// Arduino Over the Air firmware updates - setup code
void OTA_Setup(void) {  
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(HOSTNAME);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);      
      // Update the TFT to show that we are not connected
      tft.setFont(&FreeSans9pt7b);
      tft.fillScreen(ILI9341_BLUE);
      tft.setTextColor(ILI9341_YELLOW,ILI9341_WHITE);
      tft.setCursor(0,20);
      tft.println(F("Firmware upgrade in progress."));
      tft.println(F("Do not turn off"));
      tft.drawRect(10,90,100,40,ILI9341_YELLOW);
      tft.setCursor(0,180);
    })
    
    .onEnd([]() {
      Serial.println("\nEnd");      
      tft.println(F("Completed"));
    })
    
    .onProgress([](unsigned int progress, unsigned int total) {
      int percentDone = progress / (total / 100);
      Serial.printf("Progress: %u%%\r", percentDone);
      tft.fillRect(10,90,percentDone,40,ILI9341_YELLOW);
    })
    
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      Serial.printf("Error[%u]: ", error);
      tft.print(F("Error : "));
      switch (error) {
        case OTA_AUTH_ERROR:
            Serial.println(F("Auth Failed"));
            tft.println(F("Auth Failed"));
            break;

        case OTA_BEGIN_ERROR:
            Serial.println(F("Begin Failed"));
            tft.println(F("Begin Failed"));
            break;

        case OTA_CONNECT_ERROR:
            Serial.println(F("Connect Failed"));
            tft.println(F("Connect Failed"));
            break;

        case OTA_RECEIVE_ERROR:
            Serial.println(F("Receive Failed"));
            tft.println(F("Receive Failed"));
            break;

        case OTA_END_ERROR:
            Serial.println(F("End Failed"));
            tft.println(F("End Failed"));
            break;

        default:
            Serial.println(F("Unknown error"));
            tft.println(F("Unknown error"));
            break;
      }
    });

  ArduinoOTA.begin();

  Serial.println(F("OTA upgrades enabled"));
  Serial.print(F("My IP address: "));
  Serial.println(WiFi.localIP());
}

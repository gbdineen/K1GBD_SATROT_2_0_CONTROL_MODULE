#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <ArduinoJson.h>
#include <iostream>
#include <functional>
#include <string>
#include <ArduinoOTA.h>

// Custom Classes
#include "WS_Server.h"
#include "ManualInputs.h"
#include "UDPControl.h"

// const char* ssid = "ATTG2FH57i";
// const char* password = "7bc?a?x+m8ku";
        
WS_Server wsServer;
WS_Server * wsServerPtr = &wsServer;
ManualInputs manualInputs(wsServerPtr);
ManualInputs * miPtr = &manualInputs;
UDPControl udpCtrl(wsServerPtr, miPtr);


void setup()  
{
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }

    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password);
    // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //     Serial.println("Connection Failed! Rebooting...");
    //     delay(5000);
    //     ESP.restart();
    // }

    // ArduinoOTA
    // .onStart([]() {
    //   String type;
    //   if (ArduinoOTA.getCommand() == U_FLASH)
    //     type = "sketch";
    //   else // U_SPIFFS
    //     type = "filesystem";

    //   // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //   Serial.println("Start updating " + type);
    // })
    // .onEnd([]() {
    //   Serial.println("\nEnd");
    // })
    // .onProgress([](unsigned int progress, unsigned int total) {
    //   Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    // })
    // .onError([](ota_error_t error) {
    //   Serial.printf("Error[%u]: ", error);
    //   if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //   else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //   else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //   else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //   else if (error == OTA_END_ERROR) Serial.println("End Failed");
    // });

    // ArduinoOTA.begin();

    wsServer.begin();
    manualInputs.begin();
    udpCtrl.begin();

}

void loop() 
{
    //ArduinoOTA.handle();
    wsServer.loop();
    manualInputs.loop();
}


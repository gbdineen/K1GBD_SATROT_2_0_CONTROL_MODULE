#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <ArduinoJson.h>
#include <iostream>
#include <functional>
#include <string>

// Custom Classes
#include "WS_Server.h"
#include "ManualInputs.h"
#include "UDPControl.h"
        
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

    wsServer.begin();
    manualInputs.begin();
    udpCtrl.begin();

}

void loop() 
{
   wsServer.loop();
   manualInputs.loop();
}


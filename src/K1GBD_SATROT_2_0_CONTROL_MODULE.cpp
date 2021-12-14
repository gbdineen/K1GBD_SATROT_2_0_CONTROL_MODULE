#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <ArduinoJson.h>
#include <iostream>

// Custom Classes
#include "WS_Server.h"
#include "ManualInputs.h"
        
WS_Server wsServer;
WS_Server * wsServerPtr = &wsServer;
ManualInputs manualInputs(wsServerPtr);


void setup()  
{
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }

    wsServer.initWebSocketServer();
    manualInputs.initRotaryEncoders();

}

void loop() 
{
   wsServer.webSocketLoop();
   manualInputs.encoderCheck();
}

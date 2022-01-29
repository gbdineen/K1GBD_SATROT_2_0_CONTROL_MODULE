// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebSocketsServer.h>
// #include "Adafruit_seesaw.h"
// #include <seesaw_neopixel.h>
// #include <ArduinoJson.h>
// #include <iostream>
// #include <functional>
// #include <string>
// #include <ArduinoOTA.h>
// #include <TFT_eSPI.h>

// Custom Classes
#include "WS_Server.h"
#include "ManualInputs.h"
#include "UDPControl.h"
#include "DisplayControl.h"

DisplayControl disCtrl;
DisplayControl * disCtrlPtr = &disCtrl;
WS_Server wsServer(disCtrlPtr);
WS_Server * wsServerPtr = &wsServer;
ManualInputs manualInputs(wsServerPtr,disCtrlPtr);
ManualInputs * miPtr = &manualInputs;
UDPControl udpCtrl(wsServerPtr, miPtr);


void setup()  
{
    Serial.begin(115200);
    // while (!Serial) {
    //     ; // wait for serial port to connect. Needed for native USB
    // }
    

    wsServer.begin();
    manualInputs.begin();
    udpCtrl.begin();
    disCtrl.begin();

}

void loop() 
{
    //ArduinoOTA.handle();
    wsServer.loop();
    manualInputs.loop();
}


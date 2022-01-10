#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <iostream>
#include <string>

class WS_Server
{
    private:
        
        WebSocketsServer webSocket = WebSocketsServer(81);

        const char* ssid = "Radiotron_Hub";
        const char* password = "YourPASS";
        
        void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);

        uint8_t systemCalibrated;
        std::function<void(int az, int el, int roll)> targetsCallback;

    public:
        
        WS_Server();
        ~WS_Server();
        void begin();
        void broadcastToClient(String tx);
        void setCalibrationStatus(uint8_t cs);
        bool getCalibrationStatus();
        void setTargetsCallback(std::function<void(int az, int el, int roll)> cb);
        void setTargets(int az, int el, int roll);
        void loop();

        
};

#endif


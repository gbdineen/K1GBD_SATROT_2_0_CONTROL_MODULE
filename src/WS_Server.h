#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <iostream>

class WS_Server
{
    private:
        
        WebSocketsServer webSocket = WebSocketsServer(81);

        const char* ssid = "Radiotron_Hub";
        const char* password = "YourPASS";
        
        void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);

    public:
        
        WS_Server();
        ~WS_Server();
        void initWebSocketServer();
        void webSocketLoop();
        void broadcastToClient(String tx);
        
};

#endif



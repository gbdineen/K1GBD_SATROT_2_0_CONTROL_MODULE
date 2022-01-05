#ifndef UDP_CONTROL_H
#define UDP_CONTROL_H

#include <Arduino.h>
#include <AsyncUDP.h>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>

class UDPControl
{
private:
    
    AsyncUDP udp;
    const int port = 9932;
    const int rele = 23;
    bool udpPingPong = false;
    bool pauseUDP = false ;
    
    //char * initkeps[];
    char *keps[8];
    char *ptr = NULL;

    void udpCheck();

public:
    
    UDPControl(/* args */);

    void begin();

    void parseUDP(AsyncUDPPacket packet);

    void loop();



    ~UDPControl();
};



#endif
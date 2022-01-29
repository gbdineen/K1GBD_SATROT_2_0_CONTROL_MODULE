#ifndef UDP_CONTROL_H
#define UDP_CONTROL_H

#include <Arduino.h>
#include <AsyncUDP.h>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include "WS_Server.h"
#include "ManualInputs.h"
#include "DisplayControl.h"

class UDPControl
{
private:
    
    AsyncUDP udp;
    WS_Server * ws;
    ManualInputs * mi;
    const int port = 9932;
    const int rele = 23;
    bool udpPingPong = false;
    bool pauseUDP = false ;
    bool udpActive;
    
    //char * initkeps[];
    char *keps[8];
    char *ptr = NULL;

    DisplayControl * dc;

    void udpCheck();

public:
    
    UDPControl(/* args */);
    UDPControl(WS_Server * ws, ManualInputs * mi);

    void begin();

    void parseUDP(AsyncUDPPacket packet);

    void loop();



    ~UDPControl();
};



#endif
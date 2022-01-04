#ifndef UDP_CONTROL_H
#define UDP_CONTROL_H

#include <Arduino.h>
#include <AsyncUDP.h>

class UDPControl
{
private:
    
    AsyncUDP udp;
    const int port = 9932;
    const int rele = 23;
    bool udpPingPong = false;
    bool pauseUDP = true;
    
    String keps[];

    void udpCheck();

public:
    
    UDPControl(/* args */);

    void begin();

    String parseUDP(AsyncUDPPacket packet);

    void loop();



    ~UDPControl();
};



#endif
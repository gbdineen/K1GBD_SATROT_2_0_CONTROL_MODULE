#include "UDPControl.h"


UDPControl::UDPControl(/* args */)
{
}

void UDPControl::begin()
{

}

void UDPControl::udpCheck()
{
  
  auto pfn = std::bind(&UDPControl::parseUDP, this, std::placeholders::_1);
  if (udp.listen(port)) {
    if (!pauseUDP) {
		udp.onPacket(pfn);
    //   udp.onPacket([&](AsyncUDPPacket packet) {
        
    //     if (packet.data()) {

	// 		parseUDP(packet);

    //         // udpPingPong = true;
    //         // //tracking=true;

    //         // byte index = 0;
    //         // char *initkeps = (char*) packet.data();
    //         // initkeps[packet.length()] = '\0'; 
    //         // ptr = strtok(initkeps, "[:,]");  // takes a list of delimiters
            
    //         // while (ptr != NULL) {
    //         //  // Serial.print("Keps: ");
    //         //  // Serial.println(keps[index]);
    //         //   keps[index] = ptr;
    //         //   index++;
    //         //   ptr = strtok(NULL, "[:,]");
    //         // }
    //         // // Serial.println("  ");
    //         // char *whatSat = keps[7];
    //         // uint8_t kepsAz = atof(keps[3]);
    //         // uint8_t  kepsEl = atof(keps[5]);
    //         // uint8_t  kepzAzAlt = 360-kepsAz;
    
    //         packet.flush();
    //     }
    //   });
    }
  } 
}

String UDPControl::parseUDP(AsyncUDPPacket packet)
{

	//udpPingPong = true;
	//tracking=true;

	byte index = 0;
	char *initkeps = (char*) packet.data();
	initkeps[packet.length()] = '\0'; 
	char * ptr = strtok(initkeps, "[:,]");  // takes a list of delimiters
	
	while (ptr != NULL) {
		// Serial.print("Keps: ");
		// Serial.println(keps[index]);
		keps[index] = ptr;
		index++;
		ptr = strtok(NULL, "[:,]");
	}
	// Serial.println("  ");
	// char *whatSat = keps[7];
	// uint8_t kepsAz = atof(keps[3]);
	// uint8_t  kepsEl = atof(keps[5]);
	// uint8_t  kepzAzAlt = 360-kepsAz;
  
}

void UDPControl::loop()
{

}



UDPControl::~UDPControl()
{
}
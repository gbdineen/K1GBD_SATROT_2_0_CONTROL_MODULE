#include "UDPControl.h"


UDPControl::UDPControl(/* args */)
{
	//std::cout << "UDPControl init" << std::endl;
}

void UDPControl::begin()
{
	//std::cout << "UDPControl begin()" << std::endl;
	udpCheck();
}

void UDPControl::udpCheck()
{
  
  ///std::cout << "UDPControl udpCheck()" << std::endl;
  
  auto pfn = std::bind(&UDPControl::parseUDP, this, std::placeholders::_1);
  if (udp.listen(port)) {
	  //std::cout << "UDPControl packet check" << std::endl;
    if (!pauseUDP) {
		udp.onPacket(pfn);
		// udp.onPacket([&](AsyncUDPPacket packet)
		// {
		// 	parseUDP(packet);
		// 	packet.flush();
		// });
    }
  } 
}

void UDPControl::parseUDP(AsyncUDPPacket packet)
{
	if (packet.data()) {
		//udpPingPong = true;
		//tracking=true;
		//std::cout << "UDPControl parseUDP()" << std::endl;
		byte index = 0;
		char *initkeps = (char*) packet.data();
		initkeps[packet.length()] = '\0'; 
		ptr = strtok(initkeps, "[:,]");  // takes a list of delimiters
		//Serial.print(initkeps);
		
		while (ptr != NULL) {
			Serial.print("Keps: ");
			Serial.println(keps[index]);
			keps[index] = ptr;
			index++;
			ptr = strtok(NULL, "[:,]");
		}
		// Serial.println("  ");
		// //String whatSat = keps[7];
		// // uint8_t kepsAz = atof(keps[5]);
		// // uint8_t  kepsEl = atof(keps[5]);
		// // uint8_t  kepzAzAlt = 360-kepsAz;

		// Serial.println("///////////////////////////");
		// Serial.print("UDP Packet Type: ");
		// Serial.println(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
		// Serial.print("From: ");
		// Serial.print(packet.remoteIP());
		// Serial.print(":");
		// Serial.println(packet.remotePort());
		// Serial.print("To: ");
		// Serial.print(packet.localIP());
		// Serial.print(":");
		// Serial.println(packet.localPort());
		// Serial.print("Length: ");
		// Serial.println(packet.length()); //dlzka packetu
		// Serial.print("Data: ");
		// Serial.write(packet.data(), packet.length());
		// Serial.println();
		
		//packet.flush();
	}
}

void UDPControl::loop()
{

}



UDPControl::~UDPControl()
{
}
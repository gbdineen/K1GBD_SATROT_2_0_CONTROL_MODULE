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
  //Serial.print(udp.listen(port));
  if (udp.listen(port)) {
	// 192.168.4.2:52367
  //if (udp.connect(IPAddress(192,168,4,2), 52367)) {
	  //std::cout << "UDPControl packet check" << std::endl;
    if (!pauseUDP) {
		//Serial.println("udp......");
		udp.onPacket(pfn);
		// udp.onPacket([&](AsyncUDPPacket packet)
		// {
		// 	parseUDP(packet);
		// 	Serial.println("onPacket");
		// 	// Serial.println(packet.length());
		// 	// packet.flush();
		// 	// Serial.println(packet.length());
		// });
    }
  } 
}

void UDPControl::parseUDP(AsyncUDPPacket packet)
{
	//std::cout << "UDPControl parseUDP()" << std::endl;
	if (packet.data()) {
		//udpPingPong = true;
		//tracking=true;
		//std::cout << "UDPControl parseUDP()" << std::endl;
		
		byte index = 0;
		char *initkeps = (char*) packet.data();
		// //uint8_t * initkeps = packet.data();
		initkeps[packet.length()] = '\0'; 
		ptr = strtok(initkeps, "[:,]");  // takes a list of delimiters
		// //Serial.println(initkeps);
		// // std::cout << packet.length() << std::endl;
		// // std::cout << packet.peek() << std::endl;
		// // std::cout << packet.read() << std::endl;
		// //std::cout << initkeps << std::endl;
		
		while (ptr != NULL) {
			Serial.print("Keps: ");
			Serial.println(keps[index]);
			keps[index] = ptr;
			index++;
			ptr = strtok(NULL, "[:,]");
		}
		
		// Serial.println("  ");
		// char * whatSat = keps[7];
		// float kepsAz = atof(keps[3]);
		// float  kepsEl = atof(keps[5]);
		// float  kepzAzAlt = 360-kepsAz;

		// Serial.print("Satellite: "); Serial.print(whatSat); Serial.print("\tAzimuth: "); Serial.print(kepsAz);
		// 	Serial.print("\tElevation: "); Serial.println(kepsEl);

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
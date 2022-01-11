#include "UDPControl.h"


UDPControl::UDPControl(/* args */)
{
	//std::cout << "UDPControl init" << std::endl;
}

UDPControl::UDPControl(WS_Server * ws, ManualInputs * mi)
{
	this->ws = ws;
	this->mi = mi;
}

void UDPControl::begin()
{
	//std::cout << "UDPControl begin()" << std::endl;
	udpCheck();
}

void UDPControl::udpCheck()
{
	
	  
	auto pfn = std::bind(&UDPControl::parseUDP, this, std::placeholders::_1);
	if (udp.listen(port))
	{
		if (!pauseUDP)
		{
			Serial.println("UDP Listneing");
			// if (ws->getCalibrationStatus())
			// {
				udp.onPacket(pfn);
			// /}
			
			// udp.onPacket([&](AsyncUDPPacket packet)
			// {
			// 	parseUDP(packet);
			// });
    	}
  	} 
}

void UDPControl::parseUDP(AsyncUDPPacket packet)
{
	Serial.println("Parsing UDP Packets");
	if (packet.data()) {
		if (ws->getCalibrationStatus())
		{
			//udpPingPong = true;
			//tracking=true;
			mi->setControlMethod(UPD);
			udpActive=true;
			byte index = 0;
			char *initkeps = (char*) packet.data();
			initkeps[packet.length()] = '\0'; 
			ptr = strtok(initkeps, "[:,]");  // takes a list of delimiters
			while (ptr != NULL) {
				// Serial.print("Keps: ");
				// Serial.println(keps[index]);
				keps[index] = ptr;
				index++;
				ptr = strtok(NULL, "[:,]");
			}
			
			char * whatSat = keps[7];
			float kepsAz = atof(keps[3]);
			float  kepsEl = atof(keps[5]);
			float  kepzAzAlt = 360-kepsAz;

			//Serial.print("Satellite: "); Serial.print(whatSat); Serial.print("\tAzimuth: "); Serial.print(kepsAz); Serial.print("\tElevation: "); Serial.println(kepsEl);

			
			// if (mi->getControlMethod()==AUTO)
			// {
				StaticJsonDocument<200> obj;
				obj["Subject"] = "udpcontrol";
				obj["Satellite"] = whatSat;
				obj["Azimuth"] = kepsAz;
				obj["Elevation"] = kepsEl;
				String str;
				serializeJson(obj, str);
			
				ws->broadcastToClient(str);
				Serial.println(str);
			//}

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
}

void UDPControl::loop()
{

}



UDPControl::~UDPControl()
{
}
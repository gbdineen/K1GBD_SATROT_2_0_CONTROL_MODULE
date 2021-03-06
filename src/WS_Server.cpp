#include "WS_Server.h"  


WS_Server::WS_Server()
{
    //std::cout << "WS_Server constructor init\n";
    //Serial.print("Serial Constructing");
}


void WS_Server::loop()
{
  webSocket.loop();
}

void WS_Server::begin()
{
  webSocket.close();
  //Serial.print(ssid); Serial.print(" "); Serial.print(password);
  WiFi.softAP(ssid, password);  
  while (! WiFi.softAPIP()) {
     Serial.print(".");
     delay(500); 
  }
  Serial.print("=> WS Init Step 1: WS Server created on IP "); Serial.println(WiFi.softAPIP());
  // Serial.println(WiFi.softAPIP());
  webSocket.begin();
  webSocket.broadcastPing();
  webSocket.onEvent([&](byte num, WStype_t t, uint8_t * p, size_t l) {
    webSocketEvent(num, t, p, l);
  });
  webSocket.enableHeartbeat(5000,3000,100);
}


void WS_Server::broadcastToClient(String tx)
{
  
  webSocket.broadcastTXT(tx);
}

void WS_Server::setCalibrationStatus(uint8_t cs)
{
	this->systemCalibrated = cs;
	Serial.print("\n=========== [Calibration status set: "); Serial.print(cs); Serial.print(" ]===========\n");
	//return cs;
}

bool WS_Server::getCalibrationStatus()
{

	if (this->systemCalibrated)
	{
		return true;
	}
	return false;
}

void WS_Server::setTargetsCallback(std::function<void(int az, int el, int roll)> cb)
{
    this->targetsCallback=cb;
}

void WS_Server::setPositionCallback(std::function<void(int az, int el, int roll)> cb)
{
    this->positionCallback=cb;
}

void WS_Server::setTargets(int az, int el, int roll)
{
	targetsCallback(az,el,roll);
}

void WS_Server::webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length)
{
	//webSocketEventLocal(num,type,payload,length);
	//Serial.print("WS_Client::webSocketEvent: "); Serial.println(type);
	StaticJsonDocument<200> obj;
	String str;
	switch (type) {
		case WStype_DISCONNECTED: // enum that read status this is used for debugging.
			//Serial.print("WS Type ");
			//Serial.print(type);
			//Serial.println(": DISCONNECTED");
			break;
		case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
			Serial.println("==> WS Init Step 2 : WebSocket Server connected to WS Client, Sending confirmation via broadcast");
			// obj["Subject"] = "client-connection-confirmed";
			
			// //String &s = encStr;
			// serializeJson(obj, str);
			// broadcastToClient(str); 	
			break;
		case WStype_TEXT: // check responce from client
			
			String pay = (char*) payload;
			Serial.println(pay);

			DeserializationError error = deserializeJson(obj, payload);
			String subject = obj["Subject"];

			if (subject == "client-connection-confirmed")
			{
				Serial.print("====> WS Init Step 4 : WS Client JSON broadcast received. Returning confirmation via JSON broadcast.");
				
				obj["Subject"] = "server-connection-confirmed";
				serializeJson(obj,str);
				broadcastToClient(str);
			}
			else if (subject=="system-calibration-status")
			{
				setCalibrationStatus(obj["Calibration-Status"]);
			}
			else if (subject=="previous-targets")
			{
				Serial.print("\n----- [Setting Targets 3] -----]\n");
				
				this->targetsCallback(obj["Azimuth"],obj["Elevation"],obj["Roll"]);
			}
			else if (subject=="current-position")
			{
				this->positionCallback(obj["Azimuth"],obj["Elevation"],obj["Roll"]);
			}
			
			
			break;
	}
}

WS_Server::~WS_Server(){};
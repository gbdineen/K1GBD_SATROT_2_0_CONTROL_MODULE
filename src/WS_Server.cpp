#include "WS_Server.h"  


WS_Server::WS_Server()
{
    std::cout << "WS_Server constructor init\n";
    //Serial.print("Serial Constructing");
}


void WS_Server::loop()
{
  webSocket.loop();
}

void WS_Server::begin()
{
  webSocket.close();
  Serial.println("initWebSocketServer");
  //Serial.print(ssid); Serial.print(" "); Serial.print(password);
  WiFi.softAP(ssid, password);  
  while (! WiFi.softAPIP()) {
     Serial.print(".");
     delay(500); 
  }
  // Serial.println();
  // Serial.print("IP address: ");
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


void WS_Server::webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length)
{
  //webSocketEventLocal(num,type,payload,length);
  //Serial.print("WS_Client::webSocketEvent: "); Serial.println(type);

  switch (type) {
    case WStype_DISCONNECTED: // enum that read status this is used for debugging.
      //Serial.print("WS Type ");
      //Serial.print(type);
      //Serial.println(": DISCONNECTED");
      break;
    case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
      {

        //Serial.print("WS Type "); Serial.print(type); 
        //Serial.println(": CLIENT CONNECTED");
        //webSocket.broadcastTXT("CONNECTED TO SERVER");
        //broadcastToClient("WS_Server ->  CONNECTED");

        StaticJsonDocument<200> encObj;
        encObj["Subject"] = "clientconnected";
        String encStr;
        //String &s = encStr;
        serializeJson(encObj, encStr);
        broadcastToClient(encStr); 

      }
      break;
    case WStype_TEXT: // check responce from client
      //Serial.print("WS Type "); Serial.println(type);
      // String pay = String((char*) payload);
      // Serial.println(pay);
      // broadcastToClient("WS_Server -> TEXT RECEIEVED") ;
      break;
  }
}

WS_Server::~WS_Server(){};
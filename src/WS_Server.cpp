#include "WS_Server.h"  


WS_Server::WS_Server()
{
    std::cout << "WS_Server constructor init\n";
    //Serial.print("Serial Constructing");
}


void WS_Server::webSocketLoop()
{
  webSocket.loop();
}

void WS_Server::initWebSocketServer()
{
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
  webSocket.onEvent([&](byte num, WStype_t t, uint8_t * p, size_t l) {
    webSocketEvent(num, t, p, l);
  });
  webSocket.enableHeartbeat(5000,3000,100);
}


void WS_Server::broadcastToClient(String tx)
{
  //Serial.print("WS_Server::sndTXT "); Serial.println(tx);
  //Serial.print("WS_Server webSocket.broadcastTXT(tx) "); Serial.println(webSocket.broadcastTXT(tx));
  webSocket.broadcastTXT(tx);
}

void WS_Server::webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length)
{
  //webSocketEventLocal(num,type,payload,length);
  switch (type) {
    case WStype_DISCONNECTED: // enum that read status this is used for debugging.
      //Serial.print("WS Type ");
      //Serial.print(type);
      //Serial.println(": DISCONNECTED");
      break;
    case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
      //Serial.print("WS Type "); Serial.print(type); 
      //Serial.println(": CLIENT CONNECTED");
      //webSocket.broadcastTXT("CONNECTED TO SERVER");
      broadcastToClient("WS_Server ->  CONNECTED");
      break;
    case WStype_TEXT: // check responce from client
      //Serial.print("WS Type "); Serial.println(type);
      String pay = String((char*) payload);
      Serial.println(pay);
      broadcastToClient("WS_Server -> TEXT RECEIEVED") ;
      break;
  }
}

WS_Server::~WS_Server(){};
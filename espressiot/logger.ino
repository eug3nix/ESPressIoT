//
// ESPressIoT Controller for Espresso Machines
// 2016 by Eugene Dubnin
//
// TCP status logging
//

#ifdef ENABLE_TCP_STATUS
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#define MAX_SRV_CLIENTS 1
WiFiServer tcpServer(23);
WiFiClient tcpClients[MAX_SRV_CLIENTS];

void setupTCPStatus() {
  tcpServer.begin();
  tcpServer.setNoDelay(true);
}

void loopTCPStatus() {
  uint8_t i;
  //check if there are any new clients
  if (tcpServer.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!tcpClients[i] || !tcpClients[i].connected()){
        if(tcpClients[i]) tcpClients[i].stop();
        tcpClients[i] = tcpServer.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = tcpServer.available();
    serverClient.stop();
  }
}

void TCPStatus() {
  uint8_t i;
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (tcpClients[i] && tcpClients[i].connected()){
      char outstr[150];
      formatStatus(outstr);
      tcpClients[i].print(outstr);
    }
  }
}

#endif

void serialStatus() {
  char outstr[150];
  formatStatus(outstr);
  Serial.print(outstr);
}

void formatStatus(char* outstr) {
  sprintf(outstr, "%s %s %s %s %s %s %s %s %s\n",
  String(gInputTemp,2).c_str(),
  String(gTargetTemp,2).c_str(),
  String(gOutputPwr,2).c_str(),
  String(gP,2).c_str(),
  String(gI,2).c_str(),
  String(gD,2).c_str(),
  String(ESPPID.GetKp(),2).c_str(),
  String(ESPPID.GetKi(),2).c_str(),
  String(ESPPID.GetKd(),2).c_str());
}
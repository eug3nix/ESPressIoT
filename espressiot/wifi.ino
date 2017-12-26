//
// ESPressIoT Controller for Espresso Machines
// 2017 by Eugene Dubinin
//
// Wifi init and fallback to AP mode
//

IPAddress local_IP(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

void setupWiFiAP()
{
  WiFi.mode(WIFI_AP);

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String ESSID_STRING = "ESPressIoT " + macID;

  char ESSID_Char[ESSID_STRING.length() + 1];
  memset(ESSID_Char, 0, ESSID_STRING.length() + 1);

  for (int i=0; i<ESSID_STRING.length(); i++)
    ESSID_Char[i] = ESSID_STRING.charAt(i);

  WiFi.softAP(ESSID_Char);
  Serial.print("\nSetting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("IP address: ");
  Serial.println(local_IP.toString());
}

void setupWifi()
{
  WiFi.begin(gWifiSSID.c_str(), gWifiPassword.c_str());
  Serial.print("\nMAC address: ");
  Serial.println(WiFi.macAddress());
  
  int attempts = 10;
  while (WiFi.status() != WL_CONNECTED && attempts-- > 0) {
    delay(1000);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    setupWiFiAP();
  }
}

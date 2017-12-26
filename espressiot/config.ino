//
// ESPressIoT Controller for Espresso Machines
// 2016 by Roman Schmitz
//
// JSON Config File on SPIFFS
//

#ifdef ENABLE_JSON

#include <ArduinoJson.h>
#include "FS.h"

#define BUF_SIZE 1024

bool prepareFS() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return false;
  }
  return true;
}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<BUF_SIZE> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  //wifi_ssid = json["ssid"];
  //wifi_pass = json["password"];
  gTargetTemp = json["tset"];
  gBrewSeconds = json["brew_seconds"];
  gOvershoot = json["tband"];
  gP = json["P"], gI = json["I"], gD = json["D"];
  gaP = json["aP"], gaI = json["aI"], gaD = json["aD"];

  return true;
}

bool saveConfig() {
  StaticJsonBuffer<BUF_SIZE> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  //json["ssid"] = wifi_ssid;  json["password"] = wifi_pass;
  json["tset"] = gTargetTemp;  json["tband"] = gOvershoot;
  json["brew_seconds"] = gBrewSeconds;
  json["P"] = gP, json["I"] = gI, json["D"] = gD;
  json["aP"] = gaP, json["aI"] = gaI, json["aD"] = gaD;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

void resetConfig() {
 gP=S_P; gI=S_I; gD=S_D;
 gaP=S_aP; gaI=S_aI; gaD=S_aD;
 gTargetTemp=S_TSET;
 gBrewSeconds=S_BREW_SECONDS;
 gOvershoot=S_TBAND;
}

#endif

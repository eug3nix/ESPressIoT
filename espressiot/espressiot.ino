//
// ESPressIoT Controller for Espresso Machines
// 2016 by Roman Schmitz
//
// Uses PID library
//

#include <PID_v1.h>
#include <ESP8266WiFi.h>

// WIFI

#define WIFI_SSID "welcomeHome"
#define WIFI_PASS "lkbyysq123!"

// options for special modules
#define ENABLE_JSON
#define ENABLE_HTTP
// #define ENABLE_MQTT
#define ENABLE_DISPLAY
// #define ENABLE_TCP_STATUS

// use simulation or real heater and sensors
#define SIMULATION_MODE

// uncommend one of the lines below to select a proper temperature sensor
// #define TEMP_SENSOR_DS18B20
// #define TEMP_SENSOR_TSIC

//
// STANDARD reset values based on Gaggia CC
//
#define S_P 115.0
#define S_I 4.0
#define S_D 850.0
#define S_aP 100.0
#define S_aI 0.0
#define S_aD 0.0
#define S_TSET 94.0
#define S_TBAND 1.5

//
// Intervals for I/O
//
#define HEATER_INTERVAL 1000
#define DISPLAY_INTERVAL 1000
#define PID_INTERVAL 200

//
// global variables
//
double gTargetTemp=S_TSET;
double gOvershoot=S_TBAND;
double gInputTemp=20.0;
double gOutputPwr=0.0;
double gP = S_P, gI = S_I, gD = S_D;
double gaP = S_aP, gaI = S_aI, gaD = S_aD;

unsigned long time_now=0;
unsigned long time_last=0;

boolean tuning = false;
boolean osmode = false;
boolean poweroffMode = false;

//
// gloabl classes
//
PID ESPPID(&gInputTemp, &gOutputPwr, &gTargetTemp, gP, gI, gD, DIRECT);

void setup()
{
  gOutputPwr=0;

  Serial.begin(115200);

  #ifdef ENABLE_DISPLAY
  setupDisplay();
  #endif

  Serial.println("Mounting SPIFFS...");
  if(!prepareFS()) {
    Serial.println("Failed to mount SPIFFS !");
  } else {
    Serial.println("Mounted.");
  }

  /*if (!saveConfig()) {
    Serial.println("Failed to save config");
  } else {
    Serial.println("Config saved");
  }*/

  Serial.println("Loading config...");
  if (!loadConfig()) {
    Serial.println("Failed to load config. Using default values and creating config...");
    if (!saveConfig()) {
     Serial.println("Failed to save config");
    } else {
      Serial.println("Config saved");
    }
  } else {
    Serial.println("Config loaded");
  }
   
  Serial.println("Settin up PID...");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  #ifdef ENABLE_HTTP
  setupWebSrv();
  #endif
  
  #ifdef ENABLE_MQTT
  setupMQTT();
  #endif

  #ifdef ENABLE_TCP_STATUS
  setupTCPStatus();
  #endif


  // setup components
  setupHeater();
  setupSensor();

  // start PID
  ESPPID.SetTunings(gP, gI, gD);
  ESPPID.SetSampleTime(PID_INTERVAL);
  ESPPID.SetOutputLimits(0, 1000);
  ESPPID.SetMode(AUTOMATIC);
 
  time_now=millis();
  time_last=time_now;
    
}

void loop() {
  time_now=millis();
  
  updateTempSensor(); 
  gInputTemp=getTemp();

  if(abs(time_now-time_last)>=PID_INTERVAL or time_last > time_now) {
    if(poweroffMode==true) {
      gOutputPwr=0;
      setHeatPowerPercentage(0);
    }
    else if(tuning==true)
    {
      tuning_loop();
    }
    else  {
      if( !osmode && abs(gTargetTemp-gInputTemp)>=gOvershoot ) {        
        ESPPID.SetTunings(gaP, gaI, gaD);
        osmode=true;
      }
      else if( osmode && abs(gTargetTemp-gInputTemp)<gOvershoot ) {
        ESPPID.SetTunings(gP,gI,gD);
        osmode=false;
      }
      if(ESPPID.Compute()==true) {   
        setHeatPowerPercentage(gOutputPwr);
      }
    }        
    
    #ifdef ENABLE_MQTT
    loopMQTT();
    #endif
    
    serialStatus();

    #ifdef ENABLE_DISPLAY
    displayStatus();
    #endif

    #ifdef ENABLE_TCP_STATUS
    TCPStatus();
    #endif

    time_last=time_now;
  }

  updateHeater();
  
  #ifdef ENABLE_HTTP
  loopWebSrv();
  #endif 

  #ifdef ENABLE_TCP_STATUS
  loopTCPStatus();
  #endif
}


//
// ESPressIoT Controller for Espresso Machines
// 2016 by Eugene Dubinin
//
// MAXIM DS18B20 interface
//

#ifndef SIMULATION_MODE
#ifdef TEMP_SENSOR_DS18B20

#include <OneWire.h>
#include <DallasTemperature.h>

// pins for power and signal
#define DS_SIG D1 // D0 is used as 1wire signal pin

#define DS18B20_RESOLUTION 10
#define DS_WAIT_MS 750/(12 - DS18B20_RESOLUTION)

OneWire oneWire(D1);
DallasTemperature DS18B20(&oneWire);
unsigned long lastSensTime;

void setupSensor() {  
  lastSensTime=millis();
  DS18B20.begin();
  DS18B20.setResolution(DS18B20_RESOLUTION);
  DS18B20.setWaitForConversion(false);
}

void updateTempSensor() {
  if (abs(millis() - lastSensTime) >= DS_WAIT_MS) {
    DS18B20.requestTemperatures();
  }
}

float getTemp() {
  DS18B20.getTempCByIndex(0);
}

#endif
#endif

//
// ESPressIoT Controller for Espresso Machines
// 2017 by Eugene Dubnin
//
// Pump and Solenoid Valve relay control functions
//

#define PUMP_RELAY_PIN D7
#define VALVE_RELAY_PIN D8

void setupRelays() {
  pinMode(PUMP_RELAY_PIN , OUTPUT);
  pinMode(VALVE_RELAY_PIN , OUTPUT);
}

void pumpOn() {
   digitalWrite(PUMP_RELAY_PIN, 1); //turn pin high 
}

void pumpOff() {
   digitalWrite(PUMP_RELAY_PIN, 0); //turn pin low
}

void valveOn() {
   digitalWrite(VALVE_RELAY_PIN, 1); //turn pin high 
}

void valveOff() {
   digitalWrite(VALVE_RELAY_PIN, 0); //turn pin low
}
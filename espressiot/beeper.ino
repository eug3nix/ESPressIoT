//
// ESPressIoT Controller for Espresso Machines
// 2017 by Eugene Dubnin
//
// Button handling routines
//

// D9 is actually USB-UART RX pin
#define BEEPER_PIN D9

// resonant frequency sounds loudest
#define BEEPER_FREQ 2730


void beep() {
  tone(D9, BEEPER_FREQ, 250);
  
}

void click() {
  tone(D9, BEEPER_FREQ, 6);
}

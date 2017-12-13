//
// ESPressIoT Controller for Espresso Machines
// 2016 by Eugene Dubnin
//
// Button handling routines
//

#include "OneButton.h"

#define PWR_BUTTON_PIN D8
#define MODE_BUTTON_PIN D7
#define TOGGLE_BUTTON_PIN D6

// setup buttons: pin, is it active low?

// D8 pin has a pulldown on NodeMCU thus the inverted button state
OneButton btnPower(PWR_BUTTON_PIN, false);
OneButton btnMode(MODE_BUTTON_PIN, true);
OneButton btnToggle(TOGGLE_BUTTON_PIN, true);

void setupButtons() {
  btnPower.attachClick(btnPowerClick);
  btnPower.attachLongPressStart(btnPowerLongPressStart);

  btnMode.attachClick(btnModeClick);
  btnMode.attachLongPressStart(btnModeLongPressStart);

  btnToggle.attachClick(btnToggleClick);
}


void btnPowerClick() {
  Serial.println("Power click");
}

void btnPowerLongPressStart() {
  Serial.println("Power longpress");
}

void btnModeClick() {
  Serial.println("Mode click");
}

void btnModeLongPressStart() {
  Serial.println("Mode longpress");
}

void btnToggleClick() {
  Serial.println("Toggle click");
}


void loopButtons() {
  btnPower.tick();
  btnMode.tick();
  btnToggle.tick();
}

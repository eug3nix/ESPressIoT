//
// ESPressIoT Controller for Espresso Machines
// 2017 by Eugene Dubnin
//
// Button handling routines
//

#include "OneButton.h"

#define PWR_BUTTON_PIN D5
#define MODE_BUTTON_PIN D2
#define TOGGLE_BUTTON_PIN D0

// setup buttons: pin, is it active low?

// D8 pin has a pulldown on NodeMCU thus the inverted button state
OneButton btnPower(PWR_BUTTON_PIN, false);
OneButton btnMode(MODE_BUTTON_PIN, false);
OneButton btnToggle(TOGGLE_BUTTON_PIN, false);

void setupButtons() {
  btnPower.attachClick(btnPowerClick);
  btnPower.attachLongPressStart(btnPowerLongPressStart);

  btnMode.setClickTicks(300);
  btnMode.attachClick(btnModeClick);
  btnMode.attachLongPressStart(btnModeLongPressStart);

  btnMode.setClickTicks(300);
  btnToggle.attachClick(btnToggleClick);
}


void btnPowerClick() {
  fsm.trigger(EVENT_BTN_PWR_CLICK);
}

void btnPowerLongPressStart() {
  fsm.trigger(EVENT_BTN_PWR_LONGPRESS);
}

void btnModeClick() {
  if(!poweroffMode) {
    click();
  }

  fsm.trigger(EVENT_BTN_MODE_CLICK);
}

void btnModeLongPressStart() {
  fsm.trigger(EVENT_BTN_MODE_LONGPRESS);
}

void btnToggleClick() {
  if(!poweroffMode) {
    click();
  }

  fsm.trigger(EVENT_BTN_TOGGLE_CLICK);
}

void loopButtons() {
  btnPower.tick();
  btnMode.tick();
  btnToggle.tick();
}

//
// ESPressIoT Controller for Espresso Machines
// 2017 by Eugene Dubinin
//
// Coffeemaker Finite State Machine
//

// timeouts for steam->coffee_idle auto fallback and auto-poweroff
#define STEAMING_TIMEOUT 1000 * 300
#define POWEROFF_TIMEOUT 1000 * 1800

// coffe idle state handlers
void coffeeIdleEnter() {
  gTargetTemp=S_TSET;
}

void coffeeIdleRun() {
  displayCoffeeIdleScreen();
}

// coffee brewing state handlers
unsigned long brewStartTime;

void coffeeBrewingEnter() {
  brewStartTime = millis();
  valveOn();
  pumpOn();
}

void coffeeBrewingRun() {
  unsigned long timeInBrewing = millis() - brewStartTime;
  int secondsLeft = gBrewSeconds - timeInBrewing/1000;
  if (secondsLeft == 0) {
    fsm.trigger(EVENT_TIMEOUT_BREW);
    return;
  }
  displayCoffeeBrewingScreen(secondsLeft);
}

void coffeeBrewingExit() {
  pumpOff();
  valveOff();
}

// steaming state handlers
void steamingEnter() {
  gTargetTemp=S_STEAM_TSET;
}

void steamingRun() {
  displaySteamingScreen();
}

void steamingExit() {
}

void sleepEnter() {
  poweroffMode = true;
  display.setContrast(0);
}

void sleepRun() {
  displayScreensaver();
}

void sleepExit() {
  poweroffMode = false;
  display.setContrast(255);
}

void setupFSM() {  
  // add transitions between states, fromState, toState, triggeringEvent
  // sleep to coffee idle and back on power button click/longpress
  fsm.add_transition(&stateSleep, &stateCoffeeIdle, EVENT_BTN_PWR_CLICK, NULL);
  fsm.add_transition(&stateCoffeeIdle, &stateSleep, EVENT_BTN_PWR_LONGPRESS, NULL);

  // coffe idle to brewing and back on toggle button click
  fsm.add_transition(&stateCoffeeIdle, &stateCoffeeBrewing, EVENT_BTN_TOGGLE_CLICK, NULL);
  fsm.add_transition(&stateCoffeeBrewing, &stateCoffeeIdle, EVENT_BTN_TOGGLE_CLICK, NULL);

  // stop coffee brewing on timeout condition (when brew is ended)
  fsm.add_transition(&stateCoffeeBrewing, &stateCoffeeIdle, EVENT_TIMEOUT_BREW, NULL);

  // allow powering off from any state
  fsm.add_transition(&stateCoffeeBrewing, &stateSleep, EVENT_BTN_PWR_LONGPRESS, NULL);
  fsm.add_transition(&stateSteaming, &stateSleep, EVENT_BTN_PWR_LONGPRESS, NULL);

  // coffe to steaming back and forth switch on mode button click
  fsm.add_transition(&stateCoffeeIdle, &stateSteaming, EVENT_BTN_MODE_CLICK, NULL);
  fsm.add_transition(&stateSteaming, &stateCoffeeIdle, EVENT_BTN_MODE_CLICK, NULL);

  // revert back to coffee idle in 5 minutes
  fsm.add_timed_transition(&stateSteaming, &stateCoffeeIdle, STEAMING_TIMEOUT, NULL);
  fsm.add_timed_transition(&stateCoffeeIdle, &stateSleep, POWEROFF_TIMEOUT, NULL);
}


// needed for timed state transitions
void loopFSM() {
  fsm.run_machine();
}
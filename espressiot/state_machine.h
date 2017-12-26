#include "Fsm.h"

void coffeeIdleEnter(void);
void coffeeIdleRun(void);

void coffeeBrewingEnter(void);
void coffeeBrewingRun(void);
void coffeeBrewingExit(void);

void steamingEnter(void);
void steamingRun(void);
void steamingExit(void);

void sleepEnter(void);
void sleepRun(void);
void sleepExit(void);

// fsm state definition with on_enter, in_state and on_exit handlers
State stateSleep(&sleepEnter, &sleepRun, &sleepExit);
State stateCoffeeIdle(&coffeeIdleEnter, &coffeeIdleRun, NULL);
State stateCoffeeBrewing(&coffeeBrewingEnter, &coffeeBrewingRun, &coffeeBrewingExit);
State stateSteaming(&steamingEnter, &steamingRun, &steamingExit);

// default fsm state is powerOff
Fsm fsm(&stateSleep);

// fsm event defitions
#define EVENT_BTN_PWR_CLICK 1
#define EVENT_BTN_PWR_LONGPRESS 2
#define EVENT_BTN_MODE_CLICK 3
#define EVENT_BTN_MODE_LONGPRESS 4
#define EVENT_BTN_TOGGLE_CLICK 5
#define EVENT_TIMEOUT_BREW 6
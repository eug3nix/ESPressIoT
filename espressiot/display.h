#include "fonts.h"
#include "images.h"

#include <Wire.h> 
#include "SH1106.h"
#include "OLEDDisplayUi.h"

#define DISPLAY_SDA_PIN D3
#define DISPLAY_SCK_PIN D4

SH1106 display(0x3c, DISPLAY_SDA_PIN, DISPLAY_SCK_PIN);
OLEDDisplayUi ui     ( &display );

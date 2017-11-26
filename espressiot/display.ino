#ifdef ENABLE_DISPLAY

#include "fonts.h"
#include "images.h"

#include <Wire.h> 
#include "SSD1306.h"
#include "OLEDDisplayUi.h"

#define DISPLAY_SDA_PIN D3
#define DISPLAY_SCK_PIN D5

SSD1306  display(0x3c, DISPLAY_SDA_PIN, DISPLAY_SCK_PIN);
OLEDDisplayUi ui     ( &display );

void setupDisplay() {
  display.init();
  display.flipScreenVertically();
  // display.setContrast(255);
}

void drawTempWidget(int x, int y, double inputTemp, double targetTemp ) {
  char intemp1[4];
  char intemp2[3];
  char ttemp[8];

  sprintf(intemp1, "%d.",(int)inputTemp);
  sprintf(intemp2, "%01d",(int)(inputTemp*10)%10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(Nimbus_Sans_L_Regular_Condensed_32);
  display.drawString(x-8, y-8, intemp1);
  display.drawString(x+2, y-8, "°");

  sprintf(ttemp, "sv %d°",(int)targetTemp);
  display.setFont(Nimbus_Sans_L_Regular_Condensed_16);
  display.drawString(x, y+7, intemp2);
  display.drawString(x, y+24, ttemp);
}


void displayStatus() {
  display.clear();
  char degstr[12];
  display.drawXbm(0, 16, cup_width, cup_height, cup_bits);
  drawTempWidget(128, 16, gInputTemp, gTargetTemp);
  display.display();
}

#endif
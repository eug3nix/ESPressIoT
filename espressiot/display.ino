#ifdef ENABLE_DISPLAY

void setupDisplay() {
  display.init();
  display.flipScreenVertically();
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

void displayCoffeeIdleScreen() {
  display.clear();
  display.drawXbm(0, 16, cup_width, cup_height, cup_bits);
  drawTempWidget(128, 16, gInputTemp, gTargetTemp);
  display.display();
}

void displayCoffeeBrewingScreen(int seconds) {
  display.clear();
  display.drawXbm(0, 16, cup_width, cup_height, cup_bits);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Nimbus_Sans_L_Regular_Condensed_16);
  display.setColor(BLACK);
  display.drawString(24, 34, String(seconds));
  display.setColor(WHITE);
  drawTempWidget(128, 16, gInputTemp, gTargetTemp);
  display.display();
}


void displaySteamingScreen() {
  display.clear();
  display.drawXbm(0, 16, steam_width, steam_height, steam_bits);
  drawTempWidget(128, 16, gInputTemp, gTargetTemp);
  display.display();
}

void displayScreensaver() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Nimbus_Sans_L_Regular_Condensed_32);
  display.drawString(40, 10, "Zzz...");
  display.display();
}

#endif
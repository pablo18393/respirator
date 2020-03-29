void drawAxis() {
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, 320, width_heading, COLOR_HEADING);
  tft.fillRect(120 - width_space / 2, 0, width_space, width_heading, COLOR_LINE);
  tft.fillRect(0, yOffset, xAxis, axisWidth, WHITE);
  tft.fillRect(xOffset, 60, axisWidth, yAxis, WHITE);
  tft.setTextColor(WHITE);
  tftdrawStringWrap("t", 228, 295, 4);
  tftdrawStringWrap("P", 2, 40, 4);
  tft.setTextColor(BLACK);
  tftdrawStringWrap("Pmax:", xOffset, 5, 4);
  tftdrawStringWrap("Pmin:", xOffset + 120 + width_space, 5, 4);
  drawPIP();
}

void drawPIP() {
  for (int i = 0; i < xAxis; i++) {
    if (stripLineCount >= 0) {
      tft.drawPixel(i + xOffset, yOffset - 10 * (PIPpressure / scale), WHITE); //because data will come in mmH2O we multiply by 10
    }
    stripLineCount++;
    if (stripLineCount == stripLineDensity) {
      stripLineCount = -stripLineDensity;
    }
  }
}

void drawCMVselector (bool selector) {
  int color;
  if (CMV == 2) {
    color = GREEN;
  }
  else {
    color = BLACK;
  }
  tft.fillRect(120 + width_space / 2, width_heading - 1, 120 - width_space / 2, (320 - width_heading) / rectangles, color);
  if (CMV == 2) {
    color = BLACK;
  }
  else {
    color = GREEN;
  }
  tft.fillRect(width_select, width_heading - 1, 120 - width_space / 2 - width_select - 1, (320 - width_heading) / rectangles, color);
  tft.setTextColor(WHITE);
  tftdrawStringWrap("CMV", 30, ypos, 4);
  tftdrawStringWrap("A-CMV", 150, ypos, 4);
}

void tftdrawCentreStringWrap(char *string, int poX, int poY, int size) {
  tft.setTextSize(1);
  if ( size == 4 ) {
    tft.setFont(&FreeSans12pt7b);
  }
  else if ( size == 2 ) {
    tft.setFont(&FreeSans9pt7b);
  }
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(string, 0, 0, &x1, &y1, &w, &h); //calc width of new string
  tft.setCursor(poX - w / 2, poY + 16);
  tft.setTextSize(1);
  tft.println(string);
}
void tftdrawStringWrap(char *string, int poX, int poY, int size) {
  tft.setTextSize(1);
  if ( size == 4 ) {
    tft.setFont(&FreeSans12pt7b);
  }
  else if ( size == 2 ) {

    tft.setFont(&FreeSans9pt7b);
  }
  tft.setCursor(poX, poY + 16);
  tft.println(string);
}

void tftdrawRightStringWrap(char *string, int poX, int poY, int size) {
  tft.setTextSize(1);
  if ( size == 4 ) {
    tft.setFont(&FreeSans12pt7b);
  }
  else if ( size == 2 ) {
    tft.setFont(&FreeSans9pt7b);
  }
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(string, 0, 0, &x1, &y1, &w, &h); //calc width of new string

  tft.setCursor(poX - w - 8, poY + 16);

  tft.println(string);
}


void tftdrawNumberWrap(int num, int poX, int poY, int size) {
  char string[10];
  sprintf(string, "%ld", num);
  tft.setTextSize(1);
  if ( size == 4 ) {
    tft.setFont(&FreeSans12pt7b);
  }
  else if ( size == 8 ) {
    tft.setTextSize(2);
    tft.setFont(&FreeSans18pt7b);
  }
  else if ( size == 2 ) {
    tft.setFont(&FreeSans9pt7b);
  }
  tft.setCursor(poX, poY + 16);
  tft.println(string);
}

void iwdg_feed() {
  Watchdog.reset();
}

void tftsetTextColorWrapper(int color) {
  tft.setTextColor(color);
  txt_color = color;
}


void drawCentreNumber(int n, byte x, int i) {
  length = 1;
  for (long k = 10; k <= n; k *= 10) {
    length++;
  }
  if (text_size == 2) {
    tft.setTextSize(1);
    tftdrawNumberWrap(n, x - length * 13, i, 2);
  }
  else if (text_size == 8) {
    tft.setTextSize(1);
    tftdrawNumberWrap(n, x - length * 13, i, 8);
  }
  else {
    tft.setTextSize(1);
    tftdrawNumberWrap(n, x - length * text_size * 7, i, 4);
  }
  tft.setTextSize(1);
}

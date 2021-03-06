void menu() {
  if (CMV == 1) {
    page = 1;
  }
  else {
    page = 2;
  }
  tft.setTextSize(1);
  tftsetTextColorWrapper(ILI9341_WHITE);
  for (int i = 0; i <= 6; i++) {
    pos_text[i] = 0;
  }
  pos_text[0] = 1;

  words[0]  = "";
  words[1] = "PIP PRESS";
  words[2] = "OSC RATE";
  words[3] = "VOLUME";
  words[4] = "E/I";
  words[5] = "";
  if (CMV == 2) {
    words[5] = "P-TRIGGER";
  }
  if (CMV == 1) {
    rectangles = 6;
    words[5] = "ALARM SETTINGS";
    pos_text[5] = 1;
  }
  else {
    words[6] = "ALARM SETTINGS";
    pos_text[6] = 1;
    rectangles = 7;
  }
  drawRectangles();
  drawHeading();
  while (PUSHED) {
    updateData();
  }
  delay(100);
  selectMode();
}

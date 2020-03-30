void alarmSettings() {
  page = 3;
  tft.setTextSize(1);
  tftsetTextColorWrapper(ILI9341_WHITE);
  for (int i = 0; i <= 6; i++) {
    pos_text[i] = 0;
  }
  pos_text[3] = 1;

  words[0]  = "MAX OSC RATE";
  words[1] = "MIN VOL";
  words[2] = "MAX PIP";
  words[3] = "OK";
  rectangles = 4;

  drawRectangles();
  drawHeading();
  while (PUSHED) {
    updateData();
  }
  delay(100);
  selectMode();
}

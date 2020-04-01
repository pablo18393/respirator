void alarm(byte alarmCode, int alarmData) {
  int pageRetainer = page;
  bool displayAlarmData = 0;
  int unitXPos = 0;
  displayNumber = 0;
  alarmText[0] = "";
  alarmText[1] = "";
  alarmText[2] = "";
  page = 5;
  tft.fillScreen(BLACK);
  tftsetTextColorWrapper(WHITE);
  tft.setTextSize(10);
  switch (alarmCode) {
    case PressureAlarm:
      alarmText[0] = "EXCESIVE";
      alarmText[1] = "PRESSURE";
      alarmText[2] = "cmH2O";
      unitXPos = 155;
      displayNumber = 1;
      break;
    case OscRateAlarm:
      alarmText[0] = "ABNORMAL";
      alarmText[1] = "OSCILATION RATE";
      alarmText[2] = "Resp/min";
      unitXPos = 165;
      displayNumber = 1;
      break;
    case CCalarm:
      alarmText[0] = "INSPIRATION";
      alarmText[1] = "VOLUME";
      alarmText[2] = "cc";
      unitXPos = 140;
      displayNumber = 1;
      break;
    case OverloadAlarm:
      alarmText[0] = "SYSTEM";
      alarmText[1] = "OVERLOAD";
      break;
    case PowerSupplyAlarm:
      alarmText[0] = "POWER SUPPLY";
      alarmText[1] = "FAILURE";
      break;
    case AbnormalPressureAlarm:
      alarmText[0] = "ABNORMAL";
      alarmText[1] = "PRESSURE";
      break;

  }
  tft.setTextColor(WHITE);
  tftdrawCentreStringWrap("PUSH TO RESUME", 120, 290, 4);
  tft.setTextColor(LIGHT_ORANGE);
  tftdrawCentreStringWrap("ALARM", 120, 50, 4);
  if (displayNumber) {
    drawRightNumber(alarmData, 108, 210);
    tftdrawCentreStringWrap(alarmText[2], unitXPos, 210, 4);
  }
  while (PUSHED) {
    blinkDisclaimer();
    updateData();
  }
  delay(100);
  while (NOT_PUSHED) {
    blinkDisclaimer();
    updateData();
  }
  delay(100);
  while (PUSHED) {
    blinkDisclaimer();
    updateData();
  }
  delay(100);
  switch (pageRetainer) {
    case 1:
    case 2:
      menu();
      break;
    case 3:
      alarmSettings();
      break;
    case 4:
      last_something = graphicWait;
      graphic();
      break;
    default:
      menu();
      break;
  }
}

void blinkDisclaimer() {
  long blinkTime = 500;
  if (millis() - lastDisclaimerBlink > blinkTime) {
    lastDisclaimerBlink = millis();
    if (blinkState) {
      tft.setTextColor(LIGHT_ORANGE);
    }
    else {
      tft.setTextColor(BLACK);
    }
    displayDisclaimer();
    blinkState = !blinkState;
  }
}

void displayDisclaimer() {
  tftdrawCentreStringWrap(alarmText[0], 120, 150, 4);
  if (strlen (alarmText[1])) {
    tftdrawCentreStringWrap(alarmText[1], 120, 180, 4);
  }
}

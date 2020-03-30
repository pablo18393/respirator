void graphic() {
  int maxValue, minValue;
  graphicPos = 0;
  page = 0;
  drawAxis();
  resetValues();
  SerialX_empty();
  while ((millis() - last_something) > graphicWait) {
    updateData();
    if (read_pressure()) {
      if (inputBuffer[graphicPos] > (yRes * scale)) {
        inputBuffer[graphicPos] = yRes * scale;
      }
      erasePixel();
      //graphicBuffer[graphicPos] = 160 * sin(angle / 6.28) + 160;
      //angle++;
      graphicBuffer[graphicPos] = inputBuffer[graphicPos];
      if (newGraphicData) {
        drawPixel();
        if (graphicPos >= graphicX - 1) {
          graphicPos = 0;
          drawPeaks();
          drawPIP();
          assignDrawedBefore();
        }
        else {
          graphicPos++;
        }
      }
    }
  }
  menu();
}

void drawPeaks() {
  int maxValue = checkMaxPeak();
  int minValue = checkMinPeak();
  tft.setTextColor(WHITE);
  drawRightNumber(maxData / 10, 100, 5); //mm to cm
  maxData = maxValue;
  tft.setTextColor(LIGHT_ORANGE);
  drawRightNumber(maxData / 10, 100, 5); //mm to cm
  tft.setTextColor(WHITE);
  drawRightNumber(minData / 10, 215, 5); //mm to cm
  minData = minValue;
  tft.setTextColor(LIGHT_ORANGE);
  drawRightNumber(minData / 10, 215, 5); //mm to cm
}

void assignDrawedBefore() {
  for (int i = 0; i < 215; i++) {
    graphicBefore[i] = graphicBuffer[i];
  }
}

void erasePixel() {
  /*
    tft.drawPixel(xOffset + axisWidth + graphicPos + 1, yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, BLACK);
    tft.drawPixel(xOffset + axisWidth + graphicPos + 1, 1 + yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, BLACK);
    tft.drawPixel(xOffset + axisWidth + graphicPos + 1, 2 + yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, BLACK);
  */
  tft.drawLine(xOffset + axisWidth + graphicPos + 2, yOffset - axisWidth - (graphicBefore[graphicPos]) / scale, xOffset + axisWidth + graphicPos + 3, yOffset - axisWidth - (graphicBefore[graphicPos + 1]) / scale, BLACK);
}

void drawPixel() {
  /*
    tft.drawPixel(xOffset + axisWidth + graphicPos + 1, yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, WHITE);
    tft.drawPixel(xOffset + axisWidth + graphicPos + 1, 1 + yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, WHITE);
    tft.drawPixel(xOffset + axisWidth + graphicPos + 1, 2 + yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, WHITE);
  */
  if (graphicPos) {
    tft.drawLine(xOffset + axisWidth + graphicPos + 1, yOffset - axisWidth - (graphicBuffer[graphicPos - 1]) / scale, xOffset + axisWidth + graphicPos + 2, yOffset - axisWidth - (graphicBuffer[graphicPos]) / scale, WHITE);
  }
}

void resetValues() {
  for (int i = 0; i < 215; i++) {
    graphicBuffer[i] = 0;
    inputBuffer[i] = 0;
  }
  graphicPos = 0;
  graph_index = 0;
}

int checkMaxPeak() {
  int maxValue;
  for (int i = 0; i < 215; i++) {
    maxValue = max(maxValue, graphicBuffer[i]);
  }
  return maxValue;
}

int checkMinPeak() {
  int minValue = 999;
  for (int i = 0; i < 215; i++) {
    minValue = min(minValue, graphicBuffer[i]);
  }
  return minValue;
}

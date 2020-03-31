void graphic() {
  int maxValue, minValue;
  graphicPos = 0;
  page = 0;
  drawAxis();
  resetValues();
  SerialX_empty();
  while ((millis() - last_something) > graphicWait) {
    updateData();
    switch (readDriverInput()) {
      case 1:
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
        break;
      case 2:
        deleteDrawed();
        break;
      case 3:
        updateVolume();
        break;
      case 4:
        updateOscRate();
        break;
    }
  }
  menu();
}

void updateVolume() {
  tft.setTextColor(WHITE);
  drawRightNumber(volumeDetectedBefore, 120, 5 + width_heading); //mm to cm
  tft.setTextColor(LIGHT_ORANGE);
  drawRightNumber(volumeDetected, 120, 5 + width_heading); //mm to cm
  volumeDetectedBefore = volumeDetected;
}

void updateOscRate() {
  tft.setTextColor(WHITE);
  drawRightNumber(respminDetectedBefore, 235, 5 + width_heading); //mm to cm
  tft.setTextColor(LIGHT_ORANGE);
  drawRightNumber(respminDetected, 235, 5 + width_heading); //mm to cm
  respminDetectedBefore = respminDetected;
}

void deleteDrawed() {
  drawPeaks();
  assignDrawedBefore();
  for (int i = 0; i < 215; i++) {
    graphicPos = i;
    erasePixel();
  }
  resetValues();
}

void drawPeaks() {
  int maxValue = checkMaxPeak();
  int minValue = checkMinPeak();
  tft.setTextColor(WHITE);
  drawRightNumber(Pmax / 10, 120, 5); //mm to cm
  Pmax = maxValue;
  tft.setTextColor(LIGHT_ORANGE);
  drawRightNumber(Pmax / 10, 120, 5); //mm to cm
  tft.setTextColor(WHITE);
  drawRightNumber(Pmin / 10, 235, 5); //mm to cm
  Pmin = minValue;
  tft.setTextColor(LIGHT_ORANGE);
  drawRightNumber(Pmin / 10, 235, 5); //mm to cm
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
  if (graphicPos > 1) {
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
  Start_capture = 0;
}

int checkMaxPeak() {
  int maxValue;
  for (int i = 0; i < graphicPos; i++) {
    maxValue = max(maxValue, graphicBuffer[i]);
  }
  return maxValue;
}

int checkMinPeak() {
  int minValue = 999;
  for (int i = 0; i < graphicPos; i++) {
    minValue = min(minValue, graphicBuffer[i]);
  }
  return minValue;
}

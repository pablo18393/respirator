void drawRectangles() {
  /*Se dibuja la separación rectangular de la pantalla*/
  tft.fillScreen(BLACK);
  tft.fillRect(width_select, width_heading, 240 - width_select, 320 - width_heading, COLOR_MENU);
  if (print_text && page != 21 && page != 10 && page != 28) {
    if ( scrolling_down ) {
      tft.fillRect(0, (320 - width_heading) * (bar_pos - 1) / rectangles + width_heading, width_select, (320 - width_heading) / rectangles, COLOR_SELECTED);
    }
    else {
      tft.fillRect(0, width_heading, width_select, (320 - width_heading) / rectangles, COLOR_SELECTED);
    }
  }
  for (int i = 2; i <= rectangles; i++) {
    tft.fillRect(0, (320 - width_heading) * (i - 1) / rectangles + width_heading - 1, 240, width_space, COLOR_LINE);
  }
  if (page == 1 || page == 2) {
    tft.fillRect(120 - width_space / 2, width_heading - 1, width_space, (320 - width_heading) / rectangles, COLOR_LINE);
  }
  tft.drawRect(0, 319, width_select, 319, COLOR_MENU);
  bar_offset = 0;
  /*Se rellenan las cuadriculas con texto, valores y simbolos */
  if (print_text) {
    /*Se escribe el texto de las variables words[] */
    tft.setTextSize(1);
    tftsetTextColorWrapper(WHITE);
    text_size = 1;
    for (int i = 0; i < rectangles; i++) {
      tftsetTextColorWrapper(WHITE);
      ypos = (320 - width_heading) / (2 * rectangles) + i * (320 - width_heading) / (rectangles) + letter_height;
      byte text_size = 2;
      text_size = 2;
      if (pos_text[i + bar_offset] == 0) {
        tftdrawStringWrap(words[i + bar_offset], width_select + 1, ypos, text_size);
      }
      else if (pos_text[i + bar_offset] == 1) {
        tftdrawCentreStringWrap(words[i + bar_offset], width_select + (240 - width_select) / 2, ypos, text_size);
      }
      //ypos = (320 - width_heading) / (2 * rectangles) + (i + bar_offset) * (320 - width_heading) / (rectangles) + letter_height;
      /*Aprovechando el mismo bucle que escribe el texto, y tomando como referencia de posicion la variable ypos, se rellena el resto de la información, que es según en qué página estemos (variable page) */
      switch (page) {
        case 1:
          switch (i) {
            case 0:
              drawCMVselector(CMV);
              tftdrawStringWrap("CMV", 30, ypos, 4);
              tftdrawStringWrap("A-CMV", 150, ypos, 4);
              break;
            case 1:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("cmH2O", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(PIPpressure, PIPpressureXPos, ypos);
              break;
            case 2:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("Resp/min", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(oscRate, oscRateXPos, ypos);
              break;
            case 3:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("cc", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(cc, ccXPos, ypos);
              break;
            case 4:
              tft.setTextColor(LIGHT_ORANGE);
              tftdrawRightStringWrap("1/", 220, ypos, 4);
              drawRightNumber(expRatio, expRatioXPos, ypos);
              break;
          }
          break;
        case 2:
          switch (i) {
            case 0:
              drawCMVselector(CMV);
              tftdrawStringWrap("CMV", 30, ypos, 4);
              tftdrawStringWrap("A-CMV", 150, ypos, 4);
              break;
            case 1:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("cmH2O", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(PIPpressure, PIPpressureXPos, ypos);
              break;
            case 2:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("Resp/min", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(oscRate, oscRateXPos, ypos);
              break;
            case 3:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("cc", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(cc, ccXPos, ypos);
              break;
            case 4:
              tft.setTextColor(LIGHT_ORANGE);
              tftdrawRightStringWrap("1/", 220, ypos, 4);
              drawRightNumber(expRatio, expRatioXPos, ypos);
              break;
            case 5:
              tft.setTextColor(WHITE);
              tftdrawRightStringWrap("cmH2O", 240, ypos, 2);
              tft.setTextColor(LIGHT_ORANGE);
              drawRightNumber(PTrigger, PTriggerXPos, ypos);
              break;
          }
          break;
      }
    }
  }
}

void drawHeading() {
  for (int i = 0; i <= 1; i++) {
    tft.fillRect(0, 0, 320, width_heading, COLOR_HEADING);
    draw_poweroff();
  }
  battery_cleared = 1;
  int_battery_cleared = 1;
  drawBattery();
}

void clearMenu() {
  tft.fillRect(0, width_heading, 240, 320 - width_heading, BLACK);
}

void drawBack() {
  tft.fillRect(0, 0, width_back, width_heading, COLOR_HEADING);
  // tft.drawRect(0, 0, width_back, width_heading, BLACK);
  tft.fillTriangle(arrow_height, width_heading / 2, width_back / 2 , arrow_height, width_back / 2, width_heading - arrow_height, COLOR_ARROW);
  tft.fillRect(width_back / 2, width_heading / 2 - arrow_tail, width_back / 2 - arrow_height, arrow_tail, COLOR_ARROW);
  tft.fillRect(width_back / 2, width_heading / 2, width_back / 2 - arrow_height, arrow_tail, COLOR_ARROW);
}

void drawRightNumber(int n, byte x, int i) {
  byte offset0 = 0;
  length = 1;
  if ( n >= 0 ) {
    for (long k = 10; k <= n; k *= 10) {
      length++;
    }
  }
  else {
    for (long k = 10; k <= -n; k *= 10) {
      length++;
    }
    offset0 = 7;
  }

  tftdrawNumberWrap(n, x - length * 14 - offset0, i, 4);
}

void eraseRightNumber(int n, byte x, int i) {
  byte offset0 = 0;
  tftsetTextColorWrapper(BLACK);
  length = 1;
  if ( n >= 0 ) {
    for (long k = 10; k <= n; k *= 10) {
      length++;
    }

  }
  else {
    for (long k = 10; k <= -n; k *= 10) {
      length++;
    }
    offset0 = 7;
  }
  tftdrawNumberWrap(n, x - length * 14 - offset0, i, 4);
  tftsetTextColorWrapper(WHITE);
}

void drawPos() {
  long pos_color;
  if ( page == 4 || page == 6 ) {
    printTLinfo(COLOR_MSG);
    tft.drawFastHLine((240 - width_select) / 5, width_heading + 320 * 3 / (rectangles * 6), 160, WHITE);
    tft.fillCircle((240 - width_select) / 5, width_heading + 320 * 3 / (rectangles * 6), triang_pos, WHITE);
    tft.fillCircle((240 - width_select) / 5 + 160, width_heading + 320 * 3 / (rectangles * 6), triang_pos, WHITE);
    tft.fillTriangle((240 - width_select) / 5 + 80, width_heading + 320 * 3 / (rectangles * 6) - triang_pos, (240 - width_select) / 5 + 80, width_heading + 320 * 3 / (rectangles * 6) + triang_pos, (240 - width_select) / 5 + 80 + triang_pos, width_heading + 320 * 3 / (rectangles * 6), WHITE);
    if (!keep_tl) {
      tft.fillCircle((240 - width_select) / 5 + 160, width_heading + 320 * 3 / (rectangles * 6), triang_pos - 1, BLACK);
      tft.fillCircle((240 - width_select) / 5, width_heading + 320 * 3 / (rectangles * 6), triang_pos - 1, BLACK);
    }
  }

  if ( page == 3) {
    tft.drawFastHLine((240 - width_select) / 5 - 10, width_heading + 320 * 3 / (rectangles * 6) + 5, 180, WHITE);
    tft.fillCircle((240 - width_select) / 5 - 10, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos, WHITE);
    tft.fillCircle((240 - width_select) / 5 + 80, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos, WHITE);
    tft.fillCircle((240 - width_select) / 5 + 170, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos, WHITE);
    tft.fillTriangle((240 - width_select) / 5 + 35, width_heading + 320 * 3 / (rectangles * 6) - triang_pos + 5, (240 - width_select) / 5 + 35, width_heading + 320 * 3 / (rectangles * 6) + triang_pos + 5, (240 - width_select) / 5 + 35 + triang_pos, width_heading + 320 * 3 / (rectangles * 6) + 5, WHITE);
    tft.fillTriangle((240 - width_select) / 5 + 125, width_heading + 320 * 3 / (rectangles * 6) - triang_pos + 5, (240 - width_select) / 5 + 125, width_heading + 320 * 3 / (rectangles * 6) + triang_pos + 5, (240 - width_select) / 5 + 125 + triang_pos, width_heading + 320 * 3 / (rectangles * 6) + 5, WHITE);
#ifdef CASTELLANO
    tftdrawStringWrap("V1", (240 - width_select) / 5 + 35 - 4, width_heading + 320 * 3 / (rectangles * 6) - triang_pos - 12 - 1 , 2);
    tftdrawStringWrap("V2", (240 - width_select) / 5 + 125 - 4, width_heading + 320 * 3 / (rectangles * 6) - triang_pos - 12 - 1 , 2);
#else
    tftdrawStringWrap("S1", (240 - width_select) / 5 + 35 - 4, width_heading + 320 * 3 / (rectangles * 6) - triang_pos - 12 - 1 , 2);
    tftdrawStringWrap("S2", (240 - width_select) / 5 + 125 - 4, width_heading + 320 * 3 / (rectangles * 6) - triang_pos - 12 - 1 , 2);
#endif
    if (keep_am) {
      if (info_tramos[1][0] == 0 && info_tramos[1][1] == 0 && info_tramos[1][2] == 0) {
        tft.fillCircle((240 - width_select) / 5 + 170, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos - 1, BLACK);
      }
    }
    else {
      tft.fillCircle((240 - width_select) / 5 + 170, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos - 1, BLACK);
      tft.fillCircle((240 - width_select) / 5 + 80, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos - 1, BLACK);
      tft.fillCircle((240 - width_select) / 5 - 10, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos - 1, BLACK);
    }
  }
}

void printTLinfo(int color) {
  tftsetTextColorWrapper(color);
  stops = (tl_data[0] * 60 + tl_data[1]) * clip_fps;
  if ( page == 4 ) {
    tftdrawNumberWrap(stops, (240 - width_select) / 5 + 115, width_heading + 5 + 320 / (rectangles), 4);
  }
  else {
    tftdrawNumberWrap(stops, (240 - width_select) / 5 + 83, width_heading + 5 + 320 / (rectangles), 4);
  }

  if ( page == 4) {
    hours = (tl_data[2] * 60 + tl_data[3]) * stops / 3600;
    minutes = (tl_data[2] * 60 + tl_data[3]) * stops / 60 - hours * 60;
    if (hours < 10) {
      tftdrawStringWrap("0", (240 - width_select) / 5 - 5, width_heading + 5 + 320 / (rectangles), 4);
      tftdrawNumberWrap(hours, (240 - width_select) / 5 + 9, width_heading + 5 + 320 / (rectangles), 4);
    }
    else {
      tftdrawNumberWrap(hours, (240 - width_select) / 5 - 5, width_heading + 5 + 320 / (rectangles), 4);
    }
    if ( minutes > 9 ) {
      tftdrawNumberWrap(minutes, (240 - width_select) / 5 + 34, width_heading + 5 + 320 / (rectangles), 4);
    }
    else {
      tftdrawStringWrap("0", (240 - width_select) / 5 + 34, width_heading + 5 + 320 / (rectangles), 4);
      tftdrawNumberWrap(minutes, (240 - width_select) / 5 + 48, width_heading + 5 + 320 / (rectangles), 4);
    }
    tftdrawStringWrap("h", (240 - width_select) / 5 + 22, width_heading + 5 + 320 / (rectangles), 2);
    tftdrawStringWrap("m", (240 - width_select) / 5 + 61, width_heading + 5 + 320 / (rectangles), 2);

  }
}


void drawRespective(byte c) {
  unsigned int respectiveColor;
  if (c == 1) {
    respectiveColor = BLACK;
  }
  else {
    respectiveColor = WHITE;
  }
  if (page != 3 && page != 22 && page != 25 && page != 26) {
    if (first_draw) {
      tftsetTextColorWrapper(BLACK);
      tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
      tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
      travelIcon(BLACK, 1);
      panIcon(BLACK, 1);
      tiltIcon(BLACK, 1);
      first_draw = 0;
      if (sequence < 4) {
        tft.fillCircle((240 - width_select) / 5 + 160, width_heading + 320 * 3 / (rectangles * 6), triang_pos - 1, BLACK);
      }
    }
    if (sequence < 4) {
      tft.fillCircle((240 - width_select) / 5, width_heading + 320 * 3 / (rectangles * 6), triang_pos - 1, respectiveColor);
    }
    else {
      tft.fillCircle((240 - width_select) / 5 + 160, width_heading + 320 * 3 / (rectangles * 6), triang_pos - 1, respectiveColor);
    }
    tftsetTextColorWrapper(ILI9341_WHITE);
    switch (sequence) {
      case 1:
        printTLinfo(BLACK);
        eraseShot();
        eraseClock();
        tftsetTextColorWrapper(ILI9341_WHITE);
        tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        travelIcon(WHITE, 1);
        break;
      case 2:
        tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        panIcon(WHITE, 1);
        break;
      case 3:
        tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        tiltIcon(WHITE, 1);
        break;
      case 4:
        tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        travelIcon(WHITE, 1);
        break;
      case 5:
        tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        panIcon(WHITE, 1);
        break;
      case 6:
        tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        tiltIcon(WHITE, 1);
        break;
    }
  }
  else if ( page != 22 && page != 25 && page != 26) {
    if (first_draw) {
      tftsetTextColorWrapper(BLACK);
      tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
      tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
      tftdrawCentreStringWrap("3", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
      travelIcon(BLACK, 1);
      panIcon(BLACK, 1);
      tiltIcon(BLACK, 1);
      first_draw = 0;
      if ( sequence < 7 ) {
        Circle(170, BLACK);
        if (sequence < 4) {
          Circle(80, BLACK);
        }
      }
    }
    tftsetTextColorWrapper(ILI9341_WHITE);
    switch (sequence) {
      case 1:
        tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        travelIcon(WHITE, 1);
        break;
      case 2:
        tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        panIcon(WHITE, 1);
        break;
      case 3:
        tftdrawCentreStringWrap("1", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        tiltIcon(WHITE, 1);
        break;
      case 4:
        tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        travelIcon(WHITE, 1);
        break;
      case 5:
        tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        panIcon(WHITE, 1);
        break;
      case 6:
        tftdrawCentreStringWrap("2", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        tiltIcon(WHITE, 1);
        break;
      case 7:
        tftdrawCentreStringWrap("3", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        travelIcon(WHITE, 1);
        break;
      case 8:
        tftdrawCentreStringWrap("3", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        panIcon(WHITE, 1);
        break;
      case 9:
        tftdrawCentreStringWrap("3", 150, width_heading + 10 + 320 / (rectangles) - 10, 4);
        tiltIcon(WHITE, 1);
        break;
    }
    if (sequence < 4) {
      Circle(-10, respectiveColor);
    }
    else if (sequence < 7) {
      Circle(80, respectiveColor);
    }
    else {
      Circle(170, respectiveColor);
    }
  }
  else if (page != 26) {
    if (c == 2) {
      tft.fillCircle(120, 130, 16, respectiveColor);
    }
    tft.drawCircle(120, 130, 16, respectiveColor);
  }
}


void loadLogo(const uint8_t *bitmap) {
  int16_t i, j, byteWidth = (112 + 7) / 8;
  uint8_t byte_0;
  for (j = 0; j < 35; j++) {
    for (i = 0; i < 112; i++) {
      if (i & 7) byte_0 <<= 1;
      else      byte_0   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if (byte_0 & 0x80) {
        tft.drawPixel(8 + i, 120 + j, WHITE);
        tft.drawPixel(231 - i, 154 - j, WHITE);
      }
    }
  }
}


void drawBattery() {

}


void drawNextScreenArrow(int y_pos) {
  tft.drawLine(210, y_pos, 220, y_pos, COLOR_MSG);
  tft.drawLine(220, y_pos, 220, y_pos + 10, COLOR_MSG);
  tft.fillTriangle(215, y_pos + 10, 225, y_pos + 10, 220, y_pos + 16, COLOR_MSG);
}

void drawClock() {
  if (page == 4) {
    tft.fillCircle(25, width_heading + 55, 10, COLOR_MSG);
    tft.fillCircle(25, width_heading + 55, 8, BLACK);
    tft.drawLine(25, width_heading + 55, 25, width_heading + 53, COLOR_MSG);
    tft.drawLine(25, width_heading + 55, 29, width_heading + 56, COLOR_MSG);
  }
}

void eraseClock() {
  if (page == 4) {
    tft.fillCircle(27, width_heading + 55, 12, BLACK);
  }
}

void drawShot(bool erase, byte xy_pos) {
  unsigned int shot_color = COLOR_MSG;
  if (erase) {
    shot_color = BLACK;
  }
  int centro_x = 145;
  int centro_y = width_heading + 55;
  if ( page == 6) {
    centro_x = 110;
    centro_y = width_heading + 66;
  }
  tft.fillCircle(centro_x, centro_y, 10, shot_color);
  tft.fillCircle(centro_x, centro_y, 8, BLACK);
  tft.fillCircle(centro_x, centro_y, 3, shot_color);
  tft.drawLine(centro_x, centro_y + 3, centro_x - 8, centro_y + 3, shot_color);
  tft.drawLine(centro_x, centro_y - 3, centro_x + 8, centro_y - 3, shot_color);
  tft.drawLine(centro_x - 3, centro_y, centro_x - 3, centro_y - 8, shot_color);
  tft.drawLine(centro_x + 3, centro_y, centro_x + 3, centro_y + 8, shot_color);
  tft.drawLine(centro_x - 2, centro_y - 2, centro_x + 3, centro_y - 9, shot_color);
  tft.drawLine(centro_x - 2, centro_y + 2, centro_x - 9, centro_y - 3, shot_color);
  tft.drawLine(centro_x + 2, centro_y + 2, centro_x - 3, centro_y + 9, shot_color);
  tft.drawLine(centro_x + 2, centro_y - 2, centro_x + 9, centro_y + 3, shot_color);
}

void eraseShot() {
  drawShot(1, 0);
}

void travelIcon(int color, byte position) {
  int displacement_x = 0;
  int displacement_y = 0;
  if (position == 2) {
    displacement_y = 215;
    displacement_x = -60;
  }
  tft.drawFastHLine((240 - width_select) / 5 + 42 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, 40, color);
  tft.fillTriangle((240 - width_select) / 5 + 42 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, (240 - width_select) / 5 + 42 + triang + displacement_x, width_heading + 10 + 320 / (rectangles) - triang + displacement_y, (240 - width_select) / 5 + 42 + triang + displacement_x, width_heading + 10 + 320 / (rectangles) + triang + displacement_y, color);
  tft.fillTriangle((240 - width_select) / 5 + 82 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, (240 - width_select) / 5 + 82 - triang + displacement_x, width_heading + 10 + 320 / (rectangles) - triang + displacement_y, (240 - width_select) / 5 + 82 - triang + displacement_x, width_heading + 10 + 320 / (rectangles) + triang + displacement_y, color);
}

void panIcon(int color, byte position) {
  int displacement_x = 0;
  int displacement_y = 0;
  if (position == 2) {
    displacement_y = 215;
    displacement_x = 10;
  }
  for (int i = 0; i < 8; i++) {
    tft.drawPixel((240 - width_select) / 5 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) - 16 + i * 2 + displacement_y, color);
  }
  for (int i = 0; i < 4; i++) {
    tft.drawPixel((240 - width_select) / 5 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) + 10 + i * 2 + displacement_y, color);
  }
  tft.drawCircle((240 - width_select) / 5 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, radius, color);
  tft.fillTriangle((240 - width_select) / 5 - triang + 60 - radius + displacement_x, width_heading + 10 + 320 / (rectangles) + triang / 2 + displacement_y, (240 - width_select) / 5 + triang + 60 - radius + displacement_x, width_heading + 10 + 320 / (rectangles) + triang / 2 + displacement_y, (240 - width_select) / 5 + 60 - radius + displacement_x, width_heading + 10 + 320 / (rectangles) - triang / 2 + displacement_y, color);
  tft.fillTriangle((240 - width_select) / 5 + triang + 60 + radius + displacement_x, width_heading + 10 + 320 / (rectangles) - triang / 2 + displacement_y, (240 - width_select) / 5 - triang + 60 + radius + displacement_x, width_heading + 10 + 320 / (rectangles) - triang / 2 + displacement_y, (240 - width_select) / 5 + 60 + radius + displacement_x, width_heading + 10 + 320 / (rectangles) + triang / 2 + displacement_y, color);
}

void tiltIcon(int color, byte position) {
  int displacement_x = 0;
  int displacement_y = 0;
  if (position == 2) {
    displacement_y = 215;
    displacement_x = 80;
  }
  tft.drawCircle((240 - width_select) / 5 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, radius, color);
  for (int i = 0; i < 8; i++) {
    tft.drawPixel((240 - width_select) / 5 + 60 + i * 2 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, color);
  }
  for (int i = 0; i < 4; i++) {
    tft.drawPixel((240 - width_select) / 5 + 60 - radius - i * 2 + displacement_x, width_heading + 10 + 320 / (rectangles) + displacement_y, color);
  }
  tft.fillTriangle((240 - width_select) / 5 - triang / 2 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) - triang - radius + displacement_y, (240 - width_select) / 5 + 60 - triang / 2 + displacement_x, width_heading + 10 + 320 / (rectangles) + triang - radius + displacement_y, (240 - width_select) / 5 + 60 + triang / 2 + displacement_x, width_heading + 10 + 320 / (rectangles) - radius + displacement_y, color);
  tft.fillTriangle((240 - width_select) / 5 + triang / 2 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) + triang + radius + displacement_y, (240 - width_select) / 5 + triang / 2 + 60 + displacement_x, width_heading + 10 + 320 / (rectangles) - triang + radius + displacement_y, (240 - width_select) / 5 + 60 - triang / 2 + displacement_x, width_heading + 10 + 320 / (rectangles) + radius + displacement_y, color);
}


void draw_distancia(int index, int n_index, int color) {
  tftsetTextColorWrapper(color);
  if ( color == LIGHT_ORANGE && ((index + offset - 2) / 2 ) == 0 && zero_mode == 0 ) {
    tftsetTextColorWrapper(COLOR_MSG);
  }
  switch (n_index) {
    case -1:
      if (info_tramos[((index + offset - 2) / 2 )][4] >= 0) {
        tftdrawStringWrap("+", 70 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      }
      else {
        tftdrawStringWrap("-", 70 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      }
      break;
    case 0:
      if (info_tramos[((index + offset - 2) / 2 )][4] >= 0) {
        distancia_0[0] = info_tramos[((index + offset - 2) / 2 )][4] / 1000 + 48;
      }
      else {
        distancia_0[0] = -1 * info_tramos[((index + offset - 2) / 2 )][4] / 1000 + 48;
      }
      tftdrawStringWrap(distancia_0, 70 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 1:
      if (info_tramos[((index + offset - 2) / 2 )][4] >= 0) {
        distancia_1[0] = info_tramos[((index + offset - 2) / 2 )][4] / 100 % 10 + 48;
      }
      else {
        distancia_1[0] = -1 * info_tramos[((index + offset - 2) / 2 )][4] / 100 % 10 + 48;
      }
      tftdrawStringWrap(distancia_1, 70 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 2:
      if (info_tramos[((index + offset - 2) / 2 )][4] >= 0) {
        distancia_2[0] = info_tramos[((index + offset - 2) / 2 )][4] / 10 % 10 + 48;
      }
      else {
        distancia_2[0] = -1 * info_tramos[((index + offset - 2) / 2 )][4] / 10 % 10 + 48;
      }
      tftdrawStringWrap(distancia_2, 70 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 3:
      if (info_tramos[((index + offset - 2) / 2 )][4] >= 0) {
        distancia_3[0] = info_tramos[((index + offset - 2) / 2 )][4] / 1 % 10 + 48;
      }
      else {
        distancia_3[0] = -1 * info_tramos[((index + offset - 2) / 2 )][4] / 1 % 10 + 48;
      }
      tftdrawStringWrap(distancia_3, 70 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
  }
  tftsetTextColorWrapper(WHITE);
}

void draw_espera(byte index, byte n_index, int color) {
  tftsetTextColorWrapper(color);
  switch (n_index) {
    case 0:
      espera_0[0] = ( info_tramos[((index + offset - 2) / 2 )][3] / 60 ) / 10 + 48;
      tftdrawStringWrap(espera_0, 113 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height + (320 - width_heading) / (float)rectangles, 4);
      break;
    case 1:
      espera_1[0] = ( info_tramos[((index + offset - 2) / 2 )][3] / 60 ) % 10 + 48;
      tftdrawStringWrap(espera_1, 113 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height + (320 - width_heading) / (float)rectangles, 4);
      break;
    case 2:
      espera_2[0] = ( info_tramos[((index + offset - 2) / 2 )][3] % 60 )  / 10 + 48;
      tftdrawStringWrap(espera_2, 120 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height + (320 - width_heading) / (float)rectangles, 4);
      break;
    case 3:
      espera_3[0] = ( info_tramos[((index + offset - 2) / 2 )][3]  % 60 ) % 10 + 48;
      tftdrawStringWrap(espera_3, 120 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height + (320 - width_heading) / (float)rectangles, 4);
      break;
  }
  tftsetTextColorWrapper(WHITE);
}


void draw_timespeed (byte index, byte n_index, int color) {
  tftsetTextColorWrapper(color);
  switch (n_index) {
    case 0:
      timespeed_0[0] = ( info_tramos[((index + offset - 2) / 2 )][2] / 60 ) / 100 + 48;
      tftdrawStringWrap(timespeed_0, 158 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 1:
      timespeed_1[0] = ( info_tramos[((index + offset - 2) / 2 )][2] / 60 ) / 10 % 10 + 48;
      tftdrawStringWrap(timespeed_1, 158 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 2:
      timespeed_2[0] = ( info_tramos[((index + offset - 2) / 2 )][2] / 60 )  % 10 + 48;
      tftdrawStringWrap(timespeed_2, 158 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 3:
      timespeed_3[0] = ( info_tramos[((index + offset - 2) / 2 )][2]  % 60 ) / 10 + 48;
      tftdrawStringWrap(timespeed_3, 165 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
    case 4:
      timespeed_4[0] = ( info_tramos[((index + offset - 2) / 2 )][2] % 60 ) % 10 + 48;
      tftdrawStringWrap(timespeed_4, 165 + n_index * 13, (320 - width_heading) / (2 * rectangles) + index * (320 - width_heading) / (rectangles) + letter_height, 4);
      break;
  }
  tftsetTextColorWrapper(ILI9341_WHITE);
}

void draw_poweroff() {
  tft.fillRect(0, 0, width_back, width_heading, WHITE);
  //  tft.drawRect(0, 0, width_back, width_heading, BLACK);
  tft.fillCircle(25, 17, 12, BLACK);
  tft.fillCircle(25, 17, 9, WHITE);
  tft.fillRect(20, 1, 10, 10, WHITE);
  tft.fillRect(23, 5, 4, 12, BLACK);

}

void Circle(int position_x, int color) {
  tft.fillCircle((240 - width_select) / 5 + position_x, width_heading + 320 * 3 / (rectangles * 6) + 5, triang_pos - 1, color);
}

void draw_cursor_selected() {
  tft.fillRect(0, (320 - width_heading) * (bar_pos - 1) / rectangles + width_heading, width_select, (320 - width_heading) / rectangles, COLOR_CHOSEN);
}

void draw_cursor_unselected() {
  tft.fillRect(0, (320 - width_heading) * (bar_pos - 1) / rectangles + width_heading, width_select, (320 - width_heading) / rectangles, WHITE);
}

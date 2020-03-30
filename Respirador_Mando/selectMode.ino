void selectMode() {
  end2 = 0;
  bar_pos = 1;
  selected = 0;
  while (1) {
    ypos = (320 - width_heading) / (2 * rectangles) + (bar_pos - 1) * (320 - width_heading) / (rectangles) + letter_height;
    updateData();
    if (move) {
      if (selected == 0) {
        if (move > 0) {
          if (bar_pos < rectangles ) {
            eraseBar();
            bar_pos++;
            updateBar();
          }
          else if ( page == 5 && bar_pos + bar_offset < virtual_rectangles ) {
            bar_offset++;
            scrolling_down = 1;
            scrolling = 1;
          }
        }
        else {
          if (bar_pos > 1) {
            eraseBar();
            bar_pos--;
            updateBar();
          }
          else if ( page == 5 && bar_offset ) {
            bar_offset--;
          }
        }
      }
    }
    if ( PUSHED ) {
      back_mode();
      selected = 1;
      if (selected) {
        draw_cursor_selected();
      }
      else {
        draw_cursor_unselected();
      }
      while (PUSHED) {
        updateData();
      }
      switch (page) {
        case 1:
          switch (bar_pos) {
            case 1:
              previousCMV = CMV;
              while (NOT_PUSHED) {
                updateData();
                if (move) {
                  if (CMV == 1) {
                    CMV++;
                  }
                  else {
                    CMV--;
                  }
                  drawCMVselector(CMV);
                }
                move = 0;
              }
              if (previousCMV != CMV) {
                menu();
              }
              break;
            case 2:
              while (NOT_PUSHED) {
                updateData();
                if (move && -move + PIPpressure >= minPIPpressure && -move + PIPpressure <= maxPIPpressure) {
                  tft.setTextColor(BLACK);
                  drawRightNumber(PIPpressure, PIPpressureXPos, ypos);
                  PIPpressure -= move;
                  tft.setTextColor(LIGHT_ORANGE);
                  drawRightNumber(PIPpressure, PIPpressureXPos, ypos);
                }
                move = 0;
              }
              break;
            case 3:
              while (NOT_PUSHED) {
                updateData();
                if (move && -move + oscRate >= minOscRate && -move + oscRate <= maxOscRate) {
                  tft.setTextColor(BLACK);
                  drawRightNumber(oscRate, oscRateXPos, ypos);
                  oscRate -= move;
                  tft.setTextColor(LIGHT_ORANGE);
                  drawRightNumber(oscRate, oscRateXPos, ypos);
                }
                move = 0;
              }
              break;
            case 4:
              while (NOT_PUSHED) {
                updateData();
                if (move && -move + cc >= minCc && -move + cc <= maxCc) {
                  tft.setTextColor(BLACK);
                  drawRightNumber(cc, ccXPos, ypos);
                  cc -= 50 * move;
                  tft.setTextColor(LIGHT_ORANGE);
                  drawRightNumber(cc, ccXPos, ypos);
                }
                move = 0;
              }
              break;
            case 5:
              while (NOT_PUSHED) {
                updateData();
                if (move && -move + expRatio >= minExpRatio && -move + expRatio <= maxExpRatio) {
                  tft.setTextColor(BLACK);
                  drawRightNumber(expRatio, expRatioXPos, ypos);
                  expRatio -= move;
                  tft.setTextColor(LIGHT_ORANGE);
                  drawRightNumber(expRatio, expRatioXPos, ypos);
                }
                move = 0;
              }
              break;
            case 6:
              if (page == 2) {
                while (NOT_PUSHED) {
                  updateData();
                  if (move && -move + PTrigger >= minPTrigger && -move + PTrigger <= maxPTrigger) {
                    tft.setTextColor(BLACK);
                    drawRightNumber(PTrigger, PTriggerXPos, ypos);
                    PTrigger -= move;
                    tft.setTextColor(LIGHT_ORANGE);
                    drawRightNumber(PTrigger, PTriggerXPos, ypos);
                  }
                  move = 0;
                }
              }
              else {
                alarmSettings();
              }
              break;
            case 7:
              alarmSettings();
              break;
          }
          break;
        case 3:
          switch (bar_pos) {
            case 1:
              previousCMV = CMV;
              while (NOT_PUSHED) {
                updateData();
                if (move) {
                  if (CMV == 1) {
                    CMV++;
                  }
                  else {
                    CMV--;
                  }
                  drawCMVselector(CMV);
                }
                move = 0;
              }
              if (previousCMV != CMV) {
                menu();
              }
              break;
            case 2:
              while (NOT_PUSHED) {
                updateData();
                if (move && -move + cc >= minCc && -move + cc <= maxCc) {
                  tft.setTextColor(BLACK);
                  drawRightNumber(cc, ccXPos, ypos);
                  cc -= 50 * move;
                  tft.setTextColor(LIGHT_ORANGE);
                  drawRightNumber(cc, ccXPos, ypos);
                }
                move = 0;
              }
              break;
            case 4:
              menu();
              break;
          }
          break;
      }
      draw_cursor_unselected();
      while (PUSHED);
      delay(20);
      selected = 0;
    }
  }
}

void eraseBar() {
  tft.fillRect(0 + sub_toggle_d, (320 - width_heading) * (bar_pos - 1) / rectangles + width_heading, width_select, (320 - width_heading) / rectangles, COLOR_BAR);
  if ( bar_pos != 1 ) {
    tft.fillRect(0 + sub_toggle_d, (320 - width_heading) * (bar_pos - 1) / rectangles + width_heading - 1, width_select, width_space, COLOR_LINE);
  }
  tft.fillRect(0 + sub_toggle_d, (320 - width_heading) * (bar_pos) / rectangles + width_heading - 1, width_select, width_space, COLOR_LINE);
}

void updateBar() {
  tft.fillRect(0 + sub_toggle_d, (320 - width_heading) * (bar_pos - 1) / rectangles + width_heading, width_select, (320 - width_heading) / rectangles, COLOR_SELECTED);
  if ( page == 26) {
    tft.fillRect(0, (320 - width_heading) * 2 / rectangles + width_heading - 1, width_select, 4, WHITE);
    tft.fillRect(0, (320 - width_heading) * 4 / rectangles + width_heading - 1, width_select, 4, WHITE);
  }
}

void eraseBigBar() {
  tft.fillRect(0, width_heading, width_select, (320 - width_heading) * 2 / rectangles - 1, COLOR_BAR);
}

void updateBigBar() {
  tft.fillRect(0, width_heading, width_select, (320 - width_heading) * 2 / rectangles, WHITE);
  for (int i = 3; i <= rectangles; i++) {
    tft.fillRect(0, (320 - width_heading) * (i - 1) / rectangles + width_heading - 1, 240, width_space, COLOR_LINE); //mejorable
  }
}

void back_mode() {
  bool poweroff_rate = 0;
  last_pulsed = millis();
  byte back_bar = 0;
  while (PUSHED) {
    updateData();
    if (millis() - last_pulsed > time_back_wait) {
      back_bar++;
      tft.drawLine(width_back - back_bar, 0, width_back - back_bar, width_heading, BLACK);
      delay((4000) / width_back);
    }
    if (back_bar == width_back) {
      intro();
      //stopConfirmation();
    }
  }
  if (millis() - last_pulsed > time_back_wait) {
    draw_poweroff();
  }
}

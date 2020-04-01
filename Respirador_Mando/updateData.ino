void updateData() {
  Watchdog.reset();
  newPosition = myEncoderRead();
  move = ( oldPosition - newPosition );
  move_total = -move;
  if (move) {
    if (move < 0) {
      move = 1;
    }
    else {
      move = -1;
    }
  }
  lastEncoderPos[counter] = encoderpos[counter];
  oldPosition = newPosition;
  checkIdle();
  updateParam();
  readAlarms();
}

void checkIdle() {
  if (!selected && page && page != 3 && page != 4 && page != 5 && (millis() - last_something) > graphicWait) {
    graphic();
  }
}

int newGraphicData() {
  return newData;
}

void updateParam() {
  if ( CMV != CMV_set || PIPpressure_set != PIPpressure || oscRate_set != oscRate || cc_set != cc || expRatio_set != expRatio || PTrigger_set != abs(PTrigger) ) {
    int param[6];
    param[0] = PIPpressure;
    param[1] = oscRate;
    param[2] = cc;
    param[3] = expRatio;
    param[4] = abs(PTrigger);
    param[5] = CMV;
    PIPpressure_set = PIPpressure;
    oscRate_set = oscRate;
    cc_set = cc;
    expRatio_set = expRatio;
    PTrigger_set = abs(PTrigger);
    CMV_set = CMV;
    output_msg = "";
    output_msg += 'P';
    for ( int i = 0; i < 6; i++ ) {
      output_msg_index++;
      if (i == 3) {
        output_msg += String(param[i] * 10);
      }
      else {
        output_msg += String(param[i]);
      }
      output_msg += ",";
    }
    output_msg += '|';
    output_msg += '\0';
    //Serial.print( output_msg );
    Serial1.print( output_msg );
    Serial2.print( output_msg );
  }

}

byte readAlarms() {
  int serialDataRetainer;
  int alarmReadRetainer;
  if (SerialX_available()) {
    switch (SerialX_peek()) {
      case 'S':
        SerialX_read();
        alarmRead = OverloadAlarm;
        break;
      case 'Z':
        SerialX_read();
        alarmRead = PowerSupplyAlarm;
        break;
      case 'Y':
        SerialX_read();
        alarmRead = AbnormalPressureAlarm;
        break;
      case 'A':
        SerialX_read();
        alarmRead = PressureAlarm;
        break;
      case 'O':
        SerialX_read();
        alarmRead = OscRateAlarm;
        break;
      case 'C':
        SerialX_read();
        alarmRead = CCalarm;
        break;
      case ',':
        if (alarmRead) {
          SerialX_read();
          serialDataRetainer = serialDataRecieved;
          serialDataRecieved = 0;
          alarmReadRetainer = alarmRead;
          alarmRead = 0;
          alarm(alarmReadRetainer, serialDataRetainer);
        }
        break;
      default:
        if (alarmRead) {
          if ( SerialX_peek() > 47 && SerialX_peek() < 58 ) {
            serialDataRecieved *= 10;
            serialDataRecieved += SerialX_read() - 48;
          }
          else {
            SerialX_read();
          }
          break;
        }
    }
  }
  return alarmRead;
}

byte readDriverInput() {
  bool dataToPrint = 0;
  byte variableReadRetainer;
  if (!readAlarms()) {
    if (SerialX_available()) {
      switch (SerialX_peek()) {
        case 'X':
          SerialX_read();
          Start_capture = 1;
          variableRead = 2;
          dataToPrint = 1;
          break;
        case 'V':
          SerialX_read();
          variableRead = 3;
          break;
        case 'F':
          SerialX_read();
          variableRead = 4;
          break;
        case ',':
          SerialX_read();
          switch (variableRead) {
            case 1:
              inputBuffer[ graph_index ] = serialDataRecieved;
              Serial2.println(inputBuffer[ graph_index ]);
              if ( graph_index >= graphicX - 1) {
                graph_index = 0;
              }
              else {
                graph_index++;
                inputBuffer[ graph_index ] = 0;
              }
              break;
            case 3:
              volumeDetected = serialDataRecieved;
              break;
            case 4:
              respminDetected = serialDataRecieved;
              break;
          }
          serialDataRecieved = 0;
          dataToPrint = 1;
          break;
        default:
          if ( SerialX_peek() > 47 && SerialX_peek() < 58 ) {
            serialDataRecieved *= 10;
            serialDataRecieved += SerialX_read() - 48;
          }
          else {
            SerialX_read();
          }
          break;
      }
      if (dataToPrint) {
        variableReadRetainer = variableRead;
        variableRead = 1;
        return variableReadRetainer;
      }
      else {
        return false;
      }
    }
  }
}
long buffer2string() {
  long data = 0;
  bool sign = 0;
  unsigned long read_start = millis();
  if (inputBuffer[bufferIndex] == '-') {
    sign = 1;
    bufferIndex++;
  }
  while (inputBuffer[bufferIndex] != ',' && millis() - read_start < 1000 ) {
    data = data * 10 + inputBuffer[bufferIndex] - 48;
    bufferIndex++;
  }
  if (sign) {
    data *= -1;
  }
  return (data);
}

void init_rf() {
  Serial2.end();
  Serial2.begin(9600);
  pinMode(RF_SET, OUTPUT);
  digitalWrite(RF_SET, LOW);
  char rf_msg[] = {0xAA, 0xFA, 0x03, channel, 0x01, 0x03, 0x07, 0x09, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x40, 0x00};
  delay(300);
  Serial2.write(rf_msg, 19);
  delay(100);
  digitalWrite(RF_SET, HIGH);
  pinMode(RF_SET, INPUT);
  Serial2.end();
  Serial2.begin(115200);
}

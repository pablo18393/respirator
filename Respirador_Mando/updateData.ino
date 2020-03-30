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
}

void checkIdle() {
  if (page && (millis() - last_something) > graphicWait) {
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
    output_msg_index = 0;
    output_msg[ output_msg_index ] = 'P';
    for ( int i = 0; i < 6; i++ ) {
      output_msg_index++;
      int2string( param[i] );
    }
    output_msg[ ++output_msg_index ] = '|';
    output_msg[ ++output_msg_index ] = '\0';
    Serial1.print( output_msg );
    Serial2.print( output_msg );
  }

}

bool read_pressure() {
  bool dataToPrint = 0;
  if (SerialX_peek() == 'X') {
    SerialX_read();
    Start_capture = 1;
    resetValues();
  }
  else if ( SerialX_peek() > 47 && SerialX_peek() < 58 ) {
    inputBuffer[ graph_index ] *= 10;
    inputBuffer[ graph_index ] += SerialX_read() - 48;
  }
  else if ( SerialX_peek() == ',' ) {
    SerialX_read();
    Serial2.println(inputBuffer[ graph_index ]);
    if ( graph_index >= graphicX - 1) {
      graph_index = 0;
    }
    else {
      graph_index++;
      inputBuffer[ graph_index ] = 0;
    }
    dataToPrint = 1;
  }
  else {
    SerialX_read();
  }
  return dataToPrint;
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

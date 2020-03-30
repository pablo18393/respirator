void SerialX_empty() {
  while (SerialBus.available()) {
    SerialBus.read();
  }
}

byte SerialX_available() {
  return (SerialBus.available());
}

char SerialX_peek() {
  return (SerialBus.peek());
}


char SerialX_read() {
  return (SerialBus.read());
}

void SerialX_print(int payload) {
  SerialBus.print(payload);
}


void SerialX_print(long payload) {
  SerialBus.print(payload);
}



void SerialX_print(char payload) {
  SerialBus.write(payload);
}

void SerialX_print(char* payload) {
  SerialBus.write(payload);
}

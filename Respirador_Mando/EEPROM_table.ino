/*
   EMULATED EEPROM MAP - 765 positions
  */

long EEPROMread(int address) {  
  return(EEPROMreadLong(address));
}


void EEPROMWriteLong(int address, long data) {
  EEPROM.write(address, (data >> 24) & 0x000000FF);
  EEPROM.write(address + eeprom_size, (data >> 16) & 0x000000FF);
  EEPROM.write(address + 2*eeprom_size, (data >> 8) & 0x000000FF);
  EEPROM.write(address + 3*eeprom_size, (data & 0x000000FF));
 // EEPROM.commit();
}

long EEPROMreadLong(int address) {
  return((long)EEPROM.read(address) * 0x01000000 + (long)EEPROM.read(address + eeprom_size) * 0x00010000 + (long)EEPROM.read(address + 2*eeprom_size) * 0x00000100 + (long)EEPROM.read(address + 3*eeprom_size) * 0x00000001);
}


void EEPROMwrite(int jj, int xx) {
  EEPROMWriteLong(jj, xx);
 // EEPROM.commit();
}

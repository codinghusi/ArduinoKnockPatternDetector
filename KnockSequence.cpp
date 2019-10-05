#include "KnockSequence.h"
#include <Arduino.h>
#include <EEPROM.h>

KnockSequence KnockSequence::load(unsigned short address) {
  byte len = EEPROM.read(address);
  KnockSequence sequence = KnockSequence(std::vector<byte>(len));
  for (int i = 0; i < len; ++i) {
    sequence.knocks[i] = EEPROM.read(address + i);
  }
  return sequence;
}

void KnockSequence::save(unsigned short address) {
  byte len = knocks.size();
  EEPROM.write(address, len);
  for (int i = 0; i < len; ++i) {
    EEPROM.write(i + address + 1, knocks[i]);
  }
}

KnockSequence::KnockSequence() {
  
}

KnockSequence::KnockSequence(std::vector<byte> knocks): knocks(knocks) {
  
}

byte KnockSequence::test(KnockSequence sequence) {
  size_t len1 = knocks.size();
  size_t len2 = sequence.knocks.size();
  if (len1 != len2) {
    return 255;
  }
  byte error = 0;
  for (int i = 0; i < len1 && i < len2; ++i) {
    error += abs(knocks[i] - sequence.knocks[i]);
  }
  return error;
}

bool KnockSequence::empty() {
  return !knocks.size();
}

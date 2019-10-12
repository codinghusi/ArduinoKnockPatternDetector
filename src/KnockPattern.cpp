#include "KnockPattern.h"
#include <Arduino.h>
#include <EEPROM.h>

KnockPattern KnockPattern::load(unsigned int address) {
  byte len = EEPROM.read(address);
  KnockPattern pattern = KnockPattern(std::vector<byte>(len));
  for (int i = 0; i < len; ++i) {
    pattern.knocks[i] = EEPROM.read(address + i + 1);
  }
  return pattern;
}

void KnockPattern::save(unsigned int address) {
  byte len = knocks.size();
  EEPROM.write(address, len);
  for (int i = 0; i < len; ++i) {
    EEPROM.write(i + address + 1, knocks[i]);
  }
}

KnockPattern::KnockPattern() {
  
}

KnockPattern::KnockPattern(std::vector<byte> knocks): knocks(knocks) {
  
}

byte KnockPattern::test(KnockPattern pattern) {
  size_t len1 = knocks.size();
  size_t len2 = pattern.knocks.size();
  if (len1 != len2) {
    return 255;
  }
  byte maxDiff = 0;
  byte diff;
  for (int i = 0; i < len1 && i < len2; ++i) {
    diff = abs(knocks[i] - pattern.knocks[i]);
    maxDiff = diff > maxDiff ? diff : maxDiff;
  }
  return maxDiff;
}

bool KnockPattern::empty() {
  return !knocks.size();
}

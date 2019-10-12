#ifndef KNOCK_SEQUENCE_H
#define KNOCK_SEQUENCE_H

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector>

class KnockPattern {
 public:
  static KnockPattern load(unsigned int);
  std::vector<byte> knocks;
  KnockPattern();
  KnockPattern(std::vector<byte>);
  void save(unsigned int);
  byte test(KnockPattern);
  bool empty();
};


#endif

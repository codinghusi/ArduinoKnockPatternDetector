#ifndef KNOCK_SEQUENCE_H
#define KNOCK_SEQUENCE_H

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector>

class KnockSequence {
 public:
  static KnockSequence load(unsigned short address);
  std::vector<byte> knocks;
  KnockSequence();
  KnockSequence(std::vector<byte>);
  void save(unsigned short address);
  byte test(KnockSequence);
  bool empty();
};


#endif

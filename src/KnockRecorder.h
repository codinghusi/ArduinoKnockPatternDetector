#ifndef KNOCK_RECORDER_H
#define KNOCK_RECORDER_H

#include "KnockHandler.h"
#include "KnockPattern.h"
#include <ArduinoSTL.h>
#include <vector>
#include <Arduino.h>


class KnockRecorder {
 public:
  KnockHandler* handler;
  std::vector<unsigned long> knocks;
  bool lastStatus;
  unsigned long last;
  bool finished = false;
  unsigned int timeout;
  unsigned short snap;
  byte debounce = 50;

  enum class KnockStatus : byte {
    NotRecording, Recording, Knock
  };
  KnockRecorder(KnockHandler*, unsigned int = 1000, unsigned short = 64);
  KnockPattern getPattern();
  void restart();
  KnockStatus handle();
  bool recording();
  void continueRecord();
  void stop();
  bool empty();
  void clear();
};


#endif

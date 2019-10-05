#ifndef KNOCK_RECORDER_H
#define KNOCK_RECORDER_H

#include "KnockHandler.h"
#include "KnockSequence.h"
#include <ArduinoSTL.h>
#include <vector>


class KnockRecorder {
  KnockHandler* handler;
  std::vector<unsigned long> knocks;
  bool lastStatus;
  unsigned long last;
  bool finished = false;
  unsigned int timeout;
  unsigned short snap;
 public:
  enum class KnockStatus : byte;
  KnockRecorder(KnockHandler*, unsigned int = 1000, unsigned short = 64);
  KnockSequence getSequence();
  void restart();
  KnockStatus handle();
  bool recording();
  void continueRecord();
  void stop();
  bool available();
  bool empty();
  void clear();
};


#endif

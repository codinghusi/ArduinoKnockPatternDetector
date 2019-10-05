#ifndef KNOCK_ANALOG_HANDLER_H
#define KNOCK_ANALOG_HANDLER_H

#include "KnockHandler.h"


class KnockAnalogHandler: public KnockHandler {
  byte pin;
  unsigned short threshold;
  bool lower;
 public:
  KnockAnalogHandler(byte, unsigned short, bool = false);
  bool checkKnock();
};


#endif

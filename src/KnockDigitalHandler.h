#ifndef KNOCK_DIGITAL_HANDLER_H
#define KNOCK_DIGITAL_HANDLER_H

#include "KnockHandler.h"


class KnockDigitalHandler: public KnockHandler {
  byte pin;
  bool negate;
 public:
  KnockDigitalHandler(byte, bool = false);
  bool checkKnock();
};


#endif

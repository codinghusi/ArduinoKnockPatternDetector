#ifndef KNOCK_HANDLER_H
#define KNOCK_HANDLER_H

#include <ArduinoSTL.h>
#include <Arduino.h>
#include <functional>


class KnockHandler {
 public:
  virtual bool checkKnock() = 0;
};


#endif

#include "KnockAnalogHandler.h"

KnockAnalogHandler::KnockAnalogHandler(byte pin, unsigned short threshold, bool lower): pin(pin), threshold(threshold), lower(lower) {
  pinMode(pin, INPUT_PULLUP);
}

bool KnockAnalogHandler::checkKnock() {
  return lower ^ (analogRead(pin) >= threshold);
}

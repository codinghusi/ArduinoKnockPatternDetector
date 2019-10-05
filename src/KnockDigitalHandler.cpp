#include "KnockDigitalHandler.h"

KnockDigitalHandler::KnockDigitalHandler(byte pin, bool negate): pin(pin), negate(negate) {
  pinMode(pin, INPUT_PULLUP);
}

bool KnockDigitalHandler::checkKnock() {
  return negate ^ digitalRead(pin);
}

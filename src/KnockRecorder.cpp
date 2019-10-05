#include "KnockRecorder.h"
#include <numeric>

enum class KnockRecorder::KnockStatus : byte {
  NOT_RECORDING, RECORDING, KNOCK
};

KnockRecorder::KnockRecorder(KnockHandler* handler, unsigned int timeout, unsigned short snap): handler(handler), timeout(timeout) {
  lastStatus = handler->checkKnock(); 
  continueRecord();
}

KnockPattern KnockRecorder::getPattern() {
  KnockPattern pattern(std::vector<byte>(knocks.size()));
  unsigned long total = std::accumulate(knocks.begin(), knocks.end(), 0);
  size_t len = knocks.size();
  for (int i = 0; i < len; ++i) {
    pattern.knocks[i] = map(knocks[i], 0, total, 0, 255);
  }
  return pattern;
}

KnockRecorder::KnockStatus KnockRecorder::handle() {
  if (recording()) {
    bool knocked = false;
    bool status = handler->checkKnock();
    bool started = knocks.size() > 0;
    unsigned int difference = millis() - last;
    if (difference < 0) {
      // could happen (after 50 days)
      difference += pow(2, sizeof(unsigned long)) - 1;
    }
    if (started && difference >= timeout) {
      stop();
      return KnockStatus::NOT_RECORDING;
    }
    if (status != lastStatus) {
      if (status) {
        knocked = true;
        if (!started) {
         difference = 0;
         started = true;
        } else {
         difference = (unsigned long) round(float(difference) / float(snap)) * float(snap);
        }
      }
      last = millis();
      knocks.push_back(difference);
      lastStatus = status;
      delay(50);
      if (knocked) {
        return KnockStatus::KNOCK;
      }
    }
    return KnockStatus::RECORDING;
  }
  return KnockStatus::NOT_RECORDING;
}

void KnockRecorder::clear() {
  knocks.clear();
}

void KnockRecorder::restart() {
  clear();
  last = millis();
  continueRecord();
}

bool KnockRecorder::recording() {
  return !finished;
}

void KnockRecorder::continueRecord() {
  finished = false;
}

void KnockRecorder::stop() {
  finished = true;
}

bool KnockRecorder::empty() {
  return !knocks.size();
}

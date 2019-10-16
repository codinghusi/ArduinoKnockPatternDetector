#include "KnockRecorder.h"
#include <numeric>

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
    long difference = millis() - last; // could produce an overflow (see 2 lines below)
    if (difference < 0) {
      // could happen (after 50 days)
      difference += pow(2, sizeof(unsigned long)) - 1;
    }
    if (started && difference >= timeout) {
      stop();
      return KnockStatus::NotRecording;
    }
    if (difference >= debounce && status != lastStatus) {
      if (status) {
        knocked = true;
        if (!started) {
         difference = 0;
         started = true;
        } else {
         difference = (long) round(float(difference) / float(1000. / snap)) * float(1000. / snap);
        }
      }
      last = millis();
      knocks.push_back(difference);
      lastStatus = status;
      if (knocked) {
	if (knocks.size() >= max) {
	  stop();
	}
        return KnockStatus::Knock;
      }
    }
    return KnockStatus::Recording;
  }
  return KnockStatus::NotRecording;
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

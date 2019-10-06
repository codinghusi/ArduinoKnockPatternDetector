# Overview
This package allows you to record a knock pattern, saving it on the EEPROM and testing it on the fly. So you can react on a specific knock pattern.
All that happens asynchronous and with many settings!

## Example
```c++
/*
  KnockPatternDetector

  Checks on pin 2 for a signal (e.g. use a button).
  First of all you need to open the console (ctrl+shift+m)
  and send "record". Then you can 'knock' a pattern (on the button).
  After waiting one second it will confirm you that it was recorded and saved.
  Now you can 'knock' that pattern again at any time and check the console.
  If you made a mistake during knocking you also need to wait one second.

*/

#include <KnockPatternDetector.h>

KnockDigitalHandler handler = KnockDigitalHandler(2);
KnockPattern pattern = KnockPattern::load(0);
KnockRecorder recorder(&handler);

void setup() {
  Serial.begin(9600);
}

void loop() {
  recorder.handle();
  
  if (!recorder.recording()) {
    byte error = pattern.test(recorder.getPattern());
    if (error < 50) {
      Serial.println("that was right!");
    } else {
      Serial.println("incorrect");
    }
    recorder.restart();
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    if (command == "record") {
      recorder.restart();
      Serial.println("recording...");
      while ((bool) recorder.handle()) {}
      pattern = recorder.getPattern();
      pattern.save(0);
      recorder.restart();
      Serial.println("recorded!");
    } else {
      Serial.println("unknown command");
    }
  }
}
```



# Documentation
It's not finished :)

## KnockHandler
This is an interface that detects a knock. For example the KnockDigitalHandler detects a 'knock' when you press a button.
How the KnockDigitalHandler is defined:  
**Header**
```c++
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
```

**CPP**
```c++
#include "KnockDigitalHandler.h"

KnockDigitalHandler::KnockDigitalHandler(byte pin, bool negate): pin(pin), negate(negate) {
  pinMode(pin, INPUT_PULLUP);
}

bool KnockDigitalHandler::checkKnock() {
  return negate ^ digitalRead(pin);
}```

## KnockRecorder
This class records your knock pattern. First of all you need to pass a KnockHandler so it knows when a knock happens. 
Every loop you need to call the method *recorder.handle()* to detect that knocks. After reaching the timeout (default 1000ms), it stops recording. You can check that using *if (recorder.recording()) { }*.
Then you can create your KnockPattern out of that using 
```c++
KnockPattern pattern = recorder.getPattern();
```

## KnockPattern
A simple list of the knocks (timings in percent, as bytes). You can use the method test to see if another pattern matches it. The method returns an error (0-255), 0 is perfect and 255 is very bad:
```c++
// recordedPattern and pattern is somewhere defined
if (recordedPattern.test(pattern) < 50) {
	// matches
}
```
You can also save and load a pattern from the EEPROM using ```pattern.load(0)``` and ```pattern.save(0)```



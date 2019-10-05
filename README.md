# Overview
This package allows you to record a knock pattern, saving it on the EEPROM and testing it on the fly. So you can react on a specific knock pattern.

# Example
```c++
/*
  KnockDetector

  Checks on pin 2 for a signal (e.g. use a button).
  First of all you need to open the console (ctrl+shift+m)
  and send "record". Then you can 'knock' a pattern (on the button).
  After waiting one second it will confirm you that it was recorded and saved.
  Now you can 'knock' that pattern again at any time and check the console.
  If you made a mistake during knocking you also need to wait one second.

*/

#include <KnockDetector.h>

KnockAnalogHandler handler = KnockDigitalHandler(2);
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
      while ((int) recorder.handle()) {}
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

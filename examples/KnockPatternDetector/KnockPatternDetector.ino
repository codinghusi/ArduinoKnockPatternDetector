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

const byte PIN_KNOCK_SENSOR = 2;
const byte EEPROM_PATTERN = 0;
const byte MAXIMUM_ERROR = 25;

KnockDigitalHandler handler = KnockDigitalHandler(PIN_KNOCK_SENSOR);
KnockPattern pattern = KnockPattern::load(EEPROM_PATTERN);
KnockRecorder recorder(&handler);

void setup() {
  Serial.begin(9600);
}

void loop() {
  recorder.handle();
  
  if (!recorder.recording()) {
    byte error = pattern.test(recorder.getPattern());
    if (error < MAXIMUM_ERROR) {
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

#include "KnockAnalogHandler.h"
#include "KnockRecorder.h"
#include "KnockSequence.h"

#include <ArduinoSTL.h>
#include <iostream>

KnockAnalogHandler handler = KnockAnalogHandler(A0, 200, true);
KnockSequence sequence;
KnockRecorder recorder(&handler);

void setup() {
  Serial.begin(9600);
}

void loop() {
  recorder.handle();
  
  if (!recorder.recording()) {
    byte error = sequence.test(recorder.getSequence());
    if (error < 50) {
      std::cout << "success " << error << std::endl;
    } else {
      std::cout << "failed " << error << std::endl;
    }
    recorder.restart();
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    if (command == "record") {
      recorder.restart();
      while ((int) recorder.handle()) {}
      sequence = recorder.getSequence();
      sequence.save(1);
      recorder.restart();
      std::cout << "recorded! " << sequence.knocks.size() << std::endl;
    } else {
      std::cout << "unknown command" << std::endl;
    }
  }
}

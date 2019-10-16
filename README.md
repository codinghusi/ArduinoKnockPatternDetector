# Overview
This package allows you to record a knock pattern, saving it on the EEPROM and testing it on the fly. So you can react on a specific knock pattern.
All that happens asynchronous and with many settings!

# Installation (Arduino IDE)
## Manually
Download the project and extract it into the library folder of Arduino.
**Under Linux**: ~/Arduino/libraries
## Library Manager
In the IDE you can use Tools > Library Manager... (or press ctrl+shift+I) and then search for "KnockPatternDetector".
Install it. Finished.

# Example
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
```



# Documentation
It's not finished :)

## KnockHandler
This is an interface that detects a knock. For example the KnockDigitalHandler detects a 'knock' when you press a button.
So every time you press the button, it will recognise it as a knock. The negate variable.

### Details
#### KnockHandler
An interface (abstract class) that gives you the ability to detect a knock by your own sensor (maybe a complicated one)
To do that you need to override the method:
```c++
bool checkKnock();
```
this will be called whenever a recorder wants know, if someone triggered your sensor.
Return true if there was a knock, return false if not.
An example for KnockDigitalHandler:
[Header](https://github.com/codinghusi/ArduinoKnockPatternDetector/blob/master/src/KnockHandler.h)
[CPP](https://github.com/codinghusi/ArduinoKnockPatternDetector/blob/master/src/KnockHandler.cpp)

#### KnockDigitalHandler
```c++
KnockDigitalHandler(byte pin, bool negate = true);
```
* **pin**: the pin where the button is connected with the arduino
*  **negate** (defaults to true): if it's false it will handle a HIGH-Signal as knock, on true it handels a LOW-Signal as knock

> **Note**: You need to set the pinMode by yourself

#### KnockAnalogHandler
```c++
KnockAnalogHandler(byte pin, unsigned short threshold, bool lower = false);
```
* **pin**: the pin where the button is connected with the arduino
* **threshold**: the analog value of the pin when it should lead to a knock
* **lower** (defaults to false): if the analog value needs to be lower than the threshold or above it.

> **Note**: You need to set the pinMode by yourself

## KnockRecorder
This class records your knock pattern. First of all you need to pass a [KnockHandler](#KnockHandler) so it knows when a knock happens. 
Every loop you need to call the method `recorder.handle()` so that it can detect your knocks. After reaching the timeout (default 1000ms), it stops recording. You can check that using:
```c++
if (recorder.recording()) { }
```
Then you can create your [KnockPattern](#KnockPattern) out of that using 
```c++
KnockPattern pattern = recorder.getPattern();
```
> **Note**: it won't reach the timeout until the first knock happens

### Details
#### Constructor
```c++
KnockRecorder(KnockHandler* handler, unsigned int timeout = 1000, unsigned short snap = 64);
```
* **handler**: the [KnockHandler](#KnockHandler) that detects a knock
*  **timeout** (defaults to 1000ms): the time the recorder will wait, until he pauses the recording
*  **snap** (defaults to 64 per second): the recorder rounds the `knockTiming / (1000 / snap)`. The lower the value, the lower the precision. You can reduce the value if you have problems with a to high precision.

#### Variables
The recorder has some public variables for configuration:
* **handler** (set by constructor): the [KnockHandler](#KnockHandler) that detects a knock
*  **timeout** (set by constructor): the time the recorder will wait, until he pauses the recording
*  **snap** (set by constructor): the recorder rounds the `knockTiming / (1000 / snap)`. The lower the value, the lower the precision. You can reduce the value if you have problems with a to high precision.
* **debounce** (defaults to 50ms): the delay it waits after a new knock can be detected
* **max** (defaults to 20): after the recorder records more than the max value it automatically stops recording

#### Methods
```c++
KnockPattern getPattern();
```
 Generates a [KnockPattern](#KnockPattern) out of the recorded data and returns it.

---
```c++
void restart();
```
Clears all recorded data and starts a new recording

---
```c++
KnockStatus handle();
```
Needs to be called every loop, so that the recorder can work. Pretty important.
It returns the current status of the recorder:
##### KnockStatus
```c++
enum class KnockStatus : byte {
  NotRecording, Recording, Knock
};
```
* **NotRecording**: The recorder does nothing. Because it's not recording
* **Recording**: The recorder is recording, but there was no knock detected
* **Knock**: It's recording and a knock was detected. You can use this for a status LED

---
```c++
bool recording();
```
Returns if the recorder is currently recording

---
```c++
void stop();
```
Stops the recorder

---
```c++
void continueRecord();
```
*TODO: name should be changed*
If the recorder was paused by `stop()` you can continue with this method.

---
```c++
bool empty();
```
Returns if it currently recorded nothing.

---
```c++
void clear();
```
Deletes all recorded data.
> **Note**: It won't stop recording.

## KnockPattern
A simple list of the knocks (timings in percent, as bytes). You can use the method test to see if another pattern matches it. The method returns an error (0-255), 0 is perfect and 255 is very bad:
```c++
// recordedPattern and pattern is somewhere defined (type: KnockPattern)
if (recordedPattern.test(pattern) < 25) {
	// matches
}
```
You can also save and load a pattern from the EEPROM using ```pattern.load(0)``` and ```pattern.save(0)```

### Details
#### Constructor
```c++
KnockPattern();
KnockPattern(std::vector<byte> timings);
```
* **timings**: the knocks in percentage that describe the pattern

#### Methods
```c++
static KnockPattern load(unsigned int address);
```
Loads a pattern from the EEPROM beginning at *address*.

---
```c++
void save(unsigned int address);
```
Saves the pattern at the *address* in EEPROM.

---
```c++
byte test(KnockPattern pattern);
```
Tests how equal the both patterns are. Returns 0 if it was perfect, 255 if it wasn't similar at all
**Algorithm**
* If the count of knocks is different it will return error 255 (100%) 
* Else it finds the maximum difference between two knock timings


#include "ArrbotMonitor.h"
#include "kalman.h"

// attach:
//   GND -- servo lead negative
//   Vin -- servo lead positive
//   D2  -- servo lead signal
//   GND -- 8 ohm speaker, negative
//   D8  -- 8 ohm speaker, positive
//   D4  -- button
//   GND -- button


#define SERVO_PIN 2
#define SPEAKER_PIN 8

volatile int pulseWidth = 0;
volatile unsigned long risingTimestamp = 0;

int buttonPin = 4;
//int servoPin  = 2;
//int speakerPin = 8;

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(SERVO_PIN) , pulseHandler, CHANGE);
  pinMode(buttonPin, INPUT);
}

Kalman1d pulseFilter(4, 4, 0.01);
int reversed = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

void loop() {
  int smoothedPulseWidth = pulseFilter.updateEstimate(pulseWidth);

  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        reversed = !reversed;
      }
    }
  }
  lastButtonState = reading;

  MONITOR(pulseWidth);
  MONITOR(smoothedPulseWidth);

  MONITOR_ENDL();
  // pitch range seems about right for 49 cent speakers
  int pitch = map(smoothedPulseWidth, 900, 2100, 110, 110*8);
  reversed=0;
  if (reversed) {
    pitch = 110*9 - pitch;
  }
  tone(SPEAKER_PIN, pitch);
  delay(10);
}

void pulseHandler()
{
  // if the pin is high, its the rising edge of pulse
  // otherwise it's the falling edge of the pulse.
  if (digitalRead(SERVO_PIN) == HIGH) {
    risingTimestamp = micros();
  }
  else {
    if (risingTimestamp) {
      pulseWidth = (int)(micros() - risingTimestamp);
      risingTimestamp = 0;
    }
  }
}

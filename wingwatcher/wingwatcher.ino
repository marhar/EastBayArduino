#include "kalman.h"

// attach:
//   GND -- servo lead negative
//   Vin -- servo lead positive
//   D2  -- servo lead signal
//   GND -- 8 ohm speaker, negative
//   D8  -- 8 ohm speaker, positive
//   D10  -- button
//   GND -- button

volatile int pulseWidth = 0;
volatile unsigned long risingTimestamp = 0;

const int servoPin  = 2;
const int speakerPin = 8;
const int buttonPin = 10;
const int ledPin = LED_BUILTIN;

void setup() {
  attachInterrupt(digitalPinToInterrupt(servoPin) , pulseHandler, CHANGE);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

}

Kalman1d pulseFilter(4, 4, 0.01);
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
int ledState = 1;
int buttonState;
int lastButtonState = HIGH;

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
        ledState = !ledState;
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastButtonState = reading;

  // pitch range seems about right for 49 cent speakers
  int pitch;
  if (ledState) {
    pitch = map(smoothedPulseWidth, 900, 2100, 110*8, 110);
  }
  else {
    pitch = map(smoothedPulseWidth, 900, 2100, 110, 110*8);
  }
  tone(speakerPin, pitch);
  delay(10);
}

void pulseHandler()
{
  // if the pin is high, its the rising edge of pulse
  // otherwise it's the falling edge of the pulse.
  if (digitalRead(servoPin) == HIGH) {
    risingTimestamp = micros();
  }
  else {
    if (risingTimestamp) {
      pulseWidth = (int)(micros() - risingTimestamp);
      risingTimestamp = 0;
    }
  }
}

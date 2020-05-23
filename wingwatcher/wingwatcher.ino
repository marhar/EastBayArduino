#include "ArrbotMonitor.h"

// attach:
//   servo lead: - to gnd, + to Vin, signal to D2
//   speaker:  gnd, D8


// Interrupt 0 services digital pin 2.
#define PWM_INTERRUPT 0
#define SERVO_PIN 2

volatile int pulseWidth = 0;
volatile unsigned long ulStartPeriod = 0;

void setup() {
  Serial.begin(115200);
  attachInterrupt(PWM_INTERRUPT, pulseHandler, CHANGE);
}

void loop() {
    MONITOR(pulseWidth);
    MONITOR_ENDL();
    int pitch = map(pulseWidth, 900, 2100, 110, 110*8);
    tone(8, pitch);
    delay(10);
}

void pulseHandler()
{
  // if the pin is high, its the rising edge of pulse
  // otherwise it's the falling edge of the pulse.
  if (digitalRead(SERVO_PIN) == HIGH) {
    ulStartPeriod = micros();
  }
  else {
    if (ulStartPeriod) {
      pulseWidth = (int)(micros() - ulStartPeriod);
      ulStartPeriod = 0;
    }
  }
}

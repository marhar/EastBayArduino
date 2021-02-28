#include <ArrbotMonitor.h>
int sensorPin = 13;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
}

void loop() {
  sensorValue = analogRead(sensorPin);
    MONITOR(sensorValue);
    DISPLAY(sensorValue);
    MONITOR_ENDL();
    delay(10);
}

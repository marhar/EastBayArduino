// Measure the voltage of a single LiPo cell.
#include "ArrbotMonitor.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  singleCell();
  delay(1000);
}

// 4.2v is highest standard lipo cell voltage
// range of 0% - 100% is 3.3v to 4.2v
int highestReading = 1024 * 4.2/5.0;

void singleCell() {
  int x = analogRead(A0);
  int milliVolts = map(x, 0, highestReading, 0, 4200);
  int percent = map(milliVolts, 3300, 4200, 0, 100);
  MONITOR(x);
  MONITOR2("voltage", milliVolts/1000.0);
  MONITOR(percent);
  MONITOR_ENDL();
}

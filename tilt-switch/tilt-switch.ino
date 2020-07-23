#include <ArrbotMonitor.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int x = digitalRead(13);
  MONITOR(x);
  MONITOR_ENDL();
  digitalWrite(LED_BUILTIN, x);
}

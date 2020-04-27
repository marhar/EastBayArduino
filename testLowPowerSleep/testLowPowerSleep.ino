// low power blink
#include <Adafruit_SleepyDog.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

int x = 0;
void loop() {
  x = !x;
  digitalWrite(LED_BUILTIN, x);
  int sleepMS = Watchdog.sleep(1000);
}

#include <ArrbotMonitor.h>

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}


int state;
uint16_t x10 = 0;
uint16_t x100 = 0;

void loop100Hz() {
  ++x100;
}

void loop10Hz() {
  ++x10;
}

void loop1Hz() {
  digitalWrite(LED_BUILTIN, state);
  state = !state;
  MONITOR(x10);
  MONITOR(x100);
  x10 = x100 = 0;
  MONITOR_ENDL();
}

uint16_t nextTick100Hz = 0;
uint16_t nextTick10Hz = 0;
uint16_t nextTick1Hz = 0;
void loop() {
  unsigned int now = millis();
  if (now > nextTick1Hz) {
    nextTick1Hz = now + (1000 / 1);
    loop1Hz();
  }
  if (now > nextTick10Hz) {
    nextTick10Hz = now + (1000 / 10);
    loop10Hz();
  }
  if (now > nextTick100Hz) {
    nextTick100Hz = now + (1000 / 100);
    loop100Hz();
  }
}

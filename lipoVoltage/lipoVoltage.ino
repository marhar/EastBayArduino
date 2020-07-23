#include <ArrbotMonitor.h>
#include <SimpleKalmanFilter.h>
#include <FrSkySportSensorFlvss.h>
#include <FrSkySportTelemetry.h>

#define SPORT_PIN FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_2
#define MAX_VOLTAGE 5.0
#define NSTEPS 1023
SimpleKalmanFilter voltageFilter(3.7, 2, .5);
const int voltagePin = A0;
const int buttonPin = A5;   // because we can solder a button to GND and A5

// TODO: add a sensor message to give battery info.
FrSkySportSensorFlvss flvss;
FrSkySportTelemetry telemetry;

void setup() {
  //Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  telemetry.begin(SPORT_PIN, &flvss);  
}

int ledState;
uint32_t nextFlash;
uint32_t lastReadingTime;

void loop() {
  float rawVoltage = analogRead(voltagePin)*(MAX_VOLTAGE/NSTEPS);
  float voltage = voltageFilter.updateEstimate(rawVoltage);
  //MONITOR(rawVoltage); MONITOR(voltage); MONITOR_ENDL();
  flvss.setData(rawVoltage);
  //if (HAS_4_CELLS) flvss.setData(voltage, voltage, voltage, voltage);
  telemetry.send();
  uint32_t now = millis();
  if (now > nextFlash) {
    if (voltage < 3.3)
      nextFlash = now + 30;
    else {
      int ontime = map(voltage*100, 330, 420, 10, 990);
      nextFlash = now + (ledState ? ontime : 1000 - ontime) * 2;
    }
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
  }
}

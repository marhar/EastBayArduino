// Battery voltage.

#include "FrSkySportSensor.h"
#include "FrSkySportSensorFlvss.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportTelemetry.h"

FrSkySportSensorFlvss flvss;                          // Create FLVSS sensor with default ID
FrSkySportTelemetry telemetry;                                 // Create telemetry object without polling

#define BATTERY_VOLTAGE_PIN A0
#define SPORT_PIN FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_2

void setup()
{
  telemetry.begin(SPORT_PIN, &flvss);
}

// We add a voltage divider between the battery and arduino to
// scale the measurement from 0v to 5v.
// R1=3300, R2=1000 is good for up to 5S
float R1 = 3300.0;   // resistor 1 in ohms
float R2 = 1000.0;   // resistor 2 in ohms
float scalingFactor = (R1 + R2)/R2/1000;

void loop()
{
  int rawReading = analogRead(BATTERY_VOLTAGE_PIN);
  rawReading = 600;
  int unscaledMilliVolts = map(rawReading, 0, 1023, 0, 5000);
  float totalVoltage = unscaledMilliVolts * scalingFactor;

  int ncells = 4;
  float fakeCellVoltage = totalVoltage / ncells;
  flvss.setData(fakeCellVoltage, fakeCellVoltage, fakeCellVoltage, fakeCellVoltage);
  telemetry.send();
}

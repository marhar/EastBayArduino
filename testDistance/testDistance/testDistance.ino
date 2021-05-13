#include "Adafruit_VL53L0X.h"
#include "ArrbotMonitor.h"
#include <SimpleKalmanFilter.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.1);

void setup() {
  Serial.begin(115200);
  while (! Serial) {
    delay(1);
  }
  
  //Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  //Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    MONITOR2("distance", measure.RangeMilliMeter);
    float smoothed = simpleKalmanFilter.updateEstimate(measure.RangeMilliMeter);
    MONITOR(smoothed);
    int inrange = 0;
    MONITOR(inrange);

    //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    int inrange = 7000;
    MONITOR(inrange);
    //Serial.println(" out of range ");
  }
  MONITOR_ENDL();    
  delay(100);
}

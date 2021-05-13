// ESP32: 22 SCL, 21 SDA

#include "Adafruit_VL53L0X.h"
#include <SimpleKalmanFilter.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.1);

int rotation = 3;

const int displayButton = 35;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  pinMode(displayButton, INPUT);

  tft.init();
  tft.setRotation(rotation);
  tft.fillScreen(0);
  tft.setCursor(0, 0, 1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(2);
  
  if (!lox.begin()) {
    tft.setCursor(0, 0, 2);
    tft.print(F("Failed to boot VL53L0X"));
    while(1);
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  int mm;
  double smoothed;
  int inrange = 0;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    inrange = 1;
    mm = measure.RangeMilliMeter;
    smoothed = simpleKalmanFilter.updateEstimate(mm);
  }

  tft.setCursor(0, 0, 2);
  if (inrange) {
    tft.print(mm, 1);          tft.println(F("     "));
    tft.print(int(smoothed));  tft.println(F("     "));
  }
  else {
    tft.println(F("range"));
    tft.println(F("       "));
  }

  int reading = digitalRead(displayButton);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        rotation = (rotation + 1) % 4;
        tft.setRotation(rotation);
        tft.fillScreen(0);

      }
    }
  }
  lastButtonState = reading;


}

// Simple Geiger Counter display for ESP-GO
// Geiger counter: RadiationD-v1.1 (CAJOE)
//
// Wiring:
// CAJOE    TTGO
// GND   --> GND
// 5V    --> 5V
// VIN   --> 27

#include <TFT_eSPI.h>

const int inputPin = 27;
volatile unsigned long clicks = 0;

unsigned long oldclicks = 0;
volatile unsigned long oldmicros = 0;
unsigned long cpm;
unsigned long minuteclicks = 0;

TFT_eSPI tft = TFT_eSPI();

void IRAM_ATTR ISR_impulse() {
  clicks++;
}

void setup() {
  pinMode(inputPin, INPUT);
  attachInterrupt(inputPin, ISR_impulse, FALLING);

  oldmicros = micros();

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(0);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(4);
}

void loop() {
  if (clicks > oldclicks) {
    unsigned int m = micros();
    unsigned int elapsed = m - oldmicros;
    oldclicks = clicks;
    if (m > oldmicros + 60*1000*1000) {
      minuteclicks = clicks - minuteclicks;
    }
    
    tft.setCursor(0, 0);
     tft.print("clk ");tft.print(clicks); tft.println("  ");
     tft.print("gap ");tft.print(elapsed/1000000.0); tft.println("  ");
     tft.print("cpm ");tft.print(minuteclicks); tft.println("  ");
    
    oldmicros = m;
  }
}

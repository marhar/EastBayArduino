// Simple Geiger Counter display for ESP-GO

#include <TFT_eSPI.h>
#include <SPI.h>

const int inputPin = 27;
volatile unsigned long clicks = 0;
volatile unsigned long oldclicks = 0;
volatile unsigned long timestamp = 0;
volatile unsigned long oldmillis = 0;
unsigned long cpm;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

void IRAM_ATTR ISR_impulse() { // Captures count of events from Geiger counter board
  clicks++;
  timestamp = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);                            // Set pin for capturing Tube events
  attachInterrupt(inputPin, ISR_impulse, FALLING);     // Define interrupt on falling edge
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(0);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(3);
  oldmillis = millis();
}

void loop() {
  if (clicks > oldclicks) {
    unsigned int m = millis();
    unsigned int elapsed = m - oldmillis;
    oldclicks = clicks;
    Serial.print(clicks); Serial.print(" ");
    Serial.print(timestamp); Serial.print(" ");
    Serial.println(elapsed);
    
    tft.fillScreen(0);
    tft.setCursor(0, 0);
    tft.println(clicks);
    tft.println(elapsed);
    tft.println(cpm);
    
    oldmillis = m;
  }
}

// ESP32: 22 SCL, 21 SDA

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();

Adafruit_MPU6050 mpu;

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

  if (!mpu.begin()) {
    tft.setCursor(0, 0, 2);
    tft.print(F("Failed to find MPU6050"));
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  tft.setCursor(0, 0, 2);
  tft.print(a.acceleration.x); tft.println("   ");
  tft.print(a.acceleration.y); tft.println("   ");
  tft.print(a.acceleration.z); tft.println("   ");
  tft.println("");
  tft.print(g.gyro.x); tft.println("   ");
  tft.print(g.gyro.y); tft.println("   ");
  tft.print(g.gyro.z); tft.println("   ");

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

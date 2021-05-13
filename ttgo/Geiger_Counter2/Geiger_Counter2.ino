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
volatile unsigned long timerCount = 0;

unsigned long oldclicks = 0;
volatile unsigned long oldmillis = 0;
unsigned long cpm;
unsigned long seconds;

unsigned long slicecount[60];

TFT_eSPI tft = TFT_eSPI();

void IRAM_ATTR ISR_impulse() {
  clicks++;
}

hw_timer_t * timer;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR timerHandler() {
  portENTER_CRITICAL_ISR(&timerMux);
  timerCount++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

int NROWS;
int NCOLS;

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  attachInterrupt(inputPin, ISR_impulse, FALLING);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, timerHandler, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  oldmillis = millis();

  tft.init();
  tft.setRotation(1);
  NROWS=5;
  NCOLS=13;
  tft.fillScreen(0);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(3);
}

void dis(int row, char* label, char* val) {
  tft.setCursor(row, 0);
  tft.print(label);
  tft.print(val);
  for (int i = strlen(label)+strlen(val); i < NCOLS; ++i)
    tft.print(" ");
}
void loop() {
  unsigned int elapsed;
  unsigned int cpm;
  int go = 0;
  unsigned int m = millis();
  if (timerCount > 0) {
    portENTER_CRITICAL(&timerMux);
    timerCount--;
    portEXIT_CRITICAL(&timerMux);
    seconds++;
    go++;
    cpm = oldclicks - slicecount[seconds % 60];
    slicecount[seconds % 60] = oldclicks;
  }
  if (clicks > oldclicks) {
    m = millis();
    elapsed = m - oldmillis;
    oldclicks = clicks;
    oldmillis = m;
    go++;
  }

  if (go) {
    tft.setCursor(0, 0);
     tft.print("sec ");tft.print(seconds); tft.println("  ");
     tft.print("clk ");tft.print(clicks); tft.println("  ");
     tft.print("gap ");tft.print(elapsed); tft.println("  ");
     tft.print("cpm ");tft.print(cpm); tft.print("  ");
     dis(6,"abc ","12345");
    Serial.print(seconds%60);Serial.print(" ");Serial.print(oldclicks);Serial.print(" ");Serial.print(cpm);Serial.print(" ");Serial.println(elapsed);
  }
}

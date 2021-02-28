#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
volatile int timerCount;
int nseconds;
 
hw_timer_t * timer;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR timerHandler() {
  portENTER_CRITICAL_ISR(&timerMux);
  timerCount++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
 
void setup() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, timerHandler, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(0);
  tft.setCursor(0, 0, 1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(2); 
}
 
void loop() {
  if (timerCount) {
    portENTER_CRITICAL(&timerMux);
    timerCount--;
    portEXIT_CRITICAL(&timerMux);
    nseconds++;

    tft.setCursor(0, 0, 2);
    tft.print(nseconds);
  }
}

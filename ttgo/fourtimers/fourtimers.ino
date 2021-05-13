#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();

#define N 4


volatile int tCount[N];
int counter[N];
hw_timer_t * timer[N];
portMUX_TYPE tMux[N] = { portMUX_INITIALIZER_UNLOCKED,
                         portMUX_INITIALIZER_UNLOCKED,
                         portMUX_INITIALIZER_UNLOCKED,
                         portMUX_INITIALIZER_UNLOCKED,
                       };
void generalHandler(int i) {
  portENTER_CRITICAL_ISR(&tMux[i]);
  tCount[i]++;
  portEXIT_CRITICAL_ISR(&tMux[i]);
}
void IRAM_ATTR tHand0() { generalHandler(0); }
void IRAM_ATTR tHand1() { generalHandler(1); }
void IRAM_ATTR tHand2() { generalHandler(2); }
void IRAM_ATTR tHand3() { generalHandler(3); }
void (*handtable[])()= {tHand0, tHand1, tHand2, tHand3,};

void setup() {
  for (int i = 0; i < N; ++i) {
    timer[i] = timerBegin(i, 80, true);
    timerAttachInterrupt(timer[i], handtable[i], true);
    timerAlarmWrite(timer[i], 2000000/(1<<i), true);
    timerAlarmEnable(timer[i]);
  }

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(0);
  tft.setCursor(0, 0, 1);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(2); 
}

void loop() {
  int go = 0;
  for (int i = 0; i < N; ++i) {
    if (tCount[i]) {
      portENTER_CRITICAL(&tMux[i]);
      tCount[i]--;
      portEXIT_CRITICAL(&tMux[i]);
      counter[i]++;
      go = 1;
    }
  }
  if (go) {
    tft.setCursor(0, 0, 2);
    for (int i = 0; i < N; ++i) {
      tft.println(counter[i]);
    }
  }
}

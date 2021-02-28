#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup(void) {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(0);
  tft.setCursor(0, 0, 1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(2);
}

int linepix = 1;
int nrows = 4;

void t2(int x) {
  int wid = x % 200;
  tft.setCursor(0, 0, 2);
  tft.print(x,BIN);
  for (int row = 0; row < nrows; ++row) {
    if (wid == 0)
      tft.fillRect(0, 50+row*linepix, 200, linepix, TFT_BLACK);
    tft.fillRect(0, 50+row*linepix, wid, linepix, TFT_WHITE);
  }
}

void loop() {
  for (int i = 0; i <= 0xafff; ++i)
    t2(i);
}

// display

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

// 1 40 17
// 2 20 8.5
// 3 13 5.5
// 4 10 4.5
// 5 8 3.5
// 6 6 3
// 7 5 2.5

void dsetup(int rotation, int fg, int bg) {
  tft.init();
  tft.setRotation(rotation);
  tft.fillScreen(0);
  tft.setTextColor(fg, bg);
}
void dshow(int textsize, int row, const char* s1, const char* v) {
  tft.setCursor(row, 0, textsize);
  tft.print(s1);
  tft.print(" ");
  int len = strlen(s1) + 1 + strlen(v);
  tft.print(v);
}
void dshow(int textsize, int row, char* s1, int v) {
  char s[20];
  dshow(textsize, row, s1, sprintf(s, "%d", v));
}
void dshow(int textsize, int row, char* s1, double v) {
  char s[20];
  dshow(textsize, row, s1, sprintf(s, "%f", v));
}

void setup() {
  dsetup(3, TFT_GREEN, TFT_BLACK);
}

void loop() {
  int size = 3;
  dshow(size, 0, "clk", "abcd");
  //dshow(size, 1, "gap", .1234);
  //dshow(size, 2, "cpm", 3);
  delay(100000);
}

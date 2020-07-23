#include <SimpleKalmanFilter.h>

#define NCELLS 4

const int cellPins[NCELLS] = { A0, A1, A2, A3 };
// TODO: add dividers:
// R: 100k,33k; 100k,47k; 100k,100k
// R: 10k,3.3k; 10k,5.1k; 10k,10k
const double dividers[NCELLS] = { 1,1,1,1};
SimpleKalmanFilter filtered[NCELLS] = {
  SimpleKalmanFilter(.1, .1, 0.01),
  SimpleKalmanFilter(.1, .1, 0.01),
  SimpleKalmanFilter(.1, .1, 0.01),
  SimpleKalmanFilter(.1, .1, 0.01),
};

void setup() {
  Serial.begin(115200);
}

#define P(x) Serial.print(x)
void loop() {
  int i;
  float total;
  for (total = 0, i = 0; i < NCELLS; ++i) {
    int raw = analogRead(cellPins[i]);
    //int iconverted = map(raw, 0, 1023, 0, 500);
    float converted = map(raw, 0, 1023, 0, 500)/100.0;
    float f = filtered[i].updateEstimate(converted);
    total += converted;
    P("cel"); P(i); P(":"); P(f); P(" ");
    P("cel"); P(i); P("/"); P(f); P(" ");
  }
  P("total"); P("="); P(total); P(" ");
  P("\n");
  delay(100);
}

//  red green blink

const int redPin = 5;
const int greenPin = 6;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);  
}

void d() { delay(1000); }

void loop() {
  digitalWrite(redPin, HIGH);
  analogWrite(greenPin, 0);
  d();
  digitalWrite(redPin, LOW);
  analogWrite(greenPin, 10);
  d();
}

void loop2() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  d();
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  d();
}

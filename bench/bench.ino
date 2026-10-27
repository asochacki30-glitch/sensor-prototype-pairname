// bench.ino: ultrasonic distance sensor for the bench prototype.
//
// It prints one distance reading per line over USB in centimeters:
//
//   cm 42.5
//   cm 21.4
//
// The meaning lives in the browser: bench/sense-serial.js turns the distance
// into signals, and bench/table-bench.js decides whether the space is close,
// present, or empty.
//
// Wiring for an HC-SR04 ultrasonic sensor on an Arduino Nano:
//   VCC -> 5V    GND -> GND    TRIG -> D9    ECHO -> D10
//
// The sketch measures the echo delay and reports 400.0 cm when nothing is in
// range, so the browser can treat it as a clear room rather than a broken wire.

const int TRIG = 9;
const int ECHO = 10;

void setup() {
  Serial.begin(115200);          // sense-serial.js opens the port at this speed
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // The echo time in microseconds; 0 means nothing came back within 30 ms.
  unsigned long us = pulseIn(ECHO, HIGH, 30000);
  float cm = us ? us / 58.0 : 400.0; // nothing in range reads as far away

  Serial.print("cm ");
  Serial.print(cm, 1);
  Serial.println();

  delay(50);                     // about 20 readings a second
}

// bench.ino: ultrasonic distance sensor with an SSD1306 OLED display.
//
// It reads the HC-SR04 distance, shows whether the space is near or clear on
// the OLED, and prints the same data over USB for debugging and the browser.
//
// Wiring for an HC-SR04 ultrasonic sensor on an Arduino Nano:
//   VCC -> 5V    GND -> GND    TRIG -> D9    ECHO -> D10
//
// OLED wiring (I2C):
//   VCC -> 5V    GND -> GND    SDA -> A4    SCL -> A5
//
// The sketch measures the echo delay and reports 400.0 cm when nothing is in
// range, so the bench can treat it as a clear room rather than a broken wire.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int TRIG = 9;
const int ECHO = 10;
const int LINE = 30;     // cm threshold for the response
const int GAP = 5;       // hysteresis to stop flicker

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool near = false;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true) {}
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sensor Ready");
  display.display();
  delay(1000);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  unsigned long us = pulseIn(ECHO, HIGH, 30000);
  float cm = us ? us / 58.0 : 400.0;

  if (cm < LINE - GAP) near = true;
  if (cm > LINE + GAP) near = false;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  if (near) {
    display.println("NEAR");
    display.setTextSize(1);
    display.setCursor(0, 28);
    display.print("Distance: ");
    display.print(cm, 1);
    display.println(" cm");
    display.setCursor(0, 48);
    display.println("BLOCKED");
  } else {
    display.println("CLEAR");
    display.setTextSize(1);
    display.setCursor(0, 28);
    display.print("Distance: ");
    display.print(cm, 1);
    display.println(" cm");
    display.setCursor(0, 48);
    display.println("OPEN");
  }

  display.display();

  Serial.print("cm ");
  Serial.print(cm, 1);
  Serial.print(" near ");
  Serial.println(near ? 1 : 0);

  delay(50);
}

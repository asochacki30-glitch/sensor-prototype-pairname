// bench.ino: ultrasonic distance sensor with a blue LED and OLED display.
//
// States:
//   > 50 cm = ignore / CLEAR
//   50 to 30 cm = NOTICED
//   30 to 10 cm = HEAR
//   <= 10 cm = UPCLOSE
//
// The blue LED blinks once every second while a person is within the active
// sensing range (50 cm or closer). Beyond 50 cm the LED stays off.
//
// Wiring for an HC-SR04 ultrasonic sensor on an Arduino Nano:
//   VCC -> 5V    GND -> GND    TRIG -> D9    ECHO -> D10
//
// OLED wiring (I2C):
//   VCC -> 5V    GND -> GND    SDA -> A4    SCL -> A5
//
// Blue LED wiring:
//   long leg -> D5
//   short leg -> 220 ohm resistor -> GND

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int TRIG = 9;
const int ECHO = 10;
const int LED = 5;

const float IGNORE_DISTANCE = 50.0;  // ignore anything farther than 50 cm
const float NOTICE_LIMIT = 30.0;
const float HEAR_LIMIT = 10.0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool ledOn = false;
unsigned long lastBlink = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

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

  String state = "CLEAR";
  bool active = false;

  if (cm <= IGNORE_DISTANCE) {
    if (cm > NOTICE_LIMIT) {
      state = "NOTICED";
      active = true;
    } else if (cm > HEAR_LIMIT) {
      state = "HEAR";
      active = true;
    } else {
      state = "UPCLOSE";
      active = true;
    }
  } else {
    state = "CLEAR";
    active = false;
  }

  if (active) {
    unsigned long now = millis();
    if (now - lastBlink >= 1000) {
      ledOn = !ledOn;
      lastBlink = now;
    }
  } else {
    ledOn = false;
    lastBlink = millis();
  }

  digitalWrite(LED, ledOn ? HIGH : LOW);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(state);

  display.setTextSize(1);
  display.setCursor(0, 28);
  display.print("Distance: ");
  display.print(cm, 1);
  display.println(" cm");

  display.setCursor(0, 48);
  display.println(active ? "ACTIVE" : "IGNORE");
  display.display();

  Serial.print("cm ");
  Serial.print(cm, 1);
  Serial.print(" state ");
  Serial.println(state);

  delay(50);
}

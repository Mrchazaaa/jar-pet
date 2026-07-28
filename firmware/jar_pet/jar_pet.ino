#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "LedMatrix.h"
#include "MatrixAnimation.h"
#include "animations/Rainbow.h"
#include "animations/Heart.h"
#include "animations/DiagonalSweep.h"
#include "animations/Sparkle.h"

Adafruit_NeoPixel onboardPixel(
  STATUS_PIXEL_COUNT,
  PIN_NEOPIXEL,
  NEO_GRB + NEO_KHZ800
);

Adafruit_NeoPixel ledStrip(
  LED_STRIP_COUNT,
  LED_STRIP_PIN,
  NEO_GRB + NEO_KHZ800
);

LedMatrix ledMatrix(ledStrip);
const MatrixAnimation &selectedAnimation = HeartAnimation;

bool stripEnabled = false;
bool sensorArmed = true;

unsigned long lastSensorLogAt = 0;
unsigned long nextAnimationFrameAt = 0;

void turnOffOnboardLed() {
  onboardPixel.clear();
  onboardPixel.show();
}

void clearLedMatrix() {
  ledMatrix.clear();
  ledMatrix.show();
}

void resetSelectedAnimation() {
  nextAnimationFrameAt = 0;
  resetAnimation(selectedAnimation);
}

void logStripState() {
  Serial.print("strip=");
  Serial.println(stripEnabled ? "on" : "off");
}

void setStripEnabled(bool enabled) {
  stripEnabled = enabled;

  if (stripEnabled) {
    resetSelectedAnimation();
    Serial.print("LED strip enabled, animation=");
    Serial.println(selectedAnimation.name);
  } else {
    clearLedMatrix();
    Serial.println("LED strip disabled");
  }

  logStripState();
}

void logSensorReading(unsigned long now, int sensorValue) {
  if (now - lastSensorLogAt < SENSOR_LOG_INTERVAL_MS) {
    return;
  }

  lastSensorLogAt = now;

  Serial.print("sensor=");
  Serial.print(sensorValue);
  Serial.print(" armed=");
  Serial.print(sensorArmed ? "yes" : "no");
  Serial.print(" strip=");
  Serial.println(stripEnabled ? "on" : "off");
}

void updateTapToggle(int sensorValue) {
  if (sensorArmed && sensorValue >= TAP_HIT_THRESHOLD) {
    sensorArmed = false;

    Serial.print("Tap detected, sensor=");
    Serial.println(sensorValue);

    setStripEnabled(!stripEnabled);
    return;
  }

  if (!sensorArmed && sensorValue <= TAP_RESET_THRESHOLD) {
    sensorArmed = true;

    Serial.print("Sensor rearmed, sensor=");
    Serial.println(sensorValue);
  }
}

void updateLedAnimation(unsigned long now) {
  if (!stripEnabled || now < nextAnimationFrameAt) {
    return;
  }

  nextAnimationFrameAt = now + drawAnimationFrame(selectedAnimation, ledMatrix);
}

void setup() {
  Serial.begin(115200);
  pinMode(TAP_SENSOR_PIN, INPUT);

  onboardPixel.begin();
  onboardPixel.setBrightness(STATUS_PIXEL_BRIGHTNESS);
  turnOffOnboardLed();

  ledMatrix.begin(LED_STRIP_BRIGHTNESS);

  Serial.println("Jar Pet tap LED matrix toggle started");
  Serial.print("tap pin=");
  Serial.print(TAP_SENSOR_PIN);
  Serial.print(" hit threshold=");
  Serial.print(TAP_HIT_THRESHOLD);
  Serial.print(" reset threshold=");
  Serial.println(TAP_RESET_THRESHOLD);
  Serial.print("strip pin=");
  Serial.print(LED_STRIP_PIN);
  Serial.print(" matrix=");
  Serial.print(LED_MATRIX_WIDTH);
  Serial.print("x");
  Serial.print(LED_MATRIX_HEIGHT);
  Serial.print(" count=");
  Serial.println(LED_STRIP_COUNT);
  Serial.print("selected animation=");
  Serial.println(selectedAnimation.name);
}

void loop() {
  const unsigned long now = millis();
  const int sensorValue = analogRead(TAP_SENSOR_PIN);

  logSensorReading(now, sensorValue);
  updateTapToggle(sensorValue);
  updateLedAnimation(now);

  delay(2);
}

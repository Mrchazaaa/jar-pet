#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "LedMatrix.h"
#include "MatrixAnimation.h"
#include "animations/Rainbow.h"
#include "animations/Heart.h"
#include "animations/DiagonalSweep.h"
#include "animations/Sparkle.h"
#include "animations/VerticalSweep.h"
#include "animations/BarbershopPole.h"

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
const MatrixAnimation *const Animations[] = {
  &RainbowAnimation,
  &HeartAnimation,
  &DiagonalSweepAnimation,
  &SparkleAnimation,
  &VerticalSweepAnimation,
  &BarbershopPoleAnimation
};
const uint8_t AnimationCount = static_cast<uint8_t>(
  sizeof(Animations) / sizeof(Animations[0])
);
uint8_t selectedAnimationIndex = 4;

bool stripEnabled = false;
bool sensorArmed = true;
bool tapResetPending = false;
bool singleTapPending = false;

unsigned long lastSensorLogAt = 0;
unsigned long lastTapDetectedAt = 0;
unsigned long tapResetStartedAt = 0;
unsigned long singleTapDetectedAt = 0;
unsigned long nextAnimationFrameAt = 0;

const MatrixAnimation &currentAnimation() {
  return *Animations[selectedAnimationIndex];
}

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
  resetAnimation(currentAnimation());
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
    Serial.println(currentAnimation().name);
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

void skipToNextAnimation() {
  selectedAnimationIndex = (selectedAnimationIndex + 1) % AnimationCount;
  resetSelectedAnimation();

  Serial.print("Skipped to animation=");
  Serial.println(currentAnimation().name);
}

void handleTapDetected(unsigned long now, int sensorValue) {
  Serial.print("Tap detected, sensor=");
  Serial.println(sensorValue);

  if (singleTapPending && now - singleTapDetectedAt <= TAP_DOUBLE_TAP_MS) {
    singleTapPending = false;

    Serial.println("Double tap detected");
    skipToNextAnimation();
    return;
  }

  singleTapPending = true;
  singleTapDetectedAt = now;
}

void updatePendingSingleTap(unsigned long now) {
  if (!singleTapPending || now - singleTapDetectedAt <= TAP_DOUBLE_TAP_MS) {
    return;
  }

  singleTapPending = false;
  setStripEnabled(!stripEnabled);
}

void updateTapInput(unsigned long now, int sensorValue) {
  if (sensorArmed) {
    if (sensorValue >= TAP_HIT_THRESHOLD) {
      sensorArmed = false;
      tapResetPending = false;
      lastTapDetectedAt = now;

      handleTapDetected(now, sensorValue);
    }

    return;
  }

  if (now - lastTapDetectedAt < TAP_DEBOUNCE_MS) {
    return;
  }

  if (sensorValue > TAP_RESET_THRESHOLD) {
    tapResetPending = false;
    return;
  }

  if (!tapResetPending) {
    tapResetPending = true;
    tapResetStartedAt = now;
    return;
  }

  if (now - tapResetStartedAt < TAP_RESET_STABLE_MS) {
    return;
  }

  sensorArmed = true;
  tapResetPending = false;

  Serial.print("Sensor rearmed, sensor=");
  Serial.println(sensorValue);
}

void updateLedAnimation(unsigned long now) {
  if (!stripEnabled || now < nextAnimationFrameAt) {
    return;
  }

  nextAnimationFrameAt = now + drawAnimationFrame(currentAnimation(), ledMatrix);
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
  Serial.print(TAP_RESET_THRESHOLD);
  Serial.print(" debounce ms=");
  Serial.print(TAP_DEBOUNCE_MS);
  Serial.print(" reset stable ms=");
  Serial.print(TAP_RESET_STABLE_MS);
  Serial.print(" double tap ms=");
  Serial.println(TAP_DOUBLE_TAP_MS);
  Serial.print("strip pin=");
  Serial.print(LED_STRIP_PIN);
  Serial.print(" matrix=");
  Serial.print(LED_MATRIX_WIDTH);
  Serial.print("x");
  Serial.print(LED_MATRIX_HEIGHT);
  Serial.print(" count=");
  Serial.println(LED_STRIP_COUNT);
  Serial.print("selected animation=");
  Serial.println(currentAnimation().name);
}

void loop() {
  const unsigned long now = millis();
  const int sensorValue = analogRead(TAP_SENSOR_PIN);

  logSensorReading(now, sensorValue);
  updatePendingSingleTap(now);
  updateTapInput(now, sensorValue);
  updateLedAnimation(now);

  delay(2);
}

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
#include "animations/Face.h"
#include "animations/HappyFace.h"

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

/**
* The idle animation is shown when the jar is not being interacted with.
* Replace FaceAnimation/HappyFaceAnimation with any exported MatrixAnimation to change display.
*/
const MatrixAnimation &idleAnimation = FaceAnimation;
const MatrixAnimation &sensorAnimation = HappyFaceAnimation;

bool sensorArmed = true;
bool stripEnabled = false;
bool sensorAnimationActive = false;

unsigned long lastSensorLogAt = 0;
unsigned long lastMatrixActivityAt = 0;
unsigned long nextAnimationFrameAt = 0;
unsigned long sensorAnimationEndsAt = 0;
const MatrixAnimation *activeAnimation = &idleAnimation;

void turnOffOnboardLed() {
  onboardPixel.clear();
  onboardPixel.show();
}

void clearLedMatrix() {
  ledMatrix.clear();
  ledMatrix.show();
}

void setActiveAnimation(const MatrixAnimation &animation) {
  activeAnimation = &animation;
  nextAnimationFrameAt = 0;
  resetAnimation(animation);
}

void logStripState() {
  Serial.print("strip=");
  Serial.println(stripEnabled ? "on" : "off");
}

void setStripEnabled(bool enabled) {
  if (stripEnabled == enabled) {
    return;
  }

  stripEnabled = enabled;

  if (stripEnabled) {
    setActiveAnimation(idleAnimation);
    Serial.println("LED strip enabled");
  } else {
    clearLedMatrix();
    Serial.println("LED strip disabled after inactivity");
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
  Serial.print(stripEnabled ? "on" : "off");
  Serial.print(" animation=");
  Serial.println(activeAnimation->name);
}

void triggerTimedAnimation(unsigned long now, const MatrixAnimation &animation, unsigned long durationMs) {
  sensorAnimationActive = true;
  sensorAnimationEndsAt = now + durationMs;
  setActiveAnimation(animation);
  Serial.print("Sensor animation active for ms=");
  Serial.println(durationMs);
}

void triggerSensorAnimation(unsigned long now) {
  lastMatrixActivityAt = now;
  setStripEnabled(true);
  triggerTimedAnimation(now, sensorAnimation, SENSOR_ANIMATION_DURATION_MS);
  Serial.print("Sensor animation=");
  Serial.println(sensorAnimation.name);
}

void updateTapTrigger(unsigned long now, int sensorValue) {
  if (sensorArmed && sensorValue >= TAP_HIT_THRESHOLD) {
    sensorArmed = false;

    Serial.print("Tap detected, sensor=");
    Serial.println(sensorValue);

    triggerSensorAnimation(now);
    return;
  }

  if (!sensorArmed && sensorValue <= TAP_RESET_THRESHOLD) {
    sensorArmed = true;

    Serial.print("Sensor rearmed, sensor=");
    Serial.println(sensorValue);
  }
}

void updateActiveAnimation(unsigned long now) {
  if (stripEnabled && !sensorAnimationActive && now - lastMatrixActivityAt >= MATRIX_IDLE_TIMEOUT_MS) {
    setStripEnabled(false);
    return;
  }

  if (!stripEnabled) {
    return;
  }

  if (sensorAnimationActive && static_cast<long>(now - sensorAnimationEndsAt) >= 0) {
    sensorAnimationActive = false;
    setActiveAnimation(idleAnimation);
    Serial.println("Returning to idle animation");
  }

  if (now < nextAnimationFrameAt) {
    return;
  }

  nextAnimationFrameAt = now + drawAnimationFrame(*activeAnimation, ledMatrix);
}

void setup() {
  Serial.begin(115200);
  pinMode(TAP_SENSOR_PIN, INPUT);

  onboardPixel.begin();
  onboardPixel.setBrightness(STATUS_PIXEL_BRIGHTNESS);
  turnOffOnboardLed();

  ledMatrix.begin(LED_STRIP_BRIGHTNESS);
  lastMatrixActivityAt = millis();
  setStripEnabled(true);

  Serial.println("Jar Pet animation controller started");
  Serial.print("tap pin=");
  Serial.print(TAP_SENSOR_PIN);
  Serial.print(" hit threshold=");
  Serial.print(TAP_HIT_THRESHOLD);
  Serial.print(" reset threshold=");
  Serial.println(TAP_RESET_THRESHOLD);
  Serial.print(" sensor animation duration ms=");
  Serial.println(SENSOR_ANIMATION_DURATION_MS);
  Serial.print(" idle timeout ms=");
  Serial.println(MATRIX_IDLE_TIMEOUT_MS);
  Serial.print("strip pin=");
  Serial.print(LED_STRIP_PIN);
  Serial.print(" matrix=");
  Serial.print(LED_MATRIX_WIDTH);
  Serial.print("x");
  Serial.print(LED_MATRIX_HEIGHT);
  Serial.print(" count=");
  Serial.println(LED_STRIP_COUNT);
  Serial.print("idle animation=");
  Serial.println(idleAnimation.name);
  Serial.print("sensor animation=");
  Serial.println(sensorAnimation.name);
}

void loop() {
  const unsigned long now = millis();
  const int sensorValue = analogRead(TAP_SENSOR_PIN);

  logSensorReading(now, sensorValue);
  updateTapTrigger(now, sensorValue);
  updateActiveAnimation(now);

  delay(2);
}

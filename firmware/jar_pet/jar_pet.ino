#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

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

bool stripEnabled = false;
bool sensorArmed = true;

unsigned long lastSensorLogAt = 0;
unsigned long nextStripFrameAt = 0;
uint16_t rainbowOffset = 0;

void turnOffOnboardLed() {
  onboardPixel.clear();
  onboardPixel.show();
}

void clearLedStrip() {
  ledStrip.clear();
  ledStrip.show();
}

void resetLedAnimation() {
  nextStripFrameAt = 0;
  rainbowOffset = 0;
}

void logStripState() {
  Serial.print("strip=");
  Serial.println(stripEnabled ? "on" : "off");
}

void setStripEnabled(bool enabled) {
  stripEnabled = enabled;

  if (stripEnabled) {
    resetLedAnimation();
    Serial.println("LED strip enabled");
  } else {
    clearLedStrip();
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

void drawRainbowFrame() {
  for (uint16_t i = 0; i < ledStrip.numPixels(); i++) {
    const uint16_t hue = static_cast<uint16_t>(
      (static_cast<uint32_t>(i) * 65536UL / ledStrip.numPixels()) +
      (static_cast<uint32_t>(rainbowOffset) * 256UL)
    );

    ledStrip.setPixelColor(i, ledStrip.gamma32(ledStrip.ColorHSV(hue)));
  }

  ledStrip.show();
  rainbowOffset++;
}

void updateLedAnimation(unsigned long now) {
  if (!stripEnabled || now < nextStripFrameAt) {
    return;
  }

  drawRainbowFrame();
  nextStripFrameAt = now + LED_ANIMATION_FRAME_MS;
}

void setup() {
  Serial.begin(115200);
  pinMode(TAP_SENSOR_PIN, INPUT);

  onboardPixel.begin();
  onboardPixel.setBrightness(STATUS_PIXEL_BRIGHTNESS);
  turnOffOnboardLed();

  ledStrip.begin();
  ledStrip.setBrightness(LED_STRIP_BRIGHTNESS);
  clearLedStrip();

  Serial.println("Jar Pet tap LED strip toggle started");
  Serial.print("tap pin=");
  Serial.print(TAP_SENSOR_PIN);
  Serial.print(" hit threshold=");
  Serial.print(TAP_HIT_THRESHOLD);
  Serial.print(" reset threshold=");
  Serial.println(TAP_RESET_THRESHOLD);
  Serial.print("strip pin=");
  Serial.print(LED_STRIP_PIN);
  Serial.print(" count=");
  Serial.println(LED_STRIP_COUNT);
}

void loop() {
  const unsigned long now = millis();
  const int sensorValue = analogRead(TAP_SENSOR_PIN);

  logSensorReading(now, sensorValue);
  updateTapToggle(sensorValue);
  updateLedAnimation(now);

  delay(2);
}

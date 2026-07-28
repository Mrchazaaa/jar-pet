#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

Adafruit_NeoPixel pixel(
  STATUS_PIXEL_COUNT,
  PIN_NEOPIXEL,
  NEO_GRB + NEO_KHZ800
);

unsigned long greenStartTime = 0;
unsigned long lastBlinkTime = 0;

bool showingGreen = false;
bool sensorArmed = true;
bool redIsOn = false;

void setPixel(uint8_t red, uint8_t green, uint8_t blue) {
  pixel.setPixelColor(0, pixel.Color(red, green, blue));
  pixel.show();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIEZO_PIN, INPUT);

  pixel.begin();
  pixel.setBrightness(STATUS_PIXEL_BRIGHTNESS);
  pixel.clear();
  pixel.show();

  Serial.println("Piezo detector started");
}

void loop() {
  unsigned long now = millis();
  int sensorValue = analogRead(PIEZO_PIN);

  Serial.println(sensorValue);

  // Detect one hit.
  if (sensorArmed && sensorValue >= HIT_THRESHOLD) {
    sensorArmed = false;
    showingGreen = true;
    greenStartTime = now;

    setPixel(0, 255, 0);
    Serial.println("Hit detected!");
  }

  // Wait until the signal settles before allowing another hit.
  if (!sensorArmed && sensorValue <= RESET_THRESHOLD) {
    sensorArmed = true;
  }

  // Keep the light green for a fixed period.
  if (showingGreen) {
    if (now - greenStartTime >= GREEN_TIME) {
      showingGreen = false;
      redIsOn = false;
      lastBlinkTime = now;
      setPixel(0, 0, 0);
    }

    delay(2);
    return;
  }

  // Blink red when no hit is active.
  if (now - lastBlinkTime >= BLINK_TIME) {
    lastBlinkTime = now;
    redIsOn = !redIsOn;

    if (redIsOn) {
      setPixel(255, 0, 0);
    } else {
      setPixel(0, 0, 0);
    }
  }

  delay(2);
}
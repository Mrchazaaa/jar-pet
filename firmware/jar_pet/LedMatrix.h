#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

class LedMatrix {
public:
  explicit LedMatrix(Adafruit_NeoPixel &strip) : strip_(strip) {}

  void begin(uint8_t brightness) {
    strip_.begin();
    strip_.setBrightness(brightness);
    clear();
    show();
  }

  uint8_t width() const {
    return LED_MATRIX_WIDTH;
  }

  uint8_t height() const {
    return LED_MATRIX_HEIGHT;
  }

  uint16_t pixelCount() const {
    return LED_STRIP_COUNT;
  }

  uint16_t index(uint8_t x, uint8_t y) const {
    if (x >= width() || y >= height()) {
      return LED_STRIP_COUNT;
    }

    return static_cast<uint16_t>(y) * width() + x;
  }

  bool contains(uint8_t x, uint8_t y) const {
    return x < width() && y < height();
  }

  uint32_t color(uint8_t red, uint8_t green, uint8_t blue) const {
    return strip_.Color(red, green, blue);
  }

  uint32_t colorHSV(uint16_t hue, uint8_t saturation = 255, uint8_t value = 255) const {
    return strip_.gamma32(strip_.ColorHSV(hue, saturation, value));
  }

  void setPixel(uint8_t x, uint8_t y, uint32_t color) {
    const uint16_t pixel = index(x, y);

    if (pixel >= pixelCount()) {
      return;
    }

    strip_.setPixelColor(pixel, color);
  }

  void fill(uint32_t color) {
    strip_.fill(color);
  }

  void clear() {
    strip_.clear();
  }

  void show() {
    strip_.show();
  }

private:
  Adafruit_NeoPixel &strip_;
};

#endif

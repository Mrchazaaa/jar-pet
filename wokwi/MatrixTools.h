#ifndef MATRIX_TOOLS_H
#define MATRIX_TOOLS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define MATRIX_PIN 6
#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 6
#define NUM_PIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

extern Adafruit_NeoPixel matrix;

uint16_t xy(uint8_t x, uint8_t y) {
  return y * MATRIX_WIDTH + x;
}

void setPixel(uint8_t x, uint8_t y, uint32_t color) {
  if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT) {
    return;
  }
  matrix.setPixelColor(xy(x, y), color);
}

void clearMatrix() {
  for (uint8_t i = 0; i < NUM_PIXELS; i++) {
    matrix.setPixelColor(i, 0);
  }
}

#endif

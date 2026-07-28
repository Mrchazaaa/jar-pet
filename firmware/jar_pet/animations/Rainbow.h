#ifndef RAINBOW_H
#define RAINBOW_H

#include "../MatrixAnimation.h"

static uint16_t RainbowOffset = 0;

static void resetRainbow() {
  RainbowOffset = 0;
}

static unsigned long drawRainbowFrame(LedMatrix &matrix) {
  for (uint8_t y = 0; y < matrix.height(); y++) {
    for (uint8_t x = 0; x < matrix.width(); x++) {
      const uint16_t linearPosition = matrix.index(x, y);
      const uint16_t hue = static_cast<uint16_t>(
        (static_cast<uint32_t>(linearPosition) * 65536UL / matrix.pixelCount()) +
        (static_cast<uint32_t>(RainbowOffset) * 256UL)
      );

      matrix.setPixel(x, y, matrix.colorHSV(hue));
    }
  }

  matrix.show();
  RainbowOffset++;
  return LED_ANIMATION_FRAME_MS;
}

const MatrixAnimation RainbowAnimation = {
  "Rainbow",
  resetRainbow,
  drawRainbowFrame
};

#endif

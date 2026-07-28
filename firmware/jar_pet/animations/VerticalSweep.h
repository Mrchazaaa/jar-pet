#ifndef VERTICAL_SWEEP_H
#define VERTICAL_SWEEP_H

#include "../MatrixAnimation.h"

const uint8_t VerticalSweepTrailLength = 5;
static uint8_t VerticalSweepX = 0;
static uint16_t VerticalSweepHueOffset = 0;

static void resetVerticalSweep() {
  VerticalSweepX = 0;
  VerticalSweepHueOffset = 0;
}

static unsigned long drawVerticalSweepFrame(LedMatrix &matrix) {
  matrix.clear();

  for (uint8_t trail = 0; trail < VerticalSweepTrailLength; trail++) {
    const uint8_t x = (matrix.width() + VerticalSweepX - trail) % matrix.width();
    const uint8_t value = 255 - (trail * 42);
    const uint16_t hue = static_cast<uint16_t>(
      VerticalSweepHueOffset + (static_cast<uint16_t>(trail) * 5461U)
    );
    const uint32_t color = trail == 0
      ? matrix.color(255, 255, 255)
      : matrix.colorHSV(hue, 255, value);

    for (uint8_t y = 0; y < matrix.height(); y++) {
      matrix.setPixel(x, y, color);
    }
  }

  matrix.show();
  VerticalSweepX = (VerticalSweepX + 1) % matrix.width();
  VerticalSweepHueOffset += 1024;
  return 55;
}

const MatrixAnimation VerticalSweepAnimation = {
  "Vertical Sweep",
  resetVerticalSweep,
  drawVerticalSweepFrame
};

#endif

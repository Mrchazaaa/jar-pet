#ifndef DIAGONAL_SWEEP_H
#define DIAGONAL_SWEEP_H

#include "../MatrixAnimation.h"

static uint8_t DiagonalSweepOffset = 0;

static void resetDiagonalSweep() {
  DiagonalSweepOffset = 0;
}

static unsigned long drawDiagonalSweepFrame(LedMatrix &matrix) {
  matrix.clear();

  for (uint8_t y = 0; y < matrix.height(); y++) {
    const uint8_t x = (DiagonalSweepOffset + (y * 2)) % matrix.width();
    uint32_t color = 0;

    switch (y % 5) {
      case 0: color = matrix.color(255, 0, 0); break;
      case 1: color = matrix.color(255, 96, 0); break;
      case 2: color = matrix.color(255, 210, 0); break;
      case 3: color = matrix.color(0, 255, 0); break;
      default: color = matrix.color(0, 220, 255); break;
    }

    matrix.setPixel(x, y, color);
  }

  matrix.show();
  DiagonalSweepOffset = (DiagonalSweepOffset + 1) % matrix.width();
  return 90;
}

const MatrixAnimation DiagonalSweepAnimation = {
  "Diagonal Sweep",
  resetDiagonalSweep,
  drawDiagonalSweepFrame
};

#endif

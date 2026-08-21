#ifndef BARBERSHOP_POLE_H
#define BARBERSHOP_POLE_H

#include "../MatrixAnimation.h"

const uint8_t BarbershopPoleBandWidth = 3;
const uint8_t BarbershopPoleSlope = 2;
static uint8_t BarbershopPoleOffset = 0;

static void resetBarbershopPole() {
  BarbershopPoleOffset = 0;
}

static unsigned long drawBarbershopPoleFrame(LedMatrix &matrix) {
  const uint8_t cycleWidth = BarbershopPoleBandWidth * 3;

  for (uint8_t y = 0; y < matrix.height(); y++) {
    for (uint8_t x = 0; x < matrix.width(); x++) {
      const uint8_t phase = (
        x +
        ((matrix.height() - 1 - y) * BarbershopPoleSlope) +
        BarbershopPoleOffset
      ) % cycleWidth;

      uint32_t color = 0;

      if (phase < BarbershopPoleBandWidth) {
        color = matrix.color(255, 0, 0);
      } else if (phase < BarbershopPoleBandWidth * 2) {
        color = matrix.color(245, 245, 245);
      } else {
        color = matrix.color(0, 36, 255);
      }

      matrix.setPixel(x, y, color);
    }
  }

  matrix.show();
  BarbershopPoleOffset = (BarbershopPoleOffset + 1) % cycleWidth;
  return 45;
}

const MatrixAnimation BarbershopPoleAnimation = {
  "Barbershop Pole",
  resetBarbershopPole,
  drawBarbershopPoleFrame
};

#endif

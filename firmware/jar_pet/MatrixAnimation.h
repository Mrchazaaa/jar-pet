#ifndef MATRIX_ANIMATION_H
#define MATRIX_ANIMATION_H

#include <Arduino.h>
#include "LedMatrix.h"

typedef void (*AnimationResetFn)();
typedef unsigned long (*AnimationDrawFrameFn)(LedMatrix &matrix);

struct MatrixAnimation {
  const char *name;
  AnimationResetFn reset;
  AnimationDrawFrameFn drawFrame;
};

struct GlyphAnimationFrame {
  unsigned long durationMs;
  const char *rows[LED_MATRIX_HEIGHT];
};

inline void resetAnimation(const MatrixAnimation &animation) {
  if (animation.reset != nullptr) {
    animation.reset();
  }
}

inline unsigned long drawAnimationFrame(const MatrixAnimation &animation, LedMatrix &matrix) {
  return animation.drawFrame(matrix);
}

inline uint32_t animationColorForGlyph(LedMatrix &matrix, char glyph) {
  switch (glyph) {
    case 'R': return matrix.color(255, 0, 0);
    case 'G': return matrix.color(0, 255, 0);
    case 'B': return matrix.color(0, 0, 255);
    case 'Y': return matrix.color(255, 210, 0);
    case 'C': return matrix.color(0, 220, 255);
    case 'M': return matrix.color(255, 0, 220);
    case 'W': return matrix.color(255, 255, 255);
    case 'O': return matrix.color(255, 96, 0);
    case 'P': return matrix.color(160, 80, 255);
    default: return 0;
  }
}

inline void drawGlyphRows(LedMatrix &matrix, const char *const rows[], uint8_t rowCount) {
  matrix.clear();

  const uint8_t visibleRows = min(rowCount, matrix.height());
  for (uint8_t rowIndex = 0; rowIndex < visibleRows; rowIndex++) {
    const uint8_t y = matrix.height() - 1 - rowIndex;
    const char *row = rows[rowIndex];

    for (uint8_t x = 0; x < matrix.width() && row[x] != 0; x++) {
      matrix.setPixel(x, y, animationColorForGlyph(matrix, row[x]));
    }
  }

  matrix.show();
}

inline void drawGlyphFrame(LedMatrix &matrix, const GlyphAnimationFrame &frame) {
  drawGlyphRows(matrix, frame.rows, LED_MATRIX_HEIGHT);
}

#endif

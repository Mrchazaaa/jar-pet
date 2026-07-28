#ifndef FRAME_PLAYER_H
#define FRAME_PLAYER_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "MatrixTools.h"

struct MatrixFrame {
  uint16_t durationMs;
  char rows[MATRIX_HEIGHT][MATRIX_WIDTH + 1];
};

struct Animation {
  const char *name;
  const MatrixFrame *frames;
  uint8_t frameCount;
};

inline uint32_t colorForGlyph(char glyph) {
  switch (glyph) {
    case 'R': return matrix.Color(255, 0, 0);
    case 'G': return matrix.Color(0, 255, 0);
    case 'B': return matrix.Color(0, 0, 255);
    case 'Y': return matrix.Color(255, 210, 0);
    case 'C': return matrix.Color(0, 220, 255);
    case 'M': return matrix.Color(255, 0, 220);
    case 'W': return matrix.Color(255, 255, 255);
    case 'O': return matrix.Color(255, 96, 0);
    case 'P': return matrix.Color(160, 80, 255);
    default: return 0;
  }
}

inline uint16_t frameDuration(const MatrixFrame *frames, uint8_t index) {
  return pgm_read_word(&(frames[index].durationMs));
}

inline void drawFrame(const MatrixFrame *frames, uint8_t index) {
  clearMatrix();

  for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
      char glyph = pgm_read_byte(&(frames[index].rows[y][x]));
      setPixel(x, y, colorForGlyph(glyph));
    }
  }

  matrix.show();
}

#endif

#ifndef FACE_H
#define FACE_H

#include "../MatrixAnimation.h"

// A broad, simple smile remains legible through the curved jar.
const GlyphAnimationFrame FaceFrames[] = {
  {
    880,
    {
      "...............",
      "....CC..CC.....",
      "....CC..CC.....",
      "..C.........C..",
      "...CCCCCCCCC..."
    }
  },
  {
    120,
    {
      "...............",
      "...............",
      "....CC..CC.....",
      "..C.........C..",
      "...CCCCCCCCC..."
    }
  }
};

const uint8_t FaceFrameCount = sizeof(FaceFrames) / sizeof(FaceFrames[0]);
static uint8_t FaceFrameIndex = 0;

static void resetFace() {
  FaceFrameIndex = 0;
}

static unsigned long drawFaceFrame(LedMatrix &matrix) {
  const GlyphAnimationFrame &frame = FaceFrames[FaceFrameIndex];
  drawGlyphFrame(matrix, frame);

  FaceFrameIndex = (FaceFrameIndex + 1) % FaceFrameCount;
  return frame.durationMs;
}

const MatrixAnimation FaceAnimation = {
  "Blinking Face",
  resetFace,
  drawFaceFrame
};

#endif

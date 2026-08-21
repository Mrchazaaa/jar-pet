#ifndef HAPPY_FACE_H
#define HAPPY_FACE_H

#include "../MatrixAnimation.h"

// A brighter, lively expression for the short tap response. Each eye is a
// pixel ^ shape, making the expression read as happy at a glance.
const GlyphAnimationFrame HappyFaceFrames[] = {
  {
    180,
    {
      "....M....M.....",
      "...M.M..M.M....",
      "...............",
      "..Y.........Y..",
      "...MMMMMMMMM..."
    }
  },
  {
    180,
    {
      "....M....M.....",
      "...M.M..M.M....",
      ".Y...........Y.",
      "...............",
      "...MMMMMMMMM..."
    }
  }
};

const uint8_t HappyFaceFrameCount = sizeof(HappyFaceFrames) / sizeof(HappyFaceFrames[0]);
static uint8_t HappyFaceFrameIndex = 0;

static void resetHappyFace() {
  HappyFaceFrameIndex = 0;
}

static unsigned long drawHappyFaceFrame(LedMatrix &matrix) {
  const GlyphAnimationFrame &frame = HappyFaceFrames[HappyFaceFrameIndex];
  drawGlyphFrame(matrix, frame);

  HappyFaceFrameIndex = (HappyFaceFrameIndex + 1) % HappyFaceFrameCount;
  return frame.durationMs;
}

const MatrixAnimation HappyFaceAnimation = {
  "Excited Face",
  resetHappyFace,
  drawHappyFaceFrame
};

#endif

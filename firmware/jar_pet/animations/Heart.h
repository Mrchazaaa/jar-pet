#ifndef HEART_H
#define HEART_H

#include "../MatrixAnimation.h"

const GlyphAnimationFrame HeartFrames[] = {
  {
    180,
    {
      "...............",
      "......R.R......",
      ".....RRRRR.....",
      ".......R.......",
      "..............."
    }
  },
  {
    140,
    {
      ".....RR.RR.....",
      "....RRR.RRR....",
      ".....RRRRR.....",
      ".....RRRRR.....",
      "......RRR......"
    }
  }
};

const uint8_t HeartFrameCount = sizeof(HeartFrames) / sizeof(HeartFrames[0]);
static uint8_t HeartFrameIndex = 0;

static void resetHeart() {
  HeartFrameIndex = 0;
}

static unsigned long drawHeartFrame(LedMatrix &matrix) {
  const GlyphAnimationFrame &frame = HeartFrames[HeartFrameIndex];
  drawGlyphFrame(matrix, frame);

  HeartFrameIndex = (HeartFrameIndex + 1) % HeartFrameCount;
  return frame.durationMs;
}

const MatrixAnimation HeartAnimation = {
  "Heart",
  resetHeart,
  drawHeartFrame
};

#endif

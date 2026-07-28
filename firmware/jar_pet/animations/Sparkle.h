#ifndef SPARKLE_H
#define SPARKLE_H

#include "../MatrixAnimation.h"

const GlyphAnimationFrame SparkleFrames[] = {
  {
    120,
    {
      "W...C.....P....",
      "..Y.....W.....B",
      ".....M....G....",
      ".C......P......",
      "....W......Y..."
    }
  },
  {
    120,
    {
      "..C....W....Y..",
      ".....P......B..",
      ".Y......B......",
      "....W.....G....",
      "G......M......C"
    }
  },
  {
    120,
    {
      "....Y.....C....",
      "C......P.......",
      "...W......G....",
      "......G......W.",
      ".M......W.....B"
    }
  },
  {
    180,
    {
      "...............",
      "...W.....W.....",
      ".W...........W.",
      "......C........",
      "..P........P..."
    }
  }
};

const uint8_t SparkleFrameCount = sizeof(SparkleFrames) / sizeof(SparkleFrames[0]);
static uint8_t SparkleFrameIndex = 0;

static void resetSparkle() {
  SparkleFrameIndex = 0;
}

static unsigned long drawSparkleFrame(LedMatrix &matrix) {
  const GlyphAnimationFrame &frame = SparkleFrames[SparkleFrameIndex];
  drawGlyphFrame(matrix, frame);

  SparkleFrameIndex = (SparkleFrameIndex + 1) % SparkleFrameCount;
  return frame.durationMs;
}

const MatrixAnimation SparkleAnimation = {
  "Sparkle",
  resetSparkle,
  drawSparkleFrame
};

#endif

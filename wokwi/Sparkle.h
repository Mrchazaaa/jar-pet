#ifndef SPARKLE_H
#define SPARKLE_H

#include "FramePlayer.h"

const MatrixFrame SparkleFrames[] PROGMEM = {
  {
    120,
    {
      "W...C.....",
      "..P....Y..",
      ".....W....",
      ".B.....M..",
      "....G.....",
      "Y.......C."
    }
  },
  {
    120,
    {
      "..C....W..",
      ".....P....",
      ".Y......B.",
      "....W.....",
      "G......M..",
      "...C......"
    }
  },
  {
    120,
    {
      "....Y.....",
      "C......P..",
      "...W......",
      "......G...",
      ".M......W.",
      ".....B...."
    }
  },
  {
    180,
    {
      "..........",
      "...W..W...",
      ".W......W.",
      "....C.....",
      "..P....P..",
      ".........."
    }
  }
};

const Animation Sparkle = {
  "Sparkle",
  SparkleFrames,
  sizeof(SparkleFrames) / sizeof(SparkleFrames[0])
};

#endif

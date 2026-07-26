#ifndef HEART_H
#define HEART_H

#include "FramePlayer.h"

const MatrixFrame HeartFrames[] PROGMEM = {
  {
    180,
    {
      "..........",
      ".RR..RR...",
      "RRRRRRRR..",
      "RRRRRRRR..",
      ".RRRRRR...",
      "..RRRR...."
    }
  },
  {
    140,
    {
      "..........",
      ".MM..MM...",
      "MMMMMMMM..",
      "MMMMMMMM..",
      ".MMMMMM...",
      "..MMMM...."
    }
  },
  {
    220,
    {
      "..........",
      "..R..R....",
      ".RRRRRR...",
      ".RRRRRR...",
      "..RRRR....",
      "...RR....."
    }
  }
};

const Animation Heart = {
  "Heart",
  HeartFrames,
  sizeof(HeartFrames) / sizeof(HeartFrames[0])
};

#endif

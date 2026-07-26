#ifndef DIAGONAL_SWEEP_H
#define DIAGONAL_SWEEP_H

#include "FramePlayer.h"

const MatrixFrame DiagonalSweepFrames[] PROGMEM = {
  {
    90,
    {
      "R.........",
      ".O........",
      "..Y.......",
      "...G......",
      "....C.....",
      ".....B...."
    }
  },
  {
    90,
    {
      ".R........",
      "..O.......",
      "...Y......",
      "....G.....",
      ".....C....",
      "......B..."
    }
  },
  {
    90,
    {
      "..R.......",
      "...O......",
      "....Y.....",
      ".....G....",
      "......C...",
      ".......B.."
    }
  },
  {
    90,
    {
      "...R......",
      "....O.....",
      ".....Y....",
      "......G...",
      ".......C..",
      "........B."
    }
  },
  {
    90,
    {
      "....R.....",
      ".....O....",
      "......Y...",
      ".......G..",
      "........C.",
      ".........B"
    }
  },
  {
    90,
    {
      ".....R....",
      "......O...",
      ".......Y..",
      "........G.",
      ".........C",
      "B........."
    }
  }
};

const Animation DiagonalSweep = {
  "Diagonal Sweep",
  DiagonalSweepFrames,
  sizeof(DiagonalSweepFrames) / sizeof(DiagonalSweepFrames[0])
};

#endif

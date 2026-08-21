# Jar Pet Arduino Project

Arduino firmware for the Jar Pet project. The firmware drives a 15x5 strip-backed LED matrix, reads a tap sensor, and toggles the selected animation on and off.

## Firmware Structure

- `firmware/jar_pet/jar_pet.ino` is the primary Arduino sketch to open in Arduino IDE.
- `firmware/jar_pet/config.h` contains pin, sensor threshold, timing, matrix size, and brightness settings.
- `firmware/jar_pet/LedMatrix.h` maps the physical LED strip into `(x, y)` matrix coordinates.
- `firmware/jar_pet/MatrixAnimation.h` defines the shared animation interface and glyph-frame helpers.
- `firmware/jar_pet/animations/` contains one standalone animation header per animation.

## Hardware Settings

Current defaults:

- Tap sensor: `A0`
- LED strip data pin: `9`
- Matrix size: `15` columns by `5` rows
- Matrix origin: `(0, 0)` is the bottom-left LED, strip pixel `0`

## Selecting An Animation

The main sketch hardcodes the selected animation:

```cpp
#include "animations/Rainbow.h"

const MatrixAnimation &selectedAnimation = RainbowAnimation;
```

To use a different animation, include its header and assign its exported `MatrixAnimation` object:

```cpp
#include "animations/Heart.h"

const MatrixAnimation &selectedAnimation = HeartAnimation;
```

## Creating An Animation

Create one header per animation in `firmware/jar_pet/animations/`. Include `../MatrixAnimation.h`, define a reset function, define a draw function that returns the delay until the next frame, and expose a `const MatrixAnimation`.

```cpp
#ifndef SCAN_H
#define SCAN_H

#include "../MatrixAnimation.h"

static uint8_t ScanX = 0;

static void resetScan() {
  ScanX = 0;
}

static unsigned long drawScanFrame(LedMatrix &matrix) {
  matrix.clear();

  for (uint8_t y = 0; y < matrix.height(); y++) {
    matrix.setPixel(ScanX, y, matrix.color(0, 0, 255));
  }

  matrix.show();
  ScanX = (ScanX + 1) % matrix.width();
  return 40;
}

const MatrixAnimation ScanAnimation = {
  "Scan",
  resetScan,
  drawScanFrame
};

#endif
```

For hardcoded glyph frames, use `GlyphAnimationFrame` and `drawGlyphFrame()` from `MatrixAnimation.h`. Rows are written top-to-bottom, and each row should be 15 characters wide.

# Jar Pet Arduino Project

Arduino firmware for a 15x5 strip-backed LED matrix with a tap sensor. It runs an idle animation, temporarily switches to a sensor animation after a tap, then returns to idle. After five minutes without a tap, the matrix clears; the next tap wakes it.

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

## Selecting Animations

The main sketch has two interchangeable animation slots:

```cpp
const MatrixAnimation &idleAnimation = FaceAnimation;
const MatrixAnimation &sensorAnimation = HappyFaceAnimation;
```

The supplied headers are already included in `jar_pet.ino`, so swap either line to select a different built-in animation. For example, use Rainbow while idle and retain the happy face for taps:

```cpp
const MatrixAnimation &idleAnimation = RainbowAnimation;
const MatrixAnimation &sensorAnimation = HappyFaceAnimation;
```

You can use the same animation in both slots, such as `RainbowAnimation`, or mix any exported `MatrixAnimation` from `firmware/jar_pet/animations/`. When adding a new animation, include its header in `jar_pet.ino`, then assign its exported animation object to either slot.

The sensor response length and inactivity timeout are set in `config.h` as `SENSOR_ANIMATION_DURATION_MS` and `MATRIX_IDLE_TIMEOUT_MS`.

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

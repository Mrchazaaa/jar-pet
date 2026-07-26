# Wokwi 6x10 RGB Matrix

Static web project for a 6 row by 10 column RGB LED matrix animation. The Arduino sketch and Wokwi circuit files live in `wokwi/`; the served page reads the same Arduino animation headers so it can preview the animation immediately in a browser.

## Run

```sh
npm start
```

Open `http://localhost:4173`.

The server binds to `0.0.0.0` by default, so other devices on the same network can open it with this machine's LAN IP:

```sh
hostname -I
```

Then visit `http://<lan-ip>:4173` from the other device.

## Wokwi Files

- `wokwi/sketch.ino` contains the Arduino Uno NeoPixel animation sketch.
- `wokwi/diagram.json` defines an Arduino Uno connected to a 6x10 Wokwi WS2812 LED matrix on pin 6.
- `wokwi/libraries.txt` declares the Adafruit NeoPixel dependency for Wokwi.

## Project Structure

Browser preview:

- `public/app.js` controls timing, playback, file tabs, and animation selection.
- `public/matrix-renderer.js` owns the 6x10 pixel buffer and DOM rendering.
- `public/frame-player.js` parses Arduino animation headers for the browser preview.
- `public/animation-manifest.js` lists the animation headers loaded by the browser.
- `public/matrix-config.js` contains the shared browser matrix size.

Wokwi/Arduino:

- `wokwi/sketch.ino` controls setup, timing, and animation selection.
- `wokwi/MatrixTools.h` contains shared matrix constants and helpers.
- `wokwi/FramePlayer.h` renders frame-grid animation data.
- `wokwi/Heart.h`, `wokwi/DiagonalSweep.h`, etc. contain one standalone animation per file.

## Designing An Animation

Every animation is a list of frames. Each frame has a duration in milliseconds and six rows of ten characters. Each character is one LED.

Available color characters:

- `.` off
- `R` red
- `G` green
- `B` blue
- `Y` yellow
- `C` cyan
- `M` magenta
- `W` white
- `O` orange
- `P` purple

Create one header per animation in `wokwi/`:

```cpp
#ifndef SMILE_H
#define SMILE_H

#include "FramePlayer.h"

const MatrixFrame SmileFrames[] PROGMEM = {
  {
    160,
    {
      "..........",
      "..Y....Y..",
      "..........",
      ".Y......Y.",
      "..YYYYYY..",
      ".........."
    }
  }
};

const Animation Smile = {
  "Smile",
  SmileFrames,
  sizeof(SmileFrames) / sizeof(SmileFrames[0])
};

#endif
```

Then:

- Include it in `wokwi/sketch.ino`.
- Add it to the `animations[]` array in `wokwi/sketch.ino`.
- Add its path to `public/animation-manifest.js` so the browser preview loads it.

The browser reads these same `.h` files, so the Arduino data is the source of truth.

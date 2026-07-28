# Jar Pet Arduino Project

Arduino firmware for the Jar Pet project, with a small Wokwi/browser matrix harness for experimenting with display animation data.

The real firmware is kept under `firmware/jar_pet/`. The Wokwi sketch is a simulator target that includes shared firmware headers instead of owning a separate implementation.

## Run The Browser Preview

```sh
npm start
```

Open `http://localhost:4173`.

The server binds to `0.0.0.0` by default, so other devices on the same network can open it with this machine's LAN IP:

```sh
hostname -I
```

Then visit `http://<lan-ip>:4173` from the other device.

## Project Structure

Firmware:

- `firmware/jar_pet/jar_pet.ino` is the primary Arduino sketch.
- `firmware/jar_pet/config.h` contains the real firmware pin, threshold, timing, and brightness settings.
- `firmware/jar_pet/MatrixTools.h` contains reusable matrix constants and helpers.
- `firmware/jar_pet/FramePlayer.h` renders frame-grid animation data.
- `firmware/jar_pet/animations/` contains one standalone animation header per animation.

Wokwi simulator target:

- `wokwi/sketch.ino` is a thin simulator sketch that includes shared headers from `firmware/jar_pet/`.
- `wokwi/config.h` contains Wokwi-specific matrix pin and size settings.
- `wokwi/diagram.json` defines an Arduino Uno connected to a 6x10 Wokwi WS2812 LED matrix on pin 6.
- `wokwi/libraries.txt` declares the Adafruit NeoPixel dependency for Wokwi.

Browser preview:

- `server.js` serves the preview UI and source files.
- `public/app.js` controls timing, playback, file tabs, and animation selection.
- `public/matrix-renderer.js` owns the 6x10 pixel buffer and DOM rendering.
- `public/frame-player.js` parses Arduino animation headers for the browser preview.
- `public/animation-manifest.js` lists the animation headers loaded by the browser.
- `public/matrix-config.js` contains the browser matrix size.

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

Create one header per animation in `firmware/jar_pet/animations/`:

```cpp
#ifndef SMILE_H
#define SMILE_H

#include "../FramePlayer.h"

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

- Include it in `wokwi/sketch.ino` while the matrix harness lives there.
- Add it to the `animations[]` array in `wokwi/sketch.ino`.
- Add its path to `public/animation-manifest.js` so the browser preview loads it.

The animation headers are shared by Arduino/Wokwi code and the browser preview, so they remain the source of truth.

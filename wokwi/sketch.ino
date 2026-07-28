#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "../firmware/jar_pet/MatrixTools.h"
#include "../firmware/jar_pet/FramePlayer.h"
#include "../firmware/jar_pet/animations/Heart.h"
#include "../firmware/jar_pet/animations/DiagonalSweep.h"
#include "../firmware/jar_pet/animations/Sparkle.h"

Adafruit_NeoPixel matrix(NUM_PIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

const Animation animations[] = {
  Heart,
  DiagonalSweep,
  Sparkle
};

const uint8_t ANIMATION_COUNT = sizeof(animations) / sizeof(animations[0]);

uint8_t animationIndex = 0;
uint8_t frameIndex = 0;
uint32_t nextFrameAt = 0;

void setup() {
  matrix.begin();
  matrix.setBrightness(255);
  clearMatrix();
  matrix.show();
}

void loop() {
  if (millis() < nextFrameAt) {
    return;
  }

  const Animation animation = animations[animationIndex];
  drawFrame(animation.frames, frameIndex);

  nextFrameAt = millis() + frameDuration(animation.frames, frameIndex);
  frameIndex++;

  if (frameIndex >= animation.frameCount) {
    frameIndex = 0;
    animationIndex = (animationIndex + 1) % ANIMATION_COUNT;
  }
}

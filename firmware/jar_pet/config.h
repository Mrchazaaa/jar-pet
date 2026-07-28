#ifndef JAR_PET_CONFIG_H
#define JAR_PET_CONFIG_H

#include <Arduino.h>

const int PIEZO_PIN = A0;
const int HIT_THRESHOLD = 300;
const int RESET_THRESHOLD = 100;

const unsigned long GREEN_TIME = 500;
const unsigned long BLINK_TIME = 500;

const uint8_t STATUS_PIXEL_COUNT = 1;
const uint8_t STATUS_PIXEL_BRIGHTNESS = 30;

#endif

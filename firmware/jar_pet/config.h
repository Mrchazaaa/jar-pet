#ifndef JAR_PET_CONFIG_H
#define JAR_PET_CONFIG_H

#include <Arduino.h>

const int TAP_SENSOR_PIN = A0;
const int TAP_HIT_THRESHOLD = 300;
const int TAP_RESET_THRESHOLD = 100;
const unsigned long SENSOR_LOG_INTERVAL_MS = 250;

const uint8_t LED_STRIP_PIN = 9;
const uint16_t LED_STRIP_COUNT = 100;
const uint8_t LED_STRIP_BRIGHTNESS = 35;
const unsigned long LED_ANIMATION_FRAME_MS = 20;

const uint8_t STATUS_PIXEL_COUNT = 1;
const uint8_t STATUS_PIXEL_BRIGHTNESS = 30;

#endif

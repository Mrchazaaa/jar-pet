#include <Adafruit_NeoPixel.h>

constexpr uint8_t LED_PIN = 9;
constexpr uint16_t LED_COUNT = 100;

// Keep this low while powering the strip directly from a LiPo.
// Range: 0–255.
constexpr uint8_t BRIGHTNESS = 35;

Adafruit_NeoPixel strip(
    LED_COUNT,
    LED_PIN,
    NEO_GRB + NEO_KHZ800
);

void showSolid(uint32_t colour, uint16_t durationMs)
{
    strip.fill(colour);
    strip.show();
    delay(durationMs);
}

void colourWipe(uint32_t colour, uint16_t waitMs)
{
    for (uint16_t i = 0; i < strip.numPixels(); ++i) {
        strip.setPixelColor(i, colour);
        strip.show();
        delay(waitMs);
    }
}

void rainbow(uint16_t durationMs)
{
    const uint32_t startTime = millis();

    while (millis() - startTime < durationMs) {
        const uint16_t offset = static_cast<uint16_t>(millis() / 12);

        for (uint16_t i = 0; i < strip.numPixels(); ++i) {
            const uint16_t hue =
                static_cast<uint16_t>(
                    (static_cast<uint32_t>(i) * 65536UL /
                     strip.numPixels()) +
                    offset * 256UL
                );

            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
        }

        strip.show();
        delay(20);
    }
}

void movingDots(uint16_t durationMs)
{
    const uint32_t startTime = millis();
    uint16_t position = 0;

    while (millis() - startTime < durationMs) {
        strip.clear();

        // Only ten LEDs are illuminated, reducing battery use.
        for (uint16_t i = 0; i < LED_COUNT; i += 10) {
            const uint16_t pixel = (position + i) % LED_COUNT;
            strip.setPixelColor(pixel, strip.Color(0, 0, 255));
        }

        strip.show();
        position = (position + 1) % LED_COUNT;
        delay(40);
    }
}

void setup()
{
    strip.begin();
    strip.setBrightness(BRIGHTNESS);
    strip.clear();
    strip.show();

    delay(500);
}

void loop()
{
    // Single-channel colours have lower power demand than white.
    showSolid(strip.Color(255, 0, 0), 1000);   // Red
    showSolid(strip.Color(0, 255, 0), 1000);   // Green
    showSolid(strip.Color(0, 0, 255), 1000);   // Blue

    // Brief white test. Keep brightness low.
    showSolid(strip.Color(255, 255, 255), 500);

    strip.clear();
    strip.show();
    delay(500);

    colourWipe(strip.Color(255, 0, 0), 10);
    colourWipe(strip.Color(0, 255, 0), 10);
    colourWipe(strip.Color(0, 0, 255), 10);

    rainbow(5000);
    movingDots(5000);

    strip.clear();
    strip.show();
    delay(1500);
}

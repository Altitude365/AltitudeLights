#include <Adafruit_NeoPixel.h>
#include <inttypes.h>

#define NEOPIXEL_PIN 1

// We use "pixel" to refer to a single WS2812B package, which contains three LEDs (red, green and blue).

// Select number of pixels.
#define NUM_PIXELS 2700

// We copy some type name defines here to keep the sketch self contained.
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define s16 int16_t

// Create the NeoPixel driver object. On the PC, this becomes the emulator.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// This must be below the short int type names and the NeoPixel library init.
#ifndef Arduino_h
#include "arduino_sketch.hpp"
// When running on the PC, we include stdio.h for printf() debugging.
#include <stdio.h>
#endif

// Example of wrapping stuff that will only compile in the IDE.
#ifdef Arduino_h
void debugMarker()
{
  pinMode(1, OUTPUT);
  for (u8 i = 0; i < 10; ++i) {
    digitalWrite(1, HIGH);
    delay(500);
    digitalWrite(1, LOW);
    delay(500);
  }
}
#endif

// End of boilerplate.

void setup()
{
// Arduino_h is only defined when compiling the sketch in the Arduino IDE.
#ifndef Arduino_h
    // Select line, circle or grid pixel layout for emulator.
    // pixels.setPixelLayout(Strip);
    pixels.setPixelLayout(Ring);
    // pixels.setPixelLayout(Grid);
#endif
    pixels.begin();
    // setBrightness() is intended for use only in setup() and not for animations. It's ignored by the emulator (see
    // the README).
    pixels.setBrightness(50);
    pixels.show();
}


struct Segment {
  u32 color;
  s16 speed;
  u8 lengthPercent;
};


void loop()
{
    clear(0xff0000, 3000);
    delay(1000);
}




u32 colorWeightedAvg(u32 color1, u32 color2, u8 weight)
{
    u8 r1, g1, b1, r2, g2, b2;
    colorPackedToScalar(&r1, &g1, &b1, color1);
    colorPackedToScalar(&r2, &g2, &b2, color2);
    r1 = channelWeightedAvg(r1, r2, weight);
    g1 = channelWeightedAvg(g1, g2, weight);
    b1 = channelWeightedAvg(b1, b2, weight);
    return pixels.Color(r1, g1, b1);
}

u8 channelWeightedAvg(u8 a, u8 b, u8 weight)
{
    u16 aa = a * (256 - weight);
    u16 bb = b * weight;
    return (aa + bb) >> 8;
}

void addPixelColor(u16 pixelPos, u32 addColor)
{
    u32 oldColor = pixels.getPixelColor(pixelPos);
    u32 newColor = additiveColorMix(oldColor, addColor);
    pixels.setPixelColor(pixelPos, newColor);
}

u32 additiveColorMix(u32 color1, u32 color2)
{
    u8 r1, g1, b1, r2, g2, b2;
    colorPackedToScalar(&r1, &g1, &b1, color1);
    colorPackedToScalar(&r2, &g2, &b2, color2);
    r1 = colorChannelClampedAdd(r1, r2);
    g1 = colorChannelClampedAdd(g1, g2);
    b1 = colorChannelClampedAdd(b1, b2);
    u32 c = pixels.Color(r1, g1, b1);
    return c;
}

void colorPackedToScalar(u8* r, u8* g, u8* b, u32 color) {
    *b = color;
    color >>= 8;
    *g = color;
    color >>= 8;
    *r = color;
}

u8 colorChannelClampedAdd(u8 a, u8 b) {
    u16 c = a + b;
    if (c > 255) {
        c = 255;
    }
    return c;
}

void xmasRedGreenTwinkles(u16 runSec)
{
    const u8 flashOnDelayMs = 20;
    const u8 flashPauseMs = 150;
    const u8 numTwinklesPerSwap = 3;
    bool redOrGreenFirst = false;
    u32 startMs = millis();
    u8 numTwinklesToNextSwap = 0;
    while (millis() < startMs + runSec * 1000UL) {
        if (!numTwinklesToNextSwap--) {
            numTwinklesToNextSwap = numTwinklesPerSwap;
            redOrGreenFirst = !redOrGreenFirst;
        }
        u8 twinkleLedIdx = random(0, pixels.numPixels() - 1);
        pixels.setPixelColor(twinkleLedIdx, 0xffffff);
        pixels.show();
        delay(flashOnDelayMs);
        for (u8 i = 0; i < pixels.numPixels(); ++i) {
            u32 c = (i + redOrGreenFirst) & 1 ? 0xff0000 : 0x00ff00;
            pixels.setPixelColor(i, c);
        }
        pixels.show();
        delay(flashPauseMs);
    }
}

void clear(u32 color, u16 clearMs)
{
    u16 delayMs = clearMs / pixels.numPixels();
    for (u16 i = 0; i < pixels.numPixels(); ++i) {
        pixels.setPixelColor(i, color);
        if (delayMs) {
            pixels.show();
            delay(delayMs);
        }
    }
}

s16 wrap(s16 val, s16 maxVal)
{
    if (val < 0) {
        return val + maxVal;
    }
    else if (val >= maxVal) {
        return val - maxVal;
    }
    else {
        return val;
    }
}

u16 wrapAdd(s16 val, s16 addVal, s16 maxVal)
{
    s16 t = val + addVal;
    if (t > maxVal) {
        return t - maxVal - 1;
    }
    else if (t < 0) {
        return maxVal + t + 1;
    }
    else {
        return t;
    }
}


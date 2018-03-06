#include <Adafruit_NeoPixel.h>
#include <inttypes.h>

#define NEOPIXEL_PIN 1

// We use "pixel" to refer to a single WS2812B package, which contains three LEDs (red, green and blue).

// Select number of pixels.
#define NUM_PIXELS 24

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
    int runSec = 60;
    u16 numSegments;

    // Because I build the segment arrays in memory, I use both flash and sram for the segments and 5 is max for
    // what the ATtiny85 can handle with the current code. TODO: Try initializing the segments as const from literals.
    // That should cause them to stay in flash.
    const int maxSegments = 5;
    Segment segmentArr[maxSegments];

    // Xmas pattern and twinkles.
    xmasRedGreenTwinkles(runSec);

    // Example of simple debugging of something that only breaks when running on the device.
    //#ifdef Arduino_h
    //    debugMarker();
    //#endif

    // Halloween 1
    for (u8 i = 0; i < maxSegments; ++i) {
        segmentArr[i] = { 0xff6800, 30, 10 };
    }
    smoothRunners(runSec, 10, segmentArr, maxSegments);
    clear(pixels.Color(0, 0, 0), 1000);

//    // Halloween 2
//    for (u8 i = 0; i < maxSegments; ++i) {
//        segmentArr[i] = { 0xff6800, 30, 5 };
//    }
//    smoothRunners(runSec, 10, segmentArr, maxSegments);
//    clear(pixels.Color(0, 0, 0), 1000);

    // Halloween 3
    for (u8 i = 0; i < maxSegments; ++i) {
        segmentArr[i] = { 0xff6800, -60, 20 };
    }
    smoothRunners(runSec, 10, segmentArr, maxSegments);
    clear(pixels.Color(0, 0, 0), 1000);

    // RGB, even, short
    segmentArr[0] = { 0xff0000, 60, 8 };
    segmentArr[1] = { 0x00ff00, 60, 8 };
    segmentArr[2] = { 0x0000ff, 60, 8 };
    smoothRunners(runSec, 20, segmentArr, 3);
    clear(pixels.Color(0, 0, 0), 1000);

    // Took these out because I ran out of room on the ATtiny85.

//    // RGB, faster
//    segmentArr[0] = { 0xff0000, 120, 8 };
//    segmentArr[1] = { 0x00ff00, 120, 8 };
//    segmentArr[2] = { 0x0000ff, 120, 8 };
//    smoothRunners(runSec, 20, segmentArr, 3);
//    clear(pixels.Color(0, 0, 0), 1000);

//    // Misc colors, many short segments, even spacing
//    segmentArr[0] = { 0xff0000, 120, 2 };
//    segmentArr[1] = { 0x00ff00, 120, 2 };
//    segmentArr[2] = { 0x0000ff, 120, 2 };
//    segmentArr[3] = { 0xffff00, 120, 2 };
//    segmentArr[4] = { 0x00ffff, 120, 2 };
//    segmentArr[5] = { 0xff00ff, 120, 2 };
//    segmentArr[6] = { 0xfff000, 120, 2 };
//    segmentArr[7] = { 0x00fff0, 120, 2 };
//    segmentArr[8] = { 0x000fff, 120, 2 };
//    segmentArr[9] = { 0xfff000, 120, 2 };
//    segmentArr[10] = { 0x0fff00, 120, 2 };
//    segmentArr[11] = { 0x000fff, 120, 2 };
//    smoothRunners(runSec, 20, segmentArr, 12);
//    clear(pixels.Color(0, 0, 0), 1000);

//    // Misc colors, many short segments, uneven spacing
//    segmentArr[0] = { 0xff0000, 261, 2 };
//    segmentArr[1] = { 0x00ff00, 411, 2 };
//    segmentArr[2] = { 0x0000ff, 111, 2 };
//    segmentArr[3] = { 0xffff00, 125, 2 };
//    segmentArr[4] = { 0x00ffff, 163, 2 };
//    segmentArr[5] = { 0xff00ff, 188, 2 };
//    segmentArr[6] = { 0xfff000, 199, 2 };
//    segmentArr[7] = { 0x00fff0, 174, 2 };
//    segmentArr[8] = { 0x000fff, 175, 2 };
//    segmentArr[9] = { 0xfff000, 154, 2 };
//    segmentArr[10] = { 0x0fff00, 123, 2 };
//    segmentArr[11] = { 0x000fff, 100, 2 };
//    smoothRunners(runSec, 20, segmentArr, 12);
//    clear(pixels.Color(0, 0, 0), 1000);

    // Randomly colored single dots going both directions.
    for (u8 i = 0; i < maxSegments; ++i) {
        segmentArr[i].color = random(0, 0xffffff);
        if (random(0, 1)) {
            segmentArr[i].speed = random(100, 200);
        }
        else {
            segmentArr[i].speed = random(-100, -200);
        }
        segmentArr[i].lengthPercent = 10;
    };
    smoothRunners(runSec, 20, segmentArr, maxSegments);
    clear(pixels.Color(0, 0, 0), 1000);

    // even rainbow
    segmentArr[0] = { 0xff0000, 100, 100 };
    segmentArr[1] = { 0x00ff00, 100, 100 };
    segmentArr[2] = { 0x0000ff, 100, 100 };
    smoothRunners(runSec, 20, segmentArr, 3);
    clear(pixels.Color(0, 0, 0), 1000);

    // uneven rainbow
    segmentArr[0] = { 0xff0000, -100, 100 };
    segmentArr[1] = { 0x00ff00, -120, 100 };
    segmentArr[2] = { 0x0000ff, -140, 100 };
    smoothRunners(runSec, 20, segmentArr, 3);
    clear(pixels.Color(0, 0, 0), 1000);

    // darker rainbow
    segmentArr[0] = { 0xff0000, 60, 100 };
    segmentArr[1] = { 0x00ff00, 70, 100 };
    segmentArr[2] = { 0x0000ff, -80, 100 };
    smoothRunners(runSec, 20, segmentArr, 3);
    clear(pixels.Color(0, 0, 0), 1000);

    // shimmer 1
    segmentArr[0] = { 0xff0000, 123, 75 };
    segmentArr[1] = { 0x00ff00, 456, 75 };
    segmentArr[2] = { 0x0000ff, -789, 75 };
    smoothRunners(runSec, 1, segmentArr, 3);
    clear(pixels.Color(0, 0, 0), 1000);

    // shimmer 2
    segmentArr[0] = { 0xff0000, 1000, 75 };
    segmentArr[1] = { 0x00ff00, 2000, 75 };
    segmentArr[2] = { 0x0000ff, -1500, 75 };
    smoothRunners(runSec, 1, segmentArr, 3);
    clear(pixels.Color(0, 0, 0), 1000);

//    // tweaked for 16 pixel grid. darker rainbow
//    segmentArr[0] = { 0xff0000, 60, 70 };
//    segmentArr[1] = { 0x00ff00, 70, 70 };
//    segmentArr[2] = { 0x0000ff, -80, 70 };
//    smoothRunners(60*60, 20, segmentArr, 3);
//    clear(pixels.Color(0, 0, 0), 1000);

    // Show single colors
    clear(0xff0000, 3000);
    delay(1000);
    clear(0xffff00, 3000);
    delay(1000);
    clear(0x00ff00, 3000);
    delay(1000);
    clear(0x00ffff, 3000);
    delay(1000);
    clear(0x0000ff, 3000);
    delay(1000);
    clear(0xff00ff, 3000);
    delay(1000);
    clear(0xffffff, 3000);
    delay(1000);
    clear(0x000000, 3000);
}

void smoothRunners(u16 runSec, u16 delayMs, struct Segment* segmentPtrIn, u8 numSegments)
{
    u16 superPosBuf[numSegments];
    u16 numSuperPositions = pixels.numPixels() << 8;
    u16 superPosOffset = numSuperPositions / numSegments;
    for (u8 i = 0; i < numSegments; ++i) {
        superPosBuf[i] = i * superPosOffset;
    }

    u32 startMs = millis();
    while (millis() < startMs + runSec * 1000UL) {
        clear(0, 0);
        Segment* segmentPtr = segmentPtrIn;
        for (u8 i = 0; i < numSegments; ++i) {
            u16 superPos = superPosBuf[i];
            superPos = wrapAdd(superPos, segmentPtr->speed, numSuperPositions);
            superPosBuf[i] = superPos;
            u8 numSegmentPixels = segmentPtr->lengthPercent * pixels.numPixels() / 100;
            if (numSegmentPixels < 2) {
                numSegmentPixels = 2;
            }
            drawTaperedSegment(superPos, numSegmentPixels, segmentPtr->color);
            ++segmentPtr;
        }
        pixels.show();
        delay(delayMs);
    }
}

void drawTaperedSegment(u16 superPos, u8 numSegmentPixels, u32 color)
{
    u8 shiftPos = superPos & 0xff;
    u8 pixelPos = superPos >> 8;
    for (u8 i = 0; i <= numSegmentPixels; ++i) {
        u32 color1 = calcTaperedSegmentPixel(numSegmentPixels, i, color);
        u32 color2 = calcTaperedSegmentPixel(numSegmentPixels, i + 1, color);
        u32 avgColor = colorWeightedAvg(color1, color2, 255 - shiftPos);
        addPixelColor(pixelPos, avgColor);
        pixelPos = wrapAdd(pixelPos, 1, pixels.numPixels() - 1);
    }
}

// Example for 7 pixels.
//
//        X
//       X X
//      X   X
//     X     X
//
// pixel 3 is center and will be set to brightestVal.
// There are 4 pixels on each side, sharing the center pixel
// The values of the 4 pixels are assigned in such a way that an imagined 5th pixel on each side is 0.
// So, for 4 pixels:
//   single step = 255 / 4 = 63
//   steps for 7 pixels: 64, 127, 191, 255, 191, 127, 64
u32 calcTaperedSegmentPixel(u8 numSegmentPixels, u8 pos, u32 centerColor)
{
    if (pos == 0 || pos == numSegmentPixels + 1) {
        return 0x000000;
    }

    u8 centerPos = numSegmentPixels / 2;
    u8 rCenter, gCenter, bCenter;
    colorPackedToScalar(&rCenter, &gCenter, &bCenter, centerColor);
    u8 rStep = rCenter / (centerPos + 1);
    u8 gStep = gCenter / (centerPos + 1);
    u8 bStep = bCenter / (centerPos + 1);

    u8 segmentIndex;
    if (pos <= centerPos) {
        segmentIndex = pos + 1;
    }
    else {
        segmentIndex = numSegmentPixels - pos;
    }

    return pixels.Color(rStep * segmentIndex, gStep * segmentIndex, bStep * segmentIndex);
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


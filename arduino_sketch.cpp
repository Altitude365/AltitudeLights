

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

int maxBlow = 120;

int extra, extra2;

int mainColor0 = 0xff0000;
int mainColor1 = 0x33ff66;
int mainColor2 = 0x0000ff;
int noseColor = 0xffff00;

int ColorMix(int px) {
        switch (px % 3) {
                case 0:
                        return mainColor0;
                        break;
                case 1:
                        return mainColor1;
                        break;
                case 2:
                        return mainColor2;
                        break;
                }
}

int pixNumber(int stripe, int px) {
        return (stripe * 150) + px;
}

void WriteAllPixelsAt(int pos, int color) {
	for (int i=0; i < 18; i++) {
		int newPos = pos - (pos % 2 + random(1,3));
		if (newPos < pos && newPos < maxBlow && newPos > 2) {
   		      	int cpx = pixNumber(i, newPos);
         		pixels.setPixelColor(cpx, color);
		}
       	}
}
void WriteAllPixelsAt(int pos, int color, int flux) {
     	for (int i=0; i < 18; i++) {
              	int newPos = pos - (pos % 2 + random(1,3));
             	if (newPos < pos && newPos < maxBlow && newPos > 2) {
                      	int cpx = pixNumber(i, newPos);	
                     	pixels.setPixelColor(cpx, ColorMix(((i * i) / flux) % 3));
		}
      	}
}

void ClearRandom(int start, int end){
	int clearRate = 13;
	for (int r=0; r < clearRate ; r++) {
		for (int i=0; i < 18; i++) {
         		int cpx = pixNumber(i, random(start,end));
         		pixels.setPixelColor(cpx, 0x000000);
        	}
	}
}


void MoveRandom(int px) {
	if (pixels.getPixelColor(px) != 0) {
		if (px % 150 < maxBlow) {
			int cc = pixels.getPixelColor(px);
			pixels.setPixelColor(px, 0x000000);
			pixels.setPixelColor(px + 1, cc);
		}
	}
}

int rnd = 4;

int explode(int frame) {
	if (frame < 20) {
		int NewFrame = frame + ((frame * frame) / 5);
		int l = 0;
		for (int o=frame; o < NewFrame; o++) {
			WriteAllPixelsAt(o, ColorMix(o), rnd);
			l = o;
		}
		if (l > 2) { //nose color
			WriteAllPixelsAt(l, noseColor);
		}		
	} 
	if (frame > 5 && frame <= 20) { //black erace
		WriteAllPixelsAt(frame - 5, 0x000000);
	}
	if (frame > 14) {
		ClearRandom(0,maxBlow);
		for (int i=0; i < 900; i++) {
			MoveRandom(random(0,2699));
		}
	}

}

int frame=0;

void loop()
{
    //clear(0xff0000, 5);
    //delay(1000);
	delay(100);
	
	explode(frame);
	

	if (frame >= 150) {
		frame = 0;
	}
   
    	pixels.show();
	frame++;
}





void clear(u32 color, u16 clearMs)
{
    for (u16 i = 0; i < pixels.numPixels(); ++i) {
        pixels.setPixelColor(i, color);
        pixels.show();
        delay(clearMs);
    }
}


//plz dont change this part
#include <Adafruit_NeoPixel.h>
#include <inttypes.h>

#define NEOPIXEL_PIN 1
#define NUM_PIXELS 2700
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define s16 int16_t
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
    pixels.setBrightness(50);
    pixels.show();
}

//Alittude functions
int pixelsgetPixelColor(int px) {return pixels.getPixelColor(px);}
void stripsSetPixelColor(int px, int8_t r ,int8_t g , int8_t b) {pixels.setPixelColor(px, r, g, b);}
void stripsSetPixelColor(int px, int color) {pixels.setPixelColor(px, color);}
void stripsShow() {pixels.show();}
//End of altitude function 

/**
 * 
 * Please use stripsShow, pixelsgetPixelColor and stripsSetPixelColor,
 * instead of pixels.setPixelColor and pixels.getPixelColor.
 * We will change these functions in the final test to a modified version.
 * We do this because we are using 3 stripes instead on one. 
 * 
 **/

//Edit example below

int extra, extra2;

void loop()
{
    //clear(0xff0000, 5);
    //delay(1000);

    for (int i=0; i < 250 ;i++){ 
        extra = random(-30,40);
        extra2 = random(-83,40);
        pixels.setPixelColor(random(0, 2700), (36 + extra), (83 + extra2), (214 + extra));
    }
    pixels.show();
}

void clear(u32 color, u16 clearMs)
{
    for (u16 i = 0; i < pixels.numPixels(); ++i) {
        pixels.setPixelColor(i, color);
        pixels.show();
        delay(clearMs);
    }
}


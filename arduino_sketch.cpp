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
uint32_t Wheel(double strength, uint32_t WheelPos) {
	WheelPos = 255 - WheelPos;

	if(WheelPos < 85) {
		return pixels.Color((255 - WheelPos * 3) * strength, 0, WheelPos * 3 * strength);
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		return pixels.Color(0, WheelPos * 3 * strength, (255 - WheelPos * 3) * strength);
	} else {
		WheelPos -= 170;
		return pixels.Color(WheelPos * 3 * strength, (255 - WheelPos * 3) * strength, 0);
	}
}

int init_time=millis();
double timer = 0.0;

int gradientSineWave(double T, int durance, bool fadeIn) {
	double offset = 65.0;
	double dist = 65.0;
	double freq = 2.0;
	double falloff = 75.0;

	if (T > durance) {
		return 1;
	}
	for (int j = 0; j < 18; j++) { 
		double r = j * 10;
		double g = j * 10;
		double b = j * 10;

		for (int i = 0; i < 150; i++) { 
			double x = offset + dist * sin(timer * freq + j*0.1);
			double tmp = abs(i-x);
			if (tmp > falloff)
				tmp = falloff;
			double strength = (1.0 - (tmp/falloff)) * (1.0 - T/durance);
			if (fadeIn) {
				strength = (1.0 - (tmp/falloff)) * T/durance;
			}
			stripsSetPixelColor(i + 150*j, Wheel(strength, 255.0 * (sin(timer) + 1.0) * 0.5));
		}
	}
	return 0;
}

int variant = 0;
int direction = 1;
int ix = 0;
int bounce(double T, int r, int g, int b) {
	if (ix > NUM_PIXELS) {
		ix = NUM_PIXELS-variant-1;
		direction *= -1;
		return 1;
	}
	if (ix < 0) {
		ix = variant;
		direction = 1;
		return 1;
	}
	for (int i = 0; i < NUM_PIXELS; i++) {
		if (i == ix) {
			stripsSetPixelColor(i,r,g,b);
		} 
	}
	ix += direction * 5;
	return 0;
}

int snow(double T, int durance, bool randomColor) {
	int r = 255;
	int g = 255;
	int b = 255;
	double dist = 1.0;
	for (int j = 149; j >= 0; j--) {
		if (randomColor) {
			r = random(j,255);
			g = random(j,255);
			b = random(j,255);
		} else {
			dist = 0.1 + (j/150.0);
		}
		for (int i = 0; i < 18; i++) {
			int rand = random(0,36); 
			if (rand < 1) {
				stripsSetPixelColor(i*150, r * dist, g * dist, b * dist);
			} else {
				stripsSetPixelColor(i*150, 0,0,0);
			}
			int color = pixelsgetPixelColor(i*150 + j);
			if (color <= 0) {
				stripsSetPixelColor(i*150+j+1, 0,0,0);
			} else {
				stripsSetPixelColor(i*150+j+1, r * dist, g * dist, b * dist);
			}
		}
	}
	if (!randomColor) {
		delay(50);
	}
	if (T > durance) {
		return 1;
	}
	return 0;
}

void loop()
{
	timer = (millis() - init_time) / 1000.0;
	//clear(0xff0000, 5);
	//delay(1000);

	switch (variant) {
		case 0:
		for (int i=0; i < NUM_PIXELS; i++) {
			stripsSetPixelColor(i, 40, 40, 40);
		}
		variant += gradientSineWave(timer, 10, true);
		break;
		case 1:
		variant += gradientSineWave(timer, 20, false);
		break;
		case 2:
		variant += bounce(timer, 255, 0, 0);
		bounce(timer, 0, 255, 0);
		break;
		case 3:
		variant += bounce(timer, 0, 0, 255);
		bounce(timer, 255, 0, 255);
		break;
		case 4:
		variant += bounce(timer, 255, 255, 0);
		bounce(timer, 0, 255, 255);
		break;
		case 5:
		variant += snow(timer, 60, false);
		break;
		case 6:
		variant += snow(timer, 70, true);
		break;
		default:
		variant = 0;
		init_time=millis();
		break;
	}

	stripsShow();
}

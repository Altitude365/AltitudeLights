#include "NeoPixelEmulator.h"
#include "stdio.h"

#define M_PI 3.14159265358979323846

NeoPixelEmulator::NeoPixelEmulator(uint16_t numLEDs, uint8_t p, uint8_t t)
    : pixels(numLEDs, 0)
{
}

NeoPixelEmulator::~NeoPixelEmulator()
{
}

void NeoPixelEmulator::setPixelLayout(PixelLayout pixelLayout) {
    _pixelLayout = pixelLayout;
}

void NeoPixelEmulator::begin(void)
{
}

void NeoPixelEmulator::show(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawLedRing();
        
    
    // Double buffering.
    glutSwapBuffers();
    // GLUT process events, redrew screen.
    glutMainLoopEvent();
}

void NeoPixelEmulator::setPin(uint8_t)
{
}

// Set pixel color from separate R,G,B components:
void NeoPixelEmulator::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    if (n < pixels.size()) {
        setPixelColor(n, Color(r, g, b));
    }
}

// Set pixel color from 'packed' 32-bit RGB color:
void NeoPixelEmulator::setPixelColor(uint16_t n, uint32_t c)
{
    if (n < pixels.size()) {
        pixels[n] = c;
    }
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t NeoPixelEmulator::Color(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Query color from previously-set pixel (returns packed 32-bit RGB value)
uint32_t NeoPixelEmulator::getPixelColor(uint16_t n) const
{
    if (n >= pixels.size()) {
        return 0;
    }
    return pixels[n];
}

// Returns pointer to pixels[] array.  Pixel data is stored in device-
// native format and is not translated here.  Application will need to be
// aware whether pixels are RGB vs. GRB and handle colors appropriately.
// *** Interface change: Removed const on function. Don't know how
// the Adafruit implementation was able to return non-const pointer from
// const function there.
uint8_t* NeoPixelEmulator::getPixels(void)
{
    return reinterpret_cast<uint8_t*>(&pixels[0]);
}

uint16_t NeoPixelEmulator::numPixels(void) const
{
    return pixels.size();
}

void NeoPixelEmulator::setBrightness(uint8_t)
{
}

uint8_t NeoPixelEmulator::getBrightness(void) const
{
    return 255;
}

void NeoPixelEmulator::clear()
{
    std::fill(pixels.begin(), pixels.end(), 0);
}

//
// OpenGL.
//

const int maxLedRadius = 50;

void NeoPixelEmulator::drawLedRing()
{
    //new
    int NumberOfStripes = 18;
    int PxPerStripe = 150;
    float StartAngle = 1.0f;
    float OffsetAngle = 15.0f;
    float pxDistance = 3.0f;
    float centerOfset = 40.0f;

    float xCenter = 500.0f;
    float yCenter = 500.0f;
    float ledRadius = 3.0f;

    for (int i = 0; i < NumberOfStripes; i++) {
        //For each stripe
        for (int px = 0; px <PxPerStripe; px++) {
            float CircleRadiusNow = centerOfset + (px * pxDistance);
            int CurrentPixel = (150 * i) + px;
            uint32_t c = pixels[CurrentPixel];
            uint8_t R, G, B;
            colorPackedToScalar(&R, &G, &B, c);
            drawFilledCircle(xCenter + (CircleRadiusNow * cos(((NumberOfStripes - i) - 3.5) * 1.5f * M_PI / NumberOfStripes)),
                            yCenter + (CircleRadiusNow * sin(((NumberOfStripes - i) - 3.5)  * 1.5f * M_PI / NumberOfStripes)),
                            ledRadius, R, G, B);
        }
    }
}



void NeoPixelEmulator::drawFilledCircle(float x, float y, float r, u8 R, u8 G, u8 B)
{
    glColor3ub(R, G, B);
    glPointSize(r);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

// When using AA with this method, I get some undrawn pixels radiating out from
// the center. I'd like to find out why.
void NeoPixelEmulator::__drawFilledCircle(float x, float y, float r)
{
    int numTriangles = 30;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= numTriangles; ++i) {
        glVertex2f(x + (r * cos(i * 2.0f * M_PI / numTriangles)),
                   y + (r * sin(i * 2.0f * M_PI / numTriangles)));
    }
    glEnd();
}

void NeoPixelEmulator::colorPackedToScalar(uint8_t* R, uint8_t* G, uint8_t* B, uint32_t color) {
    *B = color;
    color >>= 8;
    *G = color;
    color >>= 8;
    *R = color;
}

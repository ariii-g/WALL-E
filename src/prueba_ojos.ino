#include <DIYables_TFT_Round.h>
#include <SPI.h>

#define BLACK   DIYables_TFT::colorRGB(0, 0, 0)
#define BLUE    DIYables_TFT::colorRGB(0, 180, 255)
#define WHITE   DIYables_TFT::colorRGB(255, 255, 255)

#define TFT_MOSI   18
#define TFT_SCLK   8

#define PIN_DC     17
#define PIN_RST    15

#define CS_LEFT    47

#define CS_RIGHT   46   // 

DIYables_TFT_GC9A01_Round tftLeft(PIN_RST, PIN_DC, CS_LEFT);
DIYables_TFT_GC9A01_Round tftRight(PIN_RST, PIN_DC, CS_RIGHT);

unsigned long prevBlink = 0;
unsigned long blinkDuration = 250;
unsigned long blinkInterval = 40000;
bool blinking = false;

void drawEye(DIYables_TFT_GC9A01_Round &display, bool closed, int pupilX, int pupilY) {
  display.fillScreen(BLACK);

  if (closed) {
    // ojo cerrado
    display.fillRoundRect(40, 110, 160, 20, 10, BLUE);
  } else {
    // ojo abierto
    display.fillCircle(120, 120, 70, BLUE);

    // pupila
    display.fillCircle(120 + pupilX, 120 + pupilY, 25, BLACK);

    // brillo
    display.fillCircle(130 + pupilX, 110 + pupilY, 6, WHITE);
  }
}

void drawBothEyes(bool closed) {
  drawEye(tftLeft, closed, -10, 0);
  drawEye(tftRight, closed, 10, 0);
}
void setup() {
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);

  tftLeft.begin();
  tftRight.begin();

  tftLeft.setRotation(1);
  tftRight.setRotation(1);

  drawBothEyes(false);
}

void loop() {
  unsigned long currentMillis = millis();

  if (!blinking && (currentMillis - prevBlink >= blinkInterval)) {
    blinking = true;
    prevBlink = currentMillis;
    drawBothEyes(true);
  }

  if (blinking && (currentMillis - prevBlink >= blinkDuration)) {
    blinking = false;
    prevBlink = currentMillis;
    drawBothEyes(false);
  }
}

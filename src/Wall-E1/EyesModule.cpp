#include "EyesModule.h"

#include <DIYables_TFT_Round.h>
#include <SPI.h>

#define BLACK DIYables_TFT::colorRGB(0,0,0)
#define BLUE DIYables_TFT::colorRGB(0,180,255)
#define WHITE DIYables_TFT::colorRGB(255,255,255)

#define TFT_MOSI 39
#define TFT_SCLK 47

#define PIN_DC 21
#define PIN_RST 15

#define CS_LEFT 19
#define CS_RIGHT 20

DIYables_TFT_GC9A01_Round tftLeft(PIN_RST, PIN_DC, CS_LEFT);
DIYables_TFT_GC9A01_Round tftRight(PIN_RST, PIN_DC, CS_RIGHT);

bool blinking = false;

unsigned long prevBlink = 0;

const unsigned long blinkInterval = 5000;
const unsigned long blinkDuration = 200;

hw_timer_t *eyeTimer = NULL;
volatile bool updateRightEye = false;

void IRAM_ATTR onEyeTimer() {

  updateRightEye = true;
}

void drawOpenEye(DIYables_TFT_GC9A01_Round &display) {

  display.fillScreen(BLACK);
  display.fillCircle(120,130,100,BLUE);
  display.fillCircle(120,130,80,BLACK);
  display.fillCircle(120,130,10,WHITE);
}

void drawClosedEye(DIYables_TFT_GC9A01_Round &display) {

  display.fillScreen(BLACK);
  display.fillRoundRect(40,110,160,20,10,BLUE);
}

void drawBothEyes(bool closed) {

  if(closed) {

    drawClosedEye(tftLeft);
    timerWrite(eyeTimer, 0);
    updateRightEye = false;
    timerAlarm(eyeTimer, 1000, false, 0);

  } else {

    drawOpenEye(tftLeft);
    timerWrite(eyeTimer, 0);
    updateRightEye = false;
    timerAlarm(eyeTimer, 1000, false, 0);
  }
}

void updateSecondEye(bool closed) {

  if(closed) {

    drawClosedEye(tftRight);

  } else {

    drawOpenEye(tftRight);
  }
}

void eyesTask(void * parameter) {

  bool currentState = false;
  while(true) {

    unsigned long currentMillis = millis();

    if(!blinking && currentMillis - prevBlink >= blinkInterval) {
      blinking = true;
      prevBlink = currentMillis;
      currentState = true;
      drawBothEyes(true);
    }

    if(blinking && currentMillis - prevBlink >= blinkDuration) {

      blinking = false;
      prevBlink = currentMillis;
      currentState = false;

      drawBothEyes(false);
    }

    if(updateRightEye) {

      updateRightEye = false;
      updateSecondEye(currentState);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void eyesSetup() {

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);

  tftLeft.begin();
  tftRight.begin();
  tftLeft.setRotation(1);
  tftRight.setRotation(1);
  eyeTimer = timerBegin(1000000);

  timerAttachInterrupt(eyeTimer, &onEyeTimer);
  drawBothEyes(false);

  xTaskCreatePinnedToCore(
    eyesTask,
    "EyesTask",
    10000,
    NULL,
    1,
    NULL,
    0
  );
}
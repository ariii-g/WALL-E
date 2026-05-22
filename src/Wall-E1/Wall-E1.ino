#include <WiFi.h>

#include "EyesModule.h"
#include "CarModule.h"

const char* ssid = "1";
const char* password = "12345678";

TaskHandle_t carTaskHandle;
TaskHandle_t eyesTaskHandle;

hw_timer_t *wifiTimer = NULL;

volatile bool wifiCheck = false;

void IRAM_ATTR onWifiTimer() {
  wifiCheck = true;
}

void connectWiFi() {

  if (WiFi.status() != WL_CONNECTED) {

    WiFi.begin(ssid, password);
  }
}
void eyesLoopTask(void * parameter) {

  eyesSetup();

  while(true) {
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void carLoopTask(void * parameter) {
  carSetup();

  while(true) {

    carLoop();
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.begin(115200);
  connectWiFi();
  wifiTimer = timerBegin(1000000);

  timerAttachInterrupt(wifiTimer, &onWifiTimer);
  timerAlarm(wifiTimer, 500000, true, 0);

  xTaskCreatePinnedToCore(
    eyesLoopTask,
    "EyesTask",
    10000,
    NULL,
    1,
    &eyesTaskHandle,
    0
  );

  xTaskCreatePinnedToCore(
    carLoopTask,
    "CarTask",
    10000,
    NULL,
    1,
    &carTaskHandle,
    1
  );
}

void loop() {

  if(wifiCheck) {
    wifiCheck = false;
    connectWiFi();
  }
}
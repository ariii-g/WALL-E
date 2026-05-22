#include <WiFi.h>

#include "EyesModule.h"
#include "CarModule.h"

const char* ssid = "1";
const char* password = "12345678";

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
  }

  eyesSetup();

  carSetup();
}

void loop() {

  carLoop();

  delay(1);
}
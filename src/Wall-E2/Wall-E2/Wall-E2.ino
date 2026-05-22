#include <WiFi.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <DHT.h>
#include <ThingerESP32.h>

const char* ssid = "1";
const char* password = "12345678";

#define USERNAME "silence"
#define DEVICE_ID "esp32_OP1"
#define DEVICE_CREDENTIAL "12345678"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
HardwareSerial mySerial(1);
DFRobotDFPlayerMini player;

#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
bool reproducirTemperatura = false;
unsigned long tiempoInicio = 0;
int audioTemperatura = 0;

void setup() {

  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
  }

  thing.add_wifi(ssid, password);
  mySerial.begin(9600, SERIAL_8N1, 18, 17);
  if(player.begin(mySerial)){

    player.volume(25);
  }
  thing["temperatura"] >> [](pson& out){

    float temp = dht.readTemperature();
    out = temp;
  };

  thing["saludo"] << [](pson& in){

    if(in){

      float temp = dht.readTemperature();
      player.playMp3Folder(1);

      if(temp < 20){
        audioTemperatura = 2;
      }

      else if(temp >= 20 && temp < 30){
        audioTemperatura = 3;
      }

      else{
        audioTemperatura = 4;
      }

      tiempoInicio = millis();
      reproducirTemperatura = true;
    }
  };
}

void loop() {

  thing.handle();

  if(reproducirTemperatura){
    if(millis() - tiempoInicio >= 4000){
      player.playMp3Folder(audioTemperatura);
      reproducirTemperatura = false;
    }
  }
}
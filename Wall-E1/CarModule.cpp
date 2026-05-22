#include "CarModule.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;

PubSubClient client(espClient);

#define ENA 12
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
#define ENB 13

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

int velocidad = 150;

#define SERVO1_PIN 14
#define SERVO2_PIN 37

Servo servo1;
Servo servo2;

void setVelocidad(int v) {

  ledcWrite(ENA, v);

  ledcWrite(ENB, v);
}

void moverAdelante() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setVelocidad(velocidad);
}

void moverAtras() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setVelocidad(velocidad);
}

void moverIzquierda() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setVelocidad(velocidad);
}

void moverDerecha() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setVelocidad(velocidad);
}

void parar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  setVelocidad(0);
}

void callback(char* topic, byte* payload, unsigned int length) {

  String message;

  for (int i = 0; i < length; i++) {

    message += (char)payload[i];
  }

  message.trim();

  message.toUpperCase();

  if (String(topic) == "carrito/direccion") {

    if (message == "FORWARD") moverAdelante();

    else if (message == "BACKWARD") moverAtras();

    else if (message == "LEFT") moverIzquierda();

    else if (message == "RIGHT") moverDerecha();

    else if (message == "STOP") parar();
  }

  if (String(topic) == "carrito/velocidad") {

    velocidad = constrain(message.toInt(), 0, 255);

    setVelocidad(velocidad);
  }

  if (String(topic) == "carrito/servo1") {

    servo1.write(constrain(message.toInt(), 0, 180));
  }

  if (String(topic) == "carrito/servo2") {

    servo2.write(constrain(message.toInt(), 0, 180));
  }
}

void reconnect() {

  while (!client.connected()) {

    if (client.connect("ESP32_CARRO")) {

      client.subscribe("carrito/direccion");

      client.subscribe("carrito/velocidad");

      client.subscribe("carrito/servo1");

      client.subscribe("carrito/servo2");

    } else {

      delay(2000);
    }
  }
}

void carSetup() {

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION);

  ledcAttach(ENB, PWM_FREQ, PWM_RESOLUTION);

  servo1.attach(SERVO1_PIN);

  servo2.attach(SERVO2_PIN);

  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);

  parar();
}

void carLoop() {

  if (!client.connected()) {

    reconnect();
  }

  client.loop();
}
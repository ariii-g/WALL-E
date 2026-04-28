#include <WiFi.h>
#include <PubSubClient.h>

#define ENA 12
#define IN1 9
#define IN2 10
#define IN3 3
#define IN4 8
#define ENB 13

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8
#define PWM_CHANNEL_ENA 0
#define PWM_CHANNEL_ENB 1

const char* ssid = "1";
const char* password = "12345678";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

int velocidadActual = 200;
bool prueba = true;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "carrito/velocidad") {
    velocidadActual = constrain(message.toInt(), 0, 255);
    actualizarVelocidad();
  }

  if (String(topic) == "carrito/direccion") {
    if (message == "FORWARD") moverAdelante();
    else if (message == "BACKWARD") moverAtras();
    else if (message == "LEFT") moverIzquierda();
    else if (message == "RIGHT") moverDerecha();
    else if (message == "STOP") parar();
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32S3_Carrito")) {
      client.subscribe("carrito/direccion");
      client.subscribe("carrito/velocidad");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttachChannel(ENA, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL_ENA);
  ledcAttachChannel(ENB, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL_ENB);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (prueba) {
    moverAdelante();
    delay(2000);

    parar();
    delay(1000);

    moverIzquierda();
    delay(1000);

    parar();
    delay(1000);

    moverDerecha();
    delay(1000);

    parar();
    delay(1000);

    moverAtras();
    delay(2000);

    parar();
    delay(2000);
  } 
  else {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
}

void moverAdelante() {
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
  actualizarVelocidad();
}

void moverAtras() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
  actualizarVelocidad();
}

void moverIzquierda() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
  actualizarVelocidad();
}

void moverDerecha() {
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
  actualizarVelocidad();
}

void parar() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);

  ledcWrite(PWM_CHANNEL_ENA, 0);
  ledcWrite(PWM_CHANNEL_ENB, 0);
}

void actualizarVelocidad() {
  ledcWrite(PWM_CHANNEL_ENA, velocidadActual);
  ledcWrite(PWM_CHANNEL_ENB, velocidadActual);
}

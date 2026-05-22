# WALL-E
<img width="104" height="160" alt="WhatsApp Image 2026-05-15 at 12 03 38 PM" src="https://github.com/user-attachments/assets/0df7fe9a-b929-4298-8fa0-5668af0e6ee0" /><br>
Integrantes:<br>
Ariana Sofia Garcia Duran <br>
Valentina Carrillo Cardenas<br>
Daniel Santiago Salavarrieta C<br>
Descripción:<br>
Wall-E es un robot con implementación de la tecnología IoT, tiene el proposito de monitorear la temperatura y el ambiente de nuestro entorno mediante una interfaz visual colaborativa, siendo >
Ademas, el usuario tendrá la capacidad de manipular los movimientos del robot, generando una experiencia didáctica.<br>
Diagrama Simple:<br>
![WhatsApp Image 2026-04-14 at 9 59 14 AM](https://github.com/user-attachments/assets/62454d6b-5d29-4940-9dc5-45cdefc2caae)

Librerías utilizadas:<br>
- Para el control de las pantallas se utilizan las librerías SPI.h y DIYables_TFT_Round.h, las cuales permiten la comunicación con las pantallas TFT, el dibujo de figuras y el manejo de colores.<br>
- Para el control de movimiento del robot se utilizan las librerías WiFi.h y PubSubClient.h, que permiten la conexión a una red WiFi y la comunicación mediante MQTT para enviar y recibir comandos.<br>
- Se utilizan funciones PWM del ESP32-S3 para controlar la velocidad de los motores y ESP32Servo.h para el control de brazos.<br>
- HardwareSerial.h, DFRobotDFPlayerMini.h, DHT.h, ThingerESP32.h para el monitoreo con el sensor DHT11, control de audio con el modulo DFPlayer mini y la conexión a Thinger.io mediante wi-fi para visualizar estadísticas y dar el comando de voz <br> 

Codigo:<br>
(Debido a condiciones de interferencia causada en los procesos, se tuvo que utilizar dos ESP32-S3, una encargada de movimiento de motores y de pantallas, y la otra encargada de monitorear el amibiente y reproducir el audio)<br>
ESP de Motores:<br>
- CarModule.cpp y .h -> Realizan la conexión Wi-Fi de el microcontrolador y del broker, permite el movimiento de motores con respecto a las instrucciones enviadas.<br>
- EyesModule.cpp y .h -> Envía comandos de proyección a las pantallas que estan conectadas en paralelo, indicando el ciclo a seguir para los ojos.<br>
ESP de monitoreo:<br> 
- WALL-E2.ino -> Código principal de monitoreo de la temperatura, reproducción de voz y conexión a thinger.io

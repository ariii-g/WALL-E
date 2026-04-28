# WALL-E
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
Para el control de las pantallas y los ojos animados se utilizan las librerías SPI.h y DIYables_TFT_Round.h, las cuales permiten la comunicación con las pantallas TFT, el dibujo de figuras y el manejo de colores.<br>
Para el control de movimiento del robot se utilizan las librerías WiFi.h y PubSubClient.h, que permiten la conexión a una red WiFi y la comunicación mediante MQTT para enviar y recibir comandos.<br>
Además, se utilizan funciones PWM del ESP32-S3 para controlar la velocidad de los motores.<br>

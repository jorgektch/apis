#include <WiFi.h>
#include <HTTPClient.h>
#include<ESP32Servo.h>
#include "UltraSonic.h"
#include "Controller.h"
//#include "ServoC.h"
#include "Alarm.h"

UltraSonic ultrasonic(2, 4);
//ServoC servo(23);
Alarm alarmC(5);

const char *ssid = "LT";
const char *password = "prudencio";

const char* serverUrl = "https://shark-app-ko77v.ondigitalocean.app/detections/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.19.129/capture";

void setup() {
  // put your setup code here, to run once:
  ultrasonic.begin();
  //servo.begin();
  alarmC.begin();
  WiFi.begin(ssid, password);
  Serial.begin(19200);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }
}

int find = false;

void loop() {
  // put your main code here, to run repeatedly:
  int d = ultrasonic.getDistance();
  find = d>=0 && d<=20;
  /* Serial.print("Distance: ");
  Serial.println(d); */
  delay(500);
  if (find) {
    if (WiFi.status() == WL_CONNECTED) {
      alarmC.tick();
      HTTPClient http;
      http.begin(cameraServer);

      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          // Imprimir la data de la imagen en el monitor serial
          String image_data = http.getString();
          if (isBird(image_data)) {
            Serial.println("Pajaro detectado!");
          }
        } else {
          Serial.println("Error en la solicitud HTTP");
        }
      } else {
        Serial.println("Error en la conexión HTTP");
      }

      http.end();
    }
  }
}

      /* HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", contentType);
      String payload = "{\"frame\": null, \"video\": null, \"latitude\": " + String(-10.049) + ", \"longitude\": " + String(-79.06) + "}";
      int httpResponseCode = http.POST(payload);
      String response = http.getString();
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
      http.end(); */
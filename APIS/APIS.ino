#include <WiFi.h>
#include <HTTPClient.h>
#include<ESP32Servo.h>
#include "UltraSonic.h"
#include "ServoC.h"
#include "Alarm.h"

UltraSonic ultrasonic(2, 4);
ServoC servo(23);
Alarm alarmC(5);

const char *ssid = "LTR";
const char *password = "2J8LQV5L";

const char* serverUrl = "https://shark-app-ko77v.ondigitalocean.app/detections/";
const char* contentType = "application/json";

void setup() {
  // put your setup code here, to run once:
  ultrasonic.begin();
  servo.begin();
  alarmC.begin();
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }
}
int pos = 20;
int f = 1;
int find = false;
int x = 0;

void loop() {
  // put your main code here, to run repeatedly:
  int d = ultrasonic.getDistance();
  find = d>=0 && d<=20;
  Serial.print("Distance: ");
  Serial.println(d);
  Serial.print("x: ");
  Serial.println(x);
  delay(500);
  if (find) {
    /* if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", contentType);
      String payload = "{\"frame\": null, \"video\": null, \"latitude\": " + String(-10.049) + ", \"longitude\": " + String(-79.06) + "}";
      int httpResponseCode = http.POST(payload);
      String response = http.getString();
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
      http.end();
    } */
    alarmC.tick();
    x = 0;
  } else {
    if (pos==20) {
      f = 1;
    }
    if (pos==140) {
      f = -1;
    }
    x++;
    if (x==5) {
      pos += f*40;
      servo.move(pos);
      x = 0;
    }    
  }
}

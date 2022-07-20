
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "water-dam-default-rtdb.firebaseio.com"  //Database link
#define FIREBASE_AUTH "fVZnbBvbQ5oBcTYuVm2HWAn3onEsDUK3cTqkh2IN"  //Database secrate

#define WIFI_SSID "WATER DAM"      //Router name
#define WIFI_PASSWORD "12345678"  //Router password




FirebaseData firebaseData, loadData;
FirebaseJson json;


#define echoPin D7
#define trigPin D6

float distance = 0, lowerLimit = 300, upperLimit = 600, out;//mm



void senW(String field, float value) {
  Firebase.setString(firebaseData, "/AnalogOutput/" + field, String(value) );

}



void initFire() {
  pinMode(D4, OUTPUT);
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(D4, 0);
    Serial.print(".");
    delay(200);
    digitalWrite(D4, 1);
    Serial.print(".");
    delay(200);

  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}














void setup() {
  pinSet();

  Serial.begin(9600);
  initFire();

}



void pinSet() {
  pinMode(D5, OUTPUT); //LED

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}




void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distance =  pulseIn(echoPin, HIGH) * (0.034 / 2) * 10;
  delay(1000);
  out = map(distance, lowerLimit, upperLimit, 0, 100);
  if (out < 0)
    out = 0;
  else if (out > 100)
    out = 100;

  out=100-out;

  if (out < 20) {
    digitalWrite(D5, 1);
    delay(200);
    digitalWrite(D5, 0);
    delay(200);
    digitalWrite(D5, 1);
    delay(200);
    digitalWrite(D5, 0);
  } else if (out > 90) {
    digitalWrite(D5, 1);
  }
  else {
    digitalWrite(D5, 1);
    delay(200);
    digitalWrite(D5, 0);
    delay(200);
  }


  Serial.println(String(out) + " | " + String(distance));
  senW("Water Level", out);

}

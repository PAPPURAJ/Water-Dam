#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>



#define FIREBASE_HOST "iot-based-water-dam-default-rtdb.firebaseio.com"  //Database link
#define FIREBASE_AUTH "OEPj6svcFbYMBBpSicKP13FbjO8l5eGa0MCwumOg"  //Database secrate

#define WIFI_SSID "PAPPURAJ"      //Router name
#define WIFI_PASSWORD "5555555."  //Router password




FirebaseData firebaseData, loadData;
FirebaseJson json;


#define echoPin D7
#define trigPin D6

float distance = 0, lowerLimit = 300, upperLimit = 600, out;



void senW(String field, float value) {
  Firebase.setString(firebaseData, "/AnalogOutput/" + field, String(value) );

}

String senR(String field) {
  if (Firebase.getString(loadData, "/AnalogInput/" + field)) {
    return loadData.stringData();
  }
}

//Sending data
void loadW(String field, int value) {
  Firebase.setString(firebaseData, "/Load/" + field, String(value) );

}

//Receiving data
String loadR(String field) {
  if (Firebase.getString(loadData, "/Load/" + field)) {
    return loadData.stringData();
  }
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

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}














void setup() {
  pinSet();

  Serial.begin(9600);
  initFire();

  loadW("Test", 1);

  Serial.println("Load: " + loadR("Test"));
  Serial.println("Sen: " + senR("Test"));


}



void pinSet() {
  pinMode(D1, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

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

  digitalWrite(D1, out < 10);
  digitalWrite(D5, out > 90);


  Serial.println(String(out) + " | " + String(distance));
  senW("Water Level", 100 - out);

}

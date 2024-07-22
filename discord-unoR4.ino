#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Wire.h>
#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <secrets.h>

// wifi/http constants
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;
int status = WL_IDLE_STATUS;
const char* server_address = "10.0.0.77";
const String path = "/send";
const String contentType = "application/json";
const String postData = "{\"content\":\""; 
int port = 8000;


// ultrasonic sensor constants
const int trigPin = 9;
const int echoPin = 10;
const float OFFSET = 2.5;
float duration, distance, prevDistance = 0;



WiFiClient wifi;
HttpClient http = HttpClient(wifi, server_address, port);



void sendMessage(String message);

void setup() {
  Serial.begin(115200);
  connectWifi();
  setupSensor();
  Serial.println("Sending post request");
  
  String postData = "{\"content\":\""; 
  postData += "Arduino is online";
  postData += "\"}";
  http.post(path, contentType, postData);
  Serial.println(http.responseStatusCode());
  String response = http.responseBody();
  Serial.println(response);
}

void loop() {
  triggerSensor();
  checkDistance(distance);
  delay(200);
}

void flushWiFiBuffer() {
  while (wifi.available()) {
    wifi.read();
  }
}

void connectWifi() {
  Serial.println("Connecting to WiFi...");
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Connecting");
    //printMessage(connecting);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 5 seconds for connection:
    delay(5000);    
  }
  Serial.println();
  
  Serial.println("Connected to WiFi.");
}

void checkDistance(float distance) {
  if(distance >= prevDistance - OFFSET && distance <= prevDistance + OFFSET) { Serial.print("."); }
  else if(prevDistance != 0) {
    Serial.println("");
    Serial.print("Object detected: ");
    Serial.println(distance);
    Serial.println("");
    String postData = "{\"content\":\""; 
    postData += "object detected! distance: ";
    postData += distance;
    postData += " cm";
    postData += "\"}";
    http.post(path, contentType, postData);
    Serial.println(http.responseStatusCode());
    String response = http.responseBody();
    http.read();
    Serial.println(response);
    delay(2000);
  }
  prevDistance = distance;
}


void setupSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Ultrasonic sensor ready");
}

void triggerSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
}

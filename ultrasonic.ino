/**
 * 
 * ultrasonic sensor test using Arduino UNO R4 Wifi.
 * 
 * VCC to 5V
 * Trig to pin 9
 * Echo to pin 10
 * GND to GND
 * 
 * offset is 2.5 cm to detect objects in proximity.
 * 
 */
const int trigPin = 9;
const int echoPin = 10;
const float OFFSET = 2.5;

float duration, distance, prevDistance = 0;

void checkDistance(float distance) {
  /* if distance is within offset, continue printing */
  if(distance >= prevDistance - OFFSET && distance <= prevDistance + OFFSET) {
    Serial.print(".");
  }
  /* otherwise print object detected */
  else if(prevDistance != 0) {
    Serial.println("");
    Serial.print("Object detected: ");
    Serial.println(distance);
    Serial.println("");
  }
  prevDistance = distance;
}

void setup() {
  // setting pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // start pin in low
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // set sonic burst to bounce back
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);



  // when receiver is hit, sets echoPin to high
  duration = pulseIn(echoPin, HIGH);
  // read duration of bounce and convert to centimeters
  distance = (duration*.0343)/2;
  checkDistance(distance);
  // print distance
  /*
  Serial.print("Distance: ");
  Serial.println(distance);
  */
  delay(100);
}

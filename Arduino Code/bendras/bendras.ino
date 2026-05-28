#include <RobotIRremote.h>
#include <RobotIRremoteInt.h>
#include <RobotIRremoteTools.h>
#include <MotorDriver.h>
#include <ArduinoRobotMotorBoard.h>
#include <Servo.h>
Servo myservo;
MotorDriver m;
#define echoPin 2
#define trigPin 3 
long duration;
int distance;
void setup() {
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  myservo.attach(9);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}

void loop() {
  
  m.motor(1,FORWARD,10000);
  m.motor(2,FORWARD,10000);  
  delay(100000);
  m.motor(1,BACKWARD,10000);
  m.motor(2,BACKWARD,10000);  
    delay(100000);

  m.motor(1,FORWARD,10000);
  m.motor(2,FORWARD,10000);  
    delay(100000);

  m.motor(1,BACKWARD,10000);
  m.motor(2,BACKWARD,10000);
    delay(100000);
  
  m.motor(1,FORWARD,10000);
  m.motor(2,FORWARD,10000);  
  
  myservo.write(180); //clockwise rotation
  delay(1000); //rotation duration in ms
  delay(100); //short pause
  myservo.write(0); //counterclockwise rotation
  delay(100); //rotation duration in ms
  delay(100); //short pause myservo.attach(9);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");
}

#include <MotorDriver.h>
MotorDriver m;
#include <Servo.h>
Servo myservo;
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup()
{
  myservo.attach(9);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}


void loop()
{
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
  delay(10);
  myservo.write(180); //clockwise rotation
  delay(1000); //rotation duration in ms
  myservo.write(10); //counterclockwise rotation
  delay(1000); //rotation duration in ms
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

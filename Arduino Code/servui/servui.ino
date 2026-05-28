#include <MotorDriver.h>
#include <Servo.h>

MotorDriver m;
Servo myservo;
void setup() {
myservo.attach(9);
}
void loop() {

m.motor(1, FORWARD, 500);
m.motor(2, FORWARD, 500);

delay(500);
myservo.write(180); //clockwise rotation
delay(300);//rotation duration in ms
myservo.write(0);//unterclockwise rotation
delay(600); //rotation duration in ms
myservo.write(90);//clockwise rotation
delay(1000); //rotation duration in ms
delay(500);
} 

#include <MotorDriver.h>

MotorDriver m;

void setup(){}

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
}

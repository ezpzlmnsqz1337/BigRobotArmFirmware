#include "MyServo.h"
#include "Arduino.h"

MyServo::MyServo(int32_t _servoPin, int32_t _minimum, int32_t _maximum, int32_t _pos)
    : servoPin(_servoPin), minimum(_minimum), maximum(_maximum), pos(_pos), targetPosition(_pos)
{
}

void MyServo::init()
{
  if (!servo.attached())
  {
    servo.attach(servoPin);
    servo.write(pos);
  }
}

void MyServo::deinit()
{
  if (servo.attached())
  {
    servo.detach();
  }
}

bool MyServo::isEnabled()
{
  return servo.attached();
}

int32_t MyServo::getPosition()
{
  return pos;
}

void MyServo::setTargetPosition(const int32_t target)
{
  targetPosition = target;
}

void MyServo::moveBy(int32_t value)
{
  int32_t newValue = constrain(pos + value, minimum, maximum);
  servo.write(newValue);
  pos = newValue;
}

void MyServo::moveTo(int32_t value)
{
  // if already there, return
  if (pos == value)
  {
    return;
  }

  // get value in between the servo limits
  int32_t newValue = constrain(value, minimum, maximum);

  // increment with delay to slow down the servo movement
  int32_t increment = newValue > pos ? +1 : -1;
  while (pos != newValue)
  {
    moveBy(increment);
    delay(15);
  }
  pos = newValue;
}

void MyServo::loop()
{
  // if already there, return
  if (pos == targetPosition)
  {
    return;
  }

  if (pos > targetPosition)
  {
    pos--;
    servo.write(pos);
  }

  if (pos < targetPosition)
  {
    pos++;
    servo.write(pos);
  }
}

void MyServo::sweep()
{
  for (pos = minimum; pos <= maximum; pos += 1)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);        // waits 15ms for the servo to reach the position
  }
  for (pos = maximum; pos >= minimum; pos -= 1)
  {                   // goes from 180 degrees to 0 degrees
    servo.write(pos); // tell servo to go to pos in variable 'pos'
    delay(15);        // waits 15ms for the servo to reach the pos
  }
}

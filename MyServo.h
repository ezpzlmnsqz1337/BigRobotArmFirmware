#ifndef MY_SERVO_H
#define MY_SERVO_H

#include <Servo.h>

class MyServo
{
private:
  int32_t minimum;
  int32_t maximum;
  int32_t pos;
  int32_t servoPin;
  int32_t targetPosition;
  Servo servo;

public:
  MyServo(int32_t _servoPin, int32_t _minimum, int32_t _maximum, int32_t _pos);

  void init();

  void deinit();

  bool isEnabled();

  int32_t getPosition();

  void setTargetPosition(const int32_t target);

  void moveBy(int32_t value);

  void moveTo(int32_t value);

  void loop();

  void sweep();
};

#endif

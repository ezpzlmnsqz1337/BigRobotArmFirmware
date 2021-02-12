#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "Base.h"
#include "Elbow.h"
#include "Gripper.h"
#include "Shoulder.h"
#include "Wrist.h"

#include "Structures.h"

#include "MultiStepper.h"

class ArmBuilder
{
private:
  Base mBase;
  Shoulder mShoulder;
  Elbow mElbow;
  Wrist mWristRotate;
  Wrist mWrist;
  Gripper mGripper;

protected:
  long getNormalizedValue(const long value);

  float mDefaultSpeed = 200 * 16;        // stepps per mm
  float mDefaultAcceleration = 200 * 16; // stepps per mm

  float mSpeedMultiplier = 1;
  float mAccelerationMultiplier = 1;

public:
  ArmBuilder();

  void init();

  void goTo(const JointPositions& jp);
  void move(const JointPositions& jp);

  void setSpeed(const float speed);
  void setAcceleration(const float acceleration);
  void setZeroPositoin();

  bool reachedPositions(const JointPositions& jp);

  JointPositions getPositions();

  Base& getBase();
  Shoulder& getShoulder();
  Elbow& getElbow();
  Wrist& getWristRotate();
  Wrist& getWrist();
  Gripper& getGripper();
};

// This is the end of the header guard
#endif

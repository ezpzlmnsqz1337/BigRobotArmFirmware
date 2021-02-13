#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "Config.h"
#include "Gripper.h"
#include "RobotArmJoint.h"
#include "Structures.h"

class ArmBuilder
{
private:
  RobotArmJoint mBase;
  RobotArmJoint mShoulder;
  RobotArmJoint mElbow;
  RobotArmJoint mWristRotate;
  RobotArmJoint mWrist;
  Gripper mGripper;

protected:
  long getNormalizedValue(const long value);

  float mSpeedMultiplier = 1;
  float mAccelerationMultiplier = 1;

public:
  ArmBuilder();

  void init();

  void goTo(const JointPositions& jp);
  void move(const JointPositions& jp);

  void setSpeed(const float speed);
  void setAcceleration(const float acceleration);
  void setZeroPosition();

  bool reachedPositions(const JointPositions& jp);

  JointPositions getPositions();

  RobotArmJoint& getBase();
  RobotArmJoint& getShoulder();
  RobotArmJoint& getElbow();
  RobotArmJoint& getWristRotate();
  RobotArmJoint& getWrist();
  Gripper& getGripper();
};

// This is the end of the header guard
#endif

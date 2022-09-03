#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "Gripper.h"
#include "MultiStepper.h"
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
  MultiStepper mMultiStepper;

protected:
  long getNormalizedValue(const long value);
  float calculateTimeToPosition(const long position, const long target, const float speed);
  float findMaxInArray(const float* array, const int32_t size);

  float mSpeedMultiplier = 1;
  float mAccelerationMultiplier = 1;

  bool mSyncMotors = false;

public:
  ArmBuilder();

  void init();

  void goTo(const JointOptions& jp);
  void goToSyncMultiStepper(const JointOptions& jp);
  void goToSync(const JointOptions& jp);
  void move(const JointOptions& jp);

  void handleGripper(const GripperOptions& go);

  void setSpeed(const float speed);
  void setAcceleration(const float acceleration);
  void setZeroPosition();

  void setSpeeds(const JointOptions& jp);
  void setAccelerations(const JointOptions& ja);

  bool isSyncEnabled();
  void setSyncMotors(const bool sync);

  bool reachedPositions(const JointOptions& jp);

  JointOptions getPositions();
  JointOptions getAccelerations();
  JointOptions getSpeeds();
  GripperOptions getGripperOptions();

  RobotArmJoint& getBase();
  RobotArmJoint& getShoulder();
  RobotArmJoint& getElbow();
  RobotArmJoint& getWristRotate();
  RobotArmJoint& getWrist();
  Gripper& getGripper();
};

// This is the end of the header guard
#endif

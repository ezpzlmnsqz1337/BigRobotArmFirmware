#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

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
  float calculateTimeToPosition(const long position, const long target, const float speed);
  float findMaxInArray(const float* array, const int32_t size);

  float mSpeedMultiplier = 1;
  float mAccelerationMultiplier = 1;

  bool mSyncMotors = false;

public:
  ArmBuilder();

  void init();

  void goTo(const JointPositions& jp);
  void goToSync(const JointPositions& jp);
  void move(const JointPositions& jp);

  void setSpeed(const float speed);
  void setAcceleration(const float acceleration);
  void setZeroPosition();

  void setSpeeds(const JointSpeeds& jp);
  void setAccelerations(const JointAccelerations& ja);

  bool isSyncEnabled();
  void setSyncMotors(const bool sync);

  bool reachedPositions(const JointPositions& jp);

  JointPositions getPositions();
  JointAccelerations getAccelerations();
  JointSpeeds getSpeeds();

  RobotArmJoint& getBase();
  RobotArmJoint& getShoulder();
  RobotArmJoint& getElbow();
  RobotArmJoint& getWristRotate();
  RobotArmJoint& getWrist();
  Gripper& getGripper();
};

// This is the end of the header guard
#endif

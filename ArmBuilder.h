#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "Gripper.h"
#include "MotionQueue.h"
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
  MotionQueue mMotionQueue;
  JointPositions mActiveMotionTarget{};
  bool mMotionActive = false;

protected:
  long getNormalizedValue(const long value);
  float calculateTimeToPosition(const long position, const long target, const float speed);
  float findMaxInArray(const float* array, const int32_t size);
  void startMotion(const JointPositions& jp);

  float mSpeedMultiplier = 1;
  float mAccelerationMultiplier = 1;

  bool mSyncMotors = false;

public:
  ArmBuilder();

  void init();

  void goTo(const JointPositions& jp);
  void goToSyncMultiStepper(const JointPositions& jp);
  void goToSync(const JointPositions& jp);
  bool queueMove(const JointPositions& jp);
  void serviceMotion();
  bool isMotionActive();
  bool hasQueuedMoves() const;
  bool isMotionPending() const;
  void flushMotionQueue();
  void clearMotionQueue();
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

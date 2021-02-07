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

  static const int MAX_POSITIONS = 50;

  JointPositions savedPositions[MAX_POSITIONS];
  int numOfSavedPositions = 0;
  int currentPositionId = 0;

  float mDefaultSpeed = 200 * 16;        // stepps per mm
  float mDefaultAcceleration = 200 * 16; // stepps per mm

  float mSpeedMultiplier = 1;
  float mAccelerationMultiplier = 1;

  AccelStepper mMotors[5];

public:
  ArmBuilder();

  void init();
  void loop(const long leftX, const long leftY, const long rightX, const long rightY, const bool isSwitched);
  void repeatPositions();

  void move(const long base, const long shoulder, const long elbow, const long wristRotate, const long wrist);
  void goTo(const long base, const long shoulder, const long elbow, const long wristRotate, const long wrist,
            const int gripper);

  void goTo(const JointPositions& jp);
  void move(const JointPositions& jp);

  void setSpeed(const float speed);
  void setAcceleration(const float acceleration);
  void setZeroPositoin();
  AccelStepper* getMotors();

  bool reachedPositions(const JointPositions& jp);
  JointPositions getPositions();

  void save();

  Base& getBase();
  Shoulder& getShoulder();
  Elbow& getElbow();
  Wrist& getWristRotate();
  Wrist& getWrist();
  Gripper& getGripper();
};

// This is the end of the header guard
#endif

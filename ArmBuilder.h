#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "Base.h"
#include "Elbow.h"
#include "Gripper.h"
#include "Shoulder.h"
#include "Wrist.h"

#include "MultiStepper.h"

class ArmBuilder
{
private:
  Base mBase;
  Shoulder mShoulder;
  Elbow mElbow;
  Wrist mWrist1;
  Wrist mWrist2;
  Gripper mGripper;

  MultiStepper mSteppers;

protected:
  int getNormalizedValue(const int value);

public:
  ArmBuilder();

  void init();

  void move(const int base, const int shoulder, const int elbow, const int wrist1, const int wrist2);
  void goTo(const int base, const int shoulder, const int elbow, const int wrist1, const int wrist2);

  const Base& getBase();
  const Shoulder& getShoulder();
  const Elbow& getElbow();
  const Wrist& getWrist1();
  const Wrist& getWrist2();
};

// This is the end of the header guard
#endif

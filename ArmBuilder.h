#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "Base.h"
#include "Shoulder.h"
#include "Elbow.h"
#include "Wrist.h"

#include "MultiStepper.h"

class ArmBuilder
{
private:
  Base mBase;
  Shoulder mShoulder;
  Elbow mElbow;
  Wrist mWrist;

  MultiStepper mSteppers;

public:
  ArmBuilder();

  void init();

  void move(const int base, const int shoulder, const int elbow, const int wrist);
  void goTo(const int base, const int shoulder, const int elbow, const int wrist);

  const Base& getBase();
  const Shoulder& getShoulder();
  const Elbow& getElbow();
  const Wrist& getWrist();
};

// This is the end of the header guard
#endif

#include "Elbow.h"

Elbow::Elbow() : AbstractArmPart::AbstractArmPart()
{
  mStepPin = 46;
  mDirPin = 48;
  mEnablePin = 62;
  mCsPin = 40;

  // elbow needs faster speed
  mMaxSpeed = mMaxSpeed * 2;
  mAcceleration = mAcceleration * 2;
}

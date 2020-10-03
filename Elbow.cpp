#include "Elbow.h"
#include "Arduino.h"

Elbow::Elbow() : AbstractArmPart::AbstractArmPart()
{
  mStepPin = 46;
  mDirPin = 48;
  mEnablePin = 62;
  mCsPin = 40;
}

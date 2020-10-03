#include "Shoulder.h"
#include "Arduino.h"

Shoulder::Shoulder()
{
  AbstractArmPart::AbstractArmPart();
  mStepPin = 60;
  mDirPin = 61;
  mEnablePin = 56;
  mCsPin = 49;
}

#include "Wrist.h"
#include "Arduino.h"

Wrist::Wrist()
{
  AbstractArmPart::AbstractArmPart();
  mStepPin = 26;
  mDirPin = 28;
  mEnablePin = 24;
  mCsPin = 42;
}

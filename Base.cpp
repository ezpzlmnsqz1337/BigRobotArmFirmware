#include "Base.h"

Base::Base() : AbstractArmPart::AbstractArmPart()
{
  mStepPin = 54;
  mDirPin = 55;
  mEnablePin = 38;
  mCsPin = 53;
}

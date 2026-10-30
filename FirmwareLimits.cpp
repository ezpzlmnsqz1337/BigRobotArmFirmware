#include "FirmwareLimits.h"

#include "Config.h"

int32_t clampConfiguredValue(int32_t value, long minValue, long maxValue)
{
  if (value < minValue)
  {
    return static_cast<int32_t>(minValue);
  }

  if (value > maxValue)
  {
    return static_cast<int32_t>(maxValue);
  }

  return value;
}

int32_t clampGripperPosition(const int32_t value)
{
  return clampConfiguredValue(value, Config::MIN_GRIPPER_POSITION, Config::MAX_GRIPPER_POSITION);
}
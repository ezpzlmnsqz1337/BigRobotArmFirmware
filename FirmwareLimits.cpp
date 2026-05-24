#include "FirmwareLimits.h"

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
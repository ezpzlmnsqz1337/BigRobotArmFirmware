#ifndef FIRMWARE_LIMITS_H
#define FIRMWARE_LIMITS_H

#include <stdint.h>

int32_t clampConfiguredValue(int32_t value, long minValue, long maxValue);

#endif
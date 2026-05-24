#include <unity.h>

#include "Config.h"
#include "FirmwareLimits.h"

void setUp()
{
}

void tearDown()
{
}

void test_clamp_configured_value_keeps_in_range_value()
{
  TEST_ASSERT_EQUAL_INT32(125, clampConfiguredValue(125, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
}

void test_clamp_configured_value_raises_low_value_to_minimum()
{
  TEST_ASSERT_EQUAL_INT32(Config::MIN_SPEED_MULTIPLIER,
                          clampConfiguredValue(0, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
}

void test_clamp_configured_value_caps_high_value_at_maximum()
{
  TEST_ASSERT_EQUAL_INT32(Config::MAX_ACCEL_MULTIPLIER,
                          clampConfiguredValue(900, Config::MIN_ACCEL_MULTIPLIER, Config::MAX_ACCEL_MULTIPLIER));
}

int main(int argc, char** argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_clamp_configured_value_keeps_in_range_value);
  RUN_TEST(test_clamp_configured_value_raises_low_value_to_minimum);
  RUN_TEST(test_clamp_configured_value_caps_high_value_at_maximum);

  return UNITY_END();
}
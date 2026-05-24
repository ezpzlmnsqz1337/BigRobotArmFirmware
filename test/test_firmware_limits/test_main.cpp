#include <unity.h>

#include "Config.h"
#include "FirmwareLimits.h"
#include "SequenceCommandRules.h"

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

void test_parse_sequence_repetitions_defaults_to_one_without_token()
{
  TEST_ASSERT_EQUAL_INT8(1, parseSequenceRepetitions(nullptr));
}

void test_parse_sequence_repetitions_reads_iteration_token()
{
  char repetitionsToken[] = "I5";
  TEST_ASSERT_EQUAL_INT8(5, parseSequenceRepetitions(repetitionsToken));
}

void test_parse_sequence_repetitions_defaults_to_one_for_unexpected_prefix()
{
  char repetitionsToken[] = "X5";
  TEST_ASSERT_EQUAL_INT8(1, parseSequenceRepetitions(repetitionsToken));
}

void test_can_add_sequence_command_allows_values_below_capacity()
{
  TEST_ASSERT_TRUE(canAddSequenceCommand(MAX_SEQUENCE_COMMANDS - 1));
}

void test_can_add_sequence_command_blocks_values_at_capacity()
{
  TEST_ASSERT_FALSE(canAddSequenceCommand(MAX_SEQUENCE_COMMANDS));
}

int main(int argc, char** argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_clamp_configured_value_keeps_in_range_value);
  RUN_TEST(test_clamp_configured_value_raises_low_value_to_minimum);
  RUN_TEST(test_clamp_configured_value_caps_high_value_at_maximum);
  RUN_TEST(test_parse_sequence_repetitions_defaults_to_one_without_token);
  RUN_TEST(test_parse_sequence_repetitions_reads_iteration_token);
  RUN_TEST(test_parse_sequence_repetitions_defaults_to_one_for_unexpected_prefix);
  RUN_TEST(test_can_add_sequence_command_allows_values_below_capacity);
  RUN_TEST(test_can_add_sequence_command_blocks_values_at_capacity);

  return UNITY_END();
}
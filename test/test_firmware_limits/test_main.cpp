#include <unity.h>

#include "CommandDispatchRules.h"
#include "CommandInputRules.h"
#include "CommandResponseRules.h"
#include "Config.h"
#include "FirmwareLimits.h"
#include "MotionQueue.h"
#include "SequenceLifecycleRules.h"
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

void test_classify_command_token_rejects_null_and_empty_values()
{
  char emptyCommand[] = "";
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(nullptr));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(emptyCommand));
}

void test_classify_command_token_recognizes_supported_g_commands()
{
  char g0Command[] = "G0";
  char g1Command[] = "G1";
  char g28Command[] = "G28";
  char g92Command[] = "G92";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G0, classifyCommandToken(g0Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G1, classifyCommandToken(g1Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G28, classifyCommandToken(g28Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G92, classifyCommandToken(g92Command));
}

void test_classify_command_token_recognizes_supported_m_commands()
{
  char m201Command[] = "M201";
  char m203Command[] = "M203";
  char m503Command[] = "M503";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_M201, classifyCommandToken(m201Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_M203, classifyCommandToken(m203Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_M503, classifyCommandToken(m503Command));
}

void test_classify_command_token_recognizes_sequence_and_sync_commands()
{
  char beginCommand[] = "BEGIN";
  char endCommand[] = "END";
  char syncCommand[] = "S1";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_BEGIN, classifyCommandToken(beginCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_END, classifyCommandToken(endCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_SYNC, classifyCommandToken(syncCommand));
}

void test_classify_command_token_rejects_unknown_commands()
{
  char unknownGCommand[] = "G999";
  char unknownMCommand[] = "M999";
  char unknownPrefixCommand[] = "X1";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(unknownGCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(unknownMCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(unknownPrefixCommand));
}

void test_has_processable_command_token_rejects_null_and_empty_values()
{
  char emptyCommand[] = "";

  TEST_ASSERT_FALSE(hasProcessableCommandToken(nullptr));
  TEST_ASSERT_FALSE(hasProcessableCommandToken(emptyCommand));
}

void test_has_processable_command_token_accepts_non_empty_values()
{
  char command[] = "G0";

  TEST_ASSERT_TRUE(hasProcessableCommandToken(command));
}

void test_can_append_command_byte_allows_lengths_below_buffer_size()
{
  const int commandBufferSize = 50;
  TEST_ASSERT_TRUE(canAppendCommandByte(commandBufferSize - 1, commandBufferSize));
}

void test_can_append_command_byte_blocks_length_at_buffer_size()
{
  const int commandBufferSize = 50;
  TEST_ASSERT_FALSE(canAppendCommandByte(commandBufferSize, commandBufferSize));
}

void test_can_execute_sequence_on_dispatch_requires_end_with_active_sequence()
{
  TEST_ASSERT_TRUE(canExecuteSequenceOnDispatch(COMMAND_DISPATCH_END, true));
  TEST_ASSERT_FALSE(canExecuteSequenceOnDispatch(COMMAND_DISPATCH_END, false));
}

void test_can_execute_sequence_on_dispatch_rejects_non_end_commands()
{
  TEST_ASSERT_FALSE(canExecuteSequenceOnDispatch(COMMAND_DISPATCH_BEGIN, true));
  TEST_ASSERT_FALSE(canExecuteSequenceOnDispatch(COMMAND_DISPATCH_G0, true));
}

void test_should_queue_command_in_active_sequence_reflects_sequence_state()
{
  TEST_ASSERT_TRUE(shouldQueueCommandInActiveSequence(true));
  TEST_ASSERT_FALSE(shouldQueueCommandInActiveSequence(false));
}

void test_ready_response_message_matches_ui_contract()
{
  TEST_ASSERT_EQUAL_STRING("BigRobotArm::READY", readyResponseMessage());
}

void test_invalid_command_response_message_matches_ui_contract()
{
  TEST_ASSERT_EQUAL_STRING("BigRobotArm::INVALID-COMMAND", invalidCommandResponseMessage());
}

void test_motion_queue_enqueues_and_dequeues_positions_in_order()
{
  MotionQueue queue;
  JointPositions first{1, 2, 3, 4, 5, 6};
  JointPositions second{7, 8, 9, 10, 11, 12};
  JointPositions dequeued{};

  TEST_ASSERT_TRUE(queue.enqueue(first));
  TEST_ASSERT_TRUE(queue.enqueue(second));
  TEST_ASSERT_EQUAL_UINT8(2, queue.size());

  TEST_ASSERT_TRUE(queue.dequeue(dequeued));
  TEST_ASSERT_EQUAL_INT32(1, dequeued.base);
  TEST_ASSERT_EQUAL_INT32(6, dequeued.gripper);

  TEST_ASSERT_TRUE(queue.dequeue(dequeued));
  TEST_ASSERT_EQUAL_INT32(7, dequeued.base);
  TEST_ASSERT_EQUAL_INT32(12, dequeued.gripper);
  TEST_ASSERT_TRUE(queue.isEmpty());
}

void test_motion_queue_rejects_enqueue_when_full()
{
  MotionQueue queue;
  JointPositions target{1, 2, 3, 4, 5, 6};

  for (uint8_t i = 0; i < MOTION_QUEUE_CAPACITY; i++)
  {
    TEST_ASSERT_TRUE(queue.enqueue(target));
  }

  TEST_ASSERT_TRUE(queue.isFull());
  TEST_ASSERT_FALSE(queue.enqueue(target));
}

void test_motion_queue_returns_false_when_empty()
{
  MotionQueue queue;
  JointPositions target{};

  TEST_ASSERT_FALSE(queue.dequeue(target));
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
  RUN_TEST(test_classify_command_token_rejects_null_and_empty_values);
  RUN_TEST(test_classify_command_token_recognizes_supported_g_commands);
  RUN_TEST(test_classify_command_token_recognizes_supported_m_commands);
  RUN_TEST(test_classify_command_token_recognizes_sequence_and_sync_commands);
  RUN_TEST(test_classify_command_token_rejects_unknown_commands);
  RUN_TEST(test_has_processable_command_token_rejects_null_and_empty_values);
  RUN_TEST(test_has_processable_command_token_accepts_non_empty_values);
  RUN_TEST(test_can_append_command_byte_allows_lengths_below_buffer_size);
  RUN_TEST(test_can_append_command_byte_blocks_length_at_buffer_size);
  RUN_TEST(test_can_execute_sequence_on_dispatch_requires_end_with_active_sequence);
  RUN_TEST(test_can_execute_sequence_on_dispatch_rejects_non_end_commands);
  RUN_TEST(test_should_queue_command_in_active_sequence_reflects_sequence_state);
  RUN_TEST(test_ready_response_message_matches_ui_contract);
  RUN_TEST(test_invalid_command_response_message_matches_ui_contract);
  RUN_TEST(test_motion_queue_enqueues_and_dequeues_positions_in_order);
  RUN_TEST(test_motion_queue_rejects_enqueue_when_full);
  RUN_TEST(test_motion_queue_returns_false_when_empty);

  return UNITY_END();
}
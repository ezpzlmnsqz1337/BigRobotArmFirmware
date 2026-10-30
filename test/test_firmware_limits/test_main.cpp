#include <unity.h>

#include "CommandDispatchRules.h"
#include "CommandInputRules.h"
#include "CommandResponseRules.h"
#include "Config.h"
#include "FirmwareLimits.h"
#include "JointCommandRules.h"
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

void test_parse_sequence_repetitions_rejects_unexpected_prefix()
{
  char repetitionsToken[] = "X5";
  TEST_ASSERT_EQUAL_INT8(0, parseSequenceRepetitions(repetitionsToken));
}

void test_parse_sequence_repetitions_rejects_out_of_range_and_malformed_values()
{
  char zeroToken[] = "I0";
  char negativeToken[] = "I-5";
  char tooManyToken[] = "I21";
  char wrappedToken[] = "I257";
  char textToken[] = "Iabc";

  TEST_ASSERT_EQUAL_INT8(0, parseSequenceRepetitions(zeroToken));
  TEST_ASSERT_EQUAL_INT8(0, parseSequenceRepetitions(negativeToken));
  TEST_ASSERT_EQUAL_INT8(0, parseSequenceRepetitions(tooManyToken));
  TEST_ASSERT_EQUAL_INT8(0, parseSequenceRepetitions(wrappedToken));
  TEST_ASSERT_EQUAL_INT8(0, parseSequenceRepetitions(textToken));
}

void test_parse_sequence_repetitions_accepts_inclusive_bounds()
{
  char oneToken[] = "I1";
  char maxToken[] = "I20";

  TEST_ASSERT_EQUAL_INT8(1, parseSequenceRepetitions(oneToken));
  TEST_ASSERT_EQUAL_INT8(MAX_SEQUENCE_COMMANDS, parseSequenceRepetitions(maxToken));
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
  char q0Command[] = "Q0";
  char g1Command[] = "G1";
  char g28Command[] = "G28";
  char g92Command[] = "G92";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G0, classifyCommandToken(g0Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_Q0, classifyCommandToken(q0Command));
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
  char flushCommand[] = "QFLUSH";
  char clearCommand[] = "QCLEAR";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_BEGIN, classifyCommandToken(beginCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_END, classifyCommandToken(endCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_SYNC, classifyCommandToken(syncCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_QFLUSH, classifyCommandToken(flushCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_QCLEAR, classifyCommandToken(clearCommand));
}

void test_classify_command_token_only_accepts_exact_sync_tokens()
{
  char s0Command[] = "S0";
  char s1Command[] = "S1";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_SYNC, classifyCommandToken(s0Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_SYNC, classifyCommandToken(s1Command));
}

void test_classify_command_token_rejects_non_sync_s_prefixes()
{
  char s2Command[] = "S2";
  char s10Command[] = "S10";
  char statusCommand[] = "STATUS";
  char speedCommand[] = "SPEED";
  char bareSCommand[] = "S";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(s2Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(s10Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(statusCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(speedCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(bareSCommand));
}

void test_classify_command_token_rejects_malformed_numeric_suffixes()
{
  char bareGCommand[] = "G";
  char bareMCommand[] = "M";
  char garbageGCommand[] = "Gabc";
  char mixedGCommand[] = "G0x";
  char negativeGCommand[] = "G-1";
  char garbageMCommand[] = "M20x";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(bareGCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(bareMCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(garbageGCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(mixedGCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(negativeGCommand));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_INVALID, classifyCommandToken(garbageMCommand));
}

void test_classify_command_token_accepts_leading_zeroes()
{
  char g00Command[] = "G00";
  char g028Command[] = "G028";
  char m0203Command[] = "M0203";

  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G0, classifyCommandToken(g00Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_G28, classifyCommandToken(g028Command));
  TEST_ASSERT_EQUAL(COMMAND_DISPATCH_M203, classifyCommandToken(m0203Command));
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

void test_command_input_accepts_carriage_return_termination()
{
  CommandInputBuffer input;
  resetCommandInput(input);

  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, 'G'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '0'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_READY, pushCommandInputByte(input, '\r'));
  TEST_ASSERT_EQUAL_STRING("G0", input.data);
}

void test_command_input_accepts_line_feed_termination()
{
  CommandInputBuffer input;
  resetCommandInput(input);

  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, 'M'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '5'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '0'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '3'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_READY, pushCommandInputByte(input, '\n'));
  TEST_ASSERT_EQUAL_STRING("M503", input.data);
}

void test_command_input_accepts_crlf_without_empty_command()
{
  CommandInputBuffer input;
  resetCommandInput(input);

  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, 'G'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '0'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_READY, pushCommandInputByte(input, '\r'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '\n'));
  TEST_ASSERT_EQUAL_STRING("G0", input.data);
}

void test_command_input_rejects_overlength_line_without_executing_prefix()
{
  CommandInputBuffer input;
  resetCommandInput(input);

  for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
  {
    TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, 'A'));
  }

  // Additional bytes are consumed but never appended.
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, 'B'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_INVALID, pushCommandInputByte(input, '\r'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, 'G'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_NONE, pushCommandInputByte(input, '0'));
  TEST_ASSERT_EQUAL(COMMAND_INPUT_READY, pushCommandInputByte(input, '\r'));
  TEST_ASSERT_EQUAL_STRING("G0", input.data);
}

void test_parse_joint_positions_command_reads_prefixed_values()
{
  char command[] = "G0 B10 S-20 E30 WR40 W50";
  JointPositions positions{1, 2, 3, 4, 5, 6};

  TEST_ASSERT_TRUE(parseJointPositionsCommand(command, positions));
  TEST_ASSERT_EQUAL_INT32(10, positions.base);
  TEST_ASSERT_EQUAL_INT32(-20, positions.shoulder);
  TEST_ASSERT_EQUAL_INT32(30, positions.elbow);
  TEST_ASSERT_EQUAL_INT32(40, positions.wristRotate);
  TEST_ASSERT_EQUAL_INT32(50, positions.wrist);
}

void test_parse_joint_positions_command_keeps_absent_values()
{
  char command[] = "G0 B10";
  JointPositions positions{1, 2, 3, 4, 5, 6};

  TEST_ASSERT_TRUE(parseJointPositionsCommand(command, positions));
  TEST_ASSERT_EQUAL_INT32(10, positions.base);
  TEST_ASSERT_EQUAL_INT32(2, positions.shoulder);
  TEST_ASSERT_EQUAL_INT32(3, positions.elbow);
  TEST_ASSERT_EQUAL_INT32(4, positions.wristRotate);
  TEST_ASSERT_EQUAL_INT32(5, positions.wrist);
}

void test_parse_joint_positions_command_rejects_prefix_only_tokens()
{
  char command[] = "G0 B S10 E30 WR40 W50";
  JointPositions positions{1, 2, 3, 4, 5, 6};

  TEST_ASSERT_FALSE(parseJointPositionsCommand(command, positions));
}

void test_parse_joint_positions_command_rejects_misread_wrist_rotate_token()
{
  char command[] = "G0 B10 S20 E30 W40 W50";
  JointPositions positions{1, 2, 3, 4, 5, 6};

  TEST_ASSERT_FALSE(parseJointPositionsCommand(command, positions));
}

void test_parse_joint_speeds_and_accelerations_reject_prefix_only_tokens()
{
  char speedCommand[] = "M203 B100 S";
  char accelerationCommand[] = "M201 B100 WR";
  JointSpeeds speeds{1, 2, 3, 4, 5};
  JointAccelerations accelerations{1, 2, 3, 4, 5};

  TEST_ASSERT_FALSE(parseJointSpeedsCommand(speedCommand, speeds));
  TEST_ASSERT_FALSE(parseJointAccelerationsCommand(accelerationCommand, accelerations));
}

void test_parse_joint_speeds_and_accelerations_read_prefixed_values()
{
  char speedCommand[] = "M203 B100 S200 E300 WR400 W500";
  char accelerationCommand[] = "M201 B110 S210 E310 WR410 W510";
  JointSpeeds speeds{};
  JointAccelerations accelerations{};

  TEST_ASSERT_TRUE(parseJointSpeedsCommand(speedCommand, speeds));
  TEST_ASSERT_EQUAL_INT32(100, speeds.base);
  TEST_ASSERT_EQUAL_INT32(500, speeds.wrist);

  TEST_ASSERT_TRUE(parseJointAccelerationsCommand(accelerationCommand, accelerations));
  TEST_ASSERT_EQUAL_INT32(110, accelerations.base);
  TEST_ASSERT_EQUAL_INT32(510, accelerations.wrist);
}

void test_clamp_gripper_position_enforces_mechanical_limits()
{
  TEST_ASSERT_EQUAL_INT32(Config::MIN_GRIPPER_POSITION, clampGripperPosition(0));
  TEST_ASSERT_EQUAL_INT32(90, clampGripperPosition(90));
  TEST_ASSERT_EQUAL_INT32(Config::MAX_GRIPPER_POSITION, clampGripperPosition(180));
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

void test_can_start_sequence_rejects_nested_begin()
{
  TEST_ASSERT_TRUE(canStartSequence(false));
  TEST_ASSERT_FALSE(canStartSequence(true));
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

void test_motion_queue_clear_discards_pending_positions()
{
  MotionQueue queue;
  JointPositions target{1, 2, 3, 4, 5, 6};

  TEST_ASSERT_TRUE(queue.enqueue(target));
  TEST_ASSERT_FALSE(queue.isEmpty());

  queue.clear();

  TEST_ASSERT_TRUE(queue.isEmpty());
  TEST_ASSERT_EQUAL_UINT8(0, queue.size());
}

int main(int argc, char** argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_clamp_configured_value_keeps_in_range_value);
  RUN_TEST(test_clamp_configured_value_raises_low_value_to_minimum);
  RUN_TEST(test_clamp_configured_value_caps_high_value_at_maximum);
  RUN_TEST(test_parse_sequence_repetitions_defaults_to_one_without_token);
  RUN_TEST(test_parse_sequence_repetitions_reads_iteration_token);
  RUN_TEST(test_parse_sequence_repetitions_rejects_unexpected_prefix);
  RUN_TEST(test_parse_sequence_repetitions_rejects_out_of_range_and_malformed_values);
  RUN_TEST(test_parse_sequence_repetitions_accepts_inclusive_bounds);
  RUN_TEST(test_can_add_sequence_command_allows_values_below_capacity);
  RUN_TEST(test_can_add_sequence_command_blocks_values_at_capacity);
  RUN_TEST(test_classify_command_token_rejects_null_and_empty_values);
  RUN_TEST(test_classify_command_token_recognizes_supported_g_commands);
  RUN_TEST(test_classify_command_token_recognizes_supported_m_commands);
  RUN_TEST(test_classify_command_token_recognizes_sequence_and_sync_commands);
  RUN_TEST(test_classify_command_token_only_accepts_exact_sync_tokens);
  RUN_TEST(test_classify_command_token_rejects_non_sync_s_prefixes);
  RUN_TEST(test_classify_command_token_rejects_malformed_numeric_suffixes);
  RUN_TEST(test_classify_command_token_accepts_leading_zeroes);
  RUN_TEST(test_classify_command_token_rejects_unknown_commands);
  RUN_TEST(test_has_processable_command_token_rejects_null_and_empty_values);
  RUN_TEST(test_has_processable_command_token_accepts_non_empty_values);
  RUN_TEST(test_can_append_command_byte_allows_lengths_below_buffer_size);
  RUN_TEST(test_can_append_command_byte_blocks_length_at_buffer_size);
  RUN_TEST(test_command_input_accepts_carriage_return_termination);
  RUN_TEST(test_command_input_accepts_line_feed_termination);
  RUN_TEST(test_command_input_accepts_crlf_without_empty_command);
  RUN_TEST(test_command_input_rejects_overlength_line_without_executing_prefix);
  RUN_TEST(test_parse_joint_positions_command_reads_prefixed_values);
  RUN_TEST(test_parse_joint_positions_command_keeps_absent_values);
  RUN_TEST(test_parse_joint_positions_command_rejects_prefix_only_tokens);
  RUN_TEST(test_parse_joint_positions_command_rejects_misread_wrist_rotate_token);
  RUN_TEST(test_parse_joint_speeds_and_accelerations_reject_prefix_only_tokens);
  RUN_TEST(test_parse_joint_speeds_and_accelerations_read_prefixed_values);
  RUN_TEST(test_clamp_gripper_position_enforces_mechanical_limits);
  RUN_TEST(test_can_execute_sequence_on_dispatch_requires_end_with_active_sequence);
  RUN_TEST(test_can_execute_sequence_on_dispatch_rejects_non_end_commands);
  RUN_TEST(test_can_start_sequence_rejects_nested_begin);
  RUN_TEST(test_should_queue_command_in_active_sequence_reflects_sequence_state);
  RUN_TEST(test_ready_response_message_matches_ui_contract);
  RUN_TEST(test_invalid_command_response_message_matches_ui_contract);
  RUN_TEST(test_motion_queue_enqueues_and_dequeues_positions_in_order);
  RUN_TEST(test_motion_queue_rejects_enqueue_when_full);
  RUN_TEST(test_motion_queue_returns_false_when_empty);
  RUN_TEST(test_motion_queue_clear_discards_pending_positions);

  return UNITY_END();
}
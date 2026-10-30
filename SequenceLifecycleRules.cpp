#include "SequenceLifecycleRules.h"

bool canStartSequence(const bool hasActiveSequence)
{
  // Reject a nested BEGIN instead of leaking the already buffered sequence.
  return !hasActiveSequence;
}

bool canExecuteSequenceOnDispatch(const CommandDispatchKind commandKind, const bool hasActiveSequence)
{
  return commandKind == COMMAND_DISPATCH_END && hasActiveSequence;
}

bool shouldQueueCommandInActiveSequence(const bool hasActiveSequence)
{
  return hasActiveSequence;
}
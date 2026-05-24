#include "SequenceLifecycleRules.h"

bool canExecuteSequenceOnDispatch(const CommandDispatchKind commandKind, const bool hasActiveSequence)
{
  return commandKind == COMMAND_DISPATCH_END && hasActiveSequence;
}

bool shouldQueueCommandInActiveSequence(const bool hasActiveSequence)
{
  return hasActiveSequence;
}
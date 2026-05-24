#ifndef SEQUENCE_LIFECYCLE_RULES_H
#define SEQUENCE_LIFECYCLE_RULES_H

#include "CommandDispatchRules.h"

bool canExecuteSequenceOnDispatch(CommandDispatchKind commandKind, bool hasActiveSequence);
bool shouldQueueCommandInActiveSequence(bool hasActiveSequence);

#endif
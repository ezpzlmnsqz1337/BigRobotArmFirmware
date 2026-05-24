#include "SequenceCommandRules.h"

#include <stdlib.h>

int8_t parseSequenceRepetitions(const char* repetitionsToken)
{
  if (repetitionsToken == nullptr || repetitionsToken[0] == 0)
  {
    return 1;
  }

  if (repetitionsToken[0] != 'I')
  {
    return 1;
  }

  return static_cast<int8_t>(atoi(&repetitionsToken[1]));
}

bool canAddSequenceCommand(const int8_t numOfSequenceCommands)
{
  return numOfSequenceCommands < MAX_SEQUENCE_COMMANDS;
}
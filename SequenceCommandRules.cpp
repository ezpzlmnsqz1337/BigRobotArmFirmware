#include "SequenceCommandRules.h"

int8_t parseSequenceRepetitions(const char* repetitionsToken)
{
  if (repetitionsToken == nullptr || repetitionsToken[0] == 0)
  {
    return 1;
  }

  if (repetitionsToken[0] != 'I' || repetitionsToken[1] == 0)
  {
    return 0;
  }

  int repetitions = 0;
  for (const char* cursor = &repetitionsToken[1]; *cursor != 0; cursor++)
  {
    if (*cursor < '0' || *cursor > '9')
    {
      return 0;
    }

    repetitions = repetitions * 10 + (*cursor - '0');
    if (repetitions > MAX_SEQUENCE_COMMANDS)
    {
      // Stop early so the accumulator can never overflow or wrap.
      return 0;
    }
  }

  return repetitions > 0 ? static_cast<int8_t>(repetitions) : 0;
}

bool canAddSequenceCommand(const int8_t numOfSequenceCommands)
{
  return numOfSequenceCommands < MAX_SEQUENCE_COMMANDS;
}

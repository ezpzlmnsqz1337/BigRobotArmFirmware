#include "CommandDispatchRules.h"

#include <stdlib.h>
#include <string.h>

namespace
{
bool parseNumericSuffix(const char* command, long& value)
{
  const char* digits = command + 1;
  if (*digits == 0)
  {
    return false;
  }

  for (const char* cursor = digits; *cursor != 0; cursor++)
  {
    if (*cursor < '0' || *cursor > '9')
    {
      return false;
    }
  }

  char* end = nullptr;
  const long parsed = strtol(digits, &end, 10);
  if (end == digits || *end != 0)
  {
    return false;
  }

  value = parsed;
  return true;
}
} // namespace

CommandDispatchKind classifyCommandToken(const char* command)
{
  if (command == nullptr || command[0] == 0)
  {
    return COMMAND_DISPATCH_INVALID;
  }

  if (strcmp(command, "BEGIN") == 0)
  {
    return COMMAND_DISPATCH_BEGIN;
  }

  if (strcmp(command, "END") == 0)
  {
    return COMMAND_DISPATCH_END;
  }

  if (strcmp(command, "QFLUSH") == 0)
  {
    return COMMAND_DISPATCH_QFLUSH;
  }

  if (strcmp(command, "QCLEAR") == 0)
  {
    return COMMAND_DISPATCH_QCLEAR;
  }

  if (strcmp(command, "Q0") == 0)
  {
    return COMMAND_DISPATCH_Q0;
  }

  if (strcmp(command, "S0") == 0 || strcmp(command, "S1") == 0)
  {
    return COMMAND_DISPATCH_SYNC;
  }

  if (command[0] == 'G')
  {
    long value = 0;
    if (!parseNumericSuffix(command, value))
    {
      return COMMAND_DISPATCH_INVALID;
    }

    switch (value)
    {
    case 0:
      return COMMAND_DISPATCH_G0;
    case 1:
      return COMMAND_DISPATCH_G1;
    case 28:
      return COMMAND_DISPATCH_G28;
    case 92:
      return COMMAND_DISPATCH_G92;
    default:
      return COMMAND_DISPATCH_INVALID;
    }
  }

  if (command[0] == 'M')
  {
    long value = 0;
    if (!parseNumericSuffix(command, value))
    {
      return COMMAND_DISPATCH_INVALID;
    }

    switch (value)
    {
    case 201:
      return COMMAND_DISPATCH_M201;
    case 203:
      return COMMAND_DISPATCH_M203;
    case 503:
      return COMMAND_DISPATCH_M503;
    default:
      return COMMAND_DISPATCH_INVALID;
    }
  }

  return COMMAND_DISPATCH_INVALID;
}

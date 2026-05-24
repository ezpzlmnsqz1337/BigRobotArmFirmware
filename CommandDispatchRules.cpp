#include "CommandDispatchRules.h"

#include <stdlib.h>
#include <string.h>

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

  if (command[0] == 'S')
  {
    return COMMAND_DISPATCH_SYNC;
  }

  if (command[0] == 'G')
  {
    switch (atoi(&command[1]))
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
    switch (atoi(&command[1]))
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
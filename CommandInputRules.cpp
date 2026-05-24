#include "CommandInputRules.h"

#include <string.h>

bool hasProcessableCommandToken(const char* command)
{
  return command != nullptr && strlen(command) > 0;
}

bool canAppendCommandByte(const int currentLength, const int bufferSize)
{
  return currentLength < bufferSize;
}
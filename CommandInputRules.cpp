#include "CommandInputRules.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

bool hasProcessableCommandToken(const char* command)
{
  return command != nullptr && strlen(command) > 0;
}

bool canAppendCommandByte(const int currentLength, const int bufferSize)
{
  return currentLength < bufferSize;
}

bool parsePrefixedLong(const char* token, const char* prefix, long* value)
{
  if (token == nullptr || prefix == nullptr || value == nullptr)
  {
    return false;
  }

  const size_t prefixLength = strlen(prefix);
  if (strncmp(token, prefix, prefixLength) != 0)
  {
    return false;
  }

  const char* digits = token + prefixLength;
  if (*digits == 0)
  {
    return false;
  }

  const char* cursor = digits;
  if (*cursor == '-')
  {
    cursor++;
  }

  if (*cursor == 0)
  {
    return false;
  }

  for (; *cursor != 0; cursor++)
  {
    if (*cursor < '0' || *cursor > '9')
    {
      return false;
    }
  }

  errno = 0;
  char* end = nullptr;
  const long parsed = strtol(digits, &end, 10);
  if (end == digits || *end != 0 || errno == ERANGE)
  {
    return false;
  }

  *value = parsed;
  return true;
}

bool parsePrefixedInt32(const char* token, const char* prefix, int32_t* value)
{
  if (value == nullptr)
  {
    return false;
  }

  long parsed = 0;
  if (!parsePrefixedLong(token, prefix, &parsed))
  {
    return false;
  }

  if (parsed < INT32_MIN || parsed > INT32_MAX)
  {
    return false;
  }

  *value = static_cast<int32_t>(parsed);
  return true;
}

void resetCommandInput(CommandInputBuffer& buffer)
{
  buffer.data[0] = 0;
  buffer.length = 0;
  buffer.overflowed = false;
  buffer.suppressNextLineFeed = false;
}

namespace
{
CommandInputResult finishCommandInput(CommandInputBuffer& buffer, const bool carriageReturn)
{
  buffer.data[buffer.length] = 0;

  const CommandInputResult result = buffer.overflowed ? COMMAND_INPUT_INVALID : COMMAND_INPUT_READY;
  buffer.length = 0;
  buffer.overflowed = false;
  buffer.suppressNextLineFeed = carriageReturn;
  return result;
}
} // namespace

CommandInputResult pushCommandInputByte(CommandInputBuffer& buffer, const char byte)
{
  if (byte == '\r')
  {
    return finishCommandInput(buffer, true);
  }

  if (byte == '\n')
  {
    if (buffer.suppressNextLineFeed)
    {
      // Ignore the line feed of a CRLF pair; the carriage return already completed the command.
      buffer.suppressNextLineFeed = false;
      return COMMAND_INPUT_NONE;
    }

    return finishCommandInput(buffer, false);
  }

  buffer.suppressNextLineFeed = false;

  if (canAppendCommandByte(buffer.length, COMMAND_BUFFER_SIZE))
  {
    buffer.data[buffer.length++] = byte;
  }
  else
  {
    // Keep consuming the over-length line but reject it when the terminator arrives.
    buffer.overflowed = true;
  }

  return COMMAND_INPUT_NONE;
}

#ifndef COMMAND_INPUT_RULES_H
#define COMMAND_INPUT_RULES_H

#include <stdint.h>

const int COMMAND_BUFFER_SIZE = 50;
const int COMMAND_BUFFER_CAPACITY = COMMAND_BUFFER_SIZE + 1;

enum CommandInputResult
{
  COMMAND_INPUT_NONE,
  COMMAND_INPUT_READY,
  COMMAND_INPUT_INVALID,
};

struct CommandInputBuffer
{
  char data[COMMAND_BUFFER_CAPACITY];
  int length;
  bool overflowed;
  bool suppressNextLineFeed;
};

bool hasProcessableCommandToken(const char* command);
bool canAppendCommandByte(int currentLength, int bufferSize);
bool parsePrefixedLong(const char* token, const char* prefix, long* value);
bool parsePrefixedInt32(const char* token, const char* prefix, int32_t* value);

void resetCommandInput(CommandInputBuffer& buffer);
CommandInputResult pushCommandInputByte(CommandInputBuffer& buffer, char byte);

#endif

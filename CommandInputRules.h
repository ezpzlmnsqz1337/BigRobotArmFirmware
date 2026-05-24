#ifndef COMMAND_INPUT_RULES_H
#define COMMAND_INPUT_RULES_H

bool hasProcessableCommandToken(const char* command);
bool canAppendCommandByte(int currentLength, int bufferSize);

#endif
#ifndef SEQUENCE_COMMAND_RULES_H
#define SEQUENCE_COMMAND_RULES_H

#include <stdint.h>

const int8_t MAX_SEQUENCE_COMMANDS = 20;

int8_t parseSequenceRepetitions(const char* repetitionsToken);
bool canAddSequenceCommand(int8_t numOfSequenceCommands);

#endif
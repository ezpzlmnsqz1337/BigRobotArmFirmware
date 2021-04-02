// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

const int8_t BUFFER_SIZE = 50;
const int8_t MAX_SEQUENCE_COMMANDS = 20;

class CommandHandler
{
private:
  ArmBuilder mArmBuilder;

  // serial handling
  char buffer[BUFFER_SIZE];
  char original[BUFFER_SIZE];
  int8_t sofar;

  // sequence handling
  AbstractCommand* mSequence[MAX_SEQUENCE_COMMANDS];
  int8_t numOfSequenceCommands = 0;
  int8_t numOfSequenceRepetitions = 1;
  bool isSequence = false;
  bool enableResponse = true;

  void processSequence();
  void addCommandToSequence(AbstractCommand* pCommand);
  void executeSequence();

public:
  CommandHandler();

  void init(const ArmBuilder& mArmBuilder);

  void handle();

  void processCommand(char* command);
  void printReadyResponse();
  void printInvalidCommandResponse();

  void reset();
};

// This is the end of the header guard
#endif

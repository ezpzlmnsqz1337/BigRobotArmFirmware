// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "CompositeCommand.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

const int8_t BUFFER_SIZE = 50;
const int8_t COMMAND_BUFFER_CAPACITY = BUFFER_SIZE + 1;

class CommandHandler
{
private:
  ArmBuilder mArmBuilder;

  // serial handling
  char buffer[COMMAND_BUFFER_CAPACITY];
  char original[COMMAND_BUFFER_CAPACITY];
  int8_t sofar;

  // sequence handling
  CompositeCommand* mSequence;

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

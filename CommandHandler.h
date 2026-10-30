// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "CommandInputRules.h"
#include "CompositeCommand.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

class CommandHandler
{
private:
  ArmBuilder* mArmBuilder;

  // serial handling
  CommandInputBuffer mInput;
  char original[COMMAND_BUFFER_CAPACITY];

  // sequence handling
  CompositeCommand* mSequence;

public:
  CommandHandler();

  void init(ArmBuilder& mArmBuilder);

  void handle();

  void processCommand(char* command);
  void printReadyResponse();
  void printInvalidCommandResponse();

  void reset();
};

// This is the end of the header guard
#endif

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

const int8_t BUFFER_SIZE = 50;

class CommandHandler
{
private:
  ArmBuilder mArmBuilder;

  // serial handling
  char buffer[BUFFER_SIZE];
  int8_t sofar;

protected:
  void handlePositionCommand(uint8_t* command);
  void handleGripperCommand(uint8_t* command);

public:
  CommandHandler();

  void init(const ArmBuilder& mArmBuilder);

  void handle();

  void printStatus();
  void processCommand(uint8_t* command);
  void printReadyResponse();
  void printInvalidCommandResponse();

  void reset();
};

// This is the end of the header guard
#endif

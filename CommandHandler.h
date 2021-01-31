// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "ArmBuilder.h"

// This is the content of the .h file, which is where the declarations go

const int BUFFER_SIZE = 30;

class CommandHandler
{
private:
  char buffer[BUFFER_SIZE];
  int sofar;
  ArmBuilder mArmBuilder;

public:
  CommandHandler();

  void init(const ArmBuilder& mArmBuilder);

  void handle();

  void processCommand();

  void reset();
};

// This is the end of the header guard
#endif

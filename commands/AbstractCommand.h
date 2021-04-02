// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef ABSTRACT_COMMAND_H
#define ABSTRACT_COMMAND_H

#include "ArmBuilder.h"

// This is the content of the .h file, which is where the declarations go

class AbstractCommand
{
protected:
  ArmBuilder* mArmBuilder;
  bool isValid = true;

public:
  explicit AbstractCommand(ArmBuilder* armBuilder);
  virtual ~AbstractCommand();

  virtual void parse(char* cCommand) = 0;
  virtual void execute() = 0;
  virtual void printResponse() = 0;
};

// This is the end of the header guard
#endif

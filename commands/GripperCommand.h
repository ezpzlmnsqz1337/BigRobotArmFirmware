// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef GRIPPER_COMMAND_H
#define GRIPPER_COMMAND_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"

// This is the content of the .h file, which is where the declarations go

class GripperCommand : public AbstractCommand
{
  int8_t mEnabled;
  int32_t mPosition;

public:
  explicit GripperCommand(ArmBuilder* armBuilder);
  virtual ~GripperCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif

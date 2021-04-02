// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef ACCELERATION_COMMAND_H
#define ACCELERATION_COMMAND_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

class AccelerationCommand : public AbstractCommand
{
  JointAccelerations mJA;

public:
  explicit AccelerationCommand(const ArmBuilder& armBuilder);
  virtual ~AccelerationCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef SYNC_MOTORS_COMMAND_H
#define SYNC_MOTORS_COMMAND_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

class SyncMotorsCommand : public AbstractCommand
{
  int32_t mSyncMotors;

public:
  explicit SyncMotorsCommand(ArmBuilder* armBuilder);
  virtual ~SyncMotorsCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef STATUS_COMMAND_H
#define STATUS_COMMAND_H

#include "AbstractCommand.h"
#include "AccelerationCommand.h"
#include "ArmBuilder.h"
#include "GripperCommand.h"
#include "HomeCommand.h"
#include "PositionCommand.h"
#include "ResetPositionCommand.h"
#include "SpeedCommand.h"
#include "Structures.h"
#include "SyncMotorsCommand.h"

// This is the content of the .h file, which is where the declarations go

class StatusCommand : public AbstractCommand
{
  PositionCommand* mPositionCommand;
  SpeedCommand* mSpeedCommand;
  AccelerationCommand* mAccelerationCommand;
  SyncMotorsCommand* mSyncMotorsCommand;

public:
  explicit StatusCommand(const ArmBuilder& armBuilder);
  StatusCommand(const StatusCommand& sc);
  ~StatusCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif

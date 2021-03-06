#include "StatusCommand.h"
#include "Arduino.h"

StatusCommand::StatusCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
  mPositionCommand = new PositionCommand(armBuilder);
  mAccelerationCommand = new AccelerationCommand(armBuilder);
  mSpeedCommand = new SpeedCommand(armBuilder);
  mSyncMotorsCommand = new SyncMotorsCommand(armBuilder);
}

StatusCommand::StatusCommand(const StatusCommand& sc) : AbstractCommand(sc.mArmBuilder)
{
  mPositionCommand = new PositionCommand(sc.mArmBuilder);
  mAccelerationCommand = new AccelerationCommand(sc.mArmBuilder);
  mSpeedCommand = new SpeedCommand(sc.mArmBuilder);
  mSyncMotorsCommand = new SyncMotorsCommand(sc.mArmBuilder);
}

StatusCommand::~StatusCommand()
{
  delete mPositionCommand;
  delete mAccelerationCommand;
  delete mSpeedCommand;
  delete mSyncMotorsCommand;
}

void StatusCommand::execute()
{
}

void StatusCommand::printResponse()
{
  mPositionCommand->printResponse();
  mAccelerationCommand->printResponse();
  mSpeedCommand->printResponse();
  mSyncMotorsCommand->printResponse();
}

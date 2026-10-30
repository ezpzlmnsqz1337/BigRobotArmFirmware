#include "StatusCommand.h"
#include "Arduino.h"

StatusCommand::StatusCommand(ArmBuilder* armBuilder)
  : AbstractCommand(armBuilder), mPositionCommand(armBuilder), mSpeedCommand(armBuilder),
    mAccelerationCommand(armBuilder), mSyncMotorsCommand(armBuilder)
{
}

void StatusCommand::execute()
{
}

void StatusCommand::printResponse()
{
  mPositionCommand.printResponse();
  mAccelerationCommand.printResponse();
  mSpeedCommand.printResponse();
  mSyncMotorsCommand.printResponse();
}

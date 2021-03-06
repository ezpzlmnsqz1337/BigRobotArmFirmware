#include "SyncMotorsCommand.h"
#include "Arduino.h"

SyncMotorsCommand::SyncMotorsCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder), mSyncMotors(0)
{
}

SyncMotorsCommand::~SyncMotorsCommand()
{
}

void SyncMotorsCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  mSyncMotors = atoi(&cCommand[1]);
}

void SyncMotorsCommand::execute()
{
  bool sync = mSyncMotors == 1 ? true : false;
  mArmBuilder->setSyncMotors(sync);
}

void SyncMotorsCommand::printResponse()
{
  Serial.print("BigRobotArm::SYNC-MOTORS: ");
  Serial.println(mArmBuilder->isSyncEnabled() ? 1 : 0);
}

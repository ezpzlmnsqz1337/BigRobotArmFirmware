#include "SyncMotorsCommand.h"
#include "Arduino.h"
#include <string.h>

SyncMotorsCommand::SyncMotorsCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder), mSyncMotors(0)
{
}

SyncMotorsCommand::~SyncMotorsCommand()
{
}

void SyncMotorsCommand::parse(char* cCommand)
{
  char* token = strtok(cCommand, " ");
  if (token == nullptr || (strcmp(token, "S0") != 0 && strcmp(token, "S1") != 0))
  {
    isValid = false;
    return;
  }

  mSyncMotors = token[1] - '0';
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

#include "AbstractCommand.h"
#include "Arduino.h"

AbstractCommand::AbstractCommand(const ArmBuilder& armBuilder) : mArmBuilder(armBuilder)
{
}

AbstractCommand::~AbstractCommand()
{
  Serial.println("Deleting base");
}

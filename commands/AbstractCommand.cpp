#include "AbstractCommand.h"
#include "Arduino.h"

AbstractCommand::AbstractCommand(ArmBuilder* armBuilder) : mArmBuilder(armBuilder)
{
}

AbstractCommand::~AbstractCommand()
{
}

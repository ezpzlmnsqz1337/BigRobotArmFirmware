#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"
#include "CommandHandler.h"

ArmBuilder armBuilder;
CommandHandler commandHandler;

void setup()
{
  armBuilder.init();
  commandHandler.init(armBuilder);
}

void loop()
{
  commandHandler.handle();
}

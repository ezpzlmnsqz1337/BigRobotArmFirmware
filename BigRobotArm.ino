#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"
#include "CommandHandler.h"
#include "Controls.h"

ArmBuilder armBuilder;
CommandHandler commandHandler;
Controls controls;

void setup()
{
  controls.init(armBuilder, 100);
  armBuilder.init();
  commandHandler.init(armBuilder);
}

void loop()
{
  commandHandler.handle();
}

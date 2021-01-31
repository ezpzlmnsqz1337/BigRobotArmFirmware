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
  // controls.loop();

  // if (controls.isPlaying())
  // {
  //   armBuilder.repeatPositions();
  // }
  // else
  // {
  //   // armBuilder.goTo(pos1);
  //   // delay(300);
  //   // armBuilder.goTo(pos4);
  //   // delay(300);
  //   // armBuilder.goTo(pos3);
  //   // delay(300);
  //   // armBuilder.loop(controls.getLeftX(), controls.getLeftY(), controls.getRightX(), controls.getRightY(),
  //   //                controls.isSwitched());
  // }
}

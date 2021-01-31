#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"
#include "Controls.h"

ArmBuilder armBuilder;
Controls controls;

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");

  controls.init(armBuilder, 100);
  armBuilder.init();

  Serial.println("SETUP END");
}

void loop()
{
  controls.loop();

  if (controls.isPlaying())
  {
    armBuilder.repeatPositions();
  }
  else
  {
    armBuilder.loop(controls.getLeftX(), controls.getLeftY(), controls.getRightX(), controls.getRightY(),
                    controls.isSwitched());
  }
}

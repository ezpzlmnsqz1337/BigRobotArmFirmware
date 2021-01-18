#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"
#include "Controls.h"

ArmBuilder armBuilder;
Controls controls;

int currentPosition = 0;

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
    // case: repeating positions
    if (currentPosition == numOfSavedPositions + 1)
    {
      currentPosition = 0;
    }
    armBuilder.goTo(savedPositions[currentPosition][0], savedPositions[currentPosition][1],
                    savedPositions[currentPosition][2], savedPositions[currentPosition][3],
                    savedPositions[currentPosition][4]);
    currentPosition++;
  }
  else
  {
    // case: controlled by joystics

    if (controls.isSwitched())
    {
      armBuilder.move(leftX, leftY, rightX, 0, rightY);
    }
    else
    {
      armBuilder.move(leftX, leftY, rightX, rightY, 0);
    }
  }
}

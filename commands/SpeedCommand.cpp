#include "SpeedCommand.h"
#include "Arduino.h"

SpeedCommand::SpeedCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

SpeedCommand::~SpeedCommand()
{
}

void SpeedCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // if acceleration command
  char* base = strtok(NULL, " ");
  char* shoulder = strtok(NULL, " ");
  char* elbow = strtok(NULL, " ");
  char* wristRotate = strtok(NULL, " ");
  char* wrist = strtok(NULL, " ");

  mJS = mArmBuilder->getSpeeds();
  mJS.base = base != NULL ? atoi(&base[1]) : mJS.base;
  mJS.shoulder = shoulder != NULL ? atoi(&shoulder[1]) : mJS.shoulder;
  mJS.elbow = elbow != NULL ? atoi(&elbow[1]) : mJS.elbow;
  mJS.wristRotate = wristRotate != NULL ? atoi(&wristRotate[2]) : mJS.wristRotate;
  mJS.wrist = wrist != NULL ? atoi(&wrist[1]) : mJS.wrist;
}

void SpeedCommand::execute()
{
  mArmBuilder->setSpeeds(mJS);
}

void SpeedCommand::printResponse()
{
  JointSpeeds ja = mArmBuilder->getSpeeds();
  Serial.print("BigRobotArm::SPEED: ");
  Serial.print("B");
  Serial.print(ja.base);
  Serial.print(" S");
  Serial.print(ja.shoulder);
  Serial.print(" E");
  Serial.print(ja.elbow);
  Serial.print(" WR");
  Serial.print(ja.wristRotate);
  Serial.print(" W");
  Serial.println(ja.wrist);
}

#include "CommandHandler.h"
#include "Arduino.h"

CommandHandler::CommandHandler() : buffer("")
{
  sofar = 0;
}

void CommandHandler::init(const ArmBuilder& armBuilder)
{
  Serial.begin(9600);
  mArmBuilder = armBuilder;
  Serial.println("BigRobotArm::READY");
  reset();
}

void CommandHandler::handle()
{
  if (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == 13)
    {
      // if enter, command is complete
      buffer[sofar] = 0;
      processCommand();
      reset();
      return;
    }
    if (sofar < BUFFER_SIZE)
    {
      // if space in buffer, add char to buffer
      buffer[sofar++] = c;
    }
  }
}

void CommandHandler::processCommand()
{
  char* command = strtok(buffer, " ");

  if (command[0] == 'G')
  {
    int gNumber = atoi(&command[1]);
    if (gNumber == 28)
    {
      // G28 - home
      JointPositions jp{0, 0, 0, 0, 0, 40};
      printResponse(jp, true);
      mArmBuilder.goTo(jp);
      return;
    }

    if (gNumber == 0 || gNumber == 1)
    {
      // handle positions
      char* base = strtok(NULL, " ");
      char* shoulder = strtok(NULL, " ");
      char* elbow = strtok(NULL, " ");
      char* wristRotate = strtok(NULL, " ");
      char* wrist = strtok(NULL, " ");
      char* gripper = strtok(NULL, " ");

      JointPositions jp;
      jp.base = base != NULL ? atol(&base[1]) : 0;
      jp.shoulder = shoulder != NULL ? atol(&shoulder[1]) : 0;
      jp.elbow = elbow != NULL ? atol(&elbow[1]) : 0;
      jp.wristRotate = wristRotate != NULL ? atol(&wristRotate[1]) : 0;
      jp.wrist = wrist != NULL ? atol(&wrist[1]) : 0;
      jp.gripper = gripper != NULL ? atol(&gripper[1]) : 0;

      printResponse(jp, true);

      mArmBuilder.goTo(jp);
      return;
    }

    // if unknown command
    printResponse(mArmBuilder.getPositions(), false);
  }
}

void CommandHandler::printResponse(const JointPositions& jp, bool valid)
{
  if (valid)
  {
    Serial.println("BigRobotArm::MOVING-TO");
  }
  else
  {
    Serial.println("BigRobotArm::INVALID-COMMAND");
  }
  Serial.print("B: ");
  Serial.print(jp.base);
  Serial.print(" S: ");
  Serial.print(jp.shoulder);
  Serial.print(" E: ");
  Serial.print(jp.elbow);
  Serial.print(" WR: ");
  Serial.print(jp.wristRotate);
  Serial.print(" W: ");
  Serial.print(jp.wrist);
  Serial.print(" G: ");
  Serial.println(jp.gripper);
  Serial.println("BigRobotArm::READY");
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
}
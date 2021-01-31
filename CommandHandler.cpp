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

  // X - BASE
  // Y - SHOULDER
  // Z - ELBOW
  // E0 - WRIST ROTATE
  // E1 - WRIST

  if (command[0] == 'G')
  {
    int gNumber = atoi(&command[1]);
    int gripper = 40;
    if (gNumber == 28)
    {
      // G28 - home
      JointPositions jp{0, 0, 0, 0, 0, 40};
      printResponse(jp);
      mArmBuilder.goTo(jp);
      return;
    }
    // handle G command
    if (gNumber == 0)
    {
      // handle G0 command
      gripper = 40;
    }
    if (gNumber == 1)
    {
      // handle G1 command
      gripper = 150;
    }

    // handle positions
    char* base = strtok(NULL, " ");
    char* shoulder = strtok(NULL, " ");
    char* elbow = strtok(NULL, " ");
    char* wristRotate = strtok(NULL, " ");
    char* wrist = strtok(NULL, " ");

    JointPositions jp;
    jp.base = base != NULL ? atol(&base[1]) : 0;
    jp.shoulder = shoulder != NULL ? atol(&shoulder[1]) : 0;
    jp.elbow = elbow != NULL ? atol(&elbow[1]) : 0;
    jp.wristRotate = wristRotate != NULL ? atol(&wristRotate[1]) : 0;
    jp.wrist = wrist != NULL ? atol(&wrist[1]) : 0;
    jp.gripper = gripper;

    printResponse(jp);

    mArmBuilder.goTo(jp);
  }
}

void CommandHandler::printResponse(const JointPositions& jp)
{
  Serial.println("BigRobotArm::MOVING-TO");
  Serial.print("base: ");
  Serial.print(jp.base);
  Serial.print(", shoulder: ");
  Serial.print(jp.shoulder);
  Serial.print(", elbow: ");
  Serial.print(jp.elbow);
  Serial.print(", wristRotate: ");
  Serial.print(jp.wristRotate);
  Serial.print(", wrist: ");
  Serial.print(jp.wrist);
  Serial.print(", gripper: ");
  Serial.println(jp.gripper);
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
  Serial.println("BigRobotArm::READY");
}
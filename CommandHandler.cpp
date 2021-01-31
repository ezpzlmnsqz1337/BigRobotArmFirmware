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
    Serial.print(c);
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
    int gripper = false;
    if (gNumber == 28)
    {
      // G28 - home
      mArmBuilder.goTo({0, 0, 0, 0, 0, 40});
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
    jp.base = atol(&base[1]);
    jp.shoulder = atol(&shoulder[1]);
    jp.elbow = atol(&elbow[1]);
    jp.wristRotate = atol(&wristRotate[1]);
    jp.wrist = atol(&wrist[1]);
    jp.gripper = gripper;

    Serial.print("Moving to - base:");
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

    return mArmBuilder.goTo(jp);
  }
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
  Serial.print("bigRobotArm@test: ");
}
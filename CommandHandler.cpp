#include "CommandHandler.h"
#include "Arduino.h"

CommandHandler::CommandHandler() : buffer("")
{
  sofar = 0;
}

void CommandHandler::init(const ArmBuilder& armBuilder)
{
  Serial.begin(250000);
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
    else if (gNumber == 92)
    {
      // G92 - set zero position
      JointPositions jp{0, 0, 0, 0, 0, 40};
      printResponse(jp, true);
      mArmBuilder.setZeroPosition();
      return;
    }
    else if (gNumber == 0)
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
      jp.wristRotate = wristRotate != NULL ? atol(&wristRotate[2]) : 0;
      jp.wrist = wrist != NULL ? atol(&wrist[1]) : 0;
      jp.gripper = gripper != NULL ? atol(&gripper[1]) : 0;

      printResponse(jp, true);

      mArmBuilder.goTo(jp);
      return;
    }
    else if (gNumber == 1)
    {
      // G1 - handle gripper
      char* e = strtok(NULL, " ");
      char* p = strtok(NULL, " ");
      int enable = e != NULL ? atol(&e[1]) : 0;
      int position = p != NULL ? atol(&p[1]) : 0;
      if (enable == 0 || enable == 1)
      {
        printResponse(enable, position, true);
        if (enable == 1)
        {
          mArmBuilder.enableGripper(true);
        }
        else
        {
          mArmBuilder.enableGripper(false);
        }
      }
      else
      {
        printResponse(-1, -1, false);
      }
      return;
    }
  }
  else if (command[0] == 'S')
  {
    // if speed command
    //
    int speed = atoi(&command[1]);
    if (speed > 0 && speed < 300)
    {
      mArmBuilder.setSpeed((float)speed / 100);
      printResponse((float)speed, (float)-1, true);
    }
    else
    {
      printResponse((float)-1, (float)-1, false);
    }
    return;
  }
  else if (command[0] == 'A')
  {
    // if acceleration command

    int acceleration = atoi(&command[1]);
    if (acceleration > 0 && acceleration <= 300)
    {
      mArmBuilder.setAcceleration((float)acceleration / 100);
      printResponse((float)-1, (float)acceleration, true);
    }
    else
    {
      printResponse((float)-1, (float)-1, false);
    }
    return;
  }

  // if unknown command
  printResponse(mArmBuilder.getPositions(), false);
}

void CommandHandler::printResponse(const int enable, const int position, const bool valid)
{
  if (valid)
  {
    if (enable == 0)
    {
      Serial.println("BigRobotArm::GRIPPER-DISABLED");
    }
    else
    {
      Serial.println("BigRobotArm::GRIPPER-ENABLED");
    }
    if (position >= 0)
    {
      Serial.println("BigRobotArm::GRIPPER-POSITION-SET");
    }
  }
  else
  {
    Serial.println("BigRobotArm::INVALID-GRIPPER-COMMAND");
  }

  Serial.println("BigRobotArm::READY");
}

void CommandHandler::printResponse(const float speed, const float acceleration, const bool valid)
{
  if (valid)
  {
    if (speed >= 0)
    {
      Serial.println("BigRobotArm::SPEED-SET");
      Serial.print("SPEED: ");
      Serial.println(speed);
    }
    if (acceleration >= 0)
    {
      Serial.println("BigRobotArm::ACCELERATION-SET");
      Serial.print("ACCELERATION: ");
      Serial.println(acceleration);
    }
  }
  else
  {
    Serial.println("BigRobotArm::INVALID-SPEED-ACCEL");
  }

  Serial.println("BigRobotArm::READY");
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
  Serial.print("BigRobotArm::POSITION ");
  Serial.print("B");
  Serial.print(jp.base);
  Serial.print(" S");
  Serial.print(jp.shoulder);
  Serial.print(" E");
  Serial.print(jp.elbow);
  Serial.print(" WR");
  Serial.print(jp.wristRotate);
  Serial.print(" W");
  Serial.print(jp.wrist);
  Serial.print(" G");
  Serial.println(jp.gripper);
  Serial.println("BigRobotArm::READY");
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
}
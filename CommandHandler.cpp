#include "CommandHandler.h"
#include "AbstractCommand.h"
#include "AccelerationCommand.h"
#include "Arduino.h"
#include "Config.h"
#include "GripperCommand.h"
#include "HomeCommand.h"
#include "MemoryFree.h"
#include "PositionCommand.h"
#include "ResetPositionCommand.h"
#include "SpeedCommand.h"
#include "StatusCommand.h"
#include "SyncMotorsCommand.h"

CommandHandler::CommandHandler() : buffer(""), original(""), mSequence(nullptr)
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
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == 13)
    {
      // if enter, command is complete
      buffer[sofar] = 0;
      strcpy(original, buffer);
      char* command = strtok(buffer, " ");
      processCommand(command);
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

void CommandHandler::processCommand(char* command)
{
  AbstractCommand* pCommand = nullptr;
  bool valid = false;
  Serial.print("Free mem: ");
  Serial.println(freeMemory());
  Serial.print("Process command: ");
  Serial.println(original);

  if (command[0] == 'G')
  {
    int32_t gNumber = atoi(&command[1]);
    switch (gNumber)
    {
    case 0:
      pCommand = new PositionCommand(&mArmBuilder);
      break;
    case 1:
      pCommand = new GripperCommand(&mArmBuilder);
      break;
    case 28:
      pCommand = new HomeCommand(&mArmBuilder);
      break;
    case 92:
      pCommand = new ResetPositionCommand(&mArmBuilder);
      break;
    default:
      valid = false;
    }
  }
  else if (command[0] == 'M')
  {
    int32_t mNumber = atoi(&command[1]);
    if (mNumber == 201)
    {
      pCommand = new AccelerationCommand(&mArmBuilder);
    }
    else if (mNumber == 203)
    {
      pCommand = new SpeedCommand(&mArmBuilder);
    }
    else if (mNumber == 503)
    {
      pCommand = new StatusCommand(&mArmBuilder);
    }
  }
  else if (command[0] == 'S')
  {
    pCommand = new SyncMotorsCommand(&mArmBuilder);
  }
  else if (strcmp(command, "BEGIN") == 0) // start of sequence of commands
  {
    mSequence = new CompositeCommand(&mArmBuilder);
    mSequence->parse(original);
    valid = true;
  }
  else if (strcmp(command, "END") == 0) // end of sequence of commands
  {
    mSequence->execute();
    mSequence->printResponse();
    delete mSequence;
    mSequence = nullptr;
    valid = true;
  }

  // if we have some command
  if (pCommand != nullptr)
  {
    // init command by parsing the string data
    pCommand->parse(original);
    // if we have sequence
    if (mSequence != nullptr)
    {
      // if we are now in a sequence, add command to the queue
      mSequence->addCommandToSequence(pCommand);
    }
    else
    {
      // if single command, execute it
      pCommand->execute();
      pCommand->printResponse();
      delete pCommand;
    }
    valid = true;
  }
  if (!valid)
  {
    // if unknown command
    printInvalidCommandResponse();
  }
  printReadyResponse();
}

void CommandHandler::printReadyResponse()
{
  Serial.println("BigRobotArm::READY");
}

void CommandHandler::printInvalidCommandResponse()
{
  Serial.println("BigRobotArm::INVALID-COMMAND");
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }
}
#include "CommandHandler.h"
#include "CommandDispatchRules.h"
#include "CommandInputRules.h"
#include "CommandResponseRules.h"
#include "SequenceLifecycleRules.h"
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

CommandHandler::CommandHandler() : mArmBuilder(nullptr), buffer(""), original(""), mSequence(nullptr)
{
  sofar = 0;
}

void CommandHandler::init(ArmBuilder& armBuilder)
{
  Serial.begin(250000);
  mArmBuilder = &armBuilder;
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
      if (hasProcessableCommandToken(command))
      {
        processCommand(command);
      }
      else
      {
        printInvalidCommandResponse();
        printReadyResponse();
      }
      reset();
      return;
    }
    if (canAppendCommandByte(sofar, BUFFER_SIZE))
    {
      // if space in buffer, add char to buffer
      buffer[sofar++] = c;
    }
  }
}

void CommandHandler::processCommand(char* command)
{
  const CommandDispatchKind commandKind = classifyCommandToken(command);
  if (commandKind == COMMAND_DISPATCH_INVALID)
  {
    printInvalidCommandResponse();
    printReadyResponse();
    return;
  }

  AbstractCommand* pCommand = nullptr;
  bool valid = false;
  Serial.print("Free mem: ");
  Serial.println(freeMemory());
  Serial.print("Process command: ");
  Serial.println(original);

  if (commandKind == COMMAND_DISPATCH_G0)
  {
    pCommand = new PositionCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_G1)
  {
    pCommand = new GripperCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_G28)
  {
    pCommand = new HomeCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_G92)
  {
    pCommand = new ResetPositionCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_M201)
  {
    pCommand = new AccelerationCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_M203)
  {
    pCommand = new SpeedCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_M503)
  {
    pCommand = new StatusCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_SYNC)
  {
    pCommand = new SyncMotorsCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_BEGIN) // start of sequence of commands
  {
    mSequence = new CompositeCommand(mArmBuilder);
    mSequence->parse(original);
    valid = true;
  }
  else if (commandKind == COMMAND_DISPATCH_END) // end of sequence of commands
  {
    if (canExecuteSequenceOnDispatch(commandKind, mSequence != nullptr))
    {
      mSequence->execute();
      mSequence->printResponse();
      delete mSequence;
      mSequence = nullptr;
      valid = true;
    }
  }

  // if we have some command
  if (pCommand != nullptr)
  {
    // init command by parsing the string data
    pCommand->parse(original);
    // if we have sequence
    if (shouldQueueCommandInActiveSequence(mSequence != nullptr))
    {
      // if we are now in a sequence, add command to the queue
      if (mSequence->addCommandToSequence(pCommand))
      {
        valid = true;
      }
      else
      {
        delete pCommand;
      }
    }
    else
    {
      // if single command, execute it
      pCommand->execute();
      pCommand->printResponse();
      delete pCommand;
      valid = true;
    }
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
  Serial.println(readyResponseMessage());
}

void CommandHandler::printInvalidCommandResponse()
{
  Serial.println(invalidCommandResponseMessage());
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
  for (int i = 0; i < COMMAND_BUFFER_CAPACITY; i++)
  {
    buffer[i] = 0;
    original[i] = 0;
  }
}
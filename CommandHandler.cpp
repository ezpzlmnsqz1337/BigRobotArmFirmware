#include "CommandHandler.h"
#include "CommandDispatchRules.h"
#include "CommandInputRules.h"
#include "CommandResponseRules.h"
#include "JointCommandRules.h"
#include "SequenceLifecycleRules.h"
#include "AbstractCommand.h"
#include "AccelerationCommand.h"
#include "Arduino.h"
#include "Config.h"
#include "GripperCommand.h"
#include "HomeCommand.h"
#include "PositionCommand.h"
#include "ResetPositionCommand.h"
#include "SpeedCommand.h"
#include "StatusCommand.h"
#include "SyncMotorsCommand.h"

#include <string.h>

namespace
{
bool parsePositionTarget(const char* rawCommand, ArmBuilder* armBuilder, JointPositions& jp)
{
  char commandCopy[COMMAND_BUFFER_CAPACITY];
  strncpy(commandCopy, rawCommand, COMMAND_BUFFER_CAPACITY - 1);
  commandCopy[COMMAND_BUFFER_CAPACITY - 1] = 0;

  jp = armBuilder->getPositions();
  return parseJointPositionsCommand(commandCopy, jp);
}

void printPositionResponse(const char* header, const JointPositions& jp)
{
  Serial.println(header);
  Serial.print("BigRobotArm::POSITION: ");
  Serial.print("B");
  Serial.print(jp.base);
  Serial.print(" S");
  Serial.print(jp.shoulder);
  Serial.print(" E");
  Serial.print(jp.elbow);
  Serial.print(" WR");
  Serial.print(jp.wristRotate);
  Serial.print(" W");
  Serial.println(jp.wrist);
}
} // namespace

CommandHandler::CommandHandler() : mArmBuilder(nullptr), mSequence(nullptr)
{
  reset();
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
    const CommandInputResult inputResult = pushCommandInputByte(mInput, static_cast<char>(Serial.read()));
    if (inputResult == COMMAND_INPUT_NONE)
    {
      continue;
    }

    if (inputResult == COMMAND_INPUT_INVALID)
    {
      // Over-length line: never execute the truncated prefix.
      printInvalidCommandResponse();
      printReadyResponse();
      continue;
    }

    strncpy(original, mInput.data, COMMAND_BUFFER_CAPACITY - 1);
    original[COMMAND_BUFFER_CAPACITY - 1] = 0;

    char* command = strtok(mInput.data, " ");
    if (hasProcessableCommandToken(command))
    {
      processCommand(command);
    }
    else
    {
      printInvalidCommandResponse();
      printReadyResponse();
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

  if (commandKind == COMMAND_DISPATCH_G0)
  {
    pCommand = new PositionCommand(mArmBuilder);
  }
  else if (commandKind == COMMAND_DISPATCH_Q0)
  {
    JointPositions jp;
    if (parsePositionTarget(original, mArmBuilder, jp))
    {
      if (mArmBuilder->queueMove(jp))
      {
        printPositionResponse("BigRobotArm::QUEUED-TO", jp);
      }
      else
      {
        Serial.println("BigRobotArm::QUEUE-FULL");
      }
      valid = true;
    }
  }
  else if (commandKind == COMMAND_DISPATCH_QFLUSH)
  {
    mArmBuilder->flushMotionQueue();
    Serial.println("BigRobotArm::QUEUE-DRAINED");
    valid = true;
  }
  else if (commandKind == COMMAND_DISPATCH_QCLEAR)
  {
    mArmBuilder->clearMotionQueue();
    Serial.println("BigRobotArm::QUEUE-CLEARED");
    valid = true;
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
    if (canStartSequence(mSequence != nullptr))
    {
      mSequence = new CompositeCommand(mArmBuilder);
      if (mSequence != nullptr)
      {
        mSequence->parse(original);
        if (mSequence->isValidCommand())
        {
          valid = true;
        }
        else
        {
          delete mSequence;
          mSequence = nullptr;
        }
      }
    }
  }
  else if (commandKind == COMMAND_DISPATCH_END) // end of sequence of commands
  {
    if (mSequence != nullptr && canExecuteSequenceOnDispatch(commandKind, true))
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
    if (!pCommand->isValidCommand())
    {
      delete pCommand;
      valid = false;
    }
    // if we have sequence
    else if (shouldQueueCommandInActiveSequence(mSequence != nullptr))
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
  resetCommandInput(mInput); // clear input buffer

  for (int i = 0; i < COMMAND_BUFFER_CAPACITY; i++)
  {
    original[i] = 0;
  }
}

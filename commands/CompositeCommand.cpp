#include "CompositeCommand.h"
#include "Arduino.h"
#include "StatusCommand.h"

CompositeCommand::CompositeCommand(ArmBuilder* pArmBuilder) : AbstractCommand(pArmBuilder)
{
  for (int i = 0; i < MAX_SEQUENCE_COMMANDS; i++)
  {
    mSequence[i] = nullptr;
  }
}

CompositeCommand::~CompositeCommand()
{
  // delete objects afterwards
  for (int8_t i = 0; i < MAX_SEQUENCE_COMMANDS; i++)
  {
    if (mSequence[i] != nullptr)
    {
      delete mSequence[i];
    }
  }
}

void CompositeCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // set number of repetitions
  char* repetitions = strtok(NULL, " ");
  const int8_t parsedRepetitions = parseSequenceRepetitions(repetitions);
  if (parsedRepetitions <= 0)
  {
    isValid = false;
    return;
  }

  numOfSequenceRepetitions = parsedRepetitions;
}

void CompositeCommand::execute()
{
  if (!isValid)
  {
    return;
  }

  // for number of repetitions
  for (int8_t i = 0; i < numOfSequenceRepetitions; i++)
  {
    // execute every command in the sequence
    for (int8_t j = 0; j < numOfSequenceCommands; j++)
    {
      if (mSequence[j] != nullptr)
      {
        mSequence[j]->execute();
      }
    }
  }
}

void CompositeCommand::printResponse()
{
  StatusCommand status(mArmBuilder);
  status.printResponse();
}

bool CompositeCommand::addCommandToSequence(AbstractCommand* pCommand)
{
  if (pCommand == nullptr || !canAddSequenceCommand(numOfSequenceCommands))
  {
    return false;
  }

  mSequence[numOfSequenceCommands] = pCommand;
  numOfSequenceCommands++;
  return true;
}

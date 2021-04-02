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
  numOfSequenceRepetitions = repetitions != NULL ? atoi(&repetitions[1]) : 1;
}

void CompositeCommand::execute()
{
  // for number of repetitions
  for (int8_t i = 0; i < numOfSequenceRepetitions; i++)
  {
    // execute every command in the sequence
    for (int8_t j = 0; j < numOfSequenceCommands; j++)
    {
      mSequence[j]->execute();
    }
  }
}

void CompositeCommand::printResponse()
{
  AbstractCommand* status = new StatusCommand(mArmBuilder);
  status->printResponse();
  delete status;
}

void CompositeCommand::addCommandToSequence(AbstractCommand* pCommand)
{
  mSequence[numOfSequenceCommands] = pCommand;
  numOfSequenceCommands++;
}

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMPOSITE_COMMAND_H
#define COMPOSITE_COMMAND_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

class CompositeCommand : public AbstractCommand
{
  static const int8_t MAX_SEQUENCE_COMMANDS = 20;
  AbstractCommand* mSequence[MAX_SEQUENCE_COMMANDS];
  int8_t numOfSequenceCommands = 0;
  int8_t numOfSequenceRepetitions = 1;

public:
  explicit CompositeCommand(ArmBuilder* pArmBuilder);
  virtual ~CompositeCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
  void addCommandToSequence(AbstractCommand* pCommand);
};

// This is the end of the header guard
#endif

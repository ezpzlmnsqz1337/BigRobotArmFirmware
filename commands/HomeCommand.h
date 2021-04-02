// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef HOME_COMMAND_H
#define HOME_COMMAND_H

#include "AbstractCommand.h"
#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

class HomeCommand : public AbstractCommand
{

public:
  explicit HomeCommand(const ArmBuilder& armBuilder);
  virtual ~HomeCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif

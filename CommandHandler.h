// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "DrawbotBuilder.h"

// This is the content of the .h file, which is where the declarations go

const int BUFFER_SIZE = 30;

class CommandHandler
{
private:
  DrawbotBuilder drawbotBuilder;
  char buffer[BUFFER_SIZE];
  int sofar;

public:
  explicit CommandHandler(const DrawbotBuilder& drawbotBuilder);

  void init();

  void handle();

  void processCommand();

  void reset();

  void handleOld();
};

// This is the end of the header guard
#endif

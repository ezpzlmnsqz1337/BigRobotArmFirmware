// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

const int32_t BUFFER_SIZE = 50;

class CommandHandler
{
private:
  char buffer[BUFFER_SIZE];
  int32_t sofar;
  ArmBuilder mArmBuilder;

  void processGoTo(const char* command);
  void processHome(const char* command);
  void processSetZeroPosition(const char* command);
  void processSpeed(const char* command);
  void processAccel(const char* command);
  void processGripper(const char* command);
  void processSyncMotors(const char* command);
  void processStatus(const char* command);

public:
  CommandHandler();

  void init(const ArmBuilder& mArmBuilder);

  void handle();

  void processCommand();

  void printPositionResponse(const bool valid);
  void printGripperResponse(const bool valid);
  void printSpeedResponse(const bool valid);
  void printAccelerationResponse(const bool valid);
  void printSyncMotorsResponse(const bool valid);
  void printInvalidCommandResponse();

  void reset();
};

// This is the end of the header guard
#endif

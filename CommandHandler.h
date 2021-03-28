// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "ArmBuilder.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

const int8_t BUFFER_SIZE = 50;
const int8_t MAX_SEQUENCE_COMMANDS = 20;

class CommandHandler
{
private:
  // serial handling
  char buffer[BUFFER_SIZE];
  int8_t sofar;

  // sequence handling
  char sequence[MAX_SEQUENCE_COMMANDS][BUFFER_SIZE];
  int8_t numOfSequenceCommands = 0;
  int8_t numOfSequenceRepetitions = 1;
  bool isSequence = false;
  bool enableResponse = true;

  ArmBuilder mArmBuilder;

  void processGoTo(const char* command);
  void processHome(const char* command);
  void processSetZeroPosition(const char* command);
  void processSpeed(const char* command);
  void processAccel(const char* command);
  void processGripper(const char* command);
  void processSyncMotors(const char* command);
  void processStatus();
  void processSequence(const char* command);
  void addCommandToSequence();
  void executeSequence();

public:
  CommandHandler();

  void init(const ArmBuilder& mArmBuilder);

  void handle();

  void processCommand(const char* command);

  void printPositionResponse(const bool valid);
  void printGripperResponse(const bool valid);
  void printSpeedResponse(const bool valid);
  void printAccelerationResponse(const bool valid);
  void printSyncMotorsResponse(const bool valid);
  void printSequenceResponse();
  void printInvalidCommandResponse();

  void reset();
};

// This is the end of the header guard
#endif

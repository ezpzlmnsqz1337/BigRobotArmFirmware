#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "ArmBuilder.h"
#include "Structures.h"

const int8_t BUFFER_SIZE_BYTES = 50;

class CommandHandler
{
private:
  ArmBuilder mArmBuilder;

  // serial handling
  char buffer[BUFFER_SIZE_BYTES];
  int8_t sofar;

protected:
  void handlePositionCommand(uint8_t* command);
  void handleAccelerationCommand(uint8_t* command);
  void handleSpeedCommand(uint8_t* command);
  void handleResetPositionsCommand();
  void handleHomeCommand();
  void handleGripperCommand(uint8_t* command);
  void handleSyncMotorsCommand(uint8_t* command);

public:
  CommandHandler();

  void init(const ArmBuilder& mArmBuilder);

  void handle();

  void sendInt32(const int32_t cNum);

  void printStatus();
  void processCommand(uint8_t* command);
  void printReadyResponse();
  void printInvalidCommandResponse();

  void reset();
};

#endif

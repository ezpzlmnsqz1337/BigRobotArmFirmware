#include "CommandHandler.h"
#include "Arduino.h"
#include "Commands.h"
#include "Config.h"
#include "Constants.h"
#include "MemoryFree.h"

CommandHandler::CommandHandler() : buffer("")
{
  sofar = 0;
}

void CommandHandler::init(const ArmBuilder& armBuilder)
{
  Serial.begin(250000);
  mArmBuilder = armBuilder;
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
      processCommand(reinterpret_cast<uint8_t*>(buffer));
      reset();
      return;
    }
    if (sofar < BUFFER_SIZE)
    {
      // if space in buffer, add char to buffer
      buffer[sofar++] = c;
    }
  }
}

void CommandHandler::processCommand(uint8_t* command)
{
  bool valid = false;
  // Serial.print("Free mem: ");
  // Serial.write(freeMemory());
  // Serial.print("Process command: ");
  // Serial.write(reinterpret_cast<char*>(command));

  if (command)
  {
    switch (command[0])
    {
    case CommandId::POSITION_COMMAND_ID:
      handlePositionCommand(&command[1]);
      break;
    case CommandId::HOME_COMMAND_ID:
      handleHomeCommand();
      break;
    case CommandId::RESET_POSITION_COMMAND_ID:
      handleResetPositionsCommand();
      break;
    case CommandId::SPEED_COMMAND_ID:
      handleSpeedCommand(&command[1]);
      break;
    case CommandId::ACCEL_COMMAND_ID:
      handleAccelerationCommand(&command[1]);
      break;
    case CommandId::GRIPPER_COMMAND_ID:
      handleGripperCommand(&command[1]);
      break;
    case CommandId::SYNC_MOTORS_COMMAND_ID:
      handleSyncMotorsCommand(&command[1]);
      break;
    case CommandId::STATUS_COMMAND_ID:
      printStatus();
      break;
    // case CommandId::BEGIN_COMMAND_ID:
    // mArmBuilder.goTo(reinterpret_cast<BEGIN_COMMAND_ID*>(command[1])->positions);
    // break;
    // case CommandId::END_COMMAND_ID:
    // mArmBuilder.goTo(reinterpret_cast<PositionCommand*>(command[1])->positions);
    // break;
    default:
      // if unknown command
      printInvalidCommandResponse();
    }
  }
  printReadyResponse();
}

void CommandHandler::handlePositionCommand(uint8_t* command)
{
  Serial.write(static_cast<uint8_t>(CommandId::POSITION_COMMAND_ID));
  Serial.println();

  JointOptions target = reinterpret_cast<const PositionCommand*>(command)->positions;

  mArmBuilder.goTo(target);

  JointOptions jp = mArmBuilder.getPositions();
  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);

  Serial.println();
}

void CommandHandler::handleAccelerationCommand(uint8_t* command)
{
  Serial.write(static_cast<uint8_t>(CommandId::ACCEL_COMMAND_ID));
  Serial.println();
  mArmBuilder.setAccelerations(reinterpret_cast<const AccelerationCommand*>(command)->values);

  JointOptions jp = mArmBuilder.getAccelerations();
  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);

  Serial.println();
}

void CommandHandler::handleSpeedCommand(uint8_t* command)
{
  Serial.write(static_cast<uint8_t>(CommandId::SPEED_COMMAND_ID));
  Serial.println();
  JointOptions speeds = reinterpret_cast<const SpeedCommand*>(command)->speeds;
  mArmBuilder.setSpeeds(speeds);

  JointOptions jp = mArmBuilder.getSpeeds();

  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);

  Serial.println();
}

void CommandHandler::handleGripperCommand(uint8_t* command)
{
  Serial.write(static_cast<uint8_t>(CommandId::GRIPPER_COMMAND_ID));
  Serial.println();
  const GripperOptions go = reinterpret_cast<const GripperCommand*>(command)->go;

  mArmBuilder.handleGripper(go);

  const GripperOptions result = mArmBuilder.getGripperOptions();
  Serial.write(result.enabled);
  Serial.write(result.position);

  Serial.println();
}

void CommandHandler::handleResetPositionsCommand()
{
  Serial.write(static_cast<uint8_t>(CommandId::RESET_POSITION_COMMAND_ID));
  Serial.println();
  mArmBuilder.setZeroPosition();

  JointOptions jp = mArmBuilder.getPositions();
  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);

  Serial.println();
}

void CommandHandler::handleHomeCommand()
{
  Serial.write(static_cast<uint8_t>(CommandId::HOME_COMMAND_ID));
  Serial.println();
  mArmBuilder.goTo({0, 0, 0, 0, 0});

  JointOptions jp = mArmBuilder.getPositions();
  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);

  Serial.println();
}

void CommandHandler::handleSyncMotorsCommand(uint8_t* command)
{
  Serial.write(static_cast<uint8_t>(CommandId::SYNC_MOTORS_COMMAND_ID));
  Serial.println();
  mArmBuilder.setSyncMotors(reinterpret_cast<const SyncMotorsCommand*>(command)->syncMotors);
  uint8_t enabled = mArmBuilder.isSyncEnabled() ? 1 : 0;
  Serial.write(enabled);

  Serial.println();
}

void CommandHandler::sendInt32(const int32_t cNum)
{
  Serial.write(cNum);
  Serial.write((cNum >> 8) & 0xFF);
  Serial.write((cNum >> 16) & 0xFF);
  Serial.write((cNum >> 32) & 0xFF);
}

void CommandHandler::printStatus()
{

  Serial.write(static_cast<uint8_t>(BigRobotArmStatus::STATUS));
  Serial.println();
  // positions
  JointOptions jp = mArmBuilder.getPositions();
  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);
  // accelerations
  JointOptions ja = mArmBuilder.getAccelerations();
  sendInt32(ja.base);
  sendInt32(ja.shoulder);
  sendInt32(ja.elbow);
  sendInt32(ja.wristRotate);
  sendInt32(ja.wrist);
  // speeds
  JointOptions js = mArmBuilder.getSpeeds();
  sendInt32(js.base);
  sendInt32(js.shoulder);
  sendInt32(js.elbow);
  sendInt32(js.wristRotate);
  sendInt32(js.wrist);
  // gripper
  GripperOptions go = mArmBuilder.getGripperOptions();
  Serial.write(go.enabled);
  Serial.write(go.position);
  // sync motors
  uint8_t syncEnabled = mArmBuilder.isSyncEnabled() ? 1 : 0;
  Serial.write(syncEnabled);
  Serial.println();
}

void CommandHandler::printReadyResponse()
{
  Serial.write(static_cast<uint8_t>(BigRobotArmStatus::READY));
  Serial.println();
}

void CommandHandler::printInvalidCommandResponse()
{
  Serial.write(static_cast<uint8_t>(BigRobotArmStatus::INVALID_COMMAND));
  Serial.println();
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }
}
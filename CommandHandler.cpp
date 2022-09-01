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
    case CommandId::GOTO_COMMAND_ID:
      handlePositionCommand(&command[1]);
      break;
    case CommandId::HOME_COMMAND_ID:
      mArmBuilder.goTo({0, 0, 0, 0, 0});
      break;
    case CommandId::RESET_POSITION_COMMAND_ID:
      mArmBuilder.setZeroPosition();
      break;
    case CommandId::SPEED_COMMAND_ID:
      mArmBuilder.setSpeeds(reinterpret_cast<const SpeedCommand*>(&command[1])->speeds);
      break;
    case CommandId::ACCEL_COMMAND_ID:
      mArmBuilder.setAccelerations(reinterpret_cast<const AccelerationCommand*>(&command[1])->values);
      break;
    case CommandId::GRIPPER_COMMAND_ID:
      handleGripperCommand(&command[1]);
      break;
    case CommandId::SYNC_MOTORS_COMMAND_ID:
      mArmBuilder.setSyncMotors(reinterpret_cast<const SyncMotorsCommand*>(&command[1])->syncMotors);
      break;
    case CommandId::STATUS_COMMAND_ID:
      printStatus();
      break;
    // case CommandId::BEGIN_COMMAND_ID:
    //   mArmBuilder.goTo(reinterpret_cast<BEGIN_COMMAND_ID*>(command[1])->positions);
    //   break;
    // case CommandId::END_COMMAND_ID:
    //   mArmBuilder.goTo(reinterpret_cast<PositionCommand*>(command[1])->positions);
    //   break;
    default:
      // if unknown command
      printInvalidCommandResponse();
    }
  }
  printReadyResponse();
}

void CommandHandler::handlePositionCommand(uint8_t* command)
{
  JointPositions original = mArmBuilder.getPositions();

  JointPositions target = reinterpret_cast<const PositionCommand*>(command)->positions;
  target.gripper = original.gripper;
  mArmBuilder.goTo(target);
}

void CommandHandler::handleGripperCommand(uint8_t* command)
{
  const GripperCommand* c = reinterpret_cast<const GripperCommand*>(command);
  if (c->enabled)
  {
    mArmBuilder.getGripper().getServo().init();
  }
  else
  {
    mArmBuilder.getGripper().getServo().deinit();
  }
  JointPositions jp = mArmBuilder.getPositions();
  jp.gripper = c->position;
  mArmBuilder.goTo(jp);
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

  Serial.println(BigRobotArmStatus::STATUS);
  // positions
  JointPositions jp = mArmBuilder.getPositions();
  sendInt32(jp.base);
  sendInt32(jp.shoulder);
  sendInt32(jp.elbow);
  sendInt32(jp.wristRotate);
  sendInt32(jp.wrist);
  // accelerations
  JointAccelerations ja = mArmBuilder.getAccelerations();
  sendInt32(ja.base);
  sendInt32(ja.shoulder);
  sendInt32(ja.elbow);
  sendInt32(ja.wristRotate);
  sendInt32(ja.wrist);
  // speeds
  JointSpeeds js = mArmBuilder.getSpeeds();
  sendInt32(js.base);
  sendInt32(js.shoulder);
  sendInt32(js.elbow);
  sendInt32(js.wristRotate);
  sendInt32(js.wrist);
  // gripper
  Gripper g = mArmBuilder.getGripper();
  uint8_t gripperEnabled = g.getServo().isEnabled() ? 1 : 0;
  uint8_t gripperPosition = static_cast<uint8_t>(g.getServo().getPosition());
  Serial.write(gripperEnabled);
  Serial.write(gripperPosition);
  // sync motors
  uint8_t syncEnabled = mArmBuilder.isSyncEnabled() ? 1 : 0;
  sendInt32(syncEnabled);
}

void CommandHandler::printReadyResponse()
{
  Serial.println(BigRobotArmStatus::READY);
}

void CommandHandler::printInvalidCommandResponse()
{
  Serial.write(BigRobotArmStatus::INVALID_COMMAND);
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }
}
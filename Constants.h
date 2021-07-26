#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef enum CommandId
{
  GOTO_COMMAND_ID = 1,
  HOME_COMMAND_ID = 2,
  RESET_POSITION_COMMAND_ID = 3,
  SPEED_COMMAND_ID = 4,
  ACCEL_COMMAND_ID = 5,
  GRIPPER_COMMAND_ID = 6,
  SYNC_MOTORS_COMMAND_ID = 7,
  STATUS_COMMAND_ID = 8,
  BEGIN_COMMAND_ID = 9,
  END_COMMAND_ID = 10
};

typedef enum BigRobotArmStatus
{
  READY = 200,
  INVALID_COMMAND = 201,
  STATUS = 202
};

#endif

#ifndef COMMANDS_H
#define COMMANDS_H

#include "Structures.h"

struct SyncMotorsCommand
{
  int8_t syncMotors;
};

struct AccelerationCommand
{
  JointOptions values;
};

struct SpeedCommand
{
  JointOptions speeds;
};

struct PositionCommand
{
  JointOptions positions;
};

struct GripperCommand
{
  GripperOptions go;
};

#endif

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
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

// This is the end of the header guard
#endif

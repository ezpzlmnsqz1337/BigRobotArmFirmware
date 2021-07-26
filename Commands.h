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
  JointAccelerations values;
};

struct SpeedCommand
{
  JointSpeeds speeds;
};

struct PositionCommand
{
  JointPositions positions;
};

struct GripperCommand
{
  int8_t enabled;
  int8_t position;
};

// This is the end of the header guard
#endif

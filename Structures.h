#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Inputs
{
  int32_t leftX;
  int32_t leftY;
  int32_t rightX;
  int32_t rightY;
};

struct JointPositions
{
  int32_t base;
  int32_t shoulder;
  int32_t elbow;
  int32_t wristRotate;
  int32_t wrist;
  int32_t gripper;
};

struct JointSpeeds
{
  int32_t base;
  int32_t shoulder;
  int32_t elbow;
  int32_t wristRotate;
  int32_t wrist;
};

struct JointAccelerations
{
  int32_t base;
  int32_t shoulder;
  int32_t elbow;
  int32_t wristRotate;
  int32_t wrist;
};

#endif

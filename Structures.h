#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Inputs
{
  int leftX;
  int leftY;
  int rightX;
  int rightY;
};

struct JointPositions
{
  long base;
  long shoulder;
  long elbow;
  long wristRotate;
  long wrist;
  int gripper;
};

struct JointSpeeds
{
  int base;
  int shoulder;
  int elbow;
  int wristRotate;
  int wrist;
};

struct JointAccelerations
{
  int base;
  int shoulder;
  int elbow;
  int wristRotate;
  int wrist;
};

#endif

#include "ArmBuilder.h"
#include "Arduino.h"
#include "math.h"

ArmBuilder::ArmBuilder()
{
}

void ArmBuilder::init()
{
  mBase.init(M1_STEP, M1_DIR, M1_ENABLE, Config::M1_STEPS_PER_REVOLUTION, MICRO_16);
  mBase.setMaxSpeedMultiplier(5);
  mBase.setAccelerationMultiplier(5);

  mShoulder.init(M2_STEP, M2_DIR, M2_ENABLE, Config::M2_STEPS_PER_REVOLUTION, HALF_STEPPING);
  mShoulder.getMotor().setPinsInverted(true);

  mElbow.init(M3_STEP, M3_DIR, M3_ENABLE, Config::M3_STEPS_PER_REVOLUTION, MICRO_16);

  mWristRotate.init(M4_STEP, M4_DIR, M4_ENABLE, Config::M4_STEPS_PER_REVOLUTION, MICRO_16);

  mWrist.init(M5_STEP, M5_DIR, M5_ENABLE, Config::M5_STEPS_PER_REVOLUTION, MICRO_16);

  mGripper.init();
}

void ArmBuilder::setSpeed(const float speed)
{
  if (speed > 0 && speed < 2)
  {
    mSpeedMultiplier = speed;
    mBase.setMaxSpeedMultiplier(5 * mSpeedMultiplier);
    mShoulder.setMaxSpeedMultiplier(mSpeedMultiplier);
    mElbow.setMaxSpeedMultiplier(mSpeedMultiplier);
    mWristRotate.setMaxSpeedMultiplier(mSpeedMultiplier);
    mWrist.setMaxSpeedMultiplier(mSpeedMultiplier);
  }
}

void ArmBuilder::setAcceleration(const float acceleration)
{
  if (acceleration > 0 && acceleration < 2)
  {
    mAccelerationMultiplier = acceleration;
    mBase.setAccelerationMultiplier(5 * mAccelerationMultiplier);
    mShoulder.setAccelerationMultiplier(mAccelerationMultiplier);
    mElbow.setAccelerationMultiplier(mAccelerationMultiplier);
    mWristRotate.setAccelerationMultiplier(mAccelerationMultiplier);
    mWrist.setAccelerationMultiplier(mAccelerationMultiplier);
  }
}

void ArmBuilder::setZeroPosition()
{
  mBase.getMotor().setCurrentPosition(0);
  mShoulder.getMotor().setCurrentPosition(0);
  mElbow.getMotor().setCurrentPosition(0);
  mWristRotate.getMotor().setCurrentPosition(0);
  mWrist.getMotor().setCurrentPosition(0);
}

void ArmBuilder::goTo(const JointPositions& jp)
{
  mBase.getMotor().moveTo(jp.base);
  mShoulder.getMotor().moveTo(jp.shoulder);
  mElbow.getMotor().moveTo(jp.elbow);
  mWristRotate.getMotor().moveTo(jp.wristRotate);
  mWrist.getMotor().moveTo(jp.wrist);
  mGripper.getServo().setTargetPosition(jp.gripper);

  while (!reachedPositions(jp))
  {
    mBase.getMotor().run();
    mShoulder.getMotor().run();
    mElbow.getMotor().run();
    mWristRotate.getMotor().run();
    mWrist.getMotor().run();
    mGripper.getServo().loop();
  }
}

void ArmBuilder::goToSync(const JointPositions& jp)
{
  AccelStepper b = mBase.getMotor();
  AccelStepper s = mShoulder.getMotor();
  AccelStepper e = mElbow.getMotor();
  AccelStepper wr = mWristRotate.getMotor();
  AccelStepper w = mWrist.getMotor();

  // set target positions
  b.moveTo(jp.base);
  s.moveTo(jp.shoulder);
  e.moveTo(jp.elbow);
  wr.moveTo(jp.wristRotate);
  w.moveTo(jp.wrist);
  mGripper.getServo().setTargetPosition(jp.gripper);

  // store old max speed to set it back after the movement
  float oldSpeed[5]{b.maxSpeed(), s.maxSpeed(), e.maxSpeed(), wr.maxSpeed(), w.maxSpeed()};
  // calculate how long it takes for each motor to get to target position
  float timeToPos[5]{calculateTimeToPosition(b.currentPosition(), b.targetPosition(), b.maxSpeed()),
                     calculateTimeToPosition(s.currentPosition(), s.targetPosition(), s.maxSpeed()),
                     calculateTimeToPosition(e.currentPosition(), e.targetPosition(), e.maxSpeed()),
                     calculateTimeToPosition(wr.currentPosition(), wr.targetPosition(), wr.maxSpeed()),
                     calculateTimeToPosition(w.currentPosition(), w.targetPosition(), w.maxSpeed())};
  // find slowest time
  float maxTime = findMaxInArray(timeToPos, 5);

  // for each motor set new max speed according to formula:
  // (slowest time / this motor time) * (1 / this motor max speed)
  b.setMaxSpeed((maxTime / timeToPos[0]) * (1 / b.maxSpeed()));
  s.setMaxSpeed((maxTime / timeToPos[1]) * (1 / s.maxSpeed()));
  e.setMaxSpeed((maxTime / timeToPos[2]) * (1 / e.maxSpeed()));
  wr.setMaxSpeed((maxTime / timeToPos[3]) * (1 / wr.maxSpeed()));
  w.setMaxSpeed((maxTime / timeToPos[4]) * (1 / w.maxSpeed()));

  while (!reachedPositions(jp))
  {
    mBase.getMotor().run();
    mShoulder.getMotor().run();
    mElbow.getMotor().run();
    mWristRotate.getMotor().run();
    mWrist.getMotor().run();
    mGripper.getServo().loop();
  }

  b.setMaxSpeed(oldSpeed[0]);
  s.setMaxSpeed(oldSpeed[1]);
  e.setMaxSpeed(oldSpeed[2]);
  wr.setMaxSpeed(oldSpeed[3]);
  w.setMaxSpeed(oldSpeed[4]);
}

float ArmBuilder::findMaxInArray(const float* array, const int size)
{
  float max = 0;
  for (int i = 0; i < size; i++)
  {
    if (array[i] > max)
    {
      max = array[i];
    }
  }
  return max;
}

float ArmBuilder::calculateTimeToPosition(const long position, const long target, const float speed)
{
  return abs(position - target) / speed;
}

void ArmBuilder::move(const JointPositions& jp)
{
  long dBase = getNormalizedValue(jp.base);
  long dShoulder = getNormalizedValue(jp.shoulder);
  long dElbow = getNormalizedValue(jp.elbow);
  long dWristRotate = getNormalizedValue(jp.wristRotate);
  long dWrist = getNormalizedValue(jp.wrist);

  mBase.getMotor().move(dBase);
  mShoulder.getMotor().move(dShoulder);
  mElbow.getMotor().move(dElbow);
  mWristRotate.getMotor().move(dWristRotate);
  mWrist.getMotor().move(dWrist);
}

bool ArmBuilder::reachedPositions(const JointPositions& jp)
{
  return (jp.base == mBase.getMotor().currentPosition() && jp.shoulder == mShoulder.getMotor().currentPosition() &&
          jp.elbow == mElbow.getMotor().currentPosition() &&
          jp.wristRotate == mWristRotate.getMotor().currentPosition() &&
          jp.wrist == mWrist.getMotor().currentPosition() && jp.gripper == mGripper.getServo().getPosition());
}

long ArmBuilder::getNormalizedValue(const long value)
{
  const long deadZone = 30;
  return abs(value) > deadZone ? value : 0;
}

JointPositions ArmBuilder::getPositions()
{
  return {mBase.getMotor().currentPosition(),  mShoulder.getMotor().currentPosition(),
          mElbow.getMotor().currentPosition(), mWristRotate.getMotor().currentPosition(),
          mWrist.getMotor().currentPosition(), mGripper.getServo().getPosition()};
}

RobotArmJoint& ArmBuilder::getBase()
{
  return mBase;
}

RobotArmJoint& ArmBuilder::getShoulder()
{
  return mShoulder;
}

RobotArmJoint& ArmBuilder::getElbow()
{
  return mElbow;
}

RobotArmJoint& ArmBuilder::getWristRotate()
{
  return mWristRotate;
}

RobotArmJoint& ArmBuilder::getWrist()
{
  return mWrist;
}

Gripper& ArmBuilder::getGripper()
{
  return mGripper;
}

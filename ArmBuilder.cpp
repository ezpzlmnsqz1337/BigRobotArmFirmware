#include "ArmBuilder.h"
#include "Arduino.h"
#include "math.h"

ArmBuilder::ArmBuilder()
{
}

void ArmBuilder::init()
{
  mBase.init(M1_STEP, M2_DIR, M1_ENABLE, Config::M1_STEPS_PER_REVOLUTION, MICRO_16);
  mBase.setMaxSpeedMultiplier(5);
  mBase.setAccelerationMultiplier(5);

  mShoulder.init(M2_STEP, M2_DIR, M2_ENABLE, Config::M2_STEPS_PER_REVOLUTION, MICRO_8);
  mShoulder.getMotor().setPinsInverted(true);

  mElbow.init(M3_STEP, M3_DIR, M3_ENABLE, Config::M3_STEPS_PER_REVOLUTION, MICRO_8);

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

#include "ArmBuilder.h"
#include "FirmwareLimits.h"
#include "Arduino.h"
#include "math.h"

namespace
{
float clampMultiplier(const int32_t value, const long minValue, const long maxValue)
{
  return static_cast<float>(clampConfiguredValue(value, minValue, maxValue)) / 100;
}
} // namespace

ArmBuilder::ArmBuilder()
{
}

void ArmBuilder::startMotion(const JointPositions& jp)
{
  mActiveMotionTarget = jp;

  if (mSyncMotors)
  {
    long position[]{jp.base, jp.shoulder, jp.elbow, jp.wristRotate, jp.wrist};
    mGripper.getServo().setTargetPosition(jp.gripper);
    mMultiStepper.moveTo(position);
  }
  else
  {
    mBase.getMotor().moveTo(jp.base);
    mShoulder.getMotor().moveTo(jp.shoulder);
    mElbow.getMotor().moveTo(jp.elbow);
    mWristRotate.getMotor().moveTo(jp.wristRotate);
    mWrist.getMotor().moveTo(jp.wrist);
    mGripper.getServo().setTargetPosition(jp.gripper);
  }

  mMotionActive = true;
}

void ArmBuilder::init()
{
  mBase.init(M1_STEP, M1_DIR, M1_ENABLE, Config::M1_STEPS_PER_REVOLUTION, MICRO_16);
  mBase.setMaxSpeedMultiplier(2);
  mBase.setAccelerationMultiplier(2);

  mShoulder.init(M2_STEP, M2_DIR, M2_ENABLE, Config::M2_STEPS_PER_REVOLUTION, HALF_STEPPING);
  mShoulder.getMotor().setPinsInverted(true);

  mElbow.init(M3_STEP, M3_DIR, M3_ENABLE, Config::M3_STEPS_PER_REVOLUTION, MICRO_16);

  mWristRotate.init(M4_STEP, M4_DIR, M4_ENABLE, Config::M4_STEPS_PER_REVOLUTION, MICRO_16);

  mWrist.init(M5_STEP, M5_DIR, M5_ENABLE, Config::M5_STEPS_PER_REVOLUTION, MICRO_16);

  // init multistepper
  mMultiStepper.addStepper(mBase.getMotor());
  mMultiStepper.addStepper(mShoulder.getMotor());
  mMultiStepper.addStepper(mElbow.getMotor());
  mMultiStepper.addStepper(mWristRotate.getMotor());
  mMultiStepper.addStepper(mWrist.getMotor());

  mGripper.init();
}

void ArmBuilder::setSpeed(const float speed)
{
  if (speed > 0 && speed < 5)
  {
    mSpeedMultiplier = speed;
    mBase.setMaxSpeedMultiplier(mSpeedMultiplier);
    mShoulder.setMaxSpeedMultiplier(mSpeedMultiplier);
    mElbow.setMaxSpeedMultiplier(mSpeedMultiplier);
    mWristRotate.setMaxSpeedMultiplier(mSpeedMultiplier);
    mWrist.setMaxSpeedMultiplier(mSpeedMultiplier);
  }
}

void ArmBuilder::setAcceleration(const float acceleration)
{
  if (acceleration > 0 && acceleration < 5)
  {
    mAccelerationMultiplier = acceleration;
    mBase.setAccelerationMultiplier(mAccelerationMultiplier);
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
  startMotion(jp);
  while (isMotionActive())
  {
    serviceMotion();
  }
}

bool ArmBuilder::queueMove(const JointPositions& jp)
{
  return mMotionQueue.enqueue(jp);
}

void ArmBuilder::serviceMotion()
{
  if (!mMotionActive)
  {
    JointPositions nextTarget;
    if (!mMotionQueue.dequeue(nextTarget))
    {
      return;
    }

    startMotion(nextTarget);
  }

  if (mSyncMotors)
  {
    const bool syncMoving = mMultiStepper.run();
    mGripper.getServo().loop();
    if (!(syncMoving || mGripper.getServo().getPosition() != mActiveMotionTarget.gripper))
    {
      mMotionActive = false;
    }
  }
  else
  {
    mBase.getMotor().run();
    mShoulder.getMotor().run();
    mElbow.getMotor().run();
    mWristRotate.getMotor().run();
    mWrist.getMotor().run();
    mGripper.getServo().loop();

    if (reachedPositions(mActiveMotionTarget))
    {
      mMotionActive = false;
    }
  }

  if (!mMotionActive)
  {
    JointPositions nextTarget;
    if (mMotionQueue.dequeue(nextTarget))
    {
      startMotion(nextTarget);
    }
  }
}

bool ArmBuilder::isMotionActive()
{
  return mMotionActive;
}

void ArmBuilder::goToSyncMultiStepper(const JointPositions& jp)
{
  long position[]{jp.base, jp.shoulder, jp.elbow, jp.wristRotate, jp.wrist};
  mGripper.getServo().setTargetPosition(jp.gripper);
  mMultiStepper.moveTo(position);

  while (mMultiStepper.run() || mGripper.getServo().getPosition() != jp.gripper)
  {
    mGripper.getServo().loop();
  }
}

void ArmBuilder::goToSync(const JointPositions& jp)
{
  AccelStepper& b = mBase.getMotor();
  AccelStepper& s = mShoulder.getMotor();
  AccelStepper& e = mElbow.getMotor();
  AccelStepper& wr = mWristRotate.getMotor();
  AccelStepper& w = mWrist.getMotor();

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
  float timeToPos[5]{calculateTimeToPosition(b.currentPosition(), jp.base, b.maxSpeed()),
                     calculateTimeToPosition(s.currentPosition(), jp.shoulder, s.maxSpeed()),
                     calculateTimeToPosition(e.currentPosition(), jp.elbow, e.maxSpeed()),
                     calculateTimeToPosition(wr.currentPosition(), jp.wristRotate, wr.maxSpeed()),
                     calculateTimeToPosition(w.currentPosition(), jp.wrist, w.maxSpeed())};

  // find slowest time
  float maxTime = findMaxInArray(timeToPos, 5);
  if (maxTime == 0)
  {
    return;
  }

  // for each motor set new max speed according to formula:
  // (slowest time / this motor time) * (1 / this motor max speed)
  if (timeToPos[0] > 0)
  {
    b.setMaxSpeed((timeToPos[0] / maxTime) * b.maxSpeed());
  }
  if (timeToPos[1] > 0)
  {
    s.setMaxSpeed((timeToPos[1] / maxTime) * s.maxSpeed());
  }
  if (timeToPos[2] > 0)
  {
    e.setMaxSpeed((timeToPos[2] / maxTime) * e.maxSpeed());
  }
  if (timeToPos[3] > 0)
  {
    wr.setMaxSpeed((timeToPos[3] / maxTime) * wr.maxSpeed());
  }
  if (timeToPos[4] > 0)
  {
    w.setMaxSpeed((timeToPos[4] / maxTime) * w.maxSpeed());
  }

  while (!reachedPositions(jp))
  {
    b.run();
    s.run();
    e.run();
    wr.run();
    w.run();
    mGripper.getServo().loop();
  }

  b.setMaxSpeed(oldSpeed[0]);
  s.setMaxSpeed(oldSpeed[1]);
  e.setMaxSpeed(oldSpeed[2]);
  wr.setMaxSpeed(oldSpeed[3]);
  w.setMaxSpeed(oldSpeed[4]);
}

bool ArmBuilder::isSyncEnabled()
{
  return mSyncMotors;
}

void ArmBuilder::setSyncMotors(const bool sync)
{
  mSyncMotors = sync;
}

JointSpeeds ArmBuilder::getSpeeds()
{
  return {
      static_cast<int32_t>(mBase.getMaxSpeedMultiplier() * 100),
      static_cast<int32_t>(mShoulder.getMaxSpeedMultiplier() * 100),
      static_cast<int32_t>(mElbow.getMaxSpeedMultiplier() * 100),
      static_cast<int32_t>(mWristRotate.getMaxSpeedMultiplier() * 100),
      static_cast<int32_t>(mWrist.getMaxSpeedMultiplier() * 100),
  };
}

JointAccelerations ArmBuilder::getAccelerations()
{
  return {
      static_cast<int32_t>(mBase.getAccelerationMultiplier() * 100),
      static_cast<int32_t>(mShoulder.getAccelerationMultiplier() * 100),
      static_cast<int32_t>(mElbow.getAccelerationMultiplier() * 100),
      static_cast<int32_t>(mWristRotate.getAccelerationMultiplier() * 100),
      static_cast<int32_t>(mWrist.getAccelerationMultiplier() * 100),
  };
}

void ArmBuilder::setSpeeds(const JointSpeeds& js)
{
  mBase.setMaxSpeedMultiplier(clampMultiplier(js.base, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
  mShoulder.setMaxSpeedMultiplier(
    clampMultiplier(js.shoulder, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
  mElbow.setMaxSpeedMultiplier(clampMultiplier(js.elbow, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
  mWristRotate.setMaxSpeedMultiplier(
    clampMultiplier(js.wristRotate, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
  mWrist.setMaxSpeedMultiplier(clampMultiplier(js.wrist, Config::MIN_SPEED_MULTIPLIER, Config::MAX_SPEED_MULTIPLIER));
}

void ArmBuilder::setAccelerations(const JointAccelerations& ja)
{
  mBase.setAccelerationMultiplier(
    clampMultiplier(ja.base, Config::MIN_ACCEL_MULTIPLIER, Config::MAX_ACCEL_MULTIPLIER));
  mShoulder.setAccelerationMultiplier(
    clampMultiplier(ja.shoulder, Config::MIN_ACCEL_MULTIPLIER, Config::MAX_ACCEL_MULTIPLIER));
  mElbow.setAccelerationMultiplier(
    clampMultiplier(ja.elbow, Config::MIN_ACCEL_MULTIPLIER, Config::MAX_ACCEL_MULTIPLIER));
  mWristRotate.setAccelerationMultiplier(
    clampMultiplier(ja.wristRotate, Config::MIN_ACCEL_MULTIPLIER, Config::MAX_ACCEL_MULTIPLIER));
  mWrist.setAccelerationMultiplier(
    clampMultiplier(ja.wrist, Config::MIN_ACCEL_MULTIPLIER, Config::MAX_ACCEL_MULTIPLIER));
}

float ArmBuilder::findMaxInArray(const float* array, const int32_t size)
{
  float max = 0;
  for (int32_t i = 0; i < size; i++)
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
  return {mBase.getMotor().targetPosition(),  mShoulder.getMotor().targetPosition(),
          mElbow.getMotor().targetPosition(), mWristRotate.getMotor().targetPosition(),
          mWrist.getMotor().targetPosition(), mGripper.getServo().getPosition()};
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

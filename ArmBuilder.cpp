#include "ArmBuilder.h"
#include "Arduino.h"
#include "math.h"

ArmBuilder::ArmBuilder()
{
}

void ArmBuilder::init()
{
  mBase.init(54, 55, 38); // hardcoded dir pins
  mBase.setMaxSpeed(mShoulder.getMaxSpeed() * 5);
  mBase.setAcceleration(mShoulder.getAcceleration() * 5);

  mShoulder.init(60, 61, 56);
  mShoulder.setMaxSpeed(mShoulder.getMaxSpeed());         // no microstepping
  mShoulder.setAcceleration(mShoulder.getAcceleration()); // no microstepping

  mElbow.init(46, 48, 62);
  mElbow.setMaxSpeed(mElbow.getMaxSpeed() * 8);
  mElbow.setAcceleration(mElbow.getAcceleration() * 8);

  mWristRotate.init(26, 28, 24);
  mWristRotate.setMaxSpeed(mWristRotate.getMaxSpeed());
  mWristRotate.setAcceleration(mWristRotate.getAcceleration());

  mWrist.init(36, 34, 30);
  mWrist.setMaxSpeed(mWrist.getMaxSpeed());
  mWrist.setAcceleration(mWrist.getAcceleration());

  mGripper.init();
}

void ArmBuilder::repeatPositions()
{
  // case: repeating positions
  if (currentPositionId == numOfSavedPositions + 1)
  {
    currentPositionId = 0;
  }
  goTo(savedPositions[currentPositionId]);
  currentPositionId++;
}

void ArmBuilder::loop(const long leftX, const long leftY, const long rightX, const long rightY, const bool isSwitched)
{
  if (isSwitched)
  {
    move(leftX, leftY, rightX, 0, rightY);
  }
  else
  {
    move(leftX, leftY, rightX, rightY, 0);
  }
  mBase.getMotor().run();
  mShoulder.getMotor().run();
  mElbow.getMotor().run();
  mWristRotate.getMotor().run();
  mWrist.getMotor().run();
}

void ArmBuilder::goTo(const long base, const long shoulder, const long elbow, const long wristRotate, const long wrist,
                      const int gripper)
{
  JointPositions jp; // {base, shoulder, elbow, wristRotate, wrist};
  jp.base = base;
  jp.shoulder = shoulder;
  jp.elbow = elbow;
  jp.wristRotate = wristRotate;
  jp.wrist = wrist;
  jp.gripper = gripper;

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

void ArmBuilder::move(const long base, const long shoulder, const long elbow, const long wristRotate, const long wrist)
{
  long dBase = getNormalizedValue(base);
  long dShoulder = getNormalizedValue(shoulder);
  long dElbow = getNormalizedValue(elbow);
  long dWristRotate = getNormalizedValue(wristRotate);
  long dWrist = getNormalizedValue(wrist);

  // Serial.print(", LEFT X: ");
  // Serial.println(dBase);
  // Serial.print(", LEFT Y: ");
  // Serial.println(dShoulder);
  // Serial.print(", RIGHT X: ");
  // Serial.println(dElbow);
  // Serial.print(", RIGHT Y: ");
  // Serial.println(dWristRotate);
  // Serial.print(", Wrist 2: ");
  // Serial.println(dWrist);
  // Serial.println();
  // Serial.println();

  mBase.getMotor().move(dBase);
  mShoulder.getMotor().move(dShoulder);
  mElbow.getMotor().move(dElbow);
  mWristRotate.getMotor().move(dWristRotate);
  mWrist.getMotor().move(dWrist);
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

  // Serial.print(", LEFT X: ");
  // Serial.println(dBase);
  // Serial.print(", LEFT Y: ");
  // Serial.println(dShoulder);
  // Serial.print(", RIGHT X: ");
  // Serial.println(dElbow);
  // Serial.print(", RIGHT Y: ");
  // Serial.println(dWristRotate);
  // Serial.print(", Wrist 2: ");
  // Serial.println(dWrist);
  // Serial.println();
  // Serial.println();

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

Base& ArmBuilder::getBase()
{
  return mBase;
}

Shoulder& ArmBuilder::getShoulder()
{
  return mShoulder;
}

Elbow& ArmBuilder::getElbow()
{
  return mElbow;
}

Wrist& ArmBuilder::getWristRotate()
{
  return mWristRotate;
}

Wrist& ArmBuilder::getWrist()
{
  return mWrist;
}

Gripper& ArmBuilder::getGripper()
{
  return mGripper;
}

void ArmBuilder::save()
{
  Serial.print("Save: ");
  Serial.println(numOfSavedPositions);

  if (numOfSavedPositions > MAX_POSITIONS - 1)
  {
    numOfSavedPositions = 0;
  }
  savedPositions[numOfSavedPositions].base = mBase.getMotor().currentPosition();
  savedPositions[numOfSavedPositions].shoulder = mShoulder.getMotor().currentPosition();
  savedPositions[numOfSavedPositions].elbow = mElbow.getMotor().currentPosition();
  savedPositions[numOfSavedPositions].wristRotate = mWristRotate.getMotor().currentPosition();
  savedPositions[numOfSavedPositions].wrist = mWrist.getMotor().currentPosition();
  savedPositions[numOfSavedPositions].gripper = mGripper.getServo().getPosition();

  numOfSavedPositions++;
}

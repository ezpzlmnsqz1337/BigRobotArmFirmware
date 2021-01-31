#include "Controls.h"

ezButton analog1SW(JOY_LEFT_BTN_PIN);
ezButton analog2SW(JOY_RIGHT_BTN_PIN);
ezButton analog3SW(BTN1_PIN);
ezButton analog4SW(BTN2_PIN);

Controls::Controls()
{
}

void Controls::init(ArmBuilder& armBuilder, const uint16_t nDebounceTime)
{
  pinMode(SERVO_PIN, OUTPUT);

  pinMode(JOY_LEFT_X_PIN, INPUT);
  pinMode(JOY_LEFT_Y_PIN, INPUT);

  pinMode(JOY_RIGHT_X_PIN, INPUT);
  pinMode(JOY_RIGHT_Y_PIN, INPUT);

  mArmBuilder = armBuilder;

  analog1SW.setDebounceTime(nDebounceTime); // set debounce time to nDebounceTime milliseconds
  analog2SW.setDebounceTime(nDebounceTime);
  analog3SW.setDebounceTime(nDebounceTime);
  analog4SW.setDebounceTime(nDebounceTime);
}

void Controls::loop()
{
  handleJoysticks();
  handleButtons();
}

void Controls::handleJoysticks()
{
  // low pass filter for bad input values
  mLeftValueX = 0.9 * mLeftValueX + 0.1 * analogRead(JOY_LEFT_X_PIN);
  mLeftValueY = 0.9 * mLeftValueY + 0.1 * analogRead(JOY_LEFT_Y_PIN);
  mLeftX = map(mLeftValueX, 100, 923, -400, 400);
  mLeftY = map(mLeftValueY, 100, 923, -400, 400);

  // low pass filter for bad input values
  mRightValueX = 0.9 * mRightValueX + 0.1 * analogRead(JOY_RIGHT_X_PIN);
  mRightValueY = 0.9 * mRightValueY + 0.1 * analogRead(JOY_RIGHT_Y_PIN);
  mRightX = map(mRightValueX, 100, 923, -400, 400);
  mRightY = map(mRightValueY, 100, 923, -400, 400);
}

void Controls::handleButtons()
{
  analog1SW.loop();
  analog2SW.loop();
  analog3SW.loop();
  analog4SW.loop();

  // Serial.print("analog1: ");
  // Serial.println(analog1SW);
  // Serial.print("analog2: ");
  // Serial.println(analog2SW);
  // Serial.print("analog3: ");
  // Serial.println(analog3SW);
  // Serial.print("analog4: ");
  // Serial.println(analog4SW);

  // save position
  if (analog1SW.isPressed())
  {
    if (analog1SWlastValue == 1)
    {
      analog1SWlastValue = 0;
      if (analog1SWflag == 0)
      {
        Serial.println("Switch1");
        mSwitched = false;
        analog1SWflag = 1;
      }
      else if (analog1SWflag == 1)
      {
        Serial.println("Switch2");
        mSwitched = true;
        analog1SWflag = 0;
      }
    }
  }
  else
  {
    analog1SWlastValue = 1;
  }

  // grip
  if (analog2SW.isPressed())
  {
    if (analog2SWlastValue == 1)
    {
      analog2SWlastValue = 0;
      if (analog2SWflag == 0)
      {
        Serial.println("Close");
        mArmBuilder.getGripper().close();
        analog2SWflag = 1;
      }
      else if (analog2SWflag == 1)
      {
        Serial.println("Open");
        mArmBuilder.getGripper().open();
        analog2SWflag = 0;
      }
    }
  }
  else
  {
    analog2SWlastValue = 1;
  }

  // save
  if (analog3SW.isPressed())
  {
    if (analog3SWlastValue == 1)
    {
      analog3SWlastValue = 0;
      if (analog3SWflag == 0)
      {
        mArmBuilder.save();
        analog3SWflag = 1;
      }
      else if (analog3SWflag == 1)
      {
        // save();
        analog3SWflag = 0;
      }
    }
  }
  else
  {
    analog3SWlastValue = 1;
  }

  // play
  if (analog4SW.isPressed())
  {
    if (analog4SWlastValue == 1)
    {
      analog4SWlastValue = 0;
      if (analog4SWflag == 0)
      {
        mPlaying = true;
        analog4SWflag = 1;
      }
      else if (analog4SWflag == 1)
      {
        mPlaying = false;
        analog4SWflag = 0;
      }
    }
  }
  else
  {
    analog4SWlastValue = 1;
  }
}

bool Controls::isPlaying()
{
  return mPlaying;
}

bool Controls::isSwitched()
{
  return mSwitched;
}

int Controls::getLeftX()
{
  return mLeftX;
}

int Controls::getLeftY()
{
  return mLeftY;
}

int Controls::getRightX()
{
  return mRightX;
}

int Controls::getRightY()
{
  return mRightY;
}

Inputs Controls::getInputs()
{
  return {mLeftX, mLeftY, mRightX, mRightY};
}
#include "Controls.h"

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
  leftValueX = 0.9 * leftValueX + 0.1 * analogRead(JOY_LEFT_X_PIN);
  leftValueY = 0.9 * leftValueY + 0.1 * analogRead(JOY_LEFT_Y_PIN);
  leftX = map(leftValueX, 100, 923, -400, 400);
  leftY = map(leftValueY, 100, 923, -400, 400);

  // low pass filter for bad input values
  rightValueX = 0.9 * rightValueX + 0.1 * analogRead(JOY_RIGHT_X_PIN);
  rightValueY = 0.9 * rightValueY + 0.1 * analogRead(JOY_RIGHT_Y_PIN);
  rightX = map(rightValueX, 100, 923, -400, 400);
  rightY = map(rightValueY, 100, 923, -400, 400);
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
        mSwitched = false;
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

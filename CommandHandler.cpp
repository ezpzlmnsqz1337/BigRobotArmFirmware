#include "CommandHandler.h"
#include "Arduino.h"
#include "Config.h"

CommandHandler::CommandHandler() : buffer("")
{
  sofar = 0;
}

void CommandHandler::init(const ArmBuilder& armBuilder)
{
  Serial.begin(250000);
  mArmBuilder = armBuilder;
  reset();
}

void CommandHandler::handle()
{
  if (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == 13)
    {
      // if enter, command is complete
      buffer[sofar] = 0;
      char* command = strtok(buffer, " ");
      processCommand(command);
      reset();
      return;
    }
    if (sofar < BUFFER_SIZE)
    {
      // if space in buffer, add char to buffer
      buffer[sofar++] = c;
    }
  }
}

void CommandHandler::processCommand(const char* command)
{
  if (isSequence && command[0] != 'E')
  {
    // if we are now in a sequence, add command to the queue
    addCommandToSequence();
    return;
  }
  if (command[0] == 'G')
  {
    int32_t gNumber = atoi(&command[1]);
    switch (gNumber)
    {
    case 0:
      processGoTo(command);
      break;
    case 1:
      processGripper(command);
      break;
    case 28:
      processHome(command);
      break;
    case 92:
      processSetZeroPosition(command);
      break;
    default:
      // if unknown command
      printInvalidCommandResponse();
    }
  }
  else if (command[0] == 'M')
  {
    int32_t mNumber = atoi(&command[1]);
    if (mNumber == 201)
    {
      processAccel(command);
    }
    else if (mNumber == 203)
    {
      processSpeed(command);
    }
    else if (mNumber == 503)
    {
      processStatus();
    }
  }
  else if (command[0] == 'S')
  {
    processSyncMotors(command);
  }
  else if (strcmp(command, "BEGIN") == 0) // start of sequence of commands
  {
    processSequence(command);
  }
  else if (strcmp(command, "END") == 0) // end of sequence of commands
  {
    executeSequence();
  }
  else
  {
    // if unknown command
    printInvalidCommandResponse();
  }
  Serial.println("BigRobotArm::READY");
}

void CommandHandler::processGoTo(const char* command)
{
  // handle positions
  char* base = strtok(NULL, " ");
  char* shoulder = strtok(NULL, " ");
  char* elbow = strtok(NULL, " ");
  char* wristRotate = strtok(NULL, " ");
  char* wrist = strtok(NULL, " ");

  JointPositions jp = mArmBuilder.getPositions();
  jp.base = base != NULL ? atol(&base[1]) : jp.base;
  jp.shoulder = shoulder != NULL ? atol(&shoulder[1]) : jp.shoulder;
  jp.elbow = elbow != NULL ? atol(&elbow[1]) : jp.elbow;
  jp.wristRotate = wristRotate != NULL ? atol(&wristRotate[2]) : jp.wristRotate;
  jp.wrist = wrist != NULL ? atol(&wrist[1]) : jp.wrist;

  mArmBuilder.goTo(jp);
  printPositionResponse(true);
}

void CommandHandler::processHome(const char* command)
{
  // G28 - home
  JointPositions jp{0, 0, 0, 0, 0, 40};
  mArmBuilder.goTo(jp);
  printPositionResponse(true);
}

void CommandHandler::processSetZeroPosition(const char* command)
{
  // G92 - set zero position
  mArmBuilder.setZeroPosition();
  printPositionResponse(true);
}

void CommandHandler::processSpeed(const char* command)
{
  // if speed command
  char* base = strtok(NULL, " ");
  char* shoulder = strtok(NULL, " ");
  char* elbow = strtok(NULL, " ");
  char* wristRotate = strtok(NULL, " ");
  char* wrist = strtok(NULL, " ");

  JointSpeeds js = mArmBuilder.getSpeeds();
  js.base = base != NULL ? atoi(&base[1]) : js.base;
  js.shoulder = shoulder != NULL ? atoi(&shoulder[1]) : js.shoulder;
  js.elbow = elbow != NULL ? atoi(&elbow[1]) : js.elbow;
  js.wristRotate = wristRotate != NULL ? atoi(&wristRotate[2]) : js.wristRotate;
  js.wrist = wrist != NULL ? atoi(&wrist[1]) : js.wrist;

  mArmBuilder.setSpeeds(js);
  printSpeedResponse(true);
}

void CommandHandler::processAccel(const char* command)
{
  // if acceleration command
  char* base = strtok(NULL, " ");
  char* shoulder = strtok(NULL, " ");
  char* elbow = strtok(NULL, " ");
  char* wristRotate = strtok(NULL, " ");
  char* wrist = strtok(NULL, " ");

  JointAccelerations ja = mArmBuilder.getAccelerations();
  ja.base = base != NULL ? atoi(&base[1]) : ja.base;
  ja.shoulder = shoulder != NULL ? atoi(&shoulder[1]) : ja.shoulder;
  ja.elbow = elbow != NULL ? atoi(&elbow[1]) : ja.elbow;
  ja.wristRotate = wristRotate != NULL ? atoi(&wristRotate[2]) : ja.wristRotate;
  ja.wrist = wrist != NULL ? atoi(&wrist[1]) : ja.wrist;

  mArmBuilder.setAccelerations(ja);
  printAccelerationResponse(true);
}

void CommandHandler::processGripper(const char* command)
{
  // G1 - handle gripper
  char* e = strtok(NULL, " ");
  char* p = strtok(NULL, " ");
  int32_t enable = e != NULL ? atol(&e[1]) : 0;
  int32_t position = p != NULL ? atol(&p[1]) : 0;
  if (enable == 1)
  {
    mArmBuilder.getGripper().init();
    JointPositions jp = mArmBuilder.getPositions();
    jp.gripper = position;
    mArmBuilder.goTo(jp);
    printGripperResponse(true);
  }
  else if (enable == 0)
  {
    mArmBuilder.getGripper().getServo().deinit();
    printGripperResponse(true);
  }
  else
  {
    printGripperResponse(false);
  }
}

void CommandHandler::processSyncMotors(const char* command)
{
  int32_t sync = atoi(&command[1]);
  if (sync == 0)
  {
    mArmBuilder.setSyncMotors(false);
    printSyncMotorsResponse(true);
  }
  else if (sync == 1)
  {
    mArmBuilder.setSyncMotors(true);
    printSyncMotorsResponse(true);
  }
  else
  {
    printSyncMotorsResponse(false);
  }
}

void CommandHandler::processSequence(const char* command)
{
  // set number of repetitions
  char* repetitions = strtok(NULL, " ");
  numOfSequenceRepetitions = repetitions != NULL ? atoi(&repetitions[1]) : 1;
  // set sequence to true to change behavior of process command
  isSequence = true;
  // disable Serial responses until sequence is executed
  enableResponse = false;
}

void CommandHandler::addCommandToSequence()
{
  strcpy(sequence[numOfSequenceCommands], buffer);
  numOfSequenceCommands++;
  printSequenceResponse();
}

void CommandHandler::executeSequence()
{
  // for number of repetitions
  for (int8_t i = 0; i < numOfSequenceRepetitions; i++)
  {
    // execute every command in the sequence
    for (int8_t j = 0; j < numOfSequenceCommands; j++)
    {
      processCommand(sequence[j]);
    }
  }
  // reset sequence parametes to default values
  enableResponse = true;
  isSequence = false;
  numOfSequenceCommands = 0;
  numOfSequenceRepetitions = 1;
  // send current arm state
  processStatus();
}

void CommandHandler::processStatus()
{
  printPositionResponse(true);
  printGripperResponse(true);
  printSpeedResponse(true);
  printAccelerationResponse(true);
  printSyncMotorsResponse(true);
}

void CommandHandler::printGripperResponse(const bool valid)
{
  if (enableResponse)
  {
    return; // do not send anything when playing sequence
  }
  if (valid)
  {
    bool enable = mArmBuilder.getGripper().getServo().isEnabled();
    int32_t position = mArmBuilder.getPositions().gripper;
    Serial.print("BigRobotArm::GRIPPER: ");
    Serial.print("E");
    Serial.print(enable ? 1 : 0);
    Serial.print(" P");
    Serial.println(position);
  }
  else
  {
    Serial.println("BigRobotArm::INVALID-GRIPPER-COMMAND");
  }
}

void CommandHandler::printSpeedResponse(const bool valid)
{
  if (enableResponse)
  {
    return; // do not send anything when playing sequence
  }
  if (valid)
  {
    JointSpeeds js = mArmBuilder.getSpeeds();
    Serial.print("BigRobotArm::SPEED: ");
    Serial.print("B");
    Serial.print(js.base);
    Serial.print(" S");
    Serial.print(js.shoulder);
    Serial.print(" E");
    Serial.print(js.elbow);
    Serial.print(" WR");
    Serial.print(js.wristRotate);
    Serial.print(" W");
    Serial.println(js.wrist);
  }
  else
  {
    Serial.println("BigRobotArm::INVALID-SPEED");
  }
}

void CommandHandler::printAccelerationResponse(const bool valid)
{
  if (enableResponse)
  {
    return; // do not send anything when playing sequence
  }
  if (valid)
  {
    JointAccelerations ja = mArmBuilder.getAccelerations();
    Serial.print("BigRobotArm::ACCELERATION: ");
    Serial.print("B");
    Serial.print(ja.base);
    Serial.print(" S");
    Serial.print(ja.shoulder);
    Serial.print(" E");
    Serial.print(ja.elbow);
    Serial.print(" WR");
    Serial.print(ja.wristRotate);
    Serial.print(" W");
    Serial.println(ja.wrist);
  }
  else
  {
    Serial.println("BigRobotArm::INVALID-ACCEL");
  }
}

void CommandHandler::printPositionResponse(const bool valid)
{
  if (enableResponse)
  {
    return; // do not send anything when playing sequence
  }
  JointPositions jp = mArmBuilder.getPositions();
  if (valid)
  {
    Serial.println("BigRobotArm::MOVING-TO");
    Serial.print("BigRobotArm::POSITION: ");
    Serial.print("B");
    Serial.print(jp.base);
    Serial.print(" S");
    Serial.print(jp.shoulder);
    Serial.print(" E");
    Serial.print(jp.elbow);
    Serial.print(" WR");
    Serial.print(jp.wristRotate);
    Serial.print(" W");
    Serial.println(jp.wrist);
  }
  else
  {
    printInvalidCommandResponse();
  }
}

void CommandHandler::printSyncMotorsResponse(const bool valid)
{
  if (enableResponse)
  {
    return; // do not send anything when playing sequence
  }
  if (valid)
  {
    Serial.print("BigRobotArm::SYNC-MOTORS: ");
    Serial.println(mArmBuilder.isSyncEnabled() ? 1 : 0);
  }
  else
  {
    printInvalidCommandResponse();
  }
}

void CommandHandler::printSequenceResponse()
{
  Serial.println("BigRobotArm::READY");
}

void CommandHandler::printInvalidCommandResponse()
{
  Serial.println("BigRobotArm::INVALID-COMMAND");
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
}
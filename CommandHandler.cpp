#include "CommandHandler.h"
#include "Arduino.h"

CommandHandler::CommandHandler(const DrawbotBuilder& _drawbotBuilder) : drawbotBuilder(_drawbotBuilder), buffer("")
{
  this->sofar = 0;
}

void CommandHandler::init()
{
  Serial.begin(9600);
  this->reset();
}

void CommandHandler::handle()
{

  if (Serial.available() > 0)
  {
    char c = Serial.read();
    Serial.print(c);
    if (c == 13)
    {
      // if enter, command is complete
      this->buffer[sofar] = 0;
      this->processCommand();
      this->reset();
      return;
    }
    if (sofar < BUFFER_SIZE)
    {
      // if space in buffer, add char to buffer
      this->buffer[sofar++] = c;
    }
  }
}

void CommandHandler::processCommand()
{
  char* command = strtok(this->buffer, " ");

  if (command[0] == 'G')
  {
    int gNumber = atoi(&command[1]);
    bool extrude = false;
    if (gNumber == 28)
    {
      // G28 - home
      this->drawbotBuilder.home();
      return;
    }
    // handle G command
    if (gNumber == 0)
    {
      // handle G0 command
      extrude = false;
    }
    if (gNumber == 1)
    {
      // handle G1 command
      extrude = true;
    }

    // handle positions
    char* x = strtok(NULL, " ");
    char* y = strtok(NULL, " ");

    int xPos = atoi(&x[1]);
    int yPos = atoi(&y[1]);

    Serial.println(xPos);
    Serial.print(",");
    Serial.println(yPos);

    this->drawbotBuilder.goTo(xPos, yPos, 300, extrude);
  }
}

void CommandHandler::reset()
{
  this->sofar = 0; // clear input buffer
  Serial.print("drawbot@test: ");
}

void CommandHandler::handleOld()
{
  if (Serial.available() > 0)
  {
    int userInput = Serial.read();
    Serial.println("Command: " + userInput);

    switch (userInput)
    {
    case 'h':
      drawbotBuilder.home();
      break;
    case 's':
      drawbotBuilder.moveX(-20);
      break;
    case 'a':
      drawbotBuilder.moveY(-20);
      break;
    case 'x':
      drawbotBuilder.moveX(20);
      break;
    case 'y':
      drawbotBuilder.moveY(20);
      break;
    case 'r':
      drawbotBuilder.reset();
      break;
    case 'u':
      drawbotBuilder.penUp();
      break;
    case 'd':
      drawbotBuilder.penDown();
      break;
    case 'f':
      drawbotBuilder.goTo(1020, 640, 500, true);
      break;
    case 'g':
      drawbotBuilder.goTo(1200, 340, 500, true);
      break;
    case 'b':
      drawbotBuilder.goTo(1520, 720, 500, true);
      break;
    case 'v':
      drawbotBuilder.goTo(1360, 980, 500, true);
      break;
    default:
      Serial.println("Unknown user input");
    }
  }
}
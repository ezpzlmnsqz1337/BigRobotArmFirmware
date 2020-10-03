#include "CommandHandler.h"
#include "Arduino.h"

CommandHandler::CommandHandler(const ArmBuilder& armbuilder) : armBuilder(armbuilder), buffer("")
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
      // this->armBuilder.home();
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

    this->armBuilder.goTo(xPos, yPos, 300, extrude);
  }
}

void CommandHandler::reset()
{
  this->sofar = 0; // clear input buffer
  Serial.print("drawbot@test: ");
}
#include "JointCommandRules.h"

#include <string.h>

#include "CommandInputRules.h"

namespace
{
bool parseLongToken(const char* token, const char* prefix, long& target)
{
  if (token == nullptr)
  {
    // Absent tokens keep the current value.
    return true;
  }

  long value = 0;
  if (!parsePrefixedLong(token, prefix, &value))
  {
    return false;
  }

  target = value;
  return true;
}

bool parseInt32Token(const char* token, const char* prefix, int32_t& target)
{
  if (token == nullptr)
  {
    // Absent tokens keep the current value.
    return true;
  }

  int32_t value = 0;
  if (!parsePrefixedInt32(token, prefix, &value))
  {
    return false;
  }

  target = value;
  return true;
}
} // namespace

bool parseJointPositionsCommand(char* command, JointPositions& positions)
{
  if (command == nullptr)
  {
    return false;
  }

  strtok(command, " ");
  char* base = strtok(nullptr, " ");
  char* shoulder = strtok(nullptr, " ");
  char* elbow = strtok(nullptr, " ");
  char* wristRotate = strtok(nullptr, " ");
  char* wrist = strtok(nullptr, " ");

  return parseLongToken(base, "B", positions.base) && parseLongToken(shoulder, "S", positions.shoulder) &&
         parseLongToken(elbow, "E", positions.elbow) &&
         parseLongToken(wristRotate, "WR", positions.wristRotate) && parseLongToken(wrist, "W", positions.wrist);
}

bool parseJointSpeedsCommand(char* command, JointSpeeds& speeds)
{
  if (command == nullptr)
  {
    return false;
  }

  strtok(command, " ");
  char* base = strtok(nullptr, " ");
  char* shoulder = strtok(nullptr, " ");
  char* elbow = strtok(nullptr, " ");
  char* wristRotate = strtok(nullptr, " ");
  char* wrist = strtok(nullptr, " ");

  return parseInt32Token(base, "B", speeds.base) && parseInt32Token(shoulder, "S", speeds.shoulder) &&
         parseInt32Token(elbow, "E", speeds.elbow) && parseInt32Token(wristRotate, "WR", speeds.wristRotate) &&
         parseInt32Token(wrist, "W", speeds.wrist);
}

bool parseJointAccelerationsCommand(char* command, JointAccelerations& accelerations)
{
  if (command == nullptr)
  {
    return false;
  }

  strtok(command, " ");
  char* base = strtok(nullptr, " ");
  char* shoulder = strtok(nullptr, " ");
  char* elbow = strtok(nullptr, " ");
  char* wristRotate = strtok(nullptr, " ");
  char* wrist = strtok(nullptr, " ");

  return parseInt32Token(base, "B", accelerations.base) &&
         parseInt32Token(shoulder, "S", accelerations.shoulder) &&
         parseInt32Token(elbow, "E", accelerations.elbow) &&
         parseInt32Token(wristRotate, "WR", accelerations.wristRotate) &&
         parseInt32Token(wrist, "W", accelerations.wrist);
}

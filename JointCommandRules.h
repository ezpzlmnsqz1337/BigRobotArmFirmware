#ifndef JOINT_COMMAND_RULES_H
#define JOINT_COMMAND_RULES_H

#include "Structures.h"

bool parseJointPositionsCommand(char* command, JointPositions& positions);
bool parseJointSpeedsCommand(char* command, JointSpeeds& speeds);
bool parseJointAccelerationsCommand(char* command, JointAccelerations& accelerations);

#endif

# BigRobotArmFirmware
Firmware for BigRobotArm from instructables (https://www.instructables.com/Build-a-Giant-3D-Printed-Robot-Arm/)


# Commands

## Goto
- `G0 Ba Sb Ec WRd We` where *a,b,c,d,e* are absolute steps to go to for each angle

## Home
- `G28` moves all motors in 0 position

## Reset positions
- `G92` sets current position of the motors to 0

## Speed
- `M203 Ba Sb Ec WRd We` where *a,b,c,d,e* are speed multipliers for each motor
- speed multiplier can be set from 1 to 300
- by default each motor has speed multiplier 100
- the given value is divided by 100, so that 100 multiplier is actually 1 (this is done to avoid sending floats over Serial, probably unnecessary)

## Accel
- `M201 Ba Sb Ec WRd We` where *a,b,c,d,e* are acceleration multipliers for each motor
- acceleration multiplier can be set from 1 to 300
- by default each motor has acceleration multiplier 100
- the given value is divided by 100, so that 100 multiplier is actually 1 (this is done to avoid sending floats over Serial, probably unnecessary)

## Gripper
- `G1 Ex Py` where *x* can be 0 to disable gripper, 1 to enable gripper and *y* is in range from 0 to 180 for servo position

## Sync motors
- `Sx` where *x* can be 0 to not synchronize motor movement or 1 to synchronize motor movement
- when sychronized, the motors arrive at the destination at the same time

## Status
- `M503` returns all information about current arm position and settings

## Composite command (sequences)
# Begin
- `BEGIN Ix` after this command any command sent will be added to buffer to be executed when #End command is received. *x* is number of repetitions of the sequence.

# End
- `END` starts execution of commands in buffer for number of iterations set after the #Begin command.

# Example
- `BEGIN I5` start of sequence, number of interations is 5
- `G0 B100 S200 E300 WR400 W300` move the arm to B100 S200 E300 WR400 W300
- `G0 B0 S50 E5000 WR1500 W0` move the arm to B0 S50 E5000 WR1500 W0
- `END` executes the sequence (the two commands above) 5 times


Response:
- BigRobotArm::POSITION: `Ba Sb Ec WRd We` - where *a,b,c,d,e* are current motor step positions
- BigRobotArm::GRIPPER: `Ex Py`- where *x* specifies if gripper is enabled (1) or disabled (0) and *y* specifies servo position
- BigRobotArm::SPEED: `Ba Sb Ec WRd We` where *a,b,c,d,e* are speed multipliers for each motor
- BigRobotArm::ACCELERATION: `Ba Sb Ec WRd We` where *a,b,c,d,e* are acceleration multipliers for each motor
- BigRobotArm::SYNC-MOTORS: `x` where *x* spceifies if motor movement is synced (1) or not (0)

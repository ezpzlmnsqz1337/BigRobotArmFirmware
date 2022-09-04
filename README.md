# BigRobotArmFirmware
Firmware for BigRobotArm from instructables (https://www.instructables.com/Build-a-Giant-3D-Printed-Robot-Arm/)


# Commands

## Goto
- moves all motors to given position
- example data `01 a0f6ffff 58020000 78ecffff 20030000 18fcffff`
    - `01` - uint8_t goto command id
    - `a0f6ffff`int32_t base motor position (-2400)
    - `58020000` int32_t shoulder motor position (600)
    - `78ecffff` int32_t elbow motor position (-5000)
    - `20030000` int32_t wrist rotate motor position (800)
    - `18fcffff` int32_t wrist motor position (-1000)
- example response `01 0d0a a0f6ffff 58020000 78ecffff 20030000 18fcffff 0d0a c8 0d0a`
    - `01` - uint8_t goto command id
    - `0d`, `0a` - char \r, char \n
    - `a0f6ffff 58020000 78ecffff 20030000` 5x int32_t positions
    - `0d`, `0a` - char \r, char \n
    - `c8` - uint8_t command success id
    - `0d`, `0a` - char \r, char \n

## Home
- moves all motors to 0 position
- example data `02`
    - `02` - uint8_t home command id

## Reset positions
- sets current position of the motors to 0
- example data `03`
    - `02` - uint8_t reset positions command id

## Speed
- speed multiplier can be set from 1 to 300
- by default each motor has speed multiplier 100
- example data `04 c8000000 64000000 64000000 64000000 64000000`
    - `04` - speed command id
    - `c8000000` - int32_t base motor speed (200)
    - `64000000` - int32_t shoulder motor speed (100)
    - `64000000` - int32_t elbow motor speed (100)
    - `64000000` - int32_t wrist rotate motor speed (100)
    - `64000000` - int32_t wrist motor speed (100)
- example response `04 0d0a c8000000 64000000 64000000 64000000 64000000 0d0a c8 0d0a`
    - `04` - uint8_t speed command id
    - `0d`, `0a` - char \r, char \n
    - `a0f6ffff 58020000 78ecffff 20030000` 5x int32_t speeds
    - `0d`, `0a` - char \r, char \n
    - `c8` - uint8_t command success id
    - `0d`, `0a` - char \r, char \n

## Accel
- acceleration multiplier can be set from 1 to 300
- by default each motor has acceleration multiplier 100
- example data `05 c8000000 64000000 64000000 64000000 64000000`
    - `05` - uint8_t acceleration command id
    - `c8000000` - int32_t base motor acceleration (200)
    - `64000000` - int32_t shoulder motor acceleration (100)
    - `64000000` - int32_t elbow motor acceleration (100)
    - `64000000` - int32_t wrist rotate motor acceleration (100)
    - `64000000` - int32_t wrist motor acceleration (100)
- example response `05 0d0a c8000000 64000000 64000000 64000000 64000000 0d0a c8 0d0a`
    - `05` - uint8_t acceleration command id
    - `0d`, `0a` - char \r, char \n
    - `a0f6ffff 58020000 78ecffff 20030000` 5x int32_t accelerations
    - `0d`, `0a` - char \r, char \n
    - `c8` - uint8_t command success id
    - `0d`, `0a` - char \r, char \n

## Gripper
- example data `060028`
    - `06` - uint8_t gripper command id
    - `00` - uint8_t 0 to disable gripper, 1 to enable gripper (0)
    - `28` - uint8_t range from 0 to 180 for servo position (40)
- example response `060d0a005a0d0ac80d0a`
    - `06` - uint8_t gripper command id
    - `0d`, `0a` - char \r, char \n
    - `00` - uint8_t enabled
    - `28` - uint8_t position
    - `0d`, `0a` - char \r, char \n
    - `c8` - uint8_t command success id
    - `0d`, `0a` - char \r, char \n
## Sync motors
- when sychronized, the motors arrive at the destination at the same time
- example data `0701`
    - `07` - uint8_t sync motors command id
    - `00` - uint8_t 0 to not synchronize motor movement or 1 to synchronize motor movement (0)
- example response `07 0d0a 01 0d0a c8 0d0a`
    - `07` - uint8_t sync motors command id
    - `0d`, `0a` - char \r, char \n
    - `01` - uint8_t sync motors enabled
    - `0d`, `0a` - char \r, char \n
    - `c8` - uint8_t command success id
    - `0d`, `0a` - char \r, char \n

## Status
- returns all information about current arm position and settings
- example data `08`
    - `08` uint8_t status command id
- example response
    - `ca 0d0a 00000000 00000000 00000000 00000000 00000000 40010000 c8000000 22010000 d0000000 e6000000 4a010000 54010000 be000000 4a010000 68010000 00 28 00 0d0a c8 0d0a`
    - `ca` uint8_t status command id
    - `0d`, `0a` - char \r, char \n
    - `00000000 00000000 00000000 00000000 00000000` - 5x int32 positions
    - `40010000 c8000000 22010000 d0000000 e6000000` - 5x int32 speeds
    - `40010000 c8000000 22010000 d0000000 e6000000` - 5x int32 accelerations
    - `00`, `28` - uint8_t gripper enabled, uint8_t gripper position
    - `00` - uint8_t sync motors
    - `0d`, `0a` - char \r, char \n
    - `c8` - uint8_t command success id
    - `0d`, `0a` - char \r, char \n

## Composite command (sequences)
# Begin
- `BEGIN Ix` after this command any command sent will be added to buffer to be executed when [End](#End) command is received. *x* is number of repetitions of the sequence.

# End
- `END` starts execution of commands in buffer for number of iterations set after the [Begin](#Begin) command.

# Example
- `BEGIN I5` start of sequence, number of interations is 5
- `G0 B100 S200 E300 WR400 W300` move the arm to B100 S200 E300 WR400 W300
- `G0 B0 S50 E5000 WR1500 W0` move the arm to B0 S50 E5000 WR1500 W0
- `END` executes the sequence (the two commands above) 5 times


# BigRobotArmFirmware
Firmware for BigRobotArm from instructables (https://www.instructables.com/Build-a-Giant-3D-Printed-Robot-Arm/)

## Build

This firmware is built with PlatformIO.

### Requirements

- PlatformIO Core

Preferred installation in this workspace:

```bash
uv tool install platformio
```

### Windows Git Bash

This project can be built and tested from Git Bash on Windows.

One working setup is:

```bash
uv tool install platformio
winget install --id MSYS2.MSYS2 -e --accept-package-agreements --accept-source-agreements
/c/msys64/usr/bin/bash.exe -lc "pacman -Sy --noconfirm mingw-w64-ucrt-x86_64-gcc"
```

Notes:

- If `pio` is not immediately visible after installation, restart Git Bash so the updated tool path is picked up.
- This repo has been validated with PlatformIO from Git Bash on Windows.
- Native host-side tests use the MSYS2 UCRT GCC toolchain from `C:/msys64/ucrt64/bin`.

### Linux / Raspberry Pi

Upload can be done directly with PlatformIO on Linux or Raspberry Pi by specifying the serial device.

Examples:

```bash
pio device list
pio run
pio run -t upload --upload-port /dev/ttyACM0
```

The port varies by board and USB adapter (`/dev/ttyACM0` for many ATmega2560 boards, `/dev/ttyUSB0` for common CH340 clones). Use the port reported by `pio device list`.

## PlatformIO

PlatformIO is the primary firmware workflow for this project.

The current `platformio.ini` is intentionally non-destructive:

- it keeps the existing source layout
- it adds the include paths needed by the current command files
- it builds the firmware for `megaatmega2560`
- it is intended to be the canonical build path going forward

Build with PlatformIO:

```bash
pio run
```

Run host-side tests with PlatformIO:

```bash
pio test -e native
```

Upload with PlatformIO:

```bash
pio device list
pio run -t upload --upload-port /dev/ttyACM0
```

The native test environment currently exercises pure helper code without pulling in Arduino runtime dependencies.


# Commands

## Goto
- `G0 Ba Sb Ec WRd We` where *a,b,c,d,e* are absolute steps to go to for each angle

## Home
- `G28` moves all motors in 0 position

## Reset positions
- `G92` sets current position of the motors to 0

## Speed
- `M203 Ba Sb Ec WRd We` where *a,b,c,d,e* are speed multipliers for each motor
- speed multiplier can be set from 1 to 500
- by default each motor has speed multiplier 100
- the given value is divided by 100, so that 100 multiplier is actually 1 (this is done to avoid sending floats over Serial, probably unnecessary)

## Accel
- `M201 Ba Sb Ec WRd We` where *a,b,c,d,e* are acceleration multipliers for each motor
- acceleration multiplier can be set from 1 to 500
- by default each motor has acceleration multiplier 100
- the given value is divided by 100, so that 100 multiplier is actually 1 (this is done to avoid sending floats over Serial, probably unnecessary)

## Gripper
- `G1 Ex Py` where *x* can be 0 to disable gripper, 1 to enable gripper and *y* is the servo position, clamped to the mechanical range 40 to 150

## Queue
- `Q0 Ba Sb Ec WRd We` enqueues an absolute move and responds with `BigRobotArm::QUEUED-TO` and the target position, or `BigRobotArm::QUEUE-FULL` when the queue is full
- `QFLUSH` runs all queued moves to completion and responds with `BigRobotArm::QUEUE-DRAINED`
- `QCLEAR` discards all queued moves and responds with `BigRobotArm::QUEUE-CLEARED`

## Sync motors
- `S0` or `S1` where 0 does not synchronize motor movement and 1 synchronizes motor movement
- only the exact tokens `S0` and `S1` are accepted
- when sychronized, the motors arrive at the destination at the same time

## Status
- `M503` returns all information about current arm position and settings

## Composite command (sequences)
# Begin
- `BEGIN Ix` after this command any command sent will be added to buffer to be executed when #End command is received. *x* is number of repetitions of the sequence, from 1 to 20. A second `BEGIN` before `END` is rejected.

# End
- `END` starts execution of commands in buffer for number of iterations set after the #Begin command.

# Example
- `BEGIN I5` start of sequence, number of interations is 5
- `G0 B100 S200 E300 WR400 W300` move the arm to B100 S200 E300 WR400 W300
- `G0 B0 S50 E5000 WR1500 W0` move the arm to B0 S50 E5000 WR1500 W0
- `END` executes the sequence (the two commands above) 5 times


Response:
- BigRobotArm::READY - command finished, ready for the next command
- BigRobotArm::INVALID-COMMAND - malformed or unsupported command; nothing was executed
- BigRobotArm::MOVING-TO - followed by a POSITION line for a move command
- BigRobotArm::POSITION: `Ba Sb Ec WRd We` - where *a,b,c,d,e* are the last commanded target step positions
- BigRobotArm::GRIPPER: `Ex Py`- where *x* specifies if gripper is enabled (1) or disabled (0) and *y* specifies servo position
- BigRobotArm::SPEED: `Ba Sb Ec WRd We` where *a,b,c,d,e* are speed multipliers for each motor
- BigRobotArm::ACCELERATION: `Ba Sb Ec WRd We` where *a,b,c,d,e* are acceleration multipliers for each motor
- BigRobotArm::SYNC-MOTORS: `x` where *x* spceifies if motor movement is synced (1) or not (0)
- BigRobotArm::QUEUED-TO - followed by a POSITION line for a queued move
- BigRobotArm::QUEUE-FULL - the motion queue rejected the move
- BigRobotArm::QUEUE-DRAINED - `QFLUSH` finished all queued moves
- BigRobotArm::QUEUE-CLEARED - `QCLEAR` discarded the queued moves

Commands are terminated by `\r`, `\n`, or `\r\n`. A line longer than 50 characters is rejected with `BigRobotArm::INVALID-COMMAND` instead of executing its truncated prefix.

## Legacy reference

`Example.XYZ` is an unbuilt historical Arduino sketch kept for reference only. It is not part of the PlatformIO build.

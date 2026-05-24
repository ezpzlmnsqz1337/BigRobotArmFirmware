# BigRobotArmFirmware
Firmware for BigRobotArm from instructables (https://www.instructables.com/Build-a-Giant-3D-Printed-Robot-Arm/)

## Build

This firmware is built with `arduino-cli` and a `Makefile` that prepares a temporary flat Arduino sketch layout before compiling.

Available targets:

- `make build` - prepare the temporary sketch directory and compile only
- `make upload` - prepare the temporary sketch directory, compile, and upload
- `make clean` - remove the generated sketch directory

### Requirements

- `arduino-cli`
- Arduino AVR core installed in `arduino-cli`
- Arduino libraries: `AccelStepper` and `Servo`
- `make`

Example setup with `arduino-cli`:

```bash
arduino-cli config init --overwrite
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli lib install AccelStepper
arduino-cli lib install Servo
```

### Windows Git Bash

This project can be built from Git Bash on Windows.

One working setup is:

```bash
winget install --id ArduinoSA.CLI -e
winget install --id ezwinports.make -e
```

Notes:

- After installing with `winget`, restart Git Bash so the new commands are picked up from `PATH`.
- `winget` installs `make` under its package directory and exposes it through the Windows environment, so a fresh shell is usually enough.
- `make build` works well for local validation on Windows.
- If you do upload from Windows, override the serial port as needed, for example `make upload PORT=COM3`.

### Linux / Raspberry Pi

The default upload port in the `Makefile` is `/dev/ttyUSB1`, which matches the original Raspberry Pi oriented workflow.

Examples:

```bash
make build
make upload PORT=/dev/ttyUSB1
```


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

# vsFIGHTER Firmware

Firmware for the [vsFIGHTER](https://github.com/LeafCutterLabs/vsFIGHTER) by [Leaf Cutter Labs](https://github.com/LeafCutterLabs) supporting XInput (PC, Android, Raspberry Pi, etc.), DirectInput (PC, Mac, PS3) and Nintendo Switch. Works with both the standard and Hitbox variants of the vsFIGHTER.

## Usage

![vsFIGHTER layout](.assets/images/vsFIGHTER-layout.jpg)

The vsFIGHTER uses a standard 8-button fightstick layout with 4 additional tact switches for the auxiliary buttons. Here is a button mapping table for the supported input modes:

| Generic | XInput | Switch  | PS3          | DirectInput  |
| ------  | ------ | ------- | ------------ | ------------ |
| K1      | A      | B       | Cross        | 2            |
| K2      | B      | A       | Circle       | 3            |
| P1      | X      | Y       | Square       | 1            |
| P2      | Y      | X       | Triangle     | 4            |
| P4      | LB     | L       | L1           | 5            |
| P3      | RB     | R       | R1           | 6            |
| K4      | LT     | ZL      | L2           | 7            |
| K3      | RT     | ZR      | R2           | 8            |
| Select  | Back   | Minus   | Select       | 9            |
| Start   | Start  | Plus    | Start        | 10           |
| LS      | LS     | LS      | L3           | 11           |
| RS      | RS     | RS      | R3           | 12           |

Any references to these buttons will use the `Generic` labels in this documentation.

### Home Button

There is no dedicated Home/Logo button on the vsFIGTHTER, but you can activate it via the **`SELECT + START + UP`** button combination.

> NOTE: This PS button in PS3 mode currently not supported.

### Input Modes

To change the input mode, **hold one of the following buttons as the controller is plugged in:**

* **`R3`** for Direct Input
* **`SELECT`** for Nintendo Switch
* **`START`** for XInput

Input mode is saved across power cycles.

### D-Pad Modes

You can switch between the 3 modes for the D-Pad **while the controller is in use by pressing one of the following combinations:**

* **`SELECT + START + DOWN`** - D-Pad
* **`SELECT + START + LEFT`** - Emulate Left Analog stick
* **`SELECT + START + RIGHT`** - Emulate Right Analog stick

D-Pad mode is saved across power cycles.

### SOCD Modes

Simultaneous Opposite Cardinal Direction (SOCD) cleaning will ensure the controller doesn't send invalid directional inputs to the computer/console, like Left + Right at the same time. There are 3 modes to choose from **while the controller is in use by pressing one of the following combinations:**

* **`LS + RS + UP`** - **Up Priority mode**: Up + Down = Up, Left + Right = Neutral (Hitbox behavior)
* **`LS + RS + DOWN`** - **Neutral mode**: Up + Down = Neutral, Left + Right = Neutral
* **`LS + RS + LEFT`** - **Last Input Priority (Last Win)**: Hold Up then hold Down = Down, then release and re-press Up = Up. Applies to both axes.

SOCD mode is saved across power cycles.

## Performance

Input latency is tested using the methodology outlined at [WydD's outstanding inputlag.science website](https://inputlag.science/controller/methodology), using the default 1000Hz (1ms) polling rate in the firmware.

| Poll Rate | Min | Max | Avg | Stdev | % on time | %1f skip | %2f skip |
| - | - | - | - | - | - | - | - |
| 1 ms | 0.56 ms | 1.72 ms | 1.00 ms | 0.30 ms | 95.00% | 5.00% | 0% |

This is typical performance in across all input modes and D-pad/SOCD options.

## Installation

An installation package is available in zip format and uses a small wrapper utility called [TUFUpdater](https://github.com/FeralAI/TUFUpdater) to help automate the process.

Perform the following steps to update your controller.

1. Extract the zip file to your PC.
1. Navigate into the folder, then run `UpdateVSFighter.bat`. An alternative way to launch is to drag and drop the `.hex` file onto `TUFUpdater.exe`.
1. Plug in your controller. You will be prompted to double press the reset button on your controller.
1. TUFUpdater should automatically find your controller and begin the update process.
1. After a few seconds you should see a message about flashing success.

## Building From Source

Requires the [Arduino LUFA boards package](https://github.com/CrazyRedMachine/Arduino-Lufa) and the [MPG v0.1.0](https://github.com/FeralAI/MPG/releases/tag/v0.1.0) input handling library.

1. Download and unzip the MPG library to your Arduino library folder.
1. Set up the Arduino LUFA boards package according to the [installation instructions](https://github.com/CrazyRedMachine/Arduino-Lufa#installation).
1. Open the `vsFIGHTER-Firmware.ino` file with Arduino IDE.
1. In the Arduino IDE board selection (`Tools > Board`), choose `Arduino LUFA AVR Boards > Arduino Leonardo (LUFA)`.
1. Build and upload the sketch. Double-press the reset button when you see something like this in the console output during upload:

    ```sh
    Uploading...
    PORTS {COM7, COM49, } / {COM7, COM49, } => {}
    PORTS {COM7, COM49, } / {COM7, COM49, } => {}
    ```

1. Enjoy the latest bleeding edge firmware version!

## Changelog

### v2.0.0

* Integrate [MPG v0.1.0](https://github.com/FeralAI/MPG) input handling library

### v1.0.2

* Fixed Switch LS/RS Y-axes being inverted
* Should load reasonable default if invalid values are retrieved from EEPROM

### v1.0.1

* Fixed Up Priority (Hitbox) SOCD behavior
* Fixed mode activators (`Select + Start` and `LS + RS`) not registering inputs when pressed without selecting a mode
* Slight latency reduction

### v1.0.0

Initial release supporting the following:

* XInput (PC, Android, RPi, etc.) and Nintendo Switch input modes
* D-Pad mode selection for emulation of Left or Right analog stick
* Supports 3 SOCD cleaning methods
* Options are saved to EEPROM and persist across power cycles
* 1000 Hz (1ms) polling rate on PC platforms

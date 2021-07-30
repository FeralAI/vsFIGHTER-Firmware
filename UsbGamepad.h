/************************************************************************************
 * USB Gamepad
 * 
 * Class for implementing the generic gamepad/controller I/O logic. Supports the
 * following:
 * 
 * - 1x 4-way D-Pad (HAT switch)
 * - 2x 16-bit analog joystick inputs
 * - 16x digital buttons
 * - 2x 8-bit analog triggers (on supported platforms)
 ************************************************************************************/
#ifndef _USB_GAMEPAD_
#define _USB_GAMEPAD_

#include <avr/power.h>
#include <avr/wdt.h>

#include "Descriptors.h"
#include "GamepadState.h"
#include "SwitchReport.h"
#include "XInputReport.h"

static InputMode currentInputMode;

/* Function Prototypes: */
#ifdef __cplusplus
extern "C" {
#endif
// Setup all necessary hardware, including USB initialization.
void SetupHardware(InputMode inputMode);

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void);

void sendGamepadState(GamepadState state);
SwitchInputReport toSwitchReport(GamepadState state);
XInputReport toXInputReport(GamepadState state);

// USB device event handlers.
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

#ifdef __cplusplus
}
#endif

#endif
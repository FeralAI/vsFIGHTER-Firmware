#include "UsbGamepad.h"

static USB_JoystickReport_Input_t switchReportData;
static USB_JoystickReport_XInput_t xinputReportData;

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(InputMode inputMode) {

	//set xinput_mode for descriptors.h/.c
	currentInputMode = inputMode;
	desc_set_xinput_mode(inputMode == XINPUT);
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	// We need to disable clock division before initializing the USB hardware.
	clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.
	// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {

	bool isXInput = currentInputMode == XINPUT;
	if (!isXInput) Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	else Endpoint_ConfigureEndpoint((ENDPOINT_DIR_IN | 3), EP_TYPE_INTERRUPT, 32, 1);

	Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, isXInput? JOYSTICK_EPSIZE_XINPUT: JOYSTICK_EPSIZE, 1);

}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	//No controlRequest received from the switch, so only handled in xinput mode
	if (currentInputMode == XINPUT){
		/* Handle HID Class specific requests */
		switch (USB_ControlRequest.bRequest) {
		case HID_REQ_GetReport:
			if (USB_ControlRequest.bmRequestType
					== (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) {
				Endpoint_ClearSETUP();

				/* Write the report data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&xinputReportData, 20);
				Endpoint_ClearOUT();
			}
			break;
		}
	}
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	void* Address = &xinputReportData;
	uint16_t Size = 20;

	//no OUT endpoint for xinput in this firmware
	bool isXInput = currentInputMode == XINPUT;
	if (!isXInput){
		Address = &switchReportData;
		Size = sizeof(switchReportData);

		// We'll start with the OUT endpoint.
		Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
		// We'll check to see if we received something on the OUT endpoint.
		if (Endpoint_IsOUTReceived())
		{
			// If we did, and the packet has data, we'll react to it.
			if (Endpoint_IsReadWriteAllowed())
			{
				// We'll create a place to store our data received from the host.
				USB_JoystickReport_Output_t JoystickOutputData;
				// We'll then take in that data, setting it up in our storage.
				Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL);
				// At this point, we can react to this data.
				// However, since we're not doing anything with this data, we abandon it.
			}
			// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
			Endpoint_ClearOUT();
		}
	}
	/* Select the Joystick Report Endpoint */
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);

	/* Check to see if the host is ready for another packet */
	if (Endpoint_IsINReady()) {
		/* Write Joystick Report Data */
		Endpoint_Write_Stream_LE(Address, Size, NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
		/* Clear the report data afterwards */
		memset(Address, 0, Size);
	}
}
/**
 * Retrieves the cleaned dpad state
 */
uint8_t runSOCD(bool up, bool down, bool left, bool right) {
	if (up && down)    { down = false; }
	if (left && right) { left = false; right = false; }

	return 0
		^ (up ? GAMEPAD_DPAD_UP : 0)
		^ (down ? GAMEPAD_DPAD_DOWN : 0)
		^ (left ? GAMEPAD_DPAD_LEFT : 0)
		^ (right ? GAMEPAD_DPAD_RIGHT : 0)
	;
}

void sendGamepadState(GamepadState state) {
	switch (currentInputMode) {
		case XINPUT:
			xinputReportData = toXInputReport(state);
			break;
		case SWITCH:
			switchReportData = toSwitchReport(state);
			break;
	}

	HID_Task();
	USB_USBTask();
}

inline USB_JoystickReport_Input_t toSwitchReport(GamepadState state) {
	USB_JoystickReport_Input_t report;

	// Direct assignments
	report.LX = state.lx >> 8;
	report.LY = state.ly >> 8;
	report.RX = state.rx >> 8;
	report.RY = state.ry >> 8;

	// Handle HAT switch (D-Pad)
	switch (state.dpad & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
		case GAMEPAD_DPAD_UP:                        report.HAT = SWITCH_DPAD_VALUE_UP;        break;
		case GAMEPAD_DPAD_UP | GAMEPAD_DPAD_RIGHT:   report.HAT = SWITCH_DPAD_VALUE_UPRIGHT;   break;
		case GAMEPAD_DPAD_RIGHT:                     report.HAT = SWITCH_DPAD_VALUE_RIGHT;     break;
		case GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_RIGHT: report.HAT = SWITCH_DPAD_VALUE_DOWNRIGHT; break;
		case GAMEPAD_DPAD_DOWN:                      report.HAT = SWITCH_DPAD_VALUE_DOWN;      break;
		case GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_LEFT:  report.HAT = SWITCH_DPAD_VALUE_DOWNLEFT;  break;
		case GAMEPAD_DPAD_LEFT:                      report.HAT = SWITCH_DPAD_VALUE_LEFT;      break;
		case GAMEPAD_DPAD_UP | GAMEPAD_DPAD_LEFT:    report.HAT = SWITCH_DPAD_VALUE_UPLEFT;    break;
		default:                                     report.HAT = SWITCH_DPAD_VALUE_NOTHING;   break;
	}

	// Convert button states
	report.Button = 0
		| ((state.buttons & GAMEPAD_BUTTON_03) ? 1 << 0  : 0) // Y
		| ((state.buttons & GAMEPAD_BUTTON_01) ? 1 << 1  : 0) // B
		| ((state.buttons & GAMEPAD_BUTTON_02) ? 1 << 2  : 0) // A
		| ((state.buttons & GAMEPAD_BUTTON_04) ? 1 << 3  : 0) // X
		| ((state.buttons & GAMEPAD_BUTTON_05) ? 1 << 4  : 0) // L
		| ((state.buttons & GAMEPAD_BUTTON_06) ? 1 << 5  : 0) // R
		| ((state.buttons & GAMEPAD_BUTTON_07) ? 1 << 6  : 0) // ZL
		| ((state.buttons & GAMEPAD_BUTTON_08) ? 1 << 7  : 0) // ZR
		| ((state.buttons & GAMEPAD_BUTTON_09) ? 1 << 8  : 0) // -
		| ((state.buttons & GAMEPAD_BUTTON_10) ? 1 << 9  : 0) // +
		| ((state.buttons & GAMEPAD_BUTTON_11) ? 1 << 10 : 0) // Left Stick Click
		| ((state.buttons & GAMEPAD_BUTTON_12) ? 1 << 11 : 0) // Right Stick Click
		| ((state.buttons & GAMEPAD_BUTTON_13) ? 1 << 12 : 0) // Home
		| ((state.buttons & GAMEPAD_BUTTON_14) ? 1 << 13 : 0) // Capture
	;

	return report;
}

inline USB_JoystickReport_XInput_t toXInputReport(GamepadState state) {
	USB_JoystickReport_XInput_t report;
	report.rsize = 20;

	// Direct assignments
	report.l_x = state.lx + -32768;
	report.l_y = state.ly + -32768;
	report.r_x = state.rx + -32768;
	report.r_y = state.ry + -32768;

	// Convert button states
	report.digital_buttons_1 = state.dpad
		| ((state.buttons & GAMEPAD_BUTTON_10) ? 1 << 4 : 0)          // Start/Menu
		| ((state.buttons & GAMEPAD_BUTTON_09) ? 1 << 5 : 0)          // Back/View
		| ((state.buttons & GAMEPAD_BUTTON_11) ? 1 << 6 : 0)          // Left Stick Click
		| ((state.buttons & GAMEPAD_BUTTON_12) ? 1 << 7 : 0)          // Right Stick Click
	;

	report.digital_buttons_2 = 0
		| ((state.buttons & GAMEPAD_BUTTON_05) ? 1 << 0 : 0)          // Left Bumper
		| ((state.buttons & GAMEPAD_BUTTON_06) ? 1 << 1 : 0)          // Right Bumper
		| ((state.buttons & GAMEPAD_BUTTON_13) ? 1 << 2 : 0)          // Home
		// unused
		| ((state.buttons & GAMEPAD_BUTTON_01) ? 1 << 4 : 0)          // A
		| ((state.buttons & GAMEPAD_BUTTON_02) ? 1 << 5 : 0)          // B
		| ((state.buttons & GAMEPAD_BUTTON_03) ? 1 << 6 : 0)          // X
		| ((state.buttons & GAMEPAD_BUTTON_04) ? 1 << 7 : 0)          // Y
	;

	// Handle trigger values
	if (state.hasAnalogTriggers) {
		report.lt = state.lt;                                         // Left Trigger (analog)
		report.rt = state.rt;                                         // Right Trigger (analog)
	} else {
		report.lt = (state.buttons & GAMEPAD_BUTTON_07) ? 0xFFFF : 0; // Left Trigger (digital)
		report.rt = (state.buttons & GAMEPAD_BUTTON_08) ? 0xFFFF : 0; // Right Trigger (digital)
	}

	return report;
}

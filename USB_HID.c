#include "USB_HID.h"

static SwitchInputReport switchReportData;
static XInputReport xinputReportData;

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(InputMode inputMode) {

	//set xinput_mode for descriptors.h/.c
	currentInputMode = inputMode;
	SetupDescriptor(inputMode);
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
	switch (currentInputMode) {
		case SWITCH:
			Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE_SWITCH, 1);
			Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE_SWITCH, 1);
			break;
		case XINPUT:
		default:
			Endpoint_ConfigureEndpoint((ENDPOINT_DIR_IN | 3), EP_TYPE_INTERRUPT, 32, 1);
			Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE_XINPUT, 1);
			break;
	}
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
				SwitchOutputReport JoystickOutputData;
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

void sendSwitchReport(SwitchInputReport *reportData) {
	switchReportData = *reportData;
	HID_Task();
	USB_USBTask();
}

void sendXInputReport(XInputReport *reportData) {
	xinputReportData = *reportData;
	HID_Task();
	USB_USBTask();
}

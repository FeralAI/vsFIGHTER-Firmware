#ifndef _USB_HID_
#define _USB_HID_

#include <avr/power.h>
#include <avr/wdt.h>

#include "Descriptors.h"
#include "DS3Report.h"
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

// Supported report functions
void sendDS3Report(DS3Report *reportData);
void sendSwitchReport(SwitchInputReport *reportData);
void sendXInputReport(XInputReport *reportData);

// USB device event handlers.
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

#ifdef __cplusplus
}
#endif

#endif
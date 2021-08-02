#ifndef _XINPUT_REPORT_
#define _XINPUT_REPORT_

#include <stdint.h>

// Button report 1 (8 bits)
#define XBOX_MASK_DPAD_UP      0x01
#define XBOX_MASK_DPAD_DOWN    0x02
#define XBOX_MASK_DPAD_LEFT    0x04
#define XBOX_MASK_DPAD_RIGHT   0x08
#define XBOX_MASK_START        0x10
#define XBOX_MASK_BACK         0x20
#define XBOX_MASK_LEFT_STICK   0x40
#define XBOX_MASK_RIGHT_STICK  0x80

// Button report 2 (8 bits)
#define XBOX_MASK_LB           0x01
#define XBOX_MASK_RB           0x02
#define XBOX_MASK_HOME         0x04
//#define UNUSED               0x08
#define XBOX_MASK_A            0x10
#define XBOX_MASK_B            0x20
#define XBOX_MASK_X            0x40
#define XBOX_MASK_Y            0x80

typedef struct {
  uint8_t rid;
  uint8_t rsize;
  uint8_t digital_buttons_1;
  uint8_t digital_buttons_2;
  uint8_t lt;
  uint8_t rt;
  int l_x;
  int l_y;
  int r_x;
  int r_y;
  uint8_t reserved_1[6];
} XInputReport;

__attribute__((always_inline)) static inline bool checkXInputReportChanged(XInputReport *lastReport, XInputReport *nextReport) {
	return &lastReport->digital_buttons_1 != &nextReport->digital_buttons_1
	    || &lastReport->digital_buttons_2 != &nextReport->digital_buttons_2
	    || &lastReport->l_x != &nextReport->l_x
	    || &lastReport->l_y != &nextReport->l_y
	    || &lastReport->r_x != &nextReport->r_x
	    || &lastReport->r_y != &nextReport->r_y
	    || &lastReport->lt != &nextReport->lt
	    || &lastReport->rt != &nextReport->rt;
}

#endif
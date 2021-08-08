#ifndef _XINPUT_REPORT_
#define _XINPUT_REPORT_

#include <stdint.h>

// Button report 1 (8 bits)
#define XBOX_MASK_DPAD_UP     (1U << 0)
#define XBOX_MASK_DPAD_DOWN   (1U << 1)
#define XBOX_MASK_DPAD_LEFT   (1U << 2)
#define XBOX_MASK_DPAD_RIGHT  (1U << 3)
#define XBOX_MASK_START       (1U << 4)
#define XBOX_MASK_BACK        (1U << 5)
#define XBOX_MASK_LEFT_STICK  (1U << 6)
#define XBOX_MASK_RIGHT_STICK (1U << 7)

// Button report 2 (8 bits)
#define XBOX_MASK_LB          (1U << 0)
#define XBOX_MASK_RB          (1U << 1)
#define XBOX_MASK_HOME        (1U << 2)
//#define UNUSED              (1U << 3)
#define XBOX_MASK_A           (1U << 4)
#define XBOX_MASK_B           (1U << 5)
#define XBOX_MASK_X           (1U << 6)
#define XBOX_MASK_Y           (1U << 7)

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
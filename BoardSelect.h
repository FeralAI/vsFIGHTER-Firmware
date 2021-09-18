#ifndef BOARD_SELECT_H_
#define BOARD_SELECT_H_

#ifdef REV_V3B
#include "Boards/VsFighter_v3b.h"
#elif REV_V2B
#include "Boards/VsFighter_v2b.h"
#else
#include "Boards/VsFighter.h"
#endif

#endif

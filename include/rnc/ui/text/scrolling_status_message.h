#ifndef LOMBYTE_RNC_UI_TEXT_SCROLLING_STATUS_MESSAGE_H
#define LOMBYTE_RNC_UI_TEXT_SCROLLING_STATUS_MESSAGE_H

#include "types.h"

/* Help message ids the scrolling status line picks from at random when the current
   message has scrolled out (update_scrolling_status_message: random_integer_below(0x18)). */
extern s32 scrolling_status_message_ids[24] __asm__("D_001E5FB8");

#endif /* LOMBYTE_RNC_UI_TEXT_SCROLLING_STATUS_MESSAGE_H */

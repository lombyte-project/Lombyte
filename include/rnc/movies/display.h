#ifndef RCP2P_RNC_MOVIES_DISPLAY_H
#define RCP2P_RNC_MOVIES_DISPLAY_H

#include "types.h"

void end_display(void) __asm__("ClearStageStateFlag");
s32  handler_end_image(void) __asm__("FUN_0023b540");

#endif /* RCP2P_RNC_MOVIES_DISPLAY_H */

#ifndef RCP2P_RNC_OVERLAY_SHARED_RENDERING_FRAME_H
#define RCP2P_RNC_OVERLAY_SHARED_RENDERING_FRAME_H

#include "types.h"

void frame_roll_counter(void) __asm__("FUN_L00_001f6d70");
void queue_framebuffer_upload(unsigned int) __asm__("FUN_L00_001fc558");

#endif /* RCP2P_RNC_OVERLAY_SHARED_RENDERING_FRAME_H */

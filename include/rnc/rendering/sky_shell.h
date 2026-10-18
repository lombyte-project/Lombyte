#ifndef LOMBYTE_RNC_RENDERING_SKY_SHELL_H
#define LOMBYTE_RNC_RENDERING_SKY_SHELL_H

#include "types.h"

/* Draw one shell from the current level sky, selecting its shading mode. */
void sky_draw_shell(s32 shell_index) __asm__("FUN_0022b690");

/* Quadword scaled by update_level_gameplay_frame and copied into each sky shell transform. */
/* Keep the existing byte address label: qcopy accesses 16 bytes from it. */
extern u8 D_00160460;

#endif

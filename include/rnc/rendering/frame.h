#ifndef LOMBYTE_RNC_RENDERING_FRAME_H
#define LOMBYTE_RNC_RENDERING_FRAME_H

#include "types.h"

void copy_render_buffer_pair(void) __asm__("FUN_00235840");
void reset_draw_config(void);
void reset_gs_registers_pr(void) __asm__("FUN_001f3958");
void set_background_color(s32, u64, u64);

#endif /* LOMBYTE_RNC_RENDERING_FRAME_H */

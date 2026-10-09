#ifndef LOMBYTE_RNC_RENDERING_DRAW_ENVIRONMENT_H
#define LOMBYTE_RNC_RENDERING_DRAW_ENVIRONMENT_H

#include "types.h"

/* GS drawing environment as A+D register/data pairs, filled by set_pal_mode. */
typedef struct GraphicsDrawEnvironment {
    u64 pad0[2];
    u64 frame1;
    u64 pad18;
    u64 frame2;
    u64 pad28;
    u64 zbuf1;
    u64 pad38;
    u64 zbuf2;
    u64 pad48;
    u64 xyoffset1;
    u64 pad58;
    u64 xyoffset2;
    u64 pad68;
    u64 scissor1;
    u64 pad78;
    u64 scissor2;
} GraphicsDrawEnvironment;

extern GraphicsDrawEnvironment draw_environment __asm__("D_0013CF10");
extern u64 depth_buffer_register __asm__("D_0013D100");
extern u64 masked_depth_buffer_register __asm__("D_0013D170");

#endif /* LOMBYTE_RNC_RENDERING_DRAW_ENVIRONMENT_H */

#ifndef LOMBYTE_RNC_RENDERING_GRAPHICS_BUFFER_H
#define LOMBYTE_RNC_RENDERING_GRAPHICS_BUFFER_H

#include "types.h"

struct GraphicsBufferDescriptor {
    s32 address;
    s32 flags;
};

extern struct GraphicsBufferDescriptor graphics_buffer_descriptors[5] __asm__("D_001D60B8");

#endif /* LOMBYTE_RNC_RENDERING_GRAPHICS_BUFFER_H */

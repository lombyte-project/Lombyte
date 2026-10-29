#include "types.h"
#include "rnc/rendering/render_globals.h"

extern u32 gs_texture_allocation_cursor __asm__("D_0015EE74");
extern u32 texture_alloc_counter __asm__("D_0015EF20");

void InitializeStreamingState(void) {
    gs_texture_allocation_cursor = gs_texture_allocation_base;
    texture_alloc_counter = 0;
}

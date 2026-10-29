#include "types.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/runtime/dma/initialize_streaming_state_globals.h"

extern u32 gs_texture_allocation_cursor __asm__("D_0015EE74");

void InitializeStreamingState(void) {
    gs_texture_allocation_cursor = gs_texture_allocation_base;
    texture_alloc_counter = 0;
}

u32 texture_alloc_counter = 0;

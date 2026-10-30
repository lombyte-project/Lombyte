#include "types.h"
#include "rnc/globals.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/texture/reset_gs_texture_allocation_globals.h"

void reset_gs_texture_allocation(void) {
    gs_texture_allocation_cursor = gs_texture_allocation_base;
    texture_alloc_counter = 0;
}

u32 texture_alloc_counter = 0;

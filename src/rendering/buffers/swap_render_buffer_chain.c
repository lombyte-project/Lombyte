#include "types.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/buffers/swap_render_buffer_chain_globals.h"
void swap_render_buffer_chain(void) __asm__("FUN_00233630");

void swap_render_buffer_chain(void) {
    s32 next_buffer;
    s32 buffer_limit;
    s32 next_index;

    next_index = 1 - D_00160F10;
    next_buffer = D_00160EF8[next_index];
    previous_render_packet_cursor = render_packet_cursor.addr;
    D_00160F10 = next_index;
    buffer_limit = (next_buffer + current_vu_chain) - D_0015F5B8;
    render_packet_cursor.addr = next_buffer;
    D_0015F638 = buffer_limit;
    D_0015F63C = buffer_limit - 0x2000;
}

extern __typeof__(swap_render_buffer_chain) func_00233630 __attribute__((alias("FUN_00233630")));

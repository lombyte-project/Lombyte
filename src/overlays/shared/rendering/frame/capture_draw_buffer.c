/* Frame: captures the draw buffer. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/overlay/shared/rendering/buffers/vu1_send_chain_render_buffer.h"
#include "rnc/rendering/vu1/vu1_init_chain.h"
#include "rnc/overlay/shared/rendering/buffers/transfer_draw_buffer_chunks.h"

/* Ported from rac1-decomp (src/overlays/shared/draw_001F3A78.c: func_L00_001F9D40), where it is exact; names translated to the US level program. */

extern int D_0013E05A[];
extern s32 sceGsSyncV(s32);

/* Calls 002a09d8(1), waits for vsync, counts a frame, starts the VU1 chain, calls 002a0f18. */
void capture_draw_buffer(void *destination) __asm__("FUN_L00_001f99c0");

void capture_draw_buffer(void *destination) {
    vu1_sync_chain(1);
    sceGsSyncV(0);
    D_L00_0015F3F8 = D_L00_0015F3F8 + 1;
    vu1_init_chain();
    transfer_draw_buffer_chunks(
        destination, draw_buffer_address, *(int *)((char *)D_0013E05A + 0x4AA) << 11);
}

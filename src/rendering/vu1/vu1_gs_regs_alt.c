#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
extern u8 D_001DE3F0[];

void vu1_gs_regs_alt(void) __asm__("FUN_00233c28");

void vu1_gs_regs_alt(void) {
    *render_packet_cursor.words = 0x30000003;
    *(s32 *)((u32)render_packet_cursor.words + 4) = (s32)D_001DE3F0;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0x50000003;
    render_packet_cursor.words += 4;
}
extern __typeof__(vu1_gs_regs_alt) func_00233C28 __attribute__((alias("FUN_00233c28")));
/* Recovered original symbol name. */
extern __typeof__(vu1_gs_regs_alt) VU1_gsRegsAlt__Fv __attribute__((alias("FUN_00233c28")));

#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/draw_environment.h"

void vu1_gs_regs_font(void) __asm__("FUN_00233c90");

void vu1_gs_regs_font(void) {
    *render_packet_cursor.words = 0x3000000B;
    *(s32 *)((u32)render_packet_cursor.words + 4) = (s32)&draw_environment;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0x5000000B;
    render_packet_cursor.words += 4;
}
extern __typeof__(vu1_gs_regs_font) func_00233C90 __attribute__((alias("FUN_00233c90")));
/* Recovered original symbol name. */
extern __typeof__(vu1_gs_regs_font) VU1_gsRegsFont__Fv __attribute__((alias("FUN_00233c90")));

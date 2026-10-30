#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

void vu1_init_chain(void) __asm__("FUN_002335d0");

extern s32 D_00160F10_macro __asm__("D_00160F10") MACRO_ADDR;
extern s32 D_0015F63C_macro __asm__("D_0015F63C") MACRO_ADDR;
extern s32 D_0015F638_gp __asm__("D_0015F638") __attribute__((sda));

void vu1_init_chain(void) {
    s32 start;
    s32 end;
    s32 limit;

    start = D_001940C0[1];
    end = D_001940C0[2];
    limit = start + current_vu_chain - D_0015F5B8;
    D_00160EF8[1] = end;
    D_00160F10_macro = 0;
    render_packet_cursor.addr = start;
    D_00160EF8[0] = start;
    D_0015F638_gp = limit;
    D_0015F63C_macro = limit - 0x2000;
}

extern __typeof__(vu1_init_chain) func_002335D0 __attribute__((alias("FUN_002335d0")));

#include "types.h"
#include "sda.h"

extern s32 *D_00160F00[4] MACRO_ADDR; /* packet cursor, reloaded per store */
extern s32 *D_00160F00_store;
#include "rnc/rendering/screen.h"

#define BASE D_00160F00[0]

void vu1_set_scissor(s32 x0, s32 x1, s32 y0, s32 y1) __asm__("FUN_00233a40");

/* Clamps the rectangle to the screen and queues it as GS register 0x40
   (SCISSOR_1: x0 | x1 << 16 | y0 << 32 | y1 << 48) in the packet
   vu1_add_g_sregister builds. The last field is the one unsigned term:
   it keeps fold from rebalancing the OR chain, as retail has it. */
void vu1_set_scissor(s32 x0, s32 x1, s32 y0, s32 y1) {
    x0 = x0 < 0 ? 0 : x0;
    x1 = x1 > screen_extent.width - 1 ? screen_extent.width - 1 : x1;
    y0 = y0 < 0 ? 0 : y0;
    y1 = y1 > screen_extent.height - 1 ? screen_extent.height - 1 : y1;
    BASE[0] = 0x10000002;
    BASE[1] = 0;
    BASE[2] = 0;
    BASE[3] = 0x50000002;
    BASE[4] = 0x8001;
    BASE[5] = 0x10000000;
    BASE[6] = 14;
    BASE[7] = 0;
    *(volatile long *)(BASE + 8) =
        (long)x0 | ((long)x1 << 16) | ((long)y0 << 32) | ((unsigned long)y1 << 48);
    BASE[10] = 0x40;
    BASE[11] = 0;
    D_00160F00_store = BASE + 12;
}

extern __typeof__(vu1_set_scissor) func_00233A40 __attribute__((alias("FUN_00233a40")));

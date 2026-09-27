#include "types.h"

extern int D_001940C0[];
extern s32 D_00160F0C;
extern u8 D_24135F[];

void init_mem_slots(void) __asm__("FUN_002015d8");

void init_mem_slots(void) {
    int base = (int)D_24135F & 0xFFFFC000;
    int size = D_00160F0C;
    int a = base + size;
    int b = a + size;
    int c = b + 0x64000;
    int d = c + 0x30000;
    D_001940C0[0] = base;
    D_001940C0[5] = d;
    D_001940C0[6] = d;
    D_001940C0[8] = 0x7000000;
    D_001940C0[9] = 0x7100000;
    D_001940C0[1] = base;
    D_001940C0[2] = a;
    D_001940C0[3] = b;
    D_001940C0[4] = c;
    D_001940C0[10] = 0x7200000;
}

extern __typeof__(init_mem_slots) func_002015D8 __attribute__((alias("FUN_002015d8")));

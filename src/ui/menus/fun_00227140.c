#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct Mat { u8 pad[0x30]; };
extern struct TagPtr D_00160F00;
extern struct Mat D_001D7D90[];
extern s32 D_00160360[2] __attribute__((sda));
extern void FUN_00228598(s32, s32);

void FUN_00227140(s32 arg0, s32 slot, s32 mat) {
    D_00160F00.p->w0 = 0x30000003;
    D_00160F00.p->addr = (u32)&D_001D7D90[mat];
    D_00160F00.p->w2 = 0x13000000;
    D_00160F00.p->w3 = 0x50000003;
    D_00160F00.p++;
    FUN_00228598(arg0, D_00160360[slot]);
}

extern __typeof__(FUN_00227140) func_00227140 __attribute__((alias("FUN_00227140")));

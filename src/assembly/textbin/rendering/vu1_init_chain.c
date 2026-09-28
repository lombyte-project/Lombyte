#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/vu1_init_chain/FUN_002335d0.s", FUN_002335d0);
#else
#include "rnc/rendering_vu1_init_chain_types.h"
#include "types.h"




extern s32 D_0015F5B8;
extern s32 D_0015F638;
extern s32 D_0015F63C;
extern struct M2c_D_00160EF8 D_00160EF8;
extern s32 D_00160F00[];
extern s32 D_00160F0C;
extern s32 D_00160F10;
extern struct M2c_D_001940C0 D_001940C0;
void vu1_init_chain(void) __asm__("FUN_002335d0");

void vu1_init_chain(void) {
    s32 temp_3_17;

    D_00160EF8.unk0 = (s32) D_001940C0.unk4;
    temp_3_17 = (D_001940C0.unk4 + D_00160F0C) - D_0015F5B8;
    D_00160EF8.unk4 = (s32) D_001940C0.unk8;
    D_00160F10 = 0;
    D_0015F63C = temp_3_17 - 0x2000;
    D_00160F00[0] = D_001940C0.unk4;
    D_0015F638 = temp_3_17;
}
#endif /* NON_MATCHING */

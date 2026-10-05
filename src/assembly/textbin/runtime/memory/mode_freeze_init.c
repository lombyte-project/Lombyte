#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/runtime/memory/mode_freeze_init/FUN_001fbab8.s", FUN_001fbab8);
#else
#include "types.h"

struct St {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

extern s32 D_0015F604;
extern struct St D_00193300;
extern s32 func_0012E3E8();
extern s32 InitializeRenderState();
extern s32 func_001F96F8(s32);
extern s32 func_001FDD10();
extern s32 func_001FED30();

void mode_freeze_init(u32 arg0, s32 arg1) __asm__("FUN_001fbab8");

void mode_freeze_init(u32 arg0, s32 arg1) {
    struct St *st;

    if (D_0015F604 != 3) {
        func_0012E3E8(0x1D);
        InitializeRenderState(0);
    }
    D_00193300.unk14 = D_0015F604;
    D_00193300.unk18 = arg1;
    D_0015F604 = 4;
    D_00193300.unk0 = arg0;
    switch (arg0) {
    case 0:
        st = &D_00193300;
        st->unk8 = (s32)func_001FDD10(0x4F6E);
        st->unkC = (s32)func_001FDD10(0x5248);
        st->unk10 = (s32)func_001FDD10(0x5249);
        st->unk4 = 0;
        st->unk1C = 0;
        st->unk20 = 0;
        st->unk24 = 0;
        st->unk28 = 0;
        break;
    case 2:
        D_00193300.unk8 = (s32)func_001FDD10(0x524A);
        D_00193300.unkC = 0;
        D_00193300.unk4 = 0;
        break;
    case 1:
    case 4:
        st = &D_00193300;
        st->unk8 = (s32)func_001FDD10(0x5229);
        st->unkC = (s32)func_001FDD10(0x4EE0);
        st->unk10 = (s32)func_001FDD10(0x524A);
        st->unk4 = 0;
        break;
    case 5:
        func_001FED30(0x4E2B);
        st = &D_00193300;
        st->unk4 = func_001F96F8(0x1E);
        st->unk20 = 0;
        st->unk24 = func_001F96F8(0x1E);
        break;
    case 3:
        st = &D_00193300;
        st->unk4 = func_001F96F8(0x1E);
        st->unk20 = 0;
        st->unk24 = func_001F96F8(0x1E);
        break;
    case 6:
        D_00193300.unk4 = func_001F96F8(0x1E);
        D_00193300.unk1C = 0;
        break;
    default:
        D_00193300.unk8 = 0;
        D_00193300.unkC = 0;
        D_00193300.unk10 = 0;
        D_00193300.unk4 = 0x78;
        break;
    }
}
#endif /* NON_MATCHING */

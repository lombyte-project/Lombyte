#include "types.h"
struct TtyState {
    s32 unk0;
    volatile s32 unk4;
    volatile s32 unk8;
    volatile s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
};
struct Mmio {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u32 unk8;
};
extern struct TtyState D_00154A50;
extern u8 D_00154A80[];
extern u8 D_00154BC0[];
extern s32 FlushCache();
extern s32 func_00119568();
extern s32 sceDeci2Open();
extern void sceTtyHandler();
s32 sceTtyInit(void) {
    struct TtyState *state = &D_00154A50;
    struct Mmio *hdr;

    FlushCache(0);
    *(volatile s32 *)&state->unk0 = sceDeci2Open(0x210, state, &sceTtyHandler);
    if (state->unk0 < 0) {
        return 0;
    }
    state->unkC = 0;
    state->unk4 = 0;
    state->unk8 = 0;
    state->unk14 = (u32)D_00154BC0 | 0x20000000;
    hdr = (struct Mmio *)((u32)D_00154A80 | 0x20000000);
    state->unk10 = (s32)hdr;
    hdr->unk2 = 0;
    hdr->unk4 = 0x210;
    hdr->unk6 = 'E';
    hdr->unk7 = 'H';
    hdr->unk8 = 0;
    state->unk18 = func_00119568(0x100);
    return 1;
}

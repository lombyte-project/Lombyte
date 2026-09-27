#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_transition/FUN_00215e00.s", FUN_00215e00);
#else
#include "types.h"
#include "rnc/d_001516d0.h"
struct MusicTable { u8 pad0[0x2AA8]; s32 tracks[1]; };
extern struct MusicTable D_00137B80;
extern struct M2c_D_001516D0 D_001516D0;
extern void FUN_00216a80();
extern void func_0012EC08(s32, s32, s32, s32, s16, s32, s32, s32, s32, void (*)(), u64);
s32 music_transition(s32 arg0, s32 track, s32 arg2, s32 arg3) __asm__("FUN_00215e00");
s32 music_transition(s32 arg0, s32 track, s32 arg2, s32 arg3) {
    volatile s32 *entry;
    s64 v;
    u8 *tbl;
    s32 off;

    if (D_001516D0.unk6C != 0) {
        return 0;
    }
    tbl = (u8 *)&D_00137B80;
    off = 0x2AA8;
    entry = (s32 *)(tbl + off) + track;
    if (*entry == 0) {
        return 0;
    }
    *(u32 *)&D_001516D0.unk6C = 0xFFFFFFFF;
    D_001516D0.unk84 = 48000;
    D_001516D0.unk80 = 10;
    D_001516D0.unk70 = arg0;
    D_001516D0.unk76 = 1;
    D_001516D0.unk72 = arg3;
    D_001516D0.unk7C = 1;
    D_001516D0.unk74 = arg2;
    v = *entry;
    func_0012EC08(v, 0, 0, 0, arg3, 0, 1, 0, 0x20, FUN_00216a80, (u32)&D_001516D0.unk6C);
    return 1;
}
#endif /* NON_MATCHING */

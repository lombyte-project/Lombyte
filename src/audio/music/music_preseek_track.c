#include "types.h"
#include "rnc/d_001516d0.h"

struct MusicTable { u8 pad0[0x2AA8]; s32 tracks[1]; };

extern struct MusicTable D_00137B80;
extern struct M2c_D_001516D0 D_001516D0;
extern void func_00216A20();
extern void func_0012EC08(s32, s32, s32, s32, s16, s32, s32, s32, s32, void (*)(), u64);

void music_preseek_track(s32 track, s32 arg1, s32 arg2) __asm__("FUN_00215b68");

void music_preseek_track(s32 track, s32 arg1, s32 arg2) {
    volatile s32 *entry;
    s64 v;
    u8 *tbl;
    s32 off;

    if (D_001516D0.unk34 == 0) {
        tbl = (u8 *)&D_00137B80;
        off = 0x2AA8;
        entry = (s32 *)(tbl + off) + track;
        if (*entry != 0) {
            *(u32 *)&D_001516D0.unk34 = 0xFFFFFFFF;
            D_001516D0.unk3E = 1;
            D_001516D0.unk38 = track;
            D_001516D0.unk3C = arg1;
            D_001516D0.unk48 = 10;
            D_001516D0.unk4C = 0xBB80;
            D_001516D0.unk3A = arg2;
            D_001516D0.unk44 = 0;
            v = *entry;
            func_0012EC08(v, 0, 0, 0, arg2, 0, 1, 0, 0x21, func_00216A20, (u64) ((s64) (((u8 *)&D_001516D0 + 0x34)) << 0x20) >> 0x20);
        }
    }
}

extern __typeof__(music_preseek_track) func_00215B68 __attribute__((alias("FUN_00215b68")));

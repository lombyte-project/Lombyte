#include "types.h"
#include "rnc/d_001516d0.h"

struct Tracks { u8 pad[0xF00]; s32 h[1][6]; };
extern struct Tracks D_00137B80;
extern s32 D_0015ED88;
extern struct M2c_D_001516D0 D_001516D0;
extern u8 D_002169C0[];
extern s32 func_0012EC08();

void music_start_track_50000(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_00215518");

void music_start_track_50000(s32 arg0, s32 arg1, s32 arg2) {
    s32 handle;

    handle = D_00137B80.h[arg0 - 50000][D_0015ED88];
    if (handle != 0) {
        if (D_001516D0.unk50 == 0) {
            *(u32 *)&D_001516D0.unk50 = 0xFFFFFFFF;
            D_001516D0.unk5A = 1;
            D_001516D0.unk54 = arg0;
            D_001516D0.unk58 = arg1;
            D_001516D0.unk64 = 10;
            D_001516D0.unk68 = 48000;
            D_001516D0.unk56 = arg2;
            D_001516D0.unk60 = 0;
            func_0012EC08(handle, 0, 0, 0, (s16)arg2, 0, 2, 0, 0x21,
                          D_002169C0,
                          (u64)((s64)(((u8 *)&D_001516D0 + 0x50)) << 0x20) >> 0x20);
        }
    }
}

extern __typeof__(music_start_track_50000) func_00215518 __attribute__((alias("FUN_00215518")));

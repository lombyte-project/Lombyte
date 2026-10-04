#include "types.h"
#include "rnc/music_stream_state.h"

struct Tracks { u8 pad[0x13C0]; s32 h[1][2]; };
extern struct Tracks D_00137B80;
extern struct MusicStreamState D_001516D0;
// D_002169C0 is a code address retail passes as a pointer, not a data symbol;
// config/us/undefined_syms.txt binds its absolute value so this extern links.
extern u8 D_002169C0[];
extern s32 func_0012EC08();

void music_start_track_40000(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_00215600");

void music_start_track_40000(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_4_14;

    temp_4_14 = D_00137B80.h[arg0 - 40000][0];
    if (temp_4_14 != 0) {
        if (D_001516D0.secondary_handle == 0) {
            *(u32 *)&D_001516D0.secondary_handle = 0xFFFFFFFF;
            D_001516D0.secondary_state = 1;
            D_001516D0.secondary_track = arg0;
            D_001516D0.secondary_flags = arg1;
            D_001516D0.secondary_poll_interval = 10;
            D_001516D0.secondary_remaining_time = 48000;
            D_001516D0.secondary_volume = arg2;
            D_001516D0.secondary_crossfade_enabled = 0;
            func_0012EC08(temp_4_14, 0, 0, 0, (s16) arg2, 0, 2, 0, 0x21, D_002169C0, (u64) ((s64) (((u8 *)&D_001516D0 + 0x50)) << 0x20) >> 0x20);
        }
    }
}

extern __typeof__(music_start_track_40000) func_00215600 __attribute__((alias("FUN_00215600")));

#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f96f8/FUN_001f96f8.s", FUN_001f96f8);
#else
#include "types.h"
extern f32 game_time_scale __asm__("D_0015ED68") __attribute__((sda));
s32 scale_game_frames(s32 frames) __asm__("FUN_001f96f8");
s32 scale_game_frames(s32 frames) {
    f32 scaled_frames = 0.25f;
    scaled_frames += 0.25f;
    scaled_frames = scaled_frames + (f32)frames * game_time_scale;
    return (s32)scaled_frames;
}

extern __typeof__(scale_game_frames) func_001F96F8 __attribute__((alias("FUN_001f96f8")));

#endif /* NON_MATCHING */

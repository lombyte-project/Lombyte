#include "types.h"
struct MusicChannelHandle { u32 handle; u8 pad4[6]; s16 state; };
struct MusicTrackParameters { s16 track; s16 volume; s16 flags; };
struct MusicStartParameters { u8 pad0[0x38]; struct MusicTrackParameters primary; u8 pad3e[0x16]; struct MusicTrackParameters secondary; };
extern struct MusicStartParameters D_001516D0;
extern void func_00215B68(s32, s32, s32);
void music_primary_preseek_callback(u32 handle, s64 context) __asm__("FUN_00216a20");

void music_primary_preseek_callback(u32 handle, s64 context) {
    struct MusicChannelHandle *channel = (struct MusicChannelHandle *)(s32)context;

    if (channel != 0) {
        channel->handle = handle;
        if (handle != 0) {
            if (channel->state == 1) {
                channel->state = 2;
            }
        } else {
            func_00215B68(D_001516D0.primary.track, D_001516D0.primary.flags, D_001516D0.primary.volume);
        }
    }
}

extern __typeof__(music_primary_preseek_callback) func_00216A20 __attribute__((alias("FUN_00216a20")));

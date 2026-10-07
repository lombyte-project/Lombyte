#include "types.h"
struct SndCdSyncState {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
};
extern volatile struct SndCdSyncState D_00137B00;
extern s32 D_0015EC8C __attribute__((sda));
extern s32 D_0015EC98 __attribute__((sda));
extern void FlushCache(s32);
extern void snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern s32 sceCdSync(s32);

s32 snd_stream_safe_cd_sync(s32 mode) __asm__("FUN_0012ee08");

s32 snd_stream_safe_cd_sync(s32 mode) {
    s32 ready;
    s32 next_ready;
    if (D_0015EC8C == 0) {
        return sceCdSync(mode);
    }
    FlushCache(0);
    ready = D_00137B00.unk0 == 0;
    D_0015EC98 = ready;
    if (ready != 1) {
        if (mode == 1) {
            return 1;
        }
        if (ready == 0) {
            do {
                snd_flush_sound_commands();
                FlushCache(0);
                next_ready = D_00137B00.unk0 == 0;
                D_0015EC98 = next_ready;
            } while (next_ready == 0);
        }
        return 0;
    }
    return 0;
}

extern s32 func_0012EE08(s32 mode) __attribute__((alias("FUN_0012ee08")));

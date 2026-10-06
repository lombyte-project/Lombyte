#include "types.h"
struct MusicStreamState {
    u8 pad_0[0x50];
    s32 unk50;
    u8 pad_54[0x6];
    s16 unk5A;
};

extern struct MusicStreamState D_001516D0 __attribute__((section(".data")));
extern s32 snd_continue_vag_stream() __asm__("func_0012ECA0");
s32 continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");

s32 continue_audio_stream_if_ready(void) {
    if (D_001516D0.unk50 != 0) {
        if (D_001516D0.unk5A == 3) {
            snd_continue_vag_stream(D_001516D0.unk50);
            D_001516D0.unk5A = 4;
            return 1;
        }
        return 0;
    }
    return 0;
}

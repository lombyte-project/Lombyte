#include "types.h"
#include "eetypes.h"
#include "qzero.h"

typedef struct {
    s32 x0;
    u8 x4;
    u8 pad5[0x6B];
} SndVoice;

typedef struct {
    u128 q[4];
    s32 x40;
    u8 pad44[0x2C];
    SndVoice voices[30];
} SndState;

extern SndState D_0013E550;
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_stop_all_sounds() __asm__("func_0012E3B8");
extern s32 snd_reset_state_and_flush_commands() __asm__("func_0012EB00");

void sound_stop_all_sounds(void) __asm__("FUN_0022dcd0");
void sound_stop_all_sounds(void) {
    s32 i;
    SndState *sound_state;
    u128 *q;
    s32 voice_clear_cursor;
    s32 end;

    snd_reset_state_and_flush_commands();
    snd_flush_sound_commands();
    snd_stop_all_sounds();
    while (snd_flush_sound_commands() != 0) {
    }
    for (i = 0; i < 4; i++) {
        qzero(&D_0013E550.q[i]);
    }
    sound_state = &D_0013E550;
    sound_state->x40 = 0;
    voice_clear_cursor = (s32)sound_state;
    end = voice_clear_cursor + 0xD20;
    do {
        ((SndVoice *)voice_clear_cursor)[1].x0 = 0;
        ((SndVoice *)voice_clear_cursor)[1].x4 = 0;
        voice_clear_cursor += 0x70;
    } while (voice_clear_cursor < end);
}

extern __typeof__(sound_stop_all_sounds) func_0022DCD0 __attribute__((alias("FUN_0022dcd0")));

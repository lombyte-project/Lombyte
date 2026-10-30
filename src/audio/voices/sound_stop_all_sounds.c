#include "types.h"
#include "eetypes.h"
#include "qzero.h"
#include "rnc/audio/voice_pool.h"

extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_stop_all_sounds() __asm__("func_0012E3B8");
extern s32 snd_reset_state_and_flush_commands() __asm__("func_0012EB00");

void sound_stop_all_sounds(void) __asm__("FUN_0022dcd0");

void sound_stop_all_sounds(void) {
    s32        i;
    VoicePool *sound_state;
    u128      *q;
    s32        voice_clear_cursor;
    s32        end;

    snd_reset_state_and_flush_commands();
    snd_flush_sound_commands();
    snd_stop_all_sounds();
    while (snd_flush_sound_commands() != 0) {
    }
    for (i = 0; i < 4; i++) {
        qzero(&voice_pool.listener_history[i].q);
    }
    sound_state = &voice_pool;
    sound_state->listener_history_position = 0;
    voice_clear_cursor = (s32)sound_state;
    end = voice_clear_cursor + 0xD20;
    do {
        ((VoiceSlot *)voice_clear_cursor)[1].handle = 0;
        ((VoiceSlot *)voice_clear_cursor)[1].state = 0;
        voice_clear_cursor += 0x70;
    } while (voice_clear_cursor < end);
}

extern __typeof__(sound_stop_all_sounds) func_0022DCD0 __attribute__((alias("FUN_0022dcd0")));

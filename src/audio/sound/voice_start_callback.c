/* Ported from rac1-decomp (src/game/sound.c, func_0022F0A8). */
#include "rnc/audio/voice_pool.h"

void voice_start_callback(int handle, long context) __asm__("FUN_0022dd90");

void voice_start_callback(int handle, long voice_record_address) {
    VoiceSlot *voice = (VoiceSlot *)(int)voice_record_address;
    if (voice != 0) {
        voice->handle = handle;
        if (handle != 0) {
            if (voice->state == 1) {
                voice->state = 2;
            }
        } else {
            voice->owner = 0;
            voice->owner_context = 0;
            voice->state = VOICE_SLOT_FREE;
        }
    }
}

extern __typeof__(voice_start_callback) func_0022DD90 __attribute__((alias("FUN_0022dd90")));

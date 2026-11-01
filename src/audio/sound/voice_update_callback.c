/* Ported from rac1-decomp (src/game/sound.c, func_0022F0F0). */
#include "rnc/audio/voice_pool.h"

void voice_update_callback(int handle, long context) __asm__("FUN_0022ddd8");

void voice_update_callback(int handle, long voice_state_address) {
    VoiceSlot *voice = (VoiceSlot *)(int)voice_state_address;
    if (voice != 0) {
        voice->handle = handle;
        if (handle == 0) {
            voice->owner = 0;
            voice->owner_context = 0;
            voice->state = VOICE_SLOT_FREE;
        }
    }
}

extern __typeof__(voice_update_callback) func_0022DDD8 __attribute__((alias("FUN_0022ddd8")));

#include "types.h"
#include "rnc/audio/voice_pool.h"

void enqueue_voice_request(void) __asm__("FUN_0022dc50");

void enqueue_voice_request(void) {
    s32 i;

    for (i = 0; i < voice_pool.sound_instance_count; i++) {
        SoundInstance *handler = (SoundInstance *)(i * 0x90 + (s32)voice_pool.sound_instances);

        if (handler->update != 0) {
            handler->update(handler);
        }
    }
}

extern __typeof__(enqueue_voice_request) func_0022DC50 __attribute__((alias("FUN_0022dc50")));

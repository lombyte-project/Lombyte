#ifndef LOMBYTE_RNC_AUDIO_VOICE_ALLOC_H
#define LOMBYTE_RNC_AUDIO_VOICE_ALLOC_H

#include "types.h"

struct MobyAudioView;

/* Game-side voice slots: allocate one for a sound entry, release it. */
s32 allocate_voice_for_target_entry(s32 entry_index, s32 flags, void *moby) __asm__("FUN_0022da68");
void release_voice_slot(s32 slot_index) __asm__("FUN_0022d798");

#endif /* LOMBYTE_RNC_AUDIO_VOICE_ALLOC_H */

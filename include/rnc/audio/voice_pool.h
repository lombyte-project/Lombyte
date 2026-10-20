#ifndef LOMBYTE_RNC_AUDIO_VOICE_POOL_H
#define LOMBYTE_RNC_AUDIO_VOICE_POOL_H

#include "types.h"
#include "eetypes.h"
#include "rnc/audio/voice_state.h"

/* The pool layout agrees with sound_update: a 0x70-byte header followed by
   thirty 0x70-byte voices. Position and position_offset are full quadwords. */
typedef union {
    u128 q;
    f32 f[4];
} VoiceVector;
typedef struct {
    u8 pad0[0x10];
    s32 pitch_bend_min;
    s32 pitch_bend_max;
    u8 source_state;
    u8 pad19;
    u16 sound_index; /* 0x1A: updater reads this as s16; keep this view's width/sign. */
    s32 bank_handle; /* 0x1C: passed to snd_play_sound_vol_pan_pmpb. */
} VoiceDefinition;
typedef struct {
    u8 pad0[0x10];
    VoiceVector position;
    u8 pad20[0x86];
    s16 class_id;
} VoiceMoby;
typedef struct {
    u32 handle;
    u8 state;
    u8 flags;
    u8 pad6[2];
    VoiceDefinition *definition;
    u16 sound_index;
    s16 linked_index;
    s32 volume;
    s32 pitch_bend;
    VoiceMoby *owner;
    s32 owner_context; /* Cleared on allocation/release; not unused padding. */
    VoiceVector position;
    VoiceVector position_offset;
    s32 history_position;
    u8 history_storage[0x2C]; /* 36 occlusion samples + 8 bytes not interpreted here. */
} VoiceSlot;
typedef struct {
    u8 header[0x70];
    VoiceSlot voices[30];
} VoicePool;

/* An access window, not an element of an array of windows.
 * Retail uses pool_base + index * 0x70, then offsets into voice at +0x70.
 * Advancing this pointer by sizeof(VoicePoolWindow) would select the wrong slot. */
typedef struct {
    u8 header[0x70];
    VoiceSlot voice;
} VoicePoolWindow;

/* The sound voice pool (D_0013E550). */
extern VoicePool voice_pool __asm__("D_0013E550");

#endif /* LOMBYTE_RNC_AUDIO_VOICE_POOL_H */

#ifndef LOMBYTE_RNC_AUDIO_VOICE_POOL_H
#define LOMBYTE_RNC_AUDIO_VOICE_POOL_H

#include "types.h"
#include "eetypes.h"

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
    u16 source_value;
    s32 pad1C;
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
    u16 source_value;
    s16 linked_index;
    s32 volume;
    s32 pitch_bend;
    VoiceMoby *owner;
    s32 reserved1C;
    VoiceVector position;
    VoiceVector position_offset;
    s32 history_position;
    u8 history[0x2C];
} VoiceSlot;
typedef struct {
    u8 header[0x70];
    VoiceSlot voices[30];
} VoicePool;

/* Retail accesses each selected voice relative to the pool base. */
typedef struct {
    u8 header[0x70];
    VoiceSlot voice;
} VoicePoolWindow;

/* The sound voice pool (D_0013E550). */
extern VoicePool voice_pool __asm__("D_0013E550");

#endif /* LOMBYTE_RNC_AUDIO_VOICE_POOL_H */

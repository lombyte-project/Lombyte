#ifndef LOMBYTE_RNC_AUDIO_VOICE_POOL_H
#define LOMBYTE_RNC_AUDIO_VOICE_POOL_H

#include "types.h"
#include "eetypes.h"
#include "rnc/audio/voice_state.h"

/* The pool: a 0x70-byte header, thirty 0x70-byte voices, then the level
   sound instances and the listener occlusion samples. */
typedef union {
    u128 q;
    f32  f[4];
} VoiceVector;

typedef struct {
    u8  pad0[0x10];
    s32 pitch_bend_min;
    s32 pitch_bend_max;
    u8  source_state;
    u8  attenuation_flags;
    s16 sound_index; /* 0x1A */
    s32 bank_handle; /* 0x1C: passed to snd_play_sound_vol_pan_pmpb. */
} VoiceDefinition;

typedef struct {
    u8          pad0[0x10];
    VoiceVector position;
    u8          state;
    u8          pad21[0x85];
    s16         class_id; /* 0xA6 */
    u8          padA8[0x18];
    VoiceVector rotation; /* 0xC0 */
} VoiceMoby;

typedef struct {
    u32              handle;
    u8               state;
    u8               flags;
    u8               pad6[2];
    VoiceDefinition *definition;
    u16              sound_index;
    s16              linked_index;
    s32              volume;
    s32              pitch_bend;
    VoiceMoby       *owner;
    s32              owner_context; /* Cleared on allocation/release; not unused padding. */
    VoiceVector      position;
    VoiceVector      position_offset;
    u32              occlusion_history_position;
    u8               occlusion_history[36];
    u8               pad68[8];
} VoiceSlot;

/* A level sound instance (0x90 bytes), copied from the level gameplay file. */
typedef struct SoundInstance {
    s32 id;
    void (*update)(struct SoundInstance *instance);
    s32 data; /* a data table index; the loader swaps in the address of the copied data */
    u8  padC[0x84];
} SoundInstance;

typedef struct {
    VoiceVector listener_history[4];       /* 0x000 */
    s32         listener_history_position; /* 0x040 */
    s32         bank_load_busy;            /* set while load_audio_bank_by_location runs */
    s32         group_0_volume;            /* 0x048 */
    s32         group_1_volume;
    s32         group_2_volume;
    s32         group_3_volume;
    s32         group_4_volume;
    s32         group_5_volume;
    s32         unk60;
    s32         reverb_depth; /* 0x064 */
    u8          reverb_type;
    u8          reverb_delay;
    u8          reverb_feedback;
    u8  reverb_commands;  /* 1, 2, 4: reverb changes to send; 8, 0x10: set around level loads */
    s32 pending_commands; /* 0x06C */
    VoiceSlot      voices[30];           /* 0x070 */
    s32            sound_instance_count; /* 0xD90 */
    SoundInstance *sound_instances;
    s32            occlusion_frame; /* 0xD98 */
    s32            padD9C;
    VoiceVector    occlusion_samples[6]; /* 0xDA0 */
} VoicePool;

/* An access window, not an element of an array of windows.
 * Retail uses pool_base + index * 0x70, then offsets into voice at +0x70.
 * Advancing this pointer by sizeof(VoicePoolWindow) would select the wrong slot. */
typedef struct {
    u8        header[0x70];
    VoiceSlot voice;
} VoicePoolWindow;

/* The sound voice pool (D_0013E550). */
extern VoicePool voice_pool __asm__("D_0013E550");

#endif /* LOMBYTE_RNC_AUDIO_VOICE_POOL_H */

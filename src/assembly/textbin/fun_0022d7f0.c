#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022d7f0/FUN_0022d7f0.s", FUN_0022d7f0);
#else
#include "types.h"

#include "eetypes.h"
#include "qcopy.h"
#include "qzero.h"

/* The pool layout agrees with sound_update: a 0x70-byte header followed by
   thirty 0x70-byte voices. Position and position_offset are full quadwords. */
typedef union { u128 q; f32 f[4]; } VoiceVector;
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
typedef struct { u8 header[0x70]; VoiceSlot voices[30]; } VoicePool;
/* Retail accesses each selected voice relative to the pool base. */
typedef struct { u8 header[0x70]; VoiceSlot voice; } VoicePoolWindow;
typedef struct {
    u8 pad0[0x1090];
    VoiceMoby *secondary_moby;
    u8 pad1094[0xFEC];
    VoiceMoby *primary_moby;
} VoicePlayerState;

typedef struct { VoiceVector value; u8 pad10[0x60]; } VoicePositionRecord;
extern VoicePositionRecord voice_positions[] __asm__("D_0013E5E0");
extern u8 voice_volume_records[] __asm__("D_0013E5C0");
extern VoicePool voice_pool __asm__("D_0013E550");
extern VoicePlayerState player_state __asm__("D_0013F350");
extern void clear_voice_position(VoiceVector *) __asm__("func_001F99F8");
extern s32 game_random_remainder(s32) __asm__("func_00213260");
extern s32 calculate_voice_volume(VoiceSlot *, VoiceVector *) __asm__("func_0022C7E8");

s32 allocate_voice_slot(VoiceDefinition *, u32, VoiceMoby *, VoiceVector *, s32) __asm__("FUN_0022d7f0");

s32 allocate_voice_slot(VoiceDefinition *definition, u32 flags, VoiceMoby *moby, VoiceVector *position, s32 volume) {
    s32 volume_offset;
    s32 pitch_bend_max;
    s32 pitch_bend_min;
    s32 slot_index;
    s32 result;
    s32 slot_available;
    s32 slot_limit;
    s32 pitch_bend;
    u8 *state;
    u8 source_inactive;
    VoicePoolWindow *committed_slot;
    VoicePoolWindow *slot;
    VoicePoolWindow *pitch_slot;
    VoicePoolWindow *position_slot;

    source_inactive = definition->source_state == 0;
    if (flags & 4) {
        goto require_active_source;
    }
    result = -1;
    if (source_inactive == 0) {
        goto return_result;
    }
    goto choose_slot_limit;
require_active_source:
    result = -1;
    if (source_inactive != 0) {
        goto return_result;
    }
choose_slot_limit:
    slot_limit = 0x1A;
    if (moby == NULL) {
        goto find_free_slot;
    }
    if (player_state.primary_moby == moby) {
        goto use_extended_pool;
    }
    if (player_state.secondary_moby == moby) {
        goto use_extended_pool;
    }
    if (moby->class_id != 0x472) {
        goto find_free_slot;
    }
use_extended_pool:
    slot_limit = 0x1E;
find_free_slot:
    slot_index = 0;
    if (slot_limit == 0) {
        goto allocation_failed;
    }
    slot_available = slot_index < slot_limit;
    if (voice_pool.voices[0].state == 0) {
        goto initialize_slot;
    }
    state = &voice_pool.voices[0].state;
    slot_index = 1;
scan_next_slot:
    state += 0x70;
    if (slot_index >= slot_limit) {
        goto allocation_failed;
    }
    if (*state == 0) {
        goto slot_found;
    }
    slot_index += 1;
    goto scan_next_slot;
slot_found:
    slot_available = slot_index < slot_limit;
initialize_slot:
    if (slot_available == 0) {
        goto allocation_failed;
    }
    slot = (VoicePoolWindow *)((u8 *)&voice_pool + slot_index * 0x70);
    slot->voice.definition = definition;
    slot->voice.source_value = (u16) definition->source_value;
    slot->voice.linked_index = -1;
    slot->voice.volume = volume;
    slot->voice.owner = 0;
    slot->voice.reserved1C = 0;
    qzero(&voice_pool.voices[slot_index].position_offset);
    if (position != NULL) {
        goto set_position;
    }
    if (moby == NULL) {
        goto clear_position;
    }
set_position:
    if (moby == NULL) {
        goto copy_explicit_position;
    }
    qcopy(&voice_positions[slot_index].value, &moby->position);
    position_slot = (VoicePoolWindow *)((u8 *)voice_positions + slot_index * 0x70 - 0x90);
    position_slot->voice.position.f[2] += 1.0f;
    goto calculate_volume;
copy_explicit_position:
    qcopy(&voice_positions[slot_index].value, position);
    goto calculate_volume;
clear_position:
    flags |= 0x11;
    clear_voice_position(&voice_positions[slot_index].value);
calculate_volume:
    if (flags & 0x10) {
        goto commit_if_audible;
    }
    volume_offset = slot_index * 0x70;
    volume = calculate_voice_volume((VoiceSlot *)(voice_volume_records + volume_offset), (VoiceVector *)(voice_volume_records + volume_offset + 0x20));
commit_if_audible:
    result = -1;
    if (volume < 0x20) {
        goto return_result;
    }
    committed_slot = (VoicePoolWindow *)((u8 *)&voice_pool + slot_index * 0x70);
    committed_slot->voice.flags = flags;
    committed_slot->voice.state = 7;
    committed_slot->voice.history_position = 0;
    pitch_bend_max = definition->pitch_bend_max;
    pitch_bend_min = definition->pitch_bend_min;
    pitch_bend = pitch_bend_max;
    if (pitch_bend_max == pitch_bend_min) {
        goto publish_pitch_bend;
    }
    pitch_bend = game_random_remainder(pitch_bend_max - pitch_bend_min) + definition->pitch_bend_min;
publish_pitch_bend:
    pitch_slot = (VoicePoolWindow *)((u8 *)&voice_pool + slot_index * 0x70);
    pitch_slot->voice.handle = 0xFFFFFFFF;
    pitch_slot->voice.pitch_bend = pitch_bend;
    goto return_index;
allocation_failed:
    slot_index = -1;
return_index:
    result = slot_index;
return_result:
    return result;
}

extern __typeof__(allocate_voice_slot) func_0022D7F0 __attribute__((alias("FUN_0022d7f0")));

#endif /* NON_MATCHING */

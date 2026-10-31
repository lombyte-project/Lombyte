#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

#include "eetypes.h"
#include "qcopy.h"
#include "qzero.h"

#include "rnc/audio/voice_pool.h"
#include "rnc/math/random.h"
#include "rnc/audio/sound/calculate_voice_volume.h"

typedef struct {
    VoiceVector value;
    u8          pad10[0x60];
} VoicePositionRecord;

extern VoicePositionRecord voice_positions[] __asm__("D_0013E5E0");
extern u8                  voice_volume_records[] __asm__("D_0013E5C0");
extern void                clear_u64_value(VoiceVector *) __asm__("func_001F99F8");

s32 allocate_voice_slot(VoiceDefinition *, u32, VoiceMoby *, VoiceVector *, s32) __asm__("FUN_0022d7f0");

/* Reserve a game-side slot and queue its start for sound_update.
 * Ordinary owners search 26 slots; the hero, its first item, and class 0x472
 * may use all 30. A rejected definition, full pool, or low volume returns -1.
 * Retain the goto structure and address windows for retail code generation. */
s32 allocate_voice_slot(

    VoiceDefinition *definition, u32 flags, VoiceMoby *moby, VoiceVector *position, s32 volume) {
    s32              volume_offset;
    s32              pitch_bend_max;
    s32              pitch_bend_min;
    s32              slot_index;
    s32              volume_or_slot_index;
    s32              slot_limit;
    s32              pitch_bend;
    s32              source_inactive;
    VoicePoolWindow *committed_slot;
    VoicePoolWindow *slot;
    VoicePoolWindow *pitch_slot;
    VoicePoolWindow *position_slot;

    source_inactive = definition->source_state == 0;
    if ((flags & 4) == 0) {
        if ((source_inactive ^ 1) != 0) {
            volume_or_slot_index = -1;
            goto return_result;
        }
        slot_limit = 0x1A;
        goto check_moby;
    } else {
        if (source_inactive) {
            volume_or_slot_index = -1;
            goto return_result;
        }
        slot_limit = 0x1A;
    }
check_moby:
    if (moby == NULL) {
        goto find_free_slot;
    }
    if ((VoiceMoby *)hero.moby == moby) {
        goto use_extended_pool;
    }
    if ((VoiceMoby *)hero.items[0].moby == moby) {
        goto use_extended_pool;
    }
    if (moby->class_id != 0x472) {
        goto find_free_slot;
    }
use_extended_pool:
    slot_limit = 0x1E;
find_free_slot:
    for (slot_index = 0; slot_index < slot_limit; slot_index++) {
        if (voice_pool.voices[slot_index].state == VOICE_SLOT_FREE) {
            break;
        }
    }
    if (slot_index >= slot_limit) {
        goto allocation_failed;
    }
    slot = (VoicePoolWindow *)((u8 *)&voice_pool + slot_index * 0x70);
    slot->voice.definition = definition;
    slot->voice.sound_index = (u16)definition->sound_index;
    slot->voice.linked_index = -1;
    slot->voice.volume = volume;
    slot->voice.owner = 0;
    slot->voice.owner_context = 0;
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
    clear_u64_value(&voice_positions[slot_index].value);
calculate_volume:
    if (flags & 0x10) {
        goto commit_if_audible;
    }
    volume_offset = slot_index * 0x70;
    volume_or_slot_index =
        calculate_voice_volume((VoiceSlot *)(voice_volume_records + volume_offset),
                               (VoiceVector *)(voice_volume_records + volume_offset + 0x20));
    goto check_calculated_volume;
commit_if_audible:
    volume_or_slot_index = volume;
check_calculated_volume:
    if (volume_or_slot_index < 0x20) {
        volume_or_slot_index = -1;
        goto return_result;
    }
    committed_slot = (VoicePoolWindow *)((u8 *)&voice_pool + slot_index * 0x70);
    committed_slot->voice.flags = flags;
    committed_slot->voice.state = VOICE_SLOT_START_QUEUED;
    committed_slot->voice.occlusion_history_position = 0;
    pitch_bend_max = definition->pitch_bend_max;
    pitch_bend_min = definition->pitch_bend_min;
    if (pitch_bend_max != pitch_bend_min) {
        pitch_bend =
            random_integer_below(pitch_bend_max - pitch_bend_min) + definition->pitch_bend_min;
    } else {
        pitch_bend = pitch_bend_max;
    }
    pitch_slot = (VoicePoolWindow *)((u8 *)&voice_pool + slot_index * 0x70);
    pitch_slot->voice.pitch_bend = pitch_bend;
    pitch_slot->voice.handle = 0xFFFFFFFF;
    goto return_index;
allocation_failed:
    slot_index = -1;
return_index:
    volume_or_slot_index = slot_index;
return_result:
    return volume_or_slot_index;
}

extern __typeof__(allocate_voice_slot) func_0022D7F0 __attribute__((alias("FUN_0022d7f0")));

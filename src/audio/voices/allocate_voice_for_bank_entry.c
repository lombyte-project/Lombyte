#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"

/* These offsets include the 0x70-byte pool header before each slot. */
struct VoiceBankEntryWindow {
    u8  pad_0[0x7E];
    s16 linked_index;
    u8  pad_80[0x8];
    s32 owner;
};

extern s32 allocate_voice_slot(u8 *, s32, s32, s32, s32) __asm__("func_0022D7F0");
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");

s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) {
    s32                          slot_index;
    struct VoiceBankEntryWindow *slot;

    if (entry_index >= D_0015F5B4) {
        return -1;
    }
    slot_index = allocate_voice_slot(level_sound_defs + (entry_index << 5), flags, owner, 0, 0x400);
    if (slot_index >= 0) {
        slot = (struct VoiceBankEntryWindow *)(((u8 *)&voice_pool) + slot_index * 0x70);
        slot->owner = owner;
        slot->linked_index = entry_index;
    }
    return slot_index;
}

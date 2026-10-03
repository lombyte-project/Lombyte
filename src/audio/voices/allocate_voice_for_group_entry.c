#include "types.h"

/* These offsets include the 0x70-byte pool header before each slot. */
struct VoiceGroupEntryWindow {
    u8 pad_0[0x7E];
    s16 linked_index;
    u8 pad_80[0x8];
    s32 owner;
};
extern u8 D_0013E550[];
extern s32 D_0015F5B4;
extern s32 D_0015F630;
extern u8 *D_0015F634;
extern s32 allocate_voice_slot(u8 *, s32, s32, s32, s32) __asm__("func_0022D7F0");

s32 allocate_voice_for_group_entry(s32 group_entry_index, s32 flags, s32 owner) __asm__("FUN_0022dba0");

s32 allocate_voice_for_group_entry(s32 group_entry_index, s32 flags, s32 owner) {
    s32 entry_index;
    s32 slot_index;
    struct VoiceGroupEntryWindow *slot;

    entry_index = group_entry_index + D_0015F5B4;
    if (entry_index >= D_0015F630) {
        return -1;
    }
    slot_index = allocate_voice_slot(D_0015F634 + (entry_index << 5), flags, owner, 0, 0x400);
    if (slot_index >= 0) {
        slot = (struct VoiceGroupEntryWindow *)(slot_index * 0x70 + D_0013E550);
        slot->owner = owner;
        slot->linked_index = entry_index;
    }
    return slot_index;
}

extern __typeof__(allocate_voice_for_group_entry) func_0022DBA0 __attribute__((alias("FUN_0022dba0")));

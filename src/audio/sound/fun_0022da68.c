#include "types.h"
struct VoiceTarget {
    u8 pad_0[0x24];
    struct VoiceTargetClass * voice_class;
};

struct VoiceTargetClass {
    u8 pad_0[0xD];
    u8 voice_count;
    u8 pad_E[0x1A];
    s32 definitions;
};

struct VoiceTargetPoolWindow {
    u8 pad_0[0x7E];
    s16 entry_index;
    u8 pad_80[0x8];
    s32 owner;
};

extern u8 D_0013E550[];
extern s32 func_0022D7F0();
s32 allocate_voice_for_target_entry(s32 entry_index, s32 flags, struct VoiceTarget *target) __asm__("FUN_0022da68");

s32 allocate_voice_for_target_entry(s32 entry_index, s32 flags, struct VoiceTarget *target) {
    struct VoiceTargetClass *voice_class;
    s32 slot_index;
    s32 definitions;
    struct VoiceTargetPoolWindow *slot;

    if (target == NULL) {
        return -1;
    }
    voice_class = target->voice_class;
    if (voice_class == NULL) {
        return -1;
    }
    definitions = voice_class->definitions;
    if (definitions == 0) {
        return -1;
    }
    if (entry_index >= (s32) voice_class->voice_count) {
        return -1;
    }
    slot_index = func_0022D7F0(definitions + (entry_index << 5), flags, target, 0, 0x400);
    if (slot_index >= 0) {
        slot = (struct VoiceTargetPoolWindow *)((slot_index * 0x70) + (s32) D_0013E550);
        slot->owner = target;
        slot->entry_index = entry_index;
    }
    return slot_index;
}

extern __typeof__(allocate_voice_for_target_entry) func_0022DA68 __attribute__((alias("FUN_0022da68")));

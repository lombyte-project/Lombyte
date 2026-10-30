#include "types.h"
#include "rnc/audio/voice_pool.h"

/* Partial moby/class views: Moby.pclass is at 0x24 and
 * MobyClass.sound_defs at 0x28. The byte at 0x0D bounds the entry index.
 * Keep the integer address and old-style allocator declaration: converting
 * these to typed pointers is a separate matching change. */
struct MobyAudioView {
    u8                         pad_0[0x24];
    struct MobyClassAudioView *pclass;
};

struct MobyClassAudioView {
    u8  pad_0[0xD];
    u8  sound_def_count;
    u8  pad_E[0x1A];
    s32 sound_defs;
};

/* Addresses below are pool-relative: the first slot starts at +0x70. */
struct VoiceMobyEntryWindow {
    u8  pad_0[0x7E];
    s16 entry_index;
    u8  pad_80[0x8];
    s32 owner;
};

extern s32 allocate_voice_slot() __asm__("func_0022D7F0");
s32        allocate_voice_for_target_entry(s32                   entry_index,
                                           s32                   flags,
                                           struct MobyAudioView *moby) __asm__("FUN_0022da68");

/* Queue a class sound entry for a moby; return its pool index or -1.
 * The caller supplies a nonnegative entry index: retail only checks the
 * upper bound. This does not allocate a hardware voice or play immediately. */
s32 allocate_voice_for_target_entry(s32 entry_index, s32 flags, struct MobyAudioView *moby) {
    struct MobyClassAudioView   *pclass;
    s32                          slot_index;
    s32                          sound_defs;
    struct VoiceMobyEntryWindow *slot;

    if (moby == NULL) {
        return -1;
    }
    pclass = moby->pclass;
    if (pclass == NULL) {
        return -1;
    }
    sound_defs = pclass->sound_defs;
    if (sound_defs == 0) {
        return -1;
    }
    if (entry_index >= (s32)pclass->sound_def_count) {
        return -1;
    }
    slot_index = allocate_voice_slot(sound_defs + (entry_index << 5), flags, moby, 0, 0x400);
    if (slot_index >= 0) {
        slot = (struct VoiceMobyEntryWindow *)((slot_index * 0x70) + (s32)((u8 *)&voice_pool));
        slot->owner = moby;
        slot->entry_index = entry_index;
    }
    return slot_index;
}

extern __typeof__(allocate_voice_for_target_entry) func_0022DA68
    __attribute__((alias("FUN_0022da68")));

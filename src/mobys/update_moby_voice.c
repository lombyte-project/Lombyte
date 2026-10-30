#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/audio/voice_alloc.h"

struct MobyVoiceFlags {
    u8 pad_0[0x7C];
    u8 unk7C;
    u8 unk7D;
};

struct MobyVoiceEntry {
    u8  pad_0[0x7E];
    s16 unk7E;
    u8  pad_80[0x8];
    s32 unk88;
};

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

void update_moby_voice(struct MobyVoiceFlags *moby) __asm__("FUN_0020c940");

void update_moby_voice(struct MobyVoiceFlags *moby) {
    s32                    voice_slot_index;
    struct MobyVoiceEntry *voice_slot;

    if (moby->unk7D != 0xFF) {
        voice_slot_index = moby->unk7D;
        voice_slot = (struct MobyVoiceEntry *)(voice_slot_index * 0x70 + ((u8 *)&voice_pool));
        if (voice_slot->unk88 != (s32)moby) {
            moby->unk7D = 0xFF;
        } else if (voice_slot->unk7E != moby->unk7C) {
            release_voice_slot(voice_slot_index);
            moby->unk7D = 0xFF;
        }
    } else if (moby->unk7C != 0xFF) {
        moby->unk7D = allocate_voice_for_target_entry(moby->unk7C, 4, moby);
    }
}

extern __typeof__(update_moby_voice) func_0020C940 __attribute__((alias("FUN_0020c940")));

#include "types.h"
struct MobyVoiceFlags {
    u8 pad_0[0x7C];
    u8 unk7C;
    u8 unk7D;
};
struct MobyVoiceEntry {
    u8 pad_0[0x7E];
    s16 unk7E;
    u8 pad_80[0x8];
    s32 unk88;
};

extern u8 D_0013E550[];
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

void update_moby_voice(struct MobyVoiceFlags *moby) __asm__("FUN_0020c940");

void update_moby_voice(struct MobyVoiceFlags *moby) {
    s32 idx;
    struct MobyVoiceEntry *e;

    if (moby->unk7D != 0xFF) {
        idx = moby->unk7D;
        e = (struct MobyVoiceEntry *)(idx * 0x70 + D_0013E550);
        if (e->unk88 != (s32)moby) {
            moby->unk7D = 0xFF;
        } else if (e->unk7E != moby->unk7C) {
            release_voice_slot(idx);
            moby->unk7D = 0xFF;
        }
    } else if (moby->unk7C != 0xFF) {
        moby->unk7D = allocate_voice_for_target_entry(moby->unk7C, 4, moby);
    }
}

extern __typeof__(update_moby_voice) func_0020C940 __attribute__((alias("FUN_0020c940")));

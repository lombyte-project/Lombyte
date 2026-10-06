#include "types.h"

typedef struct Manipulator Manipulator;

typedef struct MobyAttachOwner {
    u8 pad0[0x24];
    void *unk24;
    u8 pad28[0x3C];
    Manipulator *unk64;
} MobyAttachOwner;

struct Manipulator {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    void *unk4;
    void *unk8;
    u8 padC[0x10];
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

void attach_manipulator(MobyAttachOwner *owner, s32 slot_index, Manipulator *manip) __asm__("FUN_0020cb10");

void attach_manipulator(MobyAttachOwner *owner, s32 slot_index, Manipulator *manip) {
    if (manip->unk1 == 0) {
        u8 *r;

        manip->unk0 = slot_index;
        manip->unk1 = 1;
        manip->unk1C = 1.0f;
        manip->unk20 = 1.0f;
        manip->unk24 = 1.0f;
        manip->unk28 = 1.0f;

        r = *(u8 **)((u8 *)(*(s32 *)((u8 *)owner->unk24 + 0x1C) + manip->unk0 * 4) + 4);
        {
            u8 n = *r;
            manip->unk4 = (void *)((r[n + 4] << 6) + 0x70000000);
        }
        manip->unk8 = owner->unk64;
        owner->unk64 = manip;
    }
}

/* Recovered original symbol name. */
extern __typeof__(attach_manipulator) AttachManipulator __attribute__((alias("FUN_0020cb10")));

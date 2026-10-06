#include "types.h"
struct MobyRecord {
    u8 pad_0[0x20];
    u8 unk20;
    u8 pad_21[0x31];
    u8 unk52;
};

extern u8 D_001B2BC0[];
void prune_moby_references(void) __asm__("FUN_0020cc60");

void prune_moby_references(void) {
    s32 remaining;
    void **slot;
    struct MobyRecord *moby;
    s32 mask = 0xFF;

    slot = D_001B2BC0;
    remaining = 0xF;
    do {
        moby = *slot;
        remaining -= 1;
        if ((moby != NULL) && ((moby->unk20 & 0x80) || (moby->unk52 != mask))) {
            *slot = NULL;
        }
        slot += 1;
    } while (remaining >= 0);
}

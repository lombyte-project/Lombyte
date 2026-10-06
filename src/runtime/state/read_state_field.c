/* ReadStateField exact recovery: guard on the state byte bit 0x20 then forward the unk10 field; the patched profile (lq/sq saves) reproduces all 12 instructions. */

#include "types.h"
struct MobyObject {
    u8 pad_0[0x34];
    u16 unk34;
    u8 pad_36[0x42];
    struct MobyObjectData *unk78;
};

struct MobyObjectData {
    u8 pad_0[0x10];
    s32 unk10;
};

s32 ReadStateField(struct MobyObject *moby) {
    if (moby == NULL) {
        goto block_2;
    }
    goto block_4;
block_2:
block_3:
    return 0;
block_4:
    if (!(moby->unk34 & 0x20)) {
        goto block_3;
    }
    return moby->unk78->unk10;
}

#include "types.h"

struct MobyObject {
    u8                     pad_0[0x34];
    u16                    flags;
    u8                     pad_36[0x42];
    struct MobyObjectData *pvars;
};

struct MobyObjectData {
    u8  pad_0[0x10];
    s32 hit_state;
};

s32 get_moby_hit_state(struct MobyObject *moby) {
    if (moby == NULL) {
        goto block_2;
    }
    goto block_4;
block_2:
block_3:
    return 0;
block_4:
    if (!(moby->flags & 0x20)) {
        goto block_3;
    }
    return moby->pvars->hit_state;
}

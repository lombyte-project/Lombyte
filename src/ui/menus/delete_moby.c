#include "types.h"

struct MobyRef {
    u8 pad_0[0x38];
    s64 unk38;
};

extern s32 D_0015F60C;
extern s32 mark_moby_for_removal() __asm__("func_0020C828");

s32 delete_moby(struct MobyRef *moby) __asm__("FUN_00225530");

s32 delete_moby(struct MobyRef *moby) {
    if (moby == 0) {
        return 0;
    }
    mark_moby_for_removal();
    moby->unk38 = (s64)D_0015F60C;
    return 0;
}

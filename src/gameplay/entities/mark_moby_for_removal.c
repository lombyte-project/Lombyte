#include "types.h"
#include "rnc/gameplay/entities/moby.h"
extern struct Moby *D_0015FF1C;
extern s32 D_0015F60C;
extern void func_0020DC20(struct Moby *, u32);
void mark_moby_for_removal(struct Moby *moby) __asm__("FUN_0020c828");

void mark_moby_for_removal(struct Moby *moby) {
    if (moby < D_0015FF1C) {
        moby->state = 0xFD;
    } else {
        moby->state = 0xFE;
    }
    moby->spawn_frame = D_0015F60C + 2;
    func_0020DC20(moby, 0x80807F7F);
}

extern __typeof__(mark_moby_for_removal) func_0020C828 __attribute__((alias("FUN_0020c828")));

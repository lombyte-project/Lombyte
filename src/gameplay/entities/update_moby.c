#include "types.h"
struct Moby;
struct MobyClass {
    void *unk0;
    void *unk4;
    void (*update)(struct Moby *);
    void *unkC;
    void (*draw)(struct Moby *);
};
struct Moby {
    u8 pad0[0x8C];
    s16 oclass;
};
extern struct MobyClass D_001E8C00[];
void update_moby(struct Moby *moby) __asm__("FUN_001ebec8");

void update_moby(struct Moby *moby) {
    void (*fn)(struct Moby *) = D_001E8C00[moby->oclass].update;

    if (fn != 0) {
        fn(moby);
    }
}

extern __typeof__(update_moby) func_001EBEC8 __attribute__((alias("FUN_001ebec8")));

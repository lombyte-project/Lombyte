#include "types.h"
struct Moby { u8 pad0[0x86]; s16 coll; };
struct CamColl { u8 pad0[0xC4]; void *moby; };
extern struct CamColl D_001870D0;
extern void *func_001E9448(void *);
extern void func_0020C828(void *);
void handle_camera_collision_with_hero(struct Moby *hero) __asm__("FUN_001ebe68");

void handle_camera_collision_with_hero(struct Moby *hero) {
    struct CamColl *c = &D_001870D0;

    if (hero->coll == 0) {
        if (c->moby == 0) {
            c->moby = func_001E9448((u8 *)c - 0x50);
        }
    } else if (c->moby != 0) {
        func_0020C828(c->moby);
        c->moby = 0;
    }
}

extern __typeof__(handle_camera_collision_with_hero) func_001EBE68 __attribute__((alias("FUN_001ebe68")));

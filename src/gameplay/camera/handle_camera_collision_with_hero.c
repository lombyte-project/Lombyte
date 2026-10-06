#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"
struct CamColl {
    u8 pad0[0xC4];
    void *moby;
};
extern struct CamColl D_001870D0;
extern void *func_001E9448(void *);
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
void handle_camera_collision_with_hero(struct UpdateCam *cam) __asm__("FUN_001ebe68");

void handle_camera_collision_with_hero(struct UpdateCam *cam) {
    struct CamColl *c = &D_001870D0;

    if (cam->unk86 == 0) {
        if (c->moby == 0) {
            c->moby = func_001E9448((u8 *)c - 0x50);
        }
    } else if (c->moby != 0) {
        mark_moby_for_removal(c->moby);
        c->moby = 0;
    }
}

extern __typeof__(handle_camera_collision_with_hero) func_001EBE68
    __attribute__((alias("FUN_001ebe68")));

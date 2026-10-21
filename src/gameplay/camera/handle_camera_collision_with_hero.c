#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"
struct CamColl {
    u8 pad0[0xC4];
    void *moby;
};
extern struct CamColl cam_coll_state __asm__("D_001870D0");
extern void *noop_callback_l(void *) __asm__("func_001E9448");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
void handle_camera_collision_with_hero(struct UpdateCam *cam) __asm__("FUN_001ebe68");

void handle_camera_collision_with_hero(struct UpdateCam *selected_camera) {
    struct CamColl *collision_state = &cam_coll_state;

    if (selected_camera->unk86 == 0) {
        if (collision_state->moby == 0) {
            collision_state->moby = noop_callback_l((u8 *)collision_state - 0x50);
        }
    } else if (collision_state->moby != 0) {
        mark_moby_for_removal(collision_state->moby);
        collision_state->moby = 0;
    }
}

extern __typeof__(handle_camera_collision_with_hero) func_001EBE68
    __attribute__((alias("FUN_001ebe68")));

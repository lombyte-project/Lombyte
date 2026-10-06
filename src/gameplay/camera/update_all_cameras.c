#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"

extern struct UpdateCam *D_001870C0[];
extern struct UpdateCam D_00187410[];
extern s32 D_00189B50[];
extern struct CameraType camera_types[] __asm__("D_001E8C00");
extern void camera_exit(struct UpdateCam *) __asm__("func_001EC3D8");
extern s32 camera_activation_check_priority(struct UpdateCam *,
                                            struct UpdateCam *) __asm__("func_001EC210");
extern void switch_active_camera_record(struct UpdateCam *) __asm__("func_001EBF10");
extern void handle_camera_collision_with_hero(struct UpdateCam *) __asm__("func_001EBE68");
extern void execute_camera_post_update_callbacks(void) __asm__("func_001EBCF0");
s32 update_all_cameras(void) __asm__("FUN_001ec420");

s32 update_all_cameras(void) {
    struct UpdateCam *best;
    s32 changed;
    s32 i;
    void (*update)(struct UpdateCam *);
    Vec4f *src;
    Vec3 *dst;

    best = D_001870C0[0];
    changed = 0;
    camera_exit(best);
    for (i = 0; i < 48; i++) {
        if (D_00189B50[i] != 0 && &D_00187410[i] != best &&
            camera_activation_check_priority(&D_00187410[i], best) != 0) {
            best = &D_00187410[i];
            changed = 1;
        }
    }
    if (changed) {
        switch_active_camera_record(best);
    }
    update = camera_types[best->type].update;
    handle_camera_collision_with_hero(best);
    if (update != 0) {
        update(best);
    }
    src = &best->pos;
    dst = &best->prev_pos;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    execute_camera_post_update_callbacks();
    return -1;
}

extern __typeof__(update_all_cameras) func_001EC420 __attribute__((alias("FUN_001ec420")));

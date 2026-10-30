#include "types.h"
#include "rnc/camera/update_cam.h"

extern struct UpdateCam *cur_cam[] __asm__("D_001870C0");
extern void              camera_exit(struct UpdateCam *) __asm__("func_001EC3D8");
extern s32               camera_activation_check_priority(struct UpdateCam *,
                                                          struct UpdateCam *) __asm__("func_001EC210");
extern void              switch_active_camera_record(struct UpdateCam *) __asm__("func_001EBF10");
extern void handle_camera_collision_with_hero(struct UpdateCam *) __asm__("func_001EBE68");
extern void execute_camera_post_update_callbacks(void) __asm__("func_001EBCF0");
s32         update_all_cameras(void) __asm__("FUN_001ec420");

s32 update_all_cameras(void) {
    struct UpdateCam *best;
    s32               changed;
    s32               i;
    void (*update)(struct UpdateCam *);
    Vec4f *camera_position;
    Vec3  *previous_camera_position;

    best = cur_cam[0];
    changed = 0;
    camera_exit(best);
    for (i = 0; i < 48; i++) {
        if (camera_selectable_flags[i] != 0 && &camera_selection_records[i] != best &&
            camera_activation_check_priority(&camera_selection_records[i], best) != 0) {
            best = &camera_selection_records[i];
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
    camera_position = &best->pos;
    previous_camera_position = &best->prev_pos;
    previous_camera_position->x = camera_position->x;
    previous_camera_position->y = camera_position->y;
    previous_camera_position->z = camera_position->z;
    execute_camera_post_update_callbacks();
    return -1;
}

extern __typeof__(update_all_cameras) func_001EC420 __attribute__((alias("FUN_001ec420")));

struct UpdateCam camera_selection_records[48] = {0};

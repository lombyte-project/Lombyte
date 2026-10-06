#include "types.h"
typedef struct { f32 x; f32 y; f32 z; } Vec3;
struct Camera { u8 pad0[0x30]; Vec3 pos; u8 pad3C[0x28]; Vec3 prev_pos; u8 pad70[0x1C]; s16 mode; u8 pad8E[0x12]; };
struct CamMode { u8 pad0[0xC]; void (*update)(struct Camera *); u8 pad10[4]; };
extern struct Camera *D_001870C0[];
extern struct Camera D_00187410[];
extern s32 D_00189B50[];
extern struct CamMode D_001E8C00[];
extern void draw_moby(struct Camera *) __asm__("func_001EC3D8");
extern s32 camera_activation_check_priority(struct Camera *, struct Camera *) __asm__("func_001EC210");
extern void switch_active_camera_record(struct Camera *) __asm__("func_001EBF10");
extern void handle_camera_collision_with_hero(struct Camera *) __asm__("func_001EBE68");
extern void execute_camera_post_update_callbacks(void) __asm__("func_001EBCF0");
s32 update_all_cameras(void) __asm__("FUN_001ec420");

s32 update_all_cameras(void) {
    struct Camera *best;
    s32 changed;
    s32 i;
    void (*update)(struct Camera *);
    Vec3 *src;
    Vec3 *dst;

    best = D_001870C0[0];
    changed = 0;
    draw_moby(best);
    for (i = 0; i < 48; i++) {
        if (D_00189B50[i] != 0 && &D_00187410[i] != best && camera_activation_check_priority(&D_00187410[i], best) != 0) {
            best = &D_00187410[i];
            changed = 1;
        }
    }
    if (changed) {
        switch_active_camera_record(best);
    }
    update = D_001E8C00[best->mode].update;
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

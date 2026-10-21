#include "types.h"
#include "rnc/gameplay/hero.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"

#include "rnc/gameplay/entities/moby.h"

struct CamColl {
    Vec4 pos;
    f32 vel;
    u8 pad14[0xC];
    Vec4 dir;
    Vec4 unk30;
    Vec4 unk40;
    f32 dir_vel[4];
    Vec4 unk60;
    Vec4 unk70;
    Vec4 unk80;
    Vec4 unk90;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 hist[5];
    u8 padC0[0x14];
    struct Moby *unkD4;
    f32 unkD8;
    f32 unkDC;
};

extern struct CamColl cam_coll_state __asm__("D_001870D0");

extern f32 cam_interp_values(f32 *vel, f32 from, f32 to, f32 stiffness, f32 damping,
                             f32 max) __asm__("FUN_001ebd78");
extern float absolute_float(float input) __asm__("func_001F99C0");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern f32 fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern f32 fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern void fast_vec_normalize(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

void update_camera_tracking_state(void) __asm__("FUN_001ed470");

void update_camera_tracking_state(void) {
    Vec4 dir;
    Vec4 proj;
    struct CamColl *camera_tracking_state;
    f32 displacement_dot_direction;
    s32 i;
    struct Moby *ground_moby;

    camera_tracking_state = &cam_coll_state;
    fast_vec_normalize(&dir, &hero.gravity_dir, -1.0f);
    qcopy(&camera_tracking_state->unk40, &camera_tracking_state->unk30);
    qcopy(&camera_tracking_state->unk30, &dir);
    if (fast_vec_dot(&camera_tracking_state->dir, &dir) < -0.98f) {
        dir.f[0] += 0.2f;
        dir.f[1] += 0.2f;
        dir.f[2] += 0.2f;
    }
    camera_tracking_state->dir.f[0] =
        cam_interp_values(&camera_tracking_state->dir_vel[0], camera_tracking_state->dir.f[0], dir.f[0], 0.015f, 0.2f, 0.0f);
    camera_tracking_state->dir.f[1] =
        cam_interp_values(&camera_tracking_state->dir_vel[1], camera_tracking_state->dir.f[1], dir.f[1], 0.015f, 0.2f, 0.0f);
    camera_tracking_state->dir.f[2] =
        cam_interp_values(&camera_tracking_state->dir_vel[2], camera_tracking_state->dir.f[2], dir.f[2], 0.015f, 0.2f, 0.0f);
    fast_vec_normalize(&camera_tracking_state->dir, &camera_tracking_state->dir, 1.0f);

    subtract_vector_xyz(&camera_tracking_state->unk70, &hero.motion.pos, &camera_tracking_state->unk60);
    camera_tracking_state->unkA0 = fast_vec_length(&camera_tracking_state->unk70);
    displacement_dot_direction = fast_vec_dot(&camera_tracking_state->unk70, &dir);
    camera_tracking_state->unkA8 = displacement_dot_direction;
    fast_vec_normalize(&proj, &dir, displacement_dot_direction);
    qcopy(&camera_tracking_state->unk90, &proj);
    subtract_vector_xyz(&camera_tracking_state->unk80, &camera_tracking_state->unk70, &proj);
    camera_tracking_state->unkA4 = fast_vec_length(&camera_tracking_state->unk80);
    fast_vec_scale(&camera_tracking_state->unk80, &camera_tracking_state->unk80, 1.0f / camera_tracking_state->unkA4);
    qcopy(&camera_tracking_state->unk60, &hero.motion.pos);

    if (hero.unk2284 != 0x50 || hero.state.current == 0x11) {
        camera_tracking_state->pos.f[0] = hero.motion.pos.f[0];
        camera_tracking_state->pos.f[1] = hero.motion.pos.f[1];
        camera_tracking_state->pos.f[2] =
            cam_interp_values(&camera_tracking_state->vel, camera_tracking_state->pos.f[2], hero.motion.pos.f[2], 0.0075f, 0.175f, 0.0f);
        camera_tracking_state->pos.f[3] = hero.motion.pos.f[2];
    } else {
        camera_tracking_state->pos.f[0] = hero.motion.pos.f[0];
        camera_tracking_state->pos.f[1] = hero.motion.pos.f[1];
    }

    for (i = 0; i < 4; i++) {
        camera_tracking_state->hist[i] = camera_tracking_state->hist[i + 1];
    }
    camera_tracking_state->hist[i] = hero.motion.rot.f[2];

    ground_moby = hero.ground_moby;
    if (ground_moby != NULL && ground_moby->oclass != 0x4BA && ground_moby->oclass != 0x336) {
        if (ground_moby == camera_tracking_state->unkD4) {
            camera_tracking_state->unkDC = ground_moby->pos.z - camera_tracking_state->unkD8;
            if (absolute_float(camera_tracking_state->unkDC) < 0.001f) {
                camera_tracking_state->unkDC = 0.0f;
            }
            camera_tracking_state->unkD8 = camera_tracking_state->unkD4->pos.z;
        } else {
            camera_tracking_state->unkD4 = ground_moby;
            camera_tracking_state->unkDC = 0.0f;
            camera_tracking_state->unkD8 = ground_moby->pos.z;
        }
    } else {
        camera_tracking_state->unkDC = 0.0f;
        camera_tracking_state->unkD4 = NULL;
        camera_tracking_state->unkD8 = hero.motion.pos.f[2];
    }
}

extern __typeof__(update_camera_tracking_state) func_001ED470 __attribute__((alias("FUN_001ed470")));

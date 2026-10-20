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
    struct CamColl *cam;
    f32 displacement_dot_direction;
    s32 i;
    struct Moby *ground_moby;

    cam = &cam_coll_state;
    fast_vec_normalize(&dir, &hero.gravity_dir, -1.0f);
    qcopy(&cam->unk40, &cam->unk30);
    qcopy(&cam->unk30, &dir);
    if (fast_vec_dot(&cam->dir, &dir) < -0.98f) {
        dir.f[0] += 0.2f;
        dir.f[1] += 0.2f;
        dir.f[2] += 0.2f;
    }
    cam->dir.f[0] =
        cam_interp_values(&cam->dir_vel[0], cam->dir.f[0], dir.f[0], 0.015f, 0.2f, 0.0f);
    cam->dir.f[1] =
        cam_interp_values(&cam->dir_vel[1], cam->dir.f[1], dir.f[1], 0.015f, 0.2f, 0.0f);
    cam->dir.f[2] =
        cam_interp_values(&cam->dir_vel[2], cam->dir.f[2], dir.f[2], 0.015f, 0.2f, 0.0f);
    fast_vec_normalize(&cam->dir, &cam->dir, 1.0f);

    subtract_vector_xyz(&cam->unk70, &hero.motion.pos, &cam->unk60);
    cam->unkA0 = fast_vec_length(&cam->unk70);
    displacement_dot_direction = fast_vec_dot(&cam->unk70, &dir);
    cam->unkA8 = displacement_dot_direction;
    fast_vec_normalize(&proj, &dir, displacement_dot_direction);
    qcopy(&cam->unk90, &proj);
    subtract_vector_xyz(&cam->unk80, &cam->unk70, &proj);
    cam->unkA4 = fast_vec_length(&cam->unk80);
    fast_vec_scale(&cam->unk80, &cam->unk80, 1.0f / cam->unkA4);
    qcopy(&cam->unk60, &hero.motion.pos);

    if (hero.unk2284 != 0x50 || hero.state.current == 0x11) {
        cam->pos.f[0] = hero.motion.pos.f[0];
        cam->pos.f[1] = hero.motion.pos.f[1];
        cam->pos.f[2] =
            cam_interp_values(&cam->vel, cam->pos.f[2], hero.motion.pos.f[2], 0.0075f, 0.175f, 0.0f);
        cam->pos.f[3] = hero.motion.pos.f[2];
    } else {
        cam->pos.f[0] = hero.motion.pos.f[0];
        cam->pos.f[1] = hero.motion.pos.f[1];
    }

    for (i = 0; i < 4; i++) {
        cam->hist[i] = cam->hist[i + 1];
    }
    cam->hist[i] = hero.motion.rot.f[2];

    ground_moby = hero.ground_moby;
    if (ground_moby != NULL && ground_moby->oclass != 0x4BA && ground_moby->oclass != 0x336) {
        if (ground_moby == cam->unkD4) {
            cam->unkDC = ground_moby->pos.z - cam->unkD8;
            if (absolute_float(cam->unkDC) < 0.001f) {
                cam->unkDC = 0.0f;
            }
            cam->unkD8 = cam->unkD4->pos.z;
        } else {
            cam->unkD4 = ground_moby;
            cam->unkDC = 0.0f;
            cam->unkD8 = ground_moby->pos.z;
        }
    } else {
        cam->unkDC = 0.0f;
        cam->unkD4 = NULL;
        cam->unkD8 = hero.motion.pos.f[2];
    }
}

extern __typeof__(update_camera_tracking_state) func_001ED470 __attribute__((alias("FUN_001ed470")));

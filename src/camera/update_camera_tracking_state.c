#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/sdk/eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"

#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"

struct CamColl {
    Vec4         pos;
    f32          vel;
    u8           pad14[0xC];
    Vec4         dir;
    Vec4         up;
    Vec4         prev_up;
    f32          dir_vel[4];
    Vec4         prev_hero_pos;
    Vec4         hero_move;
    Vec4         hero_move_flat_dir;
    Vec4         hero_move_up;
    f32          hero_move_length;
    f32          hero_move_flat_length;
    f32          hero_move_along_up;
    f32          hist[5];
    u8           padC0[0x14];
    struct Moby *ground_moby;
    f32          ground_z;
    f32          ground_dz;
};

extern struct CamColl cam_coll_state __asm__("D_001870D0");

extern f32 cam_interp_values(
    f32 *vel, f32 from, f32 to, f32 stiffness, f32 damping, f32 max) __asm__("FUN_001ebd78");

void update_camera_tracking_state(void) __asm__("FUN_001ed470");

void update_camera_tracking_state(void) {
    Vec4            dir;
    Vec4            proj;
    struct CamColl *camera_tracking_state;
    f32             displacement_dot_direction;
    s32             i;
    struct Moby    *ground_moby;

    camera_tracking_state = &cam_coll_state;
    fast_vec_normalize(&dir, &hero.gravity_dir, -1.0f);
    qcopy(&camera_tracking_state->prev_up, &camera_tracking_state->up);
    qcopy(&camera_tracking_state->up, &dir);
    if (fast_vec_dot(&camera_tracking_state->dir, &dir) < -0.98f) {
        dir.f[0] += 0.2f;
        dir.f[1] += 0.2f;
        dir.f[2] += 0.2f;
    }
    camera_tracking_state->dir.f[0] = cam_interp_values(&camera_tracking_state->dir_vel[0],
                                                        camera_tracking_state->dir.f[0],
                                                        dir.f[0],
                                                        0.015f,
                                                        0.2f,
                                                        0.0f);
    camera_tracking_state->dir.f[1] = cam_interp_values(&camera_tracking_state->dir_vel[1],
                                                        camera_tracking_state->dir.f[1],
                                                        dir.f[1],
                                                        0.015f,
                                                        0.2f,
                                                        0.0f);
    camera_tracking_state->dir.f[2] = cam_interp_values(&camera_tracking_state->dir_vel[2],
                                                        camera_tracking_state->dir.f[2],
                                                        dir.f[2],
                                                        0.015f,
                                                        0.2f,
                                                        0.0f);
    fast_vec_normalize(&camera_tracking_state->dir, &camera_tracking_state->dir, 1.0f);

    fast_vec_sub(
        &camera_tracking_state->hero_move, &hero.motion.pos, &camera_tracking_state->prev_hero_pos);
    camera_tracking_state->hero_move_length = fast_vec_length(&camera_tracking_state->hero_move);
    displacement_dot_direction = fast_vec_dot(&camera_tracking_state->hero_move, &dir);
    camera_tracking_state->hero_move_along_up = displacement_dot_direction;
    fast_vec_normalize(&proj, &dir, displacement_dot_direction);
    qcopy(&camera_tracking_state->hero_move_up, &proj);
    fast_vec_sub(
        &camera_tracking_state->hero_move_flat_dir, &camera_tracking_state->hero_move, &proj);
    camera_tracking_state->hero_move_flat_length =
        fast_vec_length(&camera_tracking_state->hero_move_flat_dir);
    fast_vec_scale(&camera_tracking_state->hero_move_flat_dir,
                   &camera_tracking_state->hero_move_flat_dir,
                   1.0f / camera_tracking_state->hero_move_flat_length);
    qcopy(&camera_tracking_state->prev_hero_pos, &hero.motion.pos);

    if (hero.unk2284 != 0x50 || hero.state.current == 0x11) {
        camera_tracking_state->pos.f[0] = hero.motion.pos.f[0];
        camera_tracking_state->pos.f[1] = hero.motion.pos.f[1];
        camera_tracking_state->pos.f[2] = cam_interp_values(&camera_tracking_state->vel,
                                                            camera_tracking_state->pos.f[2],
                                                            hero.motion.pos.f[2],
                                                            0.0075f,
                                                            0.175f,
                                                            0.0f);
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
        if (ground_moby == camera_tracking_state->ground_moby) {
            camera_tracking_state->ground_dz = ground_moby->pos.z - camera_tracking_state->ground_z;
            if (absolute_float(camera_tracking_state->ground_dz) < 0.001f) {
                camera_tracking_state->ground_dz = 0.0f;
            }
            camera_tracking_state->ground_z = camera_tracking_state->ground_moby->pos.z;
        } else {
            camera_tracking_state->ground_moby = ground_moby;
            camera_tracking_state->ground_dz = 0.0f;
            camera_tracking_state->ground_z = ground_moby->pos.z;
        }
    } else {
        camera_tracking_state->ground_dz = 0.0f;
        camera_tracking_state->ground_moby = NULL;
        camera_tracking_state->ground_z = hero.motion.pos.f[2];
    }
}

extern __typeof__(update_camera_tracking_state) func_001ED470
    __attribute__((alias("FUN_001ed470")));

/* Level 01 moby class spark_particle_emitter. */
#include "types.h"
#include "rnc/globals.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"

#include "rnc/overlay/level_path.h"

typedef union {
    u128 q;
    f32  f[4];
} EmitVec;

typedef struct {
    EmitVec r[4];
} EmitMtx;

typedef struct {
    u8      pad0[0x70];
    s32     flags;
    u8      pad74[8];
    void   *owner;
    EmitVec vel;
    f32     vel_range[4];
    f32     life_min;
    f32     life_max;
    f32     size_min;
    f32     size_max;
    f32     fade[4];
    s32     tex;
    u8      count;
    u8      delay;
    u8      blend;
    u8      alpha;
    s32     timer;
    f32     spread[3];
} Emitter;

typedef struct {
    u8       pad0[0x10];
    EmitVec  pos;
    u8       pad20[0x20];
    f32      rot[4];
    u8       pad50[0x28];
    Emitter *vars;
} EmitterMoby;

extern void create_spark_particle(void *,
                                  void *,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  s32,
                                  u8,
                                  u8,
                                  u8) __asm__("FUN_L01_0027e900");

void spark_particle_emitter_update(EmitterMoby *self) __asm__("FUN_L01_002bd100");

/* Moby class 27 */
void spark_particle_emitter_update(EmitterMoby *emitter_moby) {
    Emitter *emitter_vars;
    EmitVec  rotated_particle_velocity;
    EmitVec  position_scratch;
    EmitVec  emission_position_offset;
    union {
        struct {
            EmitMtx m;
            EmitVec ang;
        } rot;
        struct {
            EmitVec vel;
            EmitMtx m;
        } lin;
    } u;
    f32 size;
    s32 i;

    emitter_vars = emitter_moby->vars;
    if (current_level_index == 1) {
        position_scratch.q = emitter_moby->pos.q;
        position_scratch.f[3] = 20.0f;
        if (fast_bsphere_check(&position_scratch, 240.0f) == -1) {
            return;
        }
    }
    emitter_vars->owner = emitter_moby;
    if (emitter_vars->timer <= 0) {
        emitter_vars->timer = emitter_vars->delay;
        for (i = 0; i < emitter_vars->count; i++) {
            position_scratch.q = emitter_moby->pos.q;
            emission_position_offset.f[0] =
                random_float_between(-emitter_vars->spread[0], emitter_vars->spread[0]);
            emission_position_offset.f[1] =
                random_float_between(-emitter_vars->spread[1], emitter_vars->spread[2]);
            emission_position_offset.f[2] =
                random_float_between(-emitter_vars->spread[2], emitter_vars->spread[2]);
            if (emitter_vars->flags & 0x2000) {
                euler_rotation_matrix(&u.rot.m, emitter_moby->rot);
                transform_vector(&emission_position_offset, &emission_position_offset, &u.rot.m);
            }
            fast_vec_add(&position_scratch, &position_scratch, &emission_position_offset);
            if (emitter_vars->flags & 0x1000) {
                u.rot.ang.f[0] =
                    random_float_between(-emitter_vars->vel_range[0], emitter_vars->vel_range[0]) *
                    3.1415927f / 180.0f;
                u.rot.ang.f[1] =
                    random_float_between(-emitter_vars->vel_range[1], emitter_vars->vel_range[1]) *
                    3.1415927f / 180.0f;
                u.rot.ang.f[2] =
                    random_float_between(-emitter_vars->vel_range[2], emitter_vars->vel_range[2]) *
                    3.1415927f / 180.0f;
                if (emitter_vars->flags & 0x4000) {
                    u.rot.ang.f[0] = fast_add_rotations(u.rot.ang.f[0], emitter_moby->rot[0]);
                    u.rot.ang.f[1] = fast_add_rotations(u.rot.ang.f[0], emitter_moby->rot[1]);
                    u.rot.ang.f[2] = fast_add_rotations(u.rot.ang.f[0], emitter_moby->rot[2]);
                }
                euler_rotation_matrix(&u.rot.m, &u.rot.ang);
                transform_vector(&rotated_particle_velocity, &emitter_vars->vel, &u.rot.m);
                size = random_float_between(emitter_vars->size_min, emitter_vars->size_max);
                create_spark_particle(
                    emitter_vars,
                    &position_scratch,
                    rotated_particle_velocity.f[0],
                    rotated_particle_velocity.f[1],
                    rotated_particle_velocity.f[2],
                    size,
                    random_float_between(emitter_vars->life_min, emitter_vars->life_max),
                    emitter_vars->fade[0],
                    emitter_vars->fade[1],
                    emitter_vars->fade[2],
                    emitter_vars->fade[3],
                    emitter_vars->tex,
                    emitter_vars->blend,
                    emitter_vars->alpha,
                    0xFF);
            } else {
                u.lin.vel.f[0] =
                    random_float_between(emitter_vars->vel.f[0], emitter_vars->vel_range[0]) *
                    frame_scale;
                u.lin.vel.f[1] =
                    random_float_between(emitter_vars->vel.f[1], emitter_vars->vel_range[1]) *
                    frame_scale;
                u.lin.vel.f[2] =
                    random_float_between(emitter_vars->vel.f[2], emitter_vars->vel_range[2]) *
                    frame_scale;
                if (emitter_vars->flags & 0x4000) {
                    euler_rotation_matrix(&u.lin.m, emitter_moby->rot);
                    transform_vector(&u.lin.vel, &u.lin.vel, &u.lin.m);
                }
                size = random_float_between(emitter_vars->size_min, emitter_vars->size_max);
                create_spark_particle(
                    emitter_vars,
                    &position_scratch,
                    u.lin.vel.f[0],
                    u.lin.vel.f[1],
                    u.lin.vel.f[2],
                    size,
                    random_float_between(emitter_vars->life_min, emitter_vars->life_max),
                    emitter_vars->fade[0],
                    emitter_vars->fade[1],
                    emitter_vars->fade[2],
                    emitter_vars->fade[3],
                    emitter_vars->tex,
                    emitter_vars->blend,
                    emitter_vars->alpha,
                    0xFF);
            }
        }
    } else {
        emitter_vars->timer--;
    }
}

#include "qcopy.h"

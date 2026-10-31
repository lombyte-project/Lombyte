/* Level 14 flying debris moby class and its spawn and remove helpers. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "sda.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/l14/mobys/puff.h"
#include "rnc/overlay/l14/mobys/flying_debris_globals.h"

extern void release_point_light_slot(s32) __asm__("FUN_L00_0023e838");

extern void FUN_0020c828(void *);

void remove_flying_debris(u8 *self) __asm__("FUN_L14_002bb4d8");

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

extern char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");

extern char D_L14_0015F580[] MACRO_ADDR;

extern int random_integer_below(int);

extern float random_float_between(float, float);

extern int scale_game_frames(int);

extern void build_look_at_matrix(void *, void *, void *, float);

extern f32 flying_debris_gravity_gp __asm__("D_L14_001615D0") __attribute__((sda));

extern f32 flying_debris_velocity_retention_factor_gp __asm__("D_L14_001615D4")
    __attribute__((sda));

extern f32 flying_debris_horizontal_drag_speed_threshold_gp __asm__("D_L14_001615D8")
    __attribute__((sda));

extern char D_L14_001809C0[];

extern void normalize_d(void *, void *, float) __asm__("FUN_001f9bf8");

extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");

extern void e450_b78(void *,
                     void *,
                     void *,
                     float,
                     float,
                     int,
                     int,
                     int,
                     float,
                     float,
                     float,
                     float,
                     int,
                     float,
                     int,
                     int,
                     int,
                     int) __asm__("FUN_L00_0025e450");

extern char *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");

extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");

void flying_debris_update(struct Moby *moby) __asm__("FUN_L14_002bac78");

/* Moby class 81 */
void flying_debris_update(struct Moby *moby) {
    char  *debris_vars = (char *)moby->pvars;
    float  joint[4];
    float  ofs[4];
    float  at[4];
    float  moby_to_joint_direction[4];
    float  push[4];
    int    i;
    int    signed_puff_random_value;
    int    puff_rgb_component;
    float  size;
    float  bright;
    u8    *puff_particle;
    u8    *puff_tail;
    float *light_record;
    float  range = 3.0f;
    float  green = 0.8f;
    float  blue = 0.2f;

    if (fast_dec_timer_s16(debris_vars + 0x10) != 0) {
        remove_flying_debris((u8 *)moby);
        return;
    }
    fast_vec_add(&moby->pos, &moby->pos, debris_vars);
    if (absolute_float(*(float *)(debris_vars + 0x0)) >
        flying_debris_horizontal_drag_speed_threshold_gp * frame_scale)
        *(float *)(debris_vars + 0x0) *=
            (flying_debris_velocity_retention_factor_gp - 1.0f) * frame_scale + 1.0f;
    if (absolute_float(*(float *)(debris_vars + 0x4)) >
        flying_debris_horizontal_drag_speed_threshold_gp * frame_scale)
        *(float *)(debris_vars + 0x4) *=
            (flying_debris_velocity_retention_factor_gp - 1.0f) * frame_scale + 1.0f;
    if (*(float *)(debris_vars + 0x8) > 0.0f)
        *(float *)(debris_vars + 0x8) *=
            (flying_debris_velocity_retention_factor_gp - 1.0f) * frame_scale + 1.0f;
    *(float *)(debris_vars + 0x8) -= flying_debris_gravity_gp * frame_scale_sq;
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        remove_flying_debris((u8 *)moby);
        return;
    }
    if (coll_sphere(0.75f, &moby->pos, 0, 0) != 0 ||
        (absolute_float(moby->pos.x - hero.motion.pos.f[0]) < 10.0f &&
         absolute_float(moby->pos.y - hero.motion.pos.f[1]) < 10.0f)) {
        clear_u64_value(joint);
        trooper_play_sound(1, 0, moby, (*(struct Moby **)(debris_vars + 0x20))->oclass);
        e450_b78(moby,
                 joint,
                 &moby->pos,
                 2.0f,
                 4.0f,
                 5,
                 2,
                 4,
                 1.0f,
                 0.5f,
                 9.0f,
                 1.0f,
                 -1,
                 5.0f,
                 0,
                 2,
                 -1,
                 0);
        remove_flying_debris((u8 *)moby);
        return;
    }
    moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(debris_vars + 0x14));
    moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(debris_vars + 0x18));
    moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(debris_vars + 0x1C));
    moby_point_position(moby, 0, joint);
    size = randf_sym(0.01f, 0.5f);
    fast_vec_scale(ofs, (char *)moby + 0xC0, size);
    fast_vec_add(at, joint, ofs);
    fast_vec_sub(moby_to_joint_direction, joint, &moby->pos);
    normalize_d(moby_to_joint_direction, moby_to_joint_direction, 0.05f);
    spawn_burst(at,
                moby_to_joint_direction,
                0x4F007FFF,
                0x1FFFFFFF,
                random_int_between(scale_game_frames(10), scale_game_frames(20)),
                1,
                40000.0f);
    for (i = 0; i < 3; i++) {
        if (i != 2) {
            fast_vec_scale(ofs, (char *)moby + 0xC0, randf_sym(0.01f, 0.5f));
            fast_vec_add(at, &moby->pos, ofs);
            fast_vec_scale(ofs, (char *)moby + 0xD0, randf_sym(0.01f, 0.5f));
            fast_vec_add(at, &moby->pos, ofs);
        }
        signed_puff_random_value = random_integer_below(6);
        if (random_integer_below(2) != 0)
            signed_puff_random_value = -signed_puff_random_value;
        size = random_float_between(20000.0f, 100000.0f);
        puff_rgb_component = random_int_between(0x10, 0xFF);
        puff_particle =
            (u8 *)spawn_puff(at,
                             0.025f,
                             1.0f,
                             1.0f,
                             signed_puff_random_value,
                             D_L14_0015F580,
                             size,
                             puff_rgb_component | (0x60000000 | (puff_rgb_component << 16) |
                                                   (puff_rgb_component << 8)));
        if (puff_particle != NULL) {
            puff_particle[3] = 0x44;
            puff_tail = puff_particle + 0x20;
            *(s16 *)(puff_particle + 0xA) = scale_game_frames(60);
            puff_tail[0xA] = 0x60;
            *(s32 *)(puff_tail + 4) = 2;
            puff_tail[0xB] = puff_particle[0xA];
        }
    }
    bright = 1.0f;
    fast_vec_sub(push, joint, &moby->pos);
    fast_vec_scale(push, push, 2.5f);
    fast_vec_add(at, &moby->pos, push);
    if (random_integer_below(2) != 0)
        fast_vec_scale(push, (char *)moby + 0xE0, -1.0f);
    else
        qcopy(push, (char *)moby + 0xE0);
    fast_vec_add(at, at, push);
    if (*(short *)(debris_vars + 0x12) == -1) {
        *(short *)(debris_vars + 0x12) =
            allocate_colored_effect_slot(at, range, 0.0f, bright, green, blue);
    } else {
        light_record = (float *)(D_L14_001809C0 + *(short *)(debris_vars + 0x12) * 32);
        qcopy(light_record + 4, at);
        light_record[7] = range;
        light_record[0] = bright;
        light_record[1] = green;
        light_record[2] = blue;
    }
}

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

unsigned char *
spawn_flying_debris(struct Moby *owner, float angle, char *posp) __asm__("FUN_L14_002bb310");

unsigned char *spawn_flying_debris(struct Moby *debris_source_moby, float angle, char *posp) {
    float          spawn_vector_scratch[12];
    unsigned char *spawned_moby;
    char          *spawned_vars;
    char          *hero_gravity_direction;
    float         *spawn_position = spawn_vector_scratch;
    float         *launch_vector;
    *(OvlQuad *)spawn_position = *(OvlQuad *)posp;
    spawned_moby = (unsigned char *)create_moby(0x51);
    if (spawned_moby != 0) {
        spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        qcopy(spawned_moby + 0x10, spawn_position);
        launch_vector = spawn_vector_scratch + 4;
        MOBY(spawned_moby)->rot.z = angle;
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        fast_vec_sub(launch_vector, spawn_position, &debris_source_moby->pos);
        spawn_vector_scratch[6] = 0.0f;
        fast_vec_normalize(launch_vector,
                           launch_vector,
                           random_float_between(frame_scale * 0.075f, frame_scale * 0.2f));
        hero_gravity_direction = (char *)&hero.gravity_dir;
        fast_vec_cross(spawn_vector_scratch + 8, hero_gravity_direction, launch_vector);
        build_look_at_matrix(
            spawned_vars, launch_vector, hero_gravity_direction, randf_sym(0.0f, 0.7853982f));
        *(float *)(spawned_vars + 8) =
            random_float_between(frame_scale * 0.05f, frame_scale * 0.32f);
        *(float *)(spawned_vars + 0x14) = randf_sym(0.0f, 0.034906585f);
        *(float *)(spawned_vars + 0x18) = randf_sym(0.0f, 0.034906585f);
        *(float *)(spawned_vars + 0x1C) = randf_sym(0.0f, 0.034906585f);
        *(short *)(spawned_vars + 0x10) = scale_game_frames(200);
        *(char **)(spawned_vars + 0x20) = debris_source_moby;
        *(short *)(spawned_vars + 0x12) = -1;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

void remove_flying_debris(u8 *debris_moby) {
    u8 *debris_vars = *(u8 **)(debris_moby + 0x78);
    s16 light_index = *(s16 *)(debris_vars + 0x12);

    if (light_index != -1) {
        release_point_light_slot(light_index);
        *(s16 *)(debris_vars + 0x12) = -1;
    }
    FUN_0020c828(debris_moby);
}

/* Shared moby class mine_glove: the mine glove held by the hero, with its launch velocity. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_state.h"
#include "qzero.h"

typedef struct {
    u8           pad0[0x40];
    OvlVec4      grip;
    struct Moby *held;
    s32          timer;
} CarrierVars_2c1ad0;

typedef struct {
    u8  pad0[0x88];
    u16 deliveries;
    u16 unkA2;
    u32 levels;
} DeliveryStats_2c1ad0;

typedef struct {
    u8           pad0[0x40];
    Vec4         grip;
    struct Moby *held;
    s32          timer54;
    s32          timer58;
    s32          delay;
} GloveVars;

extern float FUN_001f9b48_2c1928(void *, void *) __asm__("FUN_001f9b48");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001f9bf8_2c1928(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f9b80_2c1928(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_2c1928(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f9988_2c1928(float) __asm__("FUN_001f9988");
extern float FUN_001f9dc8_2c1928(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2c1928(float) __asm__("FUN_001f9de0");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern f32   fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern s32   collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void  initialize_moby_motion_at_position(void *, void *, void *) __asm__("FUN_L00_002aa008");
extern struct Moby *spawn_mine(void *, void *, void *) __asm__("FUN_L00_002a9ed0");
extern void         append_unique_moby_index(void *, void *, int) __asm__("FUN_L00_0025f780");
extern f32          square_root(f32) __asm__("FUN_001f9988");
extern void         fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void         copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void         transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void         transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void         transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32          vector_length_xy(void *) __asm__("FUN_001f9b20");
extern int          use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern int          get_ammo(int) __asm__("FUN_L00_00233e98");
extern void         FUN_L00_0024f7c8(void *, int, void *);
/* Float arguments first: the order they are evaluated in gives retail's registers. */
extern void FUN_L00_002c1928_f(f32, f32, f32, f32 *, f32 *, f32 *) __asm__("FUN_L00_002c1928");

extern CollisionHit   coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern f32            frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern u32 D_0013CAE4 NOT_SDA;
extern CollisionHit   coll_hit __asm__("D_L00_00173E40");
extern Vec4           D_L00_00166DC0;
extern short          D_L00_001B07B0[];
extern f32            frame_scale_inv_macro __asm__("D_0015ED68") MACRO_ADDR;
extern f32            frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern s32            current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern s32            D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
/* coll_hit's normal as a whole quadword (CollisionHit names only x/y/z) */
extern Vec4 D_L00_00166DC0 NOT_SDA;
extern float               frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

void compute_mine_launch_velocity(float   *origin_position,
                                  OvlQuad *target_position,
                                  float   *out,
                                  float    gravity,
                                  float    min_horizontal_distance,
                                  float    max_range) __asm__("FUN_L00_002c1928");

void compute_mine_launch_velocity(float   *origin_position,
                                  OvlQuad *target_position,
                                  float   *out,
                                  float    gravity,
                                  float    min_horizontal_distance,

                                  float max_range) {
    OvlQuad effective_target_position[1];
    float   half = gravity * max_range * 0.5f;
    float   effective_horizontal_distance, dz, launch_heading, launch_component_speed_squared,
        launch_component_speed;

    if (max_range < FUN_001f9b48_2c1928(origin_position, target_position)) {
        fast_vec_sub(effective_target_position, target_position, origin_position);
        FUN_001f9bf8_2c1928(
            effective_target_position, effective_target_position, max_range - 0.01f);
        fast_vec_add(effective_target_position, effective_target_position, origin_position);
    } else {
        effective_target_position[0] = *target_position;
    }
    effective_horizontal_distance = FUN_001f9b80_2c1928(origin_position, effective_target_position);
    dz = ((float *)effective_target_position)[2] - origin_position[2];
    launch_heading =
        FUN_001f9e90_2c1928(((float *)effective_target_position)[0] - origin_position[0],
                            ((float *)effective_target_position)[1] - origin_position[1]);
    if (effective_horizontal_distance < min_horizontal_distance)
        effective_horizontal_distance = min_horizontal_distance;
    if (effective_horizontal_distance <= dz)
        dz = effective_horizontal_distance - 0.01f;
    launch_component_speed_squared =
        gravity * effective_horizontal_distance * effective_horizontal_distance /
        ((effective_horizontal_distance - dz) + (effective_horizontal_distance - dz));
    if (half < launch_component_speed_squared)
        launch_component_speed_squared = half;
    launch_component_speed = FUN_001f9988_2c1928(launch_component_speed_squared);
    out[0] = FUN_001f9dc8_2c1928(launch_heading) * launch_component_speed;
    out[1] = FUN_001f9de0_2c1928(launch_heading) * launch_component_speed;
    out[2] = launch_component_speed;
}

/* Mine glove update: aims the glove and throws a mine at the target. */
void mine_glove_update(struct Moby *moby) __asm__("FUN_L00_002c1ad0");

void mine_glove_update(struct Moby *moby) {
    enum {
        MINE_GLOVE_INITIALIZE = 0,
        MINE_GLOVE_WAIT_FOR_ANIMATION = 1,
        MINE_GLOVE_RELEASE_HELD_MOBY = 3,
        MINE_GLOVE_POST_RELEASE_WAIT = 4,
        MINE_GLOVE_BEGIN_HELD_MOBY_REMOVAL = 5,
        MINE_GLOVE_REMOVE_HELD_MOBY = 6
    };
    GloveVars *vars;
    Vec4       saved, offset, rotatedOffset;
    Vec4       local[4];
    Vec4       pos, target;
    Vec4       basis[3];
    Vec4       matrix[4];
    Vec4       launch, arc, probe;
    float     *out;
    float      angle, slope, horizontal, vertical, radius, root;
    s32        hit;

    if (moby == 0)
        return;
    vars = (GloveVars *)moby->pvars;
    if (vars->delay < 3) {
        ++vars->delay;
        return;
    }
    offset.q = 0;
    offset.f[1] = -0.09f;
    offset.f[2] = -0.02f;
    copy_matrix3x4(basis, l00_camera.unk0);
    transpose_rotation_matrix(matrix, basis);
    if (vars == 0)
        return;
    FUN_L00_0024f7c8(moby, 0, &saved);
    build_moby_bone_transform(moby, 0, local);
    transform_vector(&rotatedOffset, &offset, local);
    fast_vec_add(&vars->grip, &saved, &rotatedOffset);
    if (vars->held != 0)
        vars->held->collision_disable = 1;
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD)
        vars->held = 0;
    if (hero.state.current == 1)
        hero_set_state(0x1E, 1);
    if (fast_dec_timer(&vars->timer58) && !hero.item_use_locked) {
        if ((hero.item_anim_active && hero.item_anim_frames == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) && hero.state.current == 0x1E &&
             get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            allocate_hero_voice_for_entry(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = 3;
        }
    }
    if (vars->held != 0 && !hero.item_use_locked) {
        out = (float *)vars->held->pvars;
        arc.q = 0;
        arc.f[0] = frame_time_macro * (item_text_variant[0x11]
                                           ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                           : ((controller_state.held & 5) ? 6.0f : 4.0f));
        launch.q = arc.q;
        transform_vector_by_basis(&launch, &launch, hero.basis);
        pos.f[0] = fast_cos(fast_add_rotations(-0.361968011f,
                                               fast_arc_tan(hero.facing_x, hero.facing_y))) *
                   0.859039009f;
        pos.f[1] = fast_sin(fast_add_rotations(-0.361968011f,
                                               fast_arc_tan(hero.facing_x, hero.facing_y))) *
                   0.859039009f;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &hero.motion.pos);
        pos.f[2] += 0.490819991f;
        angle = fast_arc_tan(launch.f[0], launch.f[1]);
        fast_vec_scale(&target, &launch, frame_scale_inv_macro * 60.0f);
        fast_vec_add(&target, &target, &pos);
        if (controller_state.held & 5) {
            probe.q = 0;
            probe.f[2] = item_text_variant[0x11] ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                                 : ((controller_state.held & 5) ? 6.0f : 4.0f);
            arc.q = probe.q;
            hit = 0;
            transform_vector(&arc, &arc, matrix);
            fast_subtract_rotations(hero.moby->rot.z, fast_arc_tan(arc.f[0], arc.f[1]));
            angle = fast_arc_tan(arc.f[0], arc.f[1]);
            slope = fast_arc_tan(vector_length_xy(&arc), arc.f[2]);
            if (0.593411922f < slope)
                slope = 0.593411922f;
            if (slope < -1.39626336f)
                slope = -1.39626336f;
            build_spherical_offset(&arc,
                                   item_text_variant[0x11]
                                       ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                       : ((controller_state.held & 5) ? 6.0f : 4.0f),
                                   angle,
                                   slope);
            {
                float distance = vector_length_xy(&arc);
                float z = arc.f[2];
                if (distance != 0.0f)
                    horizontal = z / distance;
                else
                    horizontal = z / 0.01f;
            }
            root = square_root(frame_time_sq_macro * 9.8f *
                               (item_text_variant[0x11]
                                    ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                    : ((controller_state.held & 5) ? 6.0f : 4.0f)) *
                               0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 9.8f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                arc.f[0] = fast_cos(angle) * radius;
                arc.f[1] = fast_sin(angle) * radius;
                arc.f[2] = vertical;
                fast_vec_add(&probe, &D_L00_00166DC0, &arc);
                if (collision_line(&D_L00_00166DC0, &probe, 2, moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    qcopy(&target, &coll_hit.point);
                    hit = 1;
                    angle = fast_arc_tan(target.f[0] - pos.f[0], target.f[1] - pos.f[1]);
                }
                if (!hit) {
                    target.f[0] = fast_cos(angle) * radius * 5.0f;
                    target.f[1] = fast_sin(angle) * radius * 5.0f;
                    target.f[2] = vertical * 5.0f;
                    fast_vec_add(&target, &target, &D_L00_00166DC0);
                }
            }
        }
        FUN_L00_002c1928_f(frame_time_sq_macro * 9.8f,
                           1.0f,
                           item_text_variant[0x11] ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                                   : ((controller_state.held & 5) ? 6.0f : 4.0f),
                           pos.f,
                           target.f,
                           out);
    }
    switch (moby->state) {
    case MINE_GLOVE_INITIALIZE:
        vars->held = 0;
        if (!(moby->anim_flags & 2))
            moby->state = 1;
        else
            moby->state = 2;
    case MINE_GLOVE_WAIT_FOR_ANIMATION:
        if (moby->anim_flags & 2)
            moby->state = 2;
        break;
    case MINE_GLOVE_RELEASE_HELD_MOBY:
        if (vars->held != 0) {
            initialize_moby_motion_at_position(&pos, vars->held, vars->held->pvars);
            if (((DeliveryStats_2c1ad0 *)item_usage)->deliveries <= 0xfffe)
                ++((DeliveryStats_2c1ad0 *)item_usage)->deliveries;
            {
                s32 frames = scale_game_frames(D_0015EEA4_macro);
                frames /= 600;
                if (((DeliveryStats_2c1ad0 *)item_usage)->unkA2 < frames)
                    ((DeliveryStats_2c1ad0 *)item_usage)->unkA2 =
                        scale_game_frames(D_0015EEA4_macro) / 600;
            }
            ((DeliveryStats_2c1ad0 *)item_usage)->levels =
                (((DeliveryStats_2c1ad0 *)item_usage)->levels | (1u << current_level_index_macro)) |
                0x80000000;
            append_unique_moby_index(vars->held, D_L00_001B07B0, 0x3F);
            vars->held = 0;
            vars->timer54 = scale_game_frames(10);
        }
        moby->state = 4;
        vars->timer58 = scale_game_frames(0x14);
        break;
    case MINE_GLOVE_POST_RELEASE_WAIT:
        if (hero.state.current != 0x23 && !hero.item_anim_active)
            moby->state = 2;
        break;
    case MINE_GLOVE_BEGIN_HELD_MOBY_REMOVAL:
        moby->state = 6;
        break;
    case MINE_GLOVE_REMOVE_HELD_MOBY:
        if (vars->held != 0) {
            mark_moby_for_removal(vars->held);
            vars->held = 0;
        }
        break;
    }
    fast_dec_timer(&vars->timer54);
    if (vars->held != 0) {
        qcopy(&vars->held->pos, &vars->grip);
    } else if (vars->timer54 == 0 && get_ammo(-1)) {
        qzero(&launch);
        vars->held = spawn_mine(moby, &vars->grip, &launch);
    }
}

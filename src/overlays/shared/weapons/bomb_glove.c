/* Shared moby class bomb_glove: the bomb glove held by the hero, which aims the throw arc and throws bombs. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef union {
    OvlQuad q;
    f32     f[4];
    s32     i[4];
} V2c2488;

typedef struct {
    u8           pad0[0x40];
    OvlVec4      grip;
    struct Moby *held;
    s32          timer;
    s32          delay;
} CarrierVars_2c26c8;

typedef struct {
    u8  pad0[0x50];
    u16 deliveries; /* D_001416D0: saturates at 0xFFFF */
    u16 unkA2;
    u32 levels;
} DeliveryStats_2c26c8;

extern void         fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int          scale_game_frames(int) __asm__("FUN_001f96f8");
void                fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern f32          fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32          fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32          vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern s32          collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
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
extern void         sub_2c(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32          scale_ticks(s32) __asm__("FUN_001fa6c0");
extern f32          FUN_L00_0025abf0(void *, void *, f32, f32, s32);
extern struct Moby *spawn_bomb(struct Moby *, void *) __asm__("FUN_L00_002ace70");
extern void         launch_bomb(void *, struct Moby *, void *) __asm__("FUN_L00_002acfd8");

extern CollisionHit   coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern f32 frame_time MACRO_ADDR;
extern f32            frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern s32            D_0015EEA4;
extern u32 D_0013CAE4 NOT_SDA;
extern LevelCamera    l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;
extern CollisionHit   coll_hit __asm__("D_L00_00173E40");
extern s32            pad_held_lui[] __asm__("D_0013CAE0") NOT_SDA;
extern u8             bomb_glove_aimed_range_modifier[] __asm__("D_0013E52A");
extern struct Moby   *D_L00_001ABA00_lui[] __asm__("D_L00_001ABA00") NOT_SDA;
extern float          frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

/*
 * TODO(route): this body is DONE - it is byte-identical to retail under the
 * bundled GNU as (572/572 bytes; the only 23 differing words are relocation
 * sites: jal/lui/%lo with 0 addends). The current game route (cc1 -> Ps2EeAs)
 * reports SIZE 580/572 because Ps2EeAs 1.9.25.758 auto-inserts 2 NOPs before a
 * div.s that sits "too near a branch instruction / possible branch
 * destination" (its divbug workaround), and the else-block div.s here is right
 * after the $L7 label. Retail never has that padding: a byte scan of the whole
 * retail corpus found 0 `nop; nop; div` in 346 div words of SCUS_971.99 and
 * 26,539 div words across all 19 level text.bin - so the retail-era assembler
 * did not apply this workaround, and our vendored Ps2EeAs is from a different
 * (later) build.
 *
 * To make this function exact, whoever owns the route must first fix the
 * toolchain mismatch; this source needs no further change. Options, both
 * fail-closed (strip only NOPs that Ps2EeAs inserted relative to the GNU-as
 * reference, and only when the trimmed stream still explains every retail
 * word; validate with the whole-ELF gate):
 *   (a) in the game repo's padless-asm.py `finish` step (and the tools'
 *       overlay checker), drop the assembler-inserted div padding using the
 *       GNU-as reference object assembled from the same .s; or
 *   (b) assemble overlays with the bundled GNU as instead of Ps2EeAs, if a
 *       corpus check shows the overlays contain no Ps2EeAs-only output
 *       (short-loop / FPU hazard NOPs) - those ARE in retail and must stay.
 * Evidence and numbers: analysis/overlays/ps2eeas-div-padding.md (tools repo).
 * After the route is fixed: re-score this file, expect EXACT 572/572, then
 * promote normally. Do NOT add inline asm or a per-unit exception for this.
 */
void aim_throw_arc(V2c2488 *pos, V2c2488 *tgt, f32 *out, f32 speed, f32 unused, f32 range) __asm__("FUN_L00_002c2488");

void aim_throw_arc(V2c2488 *origin_position,
                   V2c2488 *target_position,
                   f32     *out,
                   f32      speed,
                   f32      unused,

                   f32 range) {
    V2c2488      effective_target_position;
    V2c2488      u;
    f32          launch_heading;
    f32          horizontal_speed;
    f32          k;
    f32          v;
    struct Hero *P;
    if (range < fast_vec_dist(origin_position, target_position)) {
        sub_2c(&effective_target_position, target_position, origin_position);
        fast_vec_normalize(&effective_target_position, &effective_target_position, range - 0.01f);
        fast_vec_add(&effective_target_position, &effective_target_position, origin_position);
    } else {
        effective_target_position.q = target_position->q;
    }
    launch_heading = fast_arc_tan(effective_target_position.f[0] - origin_position->f[0],
                                  effective_target_position.f[1] - origin_position->f[1]);
    P = &hero;
    if (fast_difference_between_rotations(P->motion.rot.f[2], launch_heading) > 0.87266463f ||
        vector_distance(origin_position, &effective_target_position) < 1.5f) {
        launch_heading = P->motion.rot.f[2];
        u.f[0] = fast_cos(launch_heading) * 1.5f;
        u.f[1] = fast_sin(P->motion.rot.f[2]) * 1.5f;
        u.i[2] = 0;
        fast_vec_add(&u, &u, origin_position);
        effective_target_position.f[0] = u.f[0];
        effective_target_position.f[1] = u.f[1];
    }
    horizontal_speed = frame_time * 8.5f;
    out[0] = fast_cos(launch_heading) * horizontal_speed;
    out[1] = fast_sin(launch_heading) * horizontal_speed;
    *(s32 *)&out[2] = 0;
    out[2] =
        FUN_L00_0025abf0(origin_position, &effective_target_position, horizontal_speed, -speed, 0);
    if (pad_held_lui[0] & 5) {
        k = scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f;
        k = k / horizontal_speed;
    } else {
        k = 8.5f / horizontal_speed;
    }
    v = speed * k * 0.5f;
    if (v < out[2]) {
        out[2] = v;
    }
}

/* Updates the grip, chooses a throw target, and handles the held moby states. */
void bomb_glove_update(struct Moby *moby) __asm__("FUN_L00_002c26c8");

void bomb_glove_update(struct Moby *moby) {
    enum {
        BOMB_GLOVE_INITIALIZE = 0,
        BOMB_GLOVE_WAIT_FOR_ANIMATION = 1,
        BOMB_GLOVE_RELEASE_HELD_MOBY = 3,
        BOMB_GLOVE_POST_RELEASE_WAIT = 4,
        BOMB_GLOVE_BEGIN_HELD_MOBY_REMOVAL = 5,
        BOMB_GLOVE_REMOVE_HELD_MOBY = 6
    };
    CarrierVars_2c26c8 *vars;
    OvlVec4             local[4], saved, offset, rotatedOffset, pos, target, basis[3], matrix[4];
    OvlVec4             launch;
    union {
        OvlVec4 arc;
    } temporary;
    OvlVec4 direction;
    OvlVec4 probe;
    f32     angle, slope, distance, horizontal, vertical, radius, speed;
    f32     gravity, root;
    s32     hit = 0;

    if (moby == 0)
        return;
    vars = (CarrierVars_2c26c8 *)moby->pvars;
    offset.q = 0;
    offset.f[1] = -0.09f;
    offset.f[2] = -0.02f;
    copy_matrix3x4(basis, l00_camera_lui.unk0);
    transpose_rotation_matrix(matrix, basis);
    if (vars == 0)
        return;
    if (vars->delay < 3) {
        ++vars->delay;
        return;
    }
    FUN_L00_0024f7c8((u8 *)moby, 0, &saved);
    build_moby_bone_transform(moby, 0, local);
    transform_vector(&rotatedOffset, &offset, local);
    fast_vec_add(&vars->grip, &saved, &rotatedOffset);
    if (vars->held != 0)
        vars->held->collision_disable = 1;
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD)
        vars->held = 0;
    if (hero.state.current == 1)
        hero_set_state(0x1E, 1);
    if (fast_dec_timer(&vars->timer) && !hero.item_use_locked) {
        if ((hero.item_anim_active && hero.item_anim_frames == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) && hero.state.current == 0x1E &&
             get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            allocate_hero_voice_for_entry(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = 3;
        }
    }
    if (vars->held != 0) {
        f32  best = 15.0f;
        f32 *out = (f32 *)vars->held->pvars;
        launch.q = 0;
        launch.f[0] = 1.0f;
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
        {
            f32 speed = (controller_state.held & 5)
                            ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                            : 8.5f;
            fast_vec_scale(&target, &launch, speed);
        }
        fast_vec_add(&target, &target, &pos);
        if ((controller_state.held & 5) && !hero.item_use_locked) {
            temporary.arc.q = 0;
            temporary.arc.f[2] = 1.0f;
            transform_vector(&temporary.arc, &temporary.arc, matrix);
            hit = 0;
            fast_subtract_rotations(hero.moby->rot.z,
                                    fast_arc_tan(temporary.arc.f[0], temporary.arc.f[1]));
            angle = fast_arc_tan(temporary.arc.f[0], temporary.arc.f[1]);
            slope = fast_arc_tan(vector_length_xy(&temporary.arc), temporary.arc.f[2]);
            if (0.593411922f < slope)
                slope = 0.593411922f;
            if (slope < -1.39626336f)
                slope = -1.39626336f;
            {
                f32 speed = (controller_state.held & 5)
                                ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                                : 8.5f;
                build_spherical_offset(&temporary.arc, speed, angle, slope);
            }
            qcopy(&direction, (OvlVec4 *)g_debug_camera.position);
            {
                f32 distance = vector_length_xy(&temporary.arc);
                f32 z = temporary.arc.f[2];
                if (distance != 0.0f)
                    horizontal = z / distance;
                else
                    horizontal = z / 0.01f;
            }
            gravity = frame_time_sq_macro * 11.0f;
            root = square_root(gravity *
                               ((controller_state.held & 5)
                                    ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                                    : 8.5f) *
                               0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 11.0f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                temporary.arc.f[0] = fast_cos(angle) * radius;
                temporary.arc.f[1] = fast_sin(angle) * radius;
                temporary.arc.f[2] = vertical;
                fast_vec_add(&probe, (OvlVec4 *)g_debug_camera.position, &temporary.arc);
                if (collision_line((OvlVec4 *)g_debug_camera.position, &probe, 2, moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    qcopy(&target, &coll_hit.point);
                    hit = 1;
                    angle = fast_arc_tan(target.f[0] - pos.f[0], target.f[1] - pos.f[1]);
                }
                if (!hit) {
                    target.f[0] = fast_cos(angle) * radius * 5.0f;
                    target.f[1] = fast_sin(angle) * radius * 5.0f;
                    target.f[2] = vertical * 5.0f;
                    fast_vec_add(&target, &target, (OvlVec4 *)g_debug_camera.position);
                }
            }
        } else {
            s32          entry = 0;
            struct Moby *candidate;
            hero.throw_target = 0;
            while ((candidate = D_L00_001ABA00_lui[entry]) != 0) {
                u8 *data = suck_get_aim_height(candidate);
                if ((s8)candidate->state >= 0 && data && (candidate->flags & 0x1000)) {
                    f32 candidateAngle, elevation, difference;
                    qcopy(&temporary.arc, &candidate->pos);
                    temporary.arc.f[2] += *(f32 *)(data + 0x10);
                    distance = vector_distance(&pos, &temporary.arc);
                    candidateAngle =
                        fast_arc_tan(temporary.arc.f[0] - pos.f[0], temporary.arc.f[1] - pos.f[1]);
                    elevation = fast_arc_tan(vector_distance(&pos, &temporary.arc),
                                             temporary.arc.f[2] - pos.f[2]);
                    difference = fast_difference_between_rotations(angle, candidateAngle);
                    if ((difference < 0.785398185f ||
                         (distance < 4.0f && difference < 1.57079637f)) &&
                        distance < best && elevation < 0.959931076f &&
                        temporary.arc.f[2] - pos.f[2] < 2.0f &&
                        !collision_line(
                            (OvlVec4 *)g_debug_camera.position, &temporary.arc, 6, moby, 0)) {
                        best = distance;
                        hero.throw_target = (u8 *)candidate;
                        angle = candidateAngle;
                        qcopy(&target, &temporary.arc);
                    }
                }
                ++entry;
            }
        }
        gravity = frame_time_sq_macro * 11.0f;
        speed = (controller_state.held & 5)
                    ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                    : 8.5f;
        aim_throw_arc((V2c2488 *)&pos, (V2c2488 *)&target, out, gravity, 0.00000999999975f, speed);
    }
    switch (moby->state) {
    case BOMB_GLOVE_INITIALIZE:
        vars->held = 0;
        if (!(moby->anim_flags & 2))
            moby->state = 1;
        else
            moby->state = 2;
    case BOMB_GLOVE_WAIT_FOR_ANIMATION:
        if (moby->anim_flags & 2)
            moby->state = 2;
        break;
    case BOMB_GLOVE_RELEASE_HELD_MOBY:
        if (vars->held == 0) {
            vars->held = spawn_bomb(moby, &vars->grip);
        } else {
            launch_bomb(&pos, vars->held, vars->held->pvars);
            if (((DeliveryStats_2c26c8 *)item_usage)->deliveries <= 0xfffe)
                ++((DeliveryStats_2c26c8 *)item_usage)->deliveries;
            {
                s32 frames = scale_game_frames(D_0015EEA4);
                frames /= 600;
                if (((DeliveryStats_2c26c8 *)item_usage)->unkA2 < frames)
                    ((DeliveryStats_2c26c8 *)item_usage)->unkA2 =
                        scale_game_frames(D_0015EEA4) / 600;
            }
            ((DeliveryStats_2c26c8 *)item_usage)->levels =
                (((DeliveryStats_2c26c8 *)item_usage)->levels | (1u << current_level_index)) |
                0x80000000;
            vars->held = 0;
            moby->state = 4;
            vars->timer = scale_game_frames(0x14);
        }
        break;
    case BOMB_GLOVE_POST_RELEASE_WAIT:
        if (hero.state.current != 0x23 && !hero.item_anim_active)
            moby->state = 2;
        break;
    case BOMB_GLOVE_BEGIN_HELD_MOBY_REMOVAL:
        moby->state = 6;
        break;
    case BOMB_GLOVE_REMOVE_HELD_MOBY:
        /* Retail reloads held after destruction before the common tail. */
        if (vars->held != 0)
            mark_moby_for_removal(vars->held);
        break;
    }
    if (vars->held != 0)
        qcopy(&vars->held->pos, &vars->grip);
    else if (get_ammo(-1) || hero.item_anim_active)
        vars->held = spawn_bomb(moby, &vars->grip);
}

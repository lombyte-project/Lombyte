/* Level 10 moby class 1199, the darter (the bat-like Orxon flyer): rises from its spawn, wanders, chases and dives at its target. */
#include "types.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l10/mobys/aligned_vector.h"

typedef struct Moby Moby;
extern struct Moby *D_L10_0015FFD0_macro[] __asm__("D_L10_0015FFD0") MACRO_ADDR;
extern float        D_0015ED6C;
extern float        D_0015ED70;
extern void func_L00_00264B40_2E0630(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void func_L00_0025B178_2E0630(void *) __asm__("FUN_L00_0025a120");
extern float
advance_accelerated_scalar(float *, float, float *, float, float, float) __asm__("FUN_00213f38");
extern float
func_L00_0025CE58_2E0630(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern void func_L00_002607A8_2E0630(void *, float) __asm__("FUN_L00_0025f730");
extern int
func_L00_00259B08_2E0630(void *, void *, float, int, float, int) __asm__("FUN_L00_00258ad0");
extern float func_L00_0025CC58_2E0630(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern void  approach_w(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void
spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern AlignedVec4f D_L10_00167240;

extern char  D_0013F3D0[];
extern int   D_001413D0[];
extern float D_0015ED6C, D_0015ED70;
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void func_L00_00261B00_2E2B80(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern int  ahead_f(float, void *, void *) __asm__("FUN_L01_00274b78");
extern int
activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");

extern float  scale_time(float) __asm__("FUN_001f96b0");
extern short *moby_group_member_lists[] __asm__("D_L10_001ABCC0");
/* Darter update (class 1199): rises from its spawn, wanders, chases and dives at its target, then dies. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E1568), where it is exact; names translated to the US level program. */

typedef struct Hero Hero_2e01a8;

typedef struct Vars_2e01a8 {
    char          p0[0x20];
    float         f20;
    short         h24;
    char          p26[2];
    unsigned char b28;
    unsigned char b29;
    char          p2A[6];
    float         f30;
    char          p34[0x24];
    unsigned char b58;
    char          p59;
    unsigned char b5A;
    char          p5B[5];
    char          a60[0x80];
    AlignedVec4f  vE0;
    char          pF0[0x10];
    Moby         *target;
    int           f104;
    char          p108[0x18];
    AlignedVec4f  velocity;
    AlignedVec4f  home;
    AlignedVec4f  goal;
    char          p150[0x20];
    float         turn_speed;
    int           spawn_kind;
    char          p178[0x18];
    float         side_angle;
    int           f194;
    int           wait;
    float         bob_angle;
    float         height_offset;
    int           height_timer;
    char          p1A8[0xC8];
    char          a270[4];
} Vars_2e01a8;

extern float D_L10_00161DFC __attribute__((sda));
extern float D_L10_00161E04 __attribute__((sda));
extern float D_L10_00161E08 __attribute__((sda));
extern float D_L10_00161E0C __attribute__((sda));
extern float D_L10_00161E10 __attribute__((sda));
extern float D_L10_00161E14 __attribute__((sda));
extern float D_L10_00161E18 __attribute__((sda));
extern float D_L10_00161E1C __attribute__((sda));
extern float D_L10_00161E20 __attribute__((sda));
extern int   coll_sphere_mobys(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int   FUN_L00_001fefc8(void *);
extern void  FUN_L00_0024f7c8(void *, int, void *);
s32          is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void         update_darter_hit_reactions_and_targeting(Moby *moby) __asm__("FUN_L10_002e17c0");
void         FUN_L10_002e1cb0_2e01a8(unsigned char *moby) __asm__("FUN_L10_002e1cb0");

void update_darter_hit_reactions_and_targeting(struct Moby *moby) __asm__("FUN_L10_002e17c0");
extern float        fast_cos(float);
extern float        fast_sin(float);
typedef struct Hero Hero;
extern float        fast_vec_dist(void *, void *);

void darter_update(Moby *moby) __asm__("FUN_L10_002e01a8");

/* Moby class 1199 */
void darter_update(Moby *moby) {
    float        scratch[20];
    Vars_2e01a8 *vars = (Vars_2e01a8 *)moby->pvars;
    Moby        *target;

    update_darter_hit_reactions_and_targeting(moby);
    target = vars->target;
    func_L00_00264B40_2E0630(2.5f, moby, 1, vars->a270);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, &D_L10_00167240) < 29.0f) {
        func_L00_0025B178_2E0630(moby);
        moby->shadow_range = 0x17;
    }
    moby->anim_speed = 1.0f;
    if (moby->state != 0 && moby->state != 13) {
        moby->pos.z = moby->pos.z - D_L10_00161E0C * fast_cos(vars->bob_angle);
    }
    switch (moby->state) {
    case 0:
        vars->f30 = 0.25f;
        vars->f20 = 1.0f;
        vars->h24 = 1;
        vars->b28 = 0;
        vars->b29 = 1;
        moby->flags |= 0x1000;
        qcopy(&vars->home, &moby->pos);
        qcopy(&vars->goal, &vars->home);
        vars->bob_angle = random_angle_radians();
        vars->turn_speed = 0.0f;
        vars->b58 = truncate_float_to_s32(D_L10_00161E1C);
        vars->b5A = truncate_float_to_s32(D_L10_00161E20 * 8.0f);
        switch (vars->spawn_kind) {
        case 0:
            moby->state = 3;
            moby->pos.z = moby->pos.z + 2.0f;
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            break;
        case 1:
            mark_moby_for_removal(moby);
            return;
        case 2:
            scratch[0] = fast_cos(random_angle_radians()) * 10.0f;
            scratch[1] = fast_sin(random_angle_radians()) * 10.0f;
            moby->state = 9;
            scratch[2] = 0.0f;
            moby->pos.z = l10_cuboids[vars->f194].pos.z;
            add_vectors(&scratch[4], &moby->pos, scratch);
            vars->height_offset = random_float_between(0.0f, D_L10_00161E14);
            vars->height_timer = truncate_float_to_s32(scale_time(D_L10_00161E18 * 60.0f) *
                                                       random_float_between(0.75f, 1.25f));
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            break;
        }
        break;
    case 1:
        break;
    case 2:
        moby->anim_speed = 4.0f;
        if (advance_accelerated_scalar(&moby->pos.z,
                                       vars->home.z + 2.0f,
                                       &vars->velocity.z,
                                       D_0015ED70 * 18.0f,
                                       D_0015ED70 * 18.0f,
                                       frame_time * 6.0f) == 0.0f) {
            moby->state = 3;
        }
        break;
    case 3: {
        float dist;
        float travelled;
        float speed;
        float zero;
        moby->anim_speed = 4.0f;
        speed = fast_vec_length(&vars->velocity);
        dist = fast_vec_dist(&vars->goal, &moby->pos);
        travelled = 0.0f;
        func_L00_0025CE58_2E0630(
            &moby->rot.z,
            fast_arc_tan(vars->goal.x - moby->pos.x, vars->goal.y - moby->pos.y),
            &vars->turn_speed,
            D_0015ED70 * 25.132742f,
            D_0015ED70 * 25.132742f,
            frame_time * 50.265484f);
        if (fast_difference_between_rotations(
                fast_arc_tan(vars->goal.x - moby->pos.x, vars->goal.y - moby->pos.y), moby->rot.z) <
            0.78539819f) {
            advance_accelerated_scalar(&travelled,
                                       dist,
                                       &speed,
                                       D_0015ED70 * 20.0f,
                                       D_0015ED70 * 20.0f,
                                       frame_time * 20.0f);
            fast_vec_sub(&vars->velocity, &vars->goal, &moby->pos);
            func_L00_002607A8_2E0630(&vars->velocity, speed);
        } else {
            clear_u64_value(&vars->velocity);
        }
        zero = 0.0f;
        if (func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, zero, 0) == 1) {
            float radius = random_float_between(4.0f, 6.0f);
            float angle = random_angle_radians();
            scratch[0] = fast_cos(angle) * radius;
            scratch[1] = fast_sin(angle) * radius;
            scratch[2] = zero;
            scratch[2] = random_float_between(D_L10_00161E04, D_L10_00161E08);
            fast_vec_add(&vars->goal, scratch, &vars->home);
        } else if (dist < 0.1f) {
            moby->state = 4;
            vars->wait =
                scale_game_frames(truncate_float_to_s32(random_float_between(30.0f, 90.0f)));
        }
        if (vars->f104 == 2)
            break;
        if (!(3.0f < vector_distance_xy(&moby->pos, &target->pos)))
            break;
        if (FUN_L00_001fefc8(&moby->command) == 0)
            break;
        if (moby->group != 0xFF)
            FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        moby->state = 6;
        if (moby->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        break;
    }
    case 4:
        moby->anim_speed = 4.0f;
        if (vars->f104 != 2 && 3.0f < vector_distance_xy(&moby->pos, &target->pos) &&
            FUN_L00_001fefc8(&moby->command) != 0) {
            if (moby->group != 0xFF)
                FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
            moby->state = 6;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            break;
        }
        if (fast_dec_timer(&vars->wait) == 0)
            break;
        {
            float radius = random_float_between(4.0f, 6.0f);
            float angle = random_angle_radians();
            scratch[0] = fast_cos(angle) * radius;
            scratch[1] = fast_sin(angle) * radius;
            scratch[2] = 0.0f;
            scratch[2] = random_float_between(D_L10_00161E04, D_L10_00161E08);
            fast_vec_add(&vars->goal, scratch, &vars->home);
            moby->state = 3;
        }
        break;
    case 6:
        approach_w(moby,
                   fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 6.2831855f);
        if ((moby->anim_flags & 2) == 0)
            break;
        moby->state = 7;
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        if (random_integer_below(0xFF) & 1) {
            vars->side_angle = 0.2617994f;
        } else {
            vars->side_angle = -0.2617994f;
        }
        break;
    case 7: {
        float lim;
        approach_w(moby,
                   fast_add_rotations(
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       vars->side_angle),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 6.2831855f);
        lim = probe_ground_height(&moby->pos, 0, 0.5f);
        qcopy(scratch, &target->pos);
        if (scratch[2] < moby->pos.z) {
            scratch[2] = scratch[2] - (moby->pos.z - scratch[2]) * 4.0f;
        } else {
            scratch[2] = moby->pos.z;
        }
        scratch[2] = scratch[2] + 0.5f;
        if (scratch[2] < lim)
            scratch[2] = lim;
        fast_vec_sub(&vars->velocity, scratch, &moby->pos);
        fast_vec_normalize(&vars->velocity, &vars->velocity, frame_time * 10.0f);
        vars->velocity.x = fast_cos(moby->rot.z) * vector_length_xy(&vars->velocity);
        vars->velocity.y = fast_sin(moby->rot.z) * vector_length_xy(&vars->velocity);
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        func_L00_0025CC58_2E0630(&moby->rot.y,
                                 0,
                                 -fast_arc_tan(vector_length_xy(&vars->velocity), vars->velocity.z),
                                 frame_time * 6.2831855f);
        if (vector_distance_xy(&moby->pos, &target->pos) < D_L10_00161DFC) {
            if (absolute_float(moby->pos.z - ((Hero_2e01a8 *)((char *)&hero))->motion.pos.f[2]) <
                2.0f) {
                allocate_voice_for_target_entry(0, 0, moby);
                moby->state = 8;
                clear_u64_value(&vars->velocity);
                if (moby->prev_seq != 2)
                    blend_moby_animation(moby, 2, 4, scale_game_frames(5));
                break;
            }
        } else if (vars->f104 != 2) {
            break;
        }
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
        moby->state = 5;
        break;
    }
    case 8:
        approach_w(moby,
                   fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 6.2831855f);
        func_L00_0025CC58_2E0630(
            &moby->rot.y,
            0,
            -fast_arc_tan(vector_distance_xy(&moby->pos, &vars->vE0), vars->vE0.z - moby->pos.z),
            frame_time * 12.566371f);
        if (is_value_within_interpolated_window((char *)moby, 20.0f)) {
            float one = 1.0f;
            *(int *)&scratch[13] = 1;
            *(int *)&scratch[16] = 1;
            *(Moby **)&scratch[12] = moby;
            scratch[15] = one;
            scratch[8] = fast_cos(moby->rot.z);
            scratch[9] = fast_sin(moby->rot.z);
            ((unsigned char *)scratch)[0x39] = 1;
            scratch[10] = one;
            scratch[11] = 5627.9248f;
            *(short *)((char *)scratch + 0x3A) = moby->oclass;
            ((unsigned char *)scratch)[0x38] = 0;
            FUN_L00_0024f7c8(moby, 0, scratch);
            coll_sphere_mobys(0.4f, scratch, 0, moby, &scratch[8]);
            collision_line(&moby->pos, scratch, 9, moby, &scratch[8]);
        }
        if (vars->f104 != 2 && (moby->anim_flags & 2) == 0)
            break;
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
        moby->state = 5;
        break;
    case 5: {
        float zero = 0.0f;
        float climb;
        func_L00_0025CC58_2E0630(&moby->rot.y, 0, zero, frame_time * 12.566371f);
        approach_w(moby,
                   fast_arc_tan(vars->home.x - moby->pos.x, vars->home.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 6.2831855f);
        vars->velocity.x = fast_cos(moby->rot.z) * (frame_time * 6.0f);
        vars->velocity.y = fast_sin(moby->rot.z) * (frame_time * 6.0f);
        vars->velocity.z = zero;
        climb = vars->home.z + 2.0f - moby->pos.z;
        if (frame_time * 6.0f < climb) {
            climb = frame_time * 6.0f;
        } else if (climb < frame_time * -6.0f) {
            climb = frame_time * -6.0f;
        }
        vars->velocity.z = climb;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        if (vector_distance_xy(&moby->pos, &vars->home) < 2.0f)
            moby->state = 3;
        break;
    }
    case 9: {
        float dist = fast_vec_dist(&moby->pos, &target->pos);
        float climb;
        approach_w(moby,
                   fast_arc_tan(vars->home.x - moby->pos.x, vars->home.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 0.78539819f);
        vars->velocity.x = fast_cos(moby->rot.z) * (frame_time * 4.0f);
        vars->velocity.y = fast_sin(moby->rot.z) * (frame_time * 4.0f);
        vars->velocity.z = 0.0f;
        if (fast_dec_timer(&vars->height_timer)) {
            vars->height_offset = random_float_between(0.0f, D_L10_00161E14);
            vars->height_timer = truncate_float_to_s32(scale_time(D_L10_00161E18 * 60.0f) *
                                                       random_float_between(0.75f, 1.25f));
        }
        climb = l10_cuboids[vars->f194].pos.z - moby->pos.z - vars->height_offset;
        if (frame_time + frame_time < climb) {
            climb = frame_time + frame_time;
        } else if (climb < frame_time * -2.0f) {
            climb = frame_time * -2.0f;
        }
        vars->velocity.z = climb;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        if (vars->f104 == 2)
            break;
        if (FUN_L00_001fefc8(&moby->command) == 0)
            break;
        if (!(D_L10_00161DFC < dist))
            break;
        if (moby->group != 0xFF)
            FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        moby->state = 10;
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        break;
    }
    case 10: {
        float dist = fast_vec_dist(&moby->pos, &target->pos);
        float climb;
        if (moby->group != 0xFF)
            FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        approach_w(moby,
                   fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 1.5707964f);
        vars->velocity.x = fast_cos(moby->rot.z) * (frame_time * 4.0f);
        vars->velocity.y = fast_sin(moby->rot.z) * (frame_time * 4.0f);
        vars->velocity.z = 0.0f;
        climb = l10_cuboids[vars->f194].pos.z - moby->pos.z;
        if (frame_time * 4.0f < climb) {
            climb = frame_time * 4.0f;
        } else if (climb < frame_time * -4.0f) {
            climb = frame_time * -4.0f;
        }
        vars->velocity.z = climb;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        if (dist < D_L10_00161DFC &&
            absolute_float(l10_cuboids[vars->f194].pos.z - moby->pos.z) < 1.0f) {
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 11;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(2));
            break;
        }
        if (vars->f104 != 2)
            break;
        moby->state = 9;
        vars->height_offset = random_float_between(0.0f, D_L10_00161E14);
        vars->height_timer = truncate_float_to_s32(scale_time(D_L10_00161E18 * 60.0f) *
                                                   random_float_between(0.75f, 1.25f));
        break;
    }
    case 11: {
        float climb;
        float zero;
        float one;
        if (moby->group != 0xFF)
            FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        approach_w(moby,
                   fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 6.2831855f);
        vars->velocity.x = fast_cos(moby->rot.z) * (frame_time * 6.0f);
        vars->velocity.y = fast_sin(moby->rot.z) * (frame_time * 6.0f);
        vars->velocity.z = 0.0f;
        climb = l10_cuboids[vars->f194].pos.z - moby->pos.z;
        if (frame_time * 3.0f < climb) {
            climb = frame_time * 3.0f;
        } else if (climb < frame_time * -3.0f) {
            climb = frame_time * -3.0f;
        }
        vars->velocity.z = climb;
        zero = 0.0f;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, zero, 0);
        one = 1.0f;
        *(int *)&scratch[9] = 0x10001;
        *(int *)&scratch[12] = 1;
        *(Moby **)&scratch[8] = moby;
        scratch[11] = one;
        clear_u64_value(&scratch[4]);
        FUN_L00_0024f7c8(moby, 0, scratch);
        coll_sphere_mobys(0.2f, scratch, 0, moby, &scratch[4]);
        if (fast_vec_dist(&moby->pos, &target->pos) < one ||
            *(int *)(((char *)&hero) + 0x208C) == 7 || (moby->anim_flags & 2)) {
            moby->state = 12;
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(2));
            break;
        }
        if (vars->f104 != 2)
            break;
        moby->state = 9;
        vars->height_offset = random_float_between(zero, D_L10_00161E14);
        vars->height_timer = truncate_float_to_s32(scale_time(D_L10_00161E18 * 60.0f) *
                                                   random_float_between(0.75f, 1.25f));
        break;
    }
    case 12: {
        float climb;
        approach_w(moby,
                   fast_arc_tan(vars->home.x - moby->pos.x, vars->home.y - moby->pos.y),
                   &vars->turn_speed,
                   0.02f,
                   0.3f,
                   frame_time * 1.0471976f);
        vars->velocity.x = fast_cos(moby->rot.z) * (frame_time * 4.0f);
        vars->velocity.y = fast_sin(moby->rot.z) * (frame_time * 4.0f);
        vars->velocity.z = 0.0f;
        climb = l10_cuboids[vars->f194].pos.z - moby->pos.z;
        if (frame_time * 3.0f < climb) {
            climb = frame_time * 3.0f;
        } else if (climb < frame_time * -3.0f) {
            climb = frame_time * -3.0f;
        }
        vars->velocity.z = climb;
        add_vectors(scratch, &moby->pos, &vars->velocity);
        if (vector_distance_xy(&moby->pos, &vars->home) < 2.0f) {
            moby->state = 9;
            moby->command = scale_game_frames(0x78);
            vars->height_offset = random_float_between(0.0f, D_L10_00161E14);
            vars->height_timer = truncate_float_to_s32(scale_time(D_L10_00161E18 * 60.0f) *
                                                       random_float_between(0.75f, 1.25f));
        }
        break;
    }
    case 13:
        if ((step_moby_motion(moby, vars->a60) & 1) == 0)
            break;
        mark_moby_for_removal_cf(moby, 0x200, -1);
        spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
        mark_moby_for_removal(moby);
        return;
    }
    if (moby->state != 0 && moby->state != 13) {
        vars->bob_angle = fast_add_rotations(
            vars->bob_angle,
            360.0f / (float)truncate_float_to_s32(scale_time(D_L10_00161E10 * 60.0f)) *
                0.017453292f);
        moby->pos.z = moby->pos.z + D_L10_00161E0C * fast_cos(vars->bob_angle);
    }
}

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E2B80), where it is exact; names translated to the US level program. */
#include "rnc/overlay/quad.h"

/* Level 10 moby update: tracks the owner's height and fires a shot, then sets the moby's display state. */

typedef struct Hit_2E2B80 {
    float        x;
    float        y;
    char         pad8[8];
    float        f10[4];
    struct Moby *moby;
    char         pad24[8];
    float        f2C;
    int          f30;
} Hit_2E2B80;

typedef struct MobyData {
    char          pad0[0x20];
    float         f20;
    char          pad24[2];
    short         f26;
    char          pad28[0x10];
    int           f38;
    char          pad3C[0x24];
    char          f60[0x10];
    float         f70;
    float         f74;
    float         f78;
    float         f7C;
    char          pad80[4];
    int           f84;
    char          pad88[0x15];
    unsigned char f9D;
    char          pad9E[0x22];
    char          fC0[0x40];
    int           f100;
    int           f104;
    char          pad108[8];
    char          f110[7];
    unsigned char f117;
    char          pad118[0x18];
    char          f130[0x44];
    int           f174;
    float         f178;
    int           f17C;
    float         f180;
    int           f184;
    float         f188;
    int           f18C;
    char          pad190[4];
    int           f194;
} MobyData;

extern int resolve_moby_hit_response() __asm__("FUN_00213928");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_anim_update(void *m, void *p) __asm__("FUN_L00_0025d458");

#include "rnc/data_labels.h"

void update_darter_hit_reactions_and_targeting(struct Moby *moby) {
    MobyData   *class_1199_vars = ((struct MobyData *)moby->pvars);
    float       vector_scratch[4];
    float       transformed_offset[4];
    int         hit_processing_status;
    float       knockback_angle;
    Hit_2E2B80 *hit_record;
    int         target_selection_status;

    if (class_1199_vars->f38 != 0) {
        class_1199_vars->f184 =
            truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        class_1199_vars->f38 = 0;
    }
    fast_dec_timer(&class_1199_vars->f184);
    if (moby->state != 13) {
        hit_record = find_moby_hit(moby, 0x330000, 0);
        resolve_moby_hit_response(
            moby, hit_record, &class_1199_vars->f20, 5, &hit_processing_status, 0, 0, 4);
        if (hit_record != 0 && hit_record->moby->oclass != moby->oclass) {
            if (class_1199_vars->f20 <= hit_record->f2C) {
                class_1199_vars->f20 = 0.0f;
                moby->flags &= 0xEFFF;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                moby->state = 13;
                class_1199_vars->f70 = D_0015ED70 * 26.0f;
                class_1199_vars->f9D = 3;
                class_1199_vars->f84 = 9;
                class_1199_vars->f74 = 0.0005f;
                class_1199_vars->f78 = D_0015ED6C * 7.0f;
                class_1199_vars->f7C = D_0015ED6C * 10.0f;
                allocate_voice_for_target_entry(2, 0, moby);
                if (hit_record->f30 & 1) {
                    knockback_angle = fast_arc_tan(hit_record->f10[0], hit_record->f10[1]);
                } else {
                    knockback_angle =
                        fast_arc_tan(moby->pos.x - hit_record->x, moby->pos.y - hit_record->y);
                }
                *(OvlQuad *)vector_scratch = *(OvlQuad *)hit_record->f10;
                trooper_knockback_arc(
                    vector_scratch, &knockback_angle, &class_1199_vars->f78, &class_1199_vars->f7C);
                trooper_start_knockback(moby, class_1199_vars->f60, knockback_angle, 4, 1, 0);
                class_1199_vars->f117 = 0x78;
                trooper_anim_update(moby, class_1199_vars->f110);
                func_L00_00261B00_2E2B80(moby, 1, 3, 0, -1);
            } else {
                class_1199_vars->f20 = class_1199_vars->f20 - hit_record->f2C;
                class_1199_vars->f117 = 0xFA;
                class_1199_vars->f26 = scale_game_frames(60);
                trooper_anim_update(moby, class_1199_vars->f110);
            }
        }
        moby->hit_slot = 0xFF;
    }
    trooper_anim_finish(moby, class_1199_vars->f110);
    if (class_1199_vars->f174 == 2) {
        class_1199_vars->f180 = 42.0f;
    } else if (class_1199_vars->f184 != 0) {
        class_1199_vars->f180 = class_1199_vars->f178 + 6.0f;
    } else if (moby->state == 1) {
        class_1199_vars->f180 = class_1199_vars->f188;
    } else {
        class_1199_vars->f180 = class_1199_vars->f178;
    }
    if (class_1199_vars->f174 == 2) {
        target_selection_status = ahead_f(class_1199_vars->f180, moby, class_1199_vars->fC0);
        if (target_selection_status != 2) {
            LevelCuboid *bp = &l10_cuboids[class_1199_vars->f194];
            fast_vec_sub(vector_scratch, D_0013F3D0, &bp->pos);
            vector_scratch[3] = 0.0f;
            transform_vector_by_basis(transformed_offset, vector_scratch, bp->to_local);
            if (transformed_offset[0] <= -1.0f || 1.0f <= transformed_offset[0] ||
                transformed_offset[1] <= -1.0f || 1.0f <= transformed_offset[1] ||
                transformed_offset[2] <= -1.0f || 1.0f <= transformed_offset[2]) {
                class_1199_vars->f104 = 2;
            }
        }
    } else if (moby->state == 1 && class_1199_vars->f18C >= 0) {
        target_selection_status = activate_effect_l08(class_1199_vars->f180,
                                                      moby,
                                                      class_1199_vars->fC0,
                                                      0,
                                                      0,
                                                      l10_paths[class_1199_vars->f18C]->points,
                                                      l10_paths[class_1199_vars->f18C]->count);
    } else if (moby->state != 1 && class_1199_vars->f17C >= 0) {
        target_selection_status = activate_effect_l08(class_1199_vars->f180,
                                                      moby,
                                                      class_1199_vars->fC0,
                                                      0,
                                                      0,
                                                      l10_paths[class_1199_vars->f17C]->points,
                                                      l10_paths[class_1199_vars->f17C]->count);
    } else {
        target_selection_status = ahead_f(class_1199_vars->f180, moby, class_1199_vars->fC0);
    }
    if (target_selection_status != 2) {
        if (class_1199_vars->f180 <
            vector_distance_xy(class_1199_vars->f130, class_1199_vars->fC0)) {
            class_1199_vars->f104 = 2;
        } else if (moby->visible == 0) {
            class_1199_vars->f104 = 2;
        }
    }
    if (class_1199_vars->f100 == 0) {
        class_1199_vars->f100 = D_001413D0[0];
    }
}

#include "rnc/overlay/l10/mobys/darter_globals.h"

int  moby_group_command_frames_gp __asm__("D_L10_00161E00") = 5;
void set_moby_group_command_frames(struct Moby *moby) __asm__("FUN_L10_002e1cb0");
/* Stores a value into each table entry listed for this moby's index. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E3070), where it is exact; names translated to the US level program. */

void set_moby_group_command_frames(struct Moby *moby) {
    short *group_entry;
    if (moby->group != 0xFF) {
        group_entry = moby_group_member_lists[moby->group];
        if (group_entry != 0) {
            do {
                int listed_entity_index = *(unsigned short *)group_entry & 0x7FFF;
                ((char *)D_L10_0015FFD0_macro[2])[(listed_entity_index << 8) + 0xBC] =
                    scale_game_frames(moby_group_command_frames_gp);
            } while (*group_entry++ >= 0);
        }
    }
}

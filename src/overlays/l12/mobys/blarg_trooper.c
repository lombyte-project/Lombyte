/* Level 12 moby class blarg_trooper: path following, aiming, attack, damage and its attachment. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/l12/mobys/hoven_anklebiter.h"
#include "rnc/overlay/l12/mobys/miner.h"
#include "rnc/mobys/resolve_moby_hit_response.h"

extern char D_L12_00167240[];

#include "rnc/overlay/level_path.h"

extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void FUN_L01_0026e090(int, int);

#include "sda.h"

#include "qcopy.h"

extern int random_integer_below(int);
extern int scale_game_frames(int);

#include "rnc/overlay/quad.h"

#include "rnc/math/vector.h"

typedef struct Moby Moby;
extern float        D_0015ED64;
extern float        D_0015ED6C;
int                 FUN_L00_00257b90(int lo, int hi);
int                 step_moby_motion(void *moby, void *motion) __asm__("FUN_L00_0025c698");
s32         is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
extern int  FUN_L01_00274b78_c(char *, char *, float) __asm__("FUN_L01_00274b78");
extern void func_L01_0026E8E0_2E2EF0(char *) __asm__("FUN_L01_0026d930");
extern int  func_001160D8_2E2EF0(void) __asm__("FUN_001160d8");
extern int  step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int  point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");

#include "rnc/overlay/level_globals.h"

extern s32   count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern void  trooper_knockback_arc(void *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern float fast_subtract_rotations(float, float);
extern float vector_length_xy(void *);
extern int   D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern s32   FUN_L00_00258b50(void *, void *, void *, void *, f32);

/* Steers the moby toward a point 100 units ahead along its heading. */
s32 move_moby_along_heading(struct Moby *moby, void *ctl) __asm__("FUN_L12_00271dd8");

s32 move_moby_along_heading(struct Moby *moby, void *ctl) {
    Vec4 ahead;
    Vec4 out;

    qcopy(&ahead, &moby->pos);
    ahead.f[0] += fast_cos(moby->rot.z) * 100.0f;
    ahead.f[1] += fast_sin(moby->rot.z) * 100.0f;
    return FUN_L00_00258b50(moby, ctl, &ahead, &out, 1.0f);
}

/* Tests whether a point lies within height, distance and heading limits of a moby. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/mobyutil_00272D90.c: func_L12_0027C8D8), where it is exact; names translated to the US level program. */

int test_point_within_moby_limits(
    char *moby, float *point_position, float x, float y, float z) __asm__("FUN_L12_0027b8e0");

int test_point_within_moby_limits(char *moby, float *point_position, float x, float y, float z) {
    float absolute_height_difference = absolute_float(point_position[2] - MOBY(moby)->pos.z);
    if (absolute_height_difference > y)
        return 0;
    if (fast_vec_dist(point_position, moby + 0x10) > x)
        return 0;
    if (!(0.0f < z))
        return 1;
    {
        float heading_test_angle = fast_arc_tan(point_position[0] - MOBY(moby)->pos.x,
                                                point_position[1] - MOBY(moby)->pos.y);
        heading_test_angle =
            fast_difference_between_rotations(MOBY(moby)->rot.z, heading_test_angle);
        if (heading_test_angle > z)
            return 0;
    }
    return 1;
}

/* Forward the vendor parameters with fixed range and scale values. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8610), where it is exact; names translated to the US level program. */

extern void
spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");

void spawn_blarg_trooper_death_effects(void *moby,
                                       void *effect_position) __asm__("FUN_L12_002e7290");

void spawn_blarg_trooper_death_effects(void *moby, void *effect_position) {
    spawn_color_swapped_explosion_burst(moby, effect_position, -1, 1.0f, 10.0f);
}

extern void process_blarg_trooper_damage_and_alerts(struct Moby *) __asm__("FUN_L12_002e81e0");
void set_blarg_trooper_aim_yaw_offsets(struct Moby *moby, float *pos) __asm__("FUN_L12_002e8608");

extern void set_blarg_trooper_aim_yaw_offsets(struct Moby *, float *) __asm__("FUN_L12_002e8608");
void        remove_blarg_trooper_and_attachment(struct Moby *moby) __asm__("FUN_L12_002e86d0");

extern void remove_blarg_trooper_and_attachment(struct Moby *) __asm__("FUN_L12_002e86d0");
void        update_blarg_trooper_attachment(struct Moby *moby) __asm__("FUN_L12_002e8720");

extern void update_blarg_trooper_attachment(struct Moby *) __asm__("FUN_L12_002e8720");
void        ease_blarg_trooper_height_to_ground(char *moby) __asm__("FUN_L12_002e87b0");

extern void ease_blarg_trooper_height_to_ground(char *) __asm__("FUN_L12_002e87b0");
void        steer_blarg_trooper_toward_point(struct Moby *moby,
                                             float       *target,
                                             float        max_angle,
                                             float        approach_speed) __asm__("FUN_L12_002e8808");

extern void
steer_blarg_trooper_toward_point(struct Moby *, float *, float, float) __asm__("FUN_L12_002e8808");
extern s32 D_001413D0_e81e0 __asm__("D_001413D0") NOT_SDA;
extern void
call_for_help_e81e0(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern void         moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void         allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void         FUN_L00_00257470(struct Moby *, int, int);
extern struct Moby *D_L12_00178380[];
extern int          collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int   collect_mobies_in_sphere(void *, int, void *, int, float) __asm__("FUN_L00_001f2868");
extern int   is_moby_in_class_range_500_to_540(void *) __asm__("FUN_L00_0025e3b8");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void  blend_moby_animation_ex(void *, int, int, int, int) __asm__("FUN_002130d8");
extern int   select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern void spawn_blarg_trooper_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");

#define TROOPER_I(offset) (*(int *)(data + (offset)))
#define TROOPER_F(offset) (*(float *)(data + (offset)))
#define TROOPER_B(offset) (*(u8 *)(data + (offset)))
#define TROOPER_H(offset) (*(u16 *)(data + (offset)))
#define TROOPER_CHILD     (*(struct Moby **)(data + 0x248))

/* Blarg trooper update: initialization, path following, aiming, attack and removal.
   The retail unsigned group sentinel is 0xff; unordered angle comparisons
   keep the original branch outcomes. Quad copies include the fourth word. */
void       blarg_trooper_update(struct Moby *moby) __asm__("FUN_L12_002e72c0");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");

void blarg_trooper_update(struct Moby *moby) {
    enum {
        BLARG_TROOPER_INITIALIZE = 0,
        BLARG_TROOPER_FOLLOW_WAYPOINTS = 1,
        BLARG_TROOPER_WAIT_FOR_ACTIVATION = 2,
        BLARG_TROOPER_APPROACH_TARGET = 3,
        BLARG_TROOPER_KNOCKBACK_RECOVERY = 4,
        BLARG_TROOPER_PREPARE_SHOT = 5,
        BLARG_TROOPER_FIRE_PROJECTILE = 6,
        BLARG_TROOPER_WAIT_BETWEEN_SHOTS = 7,
        BLARG_TROOPER_RETURN_HOME = 8,
        BLARG_TROOPER_DEATH_FALL = 9
    };
    TargetReport work;
    Vec4         query_position_or_projectile_origin;
    Vec4         delta;
    char        *data = (char *)moby->pvars;
    struct Moby *target;

    manipulator_set_scale(data + 0x120, 3.0f);
    if (D_L12_0015F5C4 == 2)
        goto hide;
    if (moby->state >= 3) {
        moby->flags &= 0xffbe;
        if (TROOPER_CHILD)
            TROOPER_CHILD->flags &= 0xffbe;
    }
    if (TROOPER_B(0x269) && hero.state.current == 0 && hero.state.prev == 0x67) {
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 30.0f)
            goto leave;
    }
    if (TROOPER_I(0x270) != -1) {
        if (!is_point_inside_clip_volume(D_L12_00167240, TROOPER_I(0x270))) {
        hide:
            moby->flags |= 0x41;
            if (TROOPER_CHILD)
                TROOPER_CHILD->flags |= 0x41;
            return;
        }
        moby->flags &= 0xffbe;
        if (TROOPER_CHILD)
            TROOPER_CHILD->flags &= 0xffbe;
    }
    if (TROOPER_B(0x2e) == 2)
        goto leave;
    process_blarg_trooper_damage_and_alerts(moby);
    FUN_L01_00274b78_c((char *)moby, (char *)&work, TROOPER_F(0x230));
    target = (struct Moby *)work.moby;
    if (!target) {
        target = hero.moby;
        qcopy(work.v20, hero.motion.body_center.f);
    }
    switch (moby->state) {
    case BLARG_TROOPER_INITIALIZE: {
        float value;
        float other = TROOPER_F(0x230);
        TROOPER_F(0x20) = 2.0f;
        TROOPER_H(0x24) = 2;
        TROOPER_B(0x29) = 1;
        TROOPER_B(0x2e) = 1;
        TROOPER_B(0x58) = 8;
        TROOPER_B(0x5a) = 8;
        TROOPER_F(0x28c) = other;
        TROOPER_I(0x90) = 0x200;
        TROOPER_F(0x98) = 0.5f;
        func_L01_0026E8E0_2E2EF0(data + 0xd0);
        value = D_0015ED6C * 5.5f;
        TROOPER_I(0xd0) = 0x38d;
        *(volatile float *)(data + 0xd8) = 1.0f;
        TROOPER_I(0x108) = 0x18;
        *(volatile float *)(data + 0xf4) = value;
        *(volatile float *)(data + 0xdc) = 1.0f;
        qcopy(data + 0x220, &moby->pos);
        if (moby->group != 0xff) {
            if (**(int **)(data + 0x24c) == 1)
                moby->flags |= 0x8000;
            **(int **)(data + 0x24c) = (**(int **)(data + 0x24c) == 0);
        } else if (func_001160D8_2E2EF0() & 1) {
            moby->flags |= 0x8000;
        }
        if (TROOPER_I(0x274))
            TROOPER_I(0x288) = scale_game_frames(FUN_L00_00257b90(0, 300));
        moby->rot.x = 0.0f;
        moby->rot.y = 0.0f;
        if (TROOPER_F(0x244) == moby->rot.x) {
            if (moby->group != 0xff) {
                int *path = *(int **)(data + 0x250);
                int  count;
                TROOPER_F(0x244) = (float)*path;
                count = *path;
                if (count > 0)
                    *path = -count;
                else
                    *path = count + 20;
            } else
                TROOPER_F(0x244) = randf_sym(15.0f, 35.0f);
        }
        TROOPER_F(0x244) *= 0.017453292f;
        moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
        if (TROOPER_B(0x268)) {
            moby->flags |= 0x41;
            if (TROOPER_CHILD)
                TROOPER_CHILD->flags |= 0x41;
        }
        moby->state = 2;
        break;
    }
    case BLARG_TROOPER_WAIT_FOR_ACTIVATION: {
        if (fast_vec_dist(&moby->pos, &hero.motion.pos) > 64.0f)
            return;
        if (moby->prev_seq != 0 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 0, 0, scale_game_frames(8), 5);
        if (!test_point_within_moby_limits(moby, &target->pos, TROOPER_F(0x230), 3.0f, 0.0f)) {
            if ((TROOPER_I(0x25c) == -1 ||
                 !is_point_inside_clip_volume(&target->pos, TROOPER_I(0x25c))) &&
                moby->command == 0)
                break;
        }
        if (moby->flags & 1) {
            qcopy(query_position_or_projectile_origin.f, &moby->pos);
            query_position_or_projectile_origin.f[2] += 0.5f;
            if (collision_line(&query_position_or_projectile_origin, D_L12_00167240, 2, moby, 0)) {
                moby->flags &= 0xffbe;
                if (TROOPER_CHILD)
                    TROOPER_CHILD->flags &= 0xffbe;
            } else {
                if (TROOPER_CHILD)
                    mark_moby_for_removal(TROOPER_CHILD);
                mark_moby_for_removal(moby);
                return;
            }
        }
        if (moby->group != 0xff)
            FUN_L01_0026e090(moby->group, 1);
        if (TROOPER_I(0x254) == -1)
            goto state3;
        else {
            TROOPER_I(0x260) = select_point_index_by_distance(
                &moby->pos, ((char *)l12_paths[TROOPER_I(0x254)]), 0.0f);
            moby->state = 1;
        }
        break;
    }
    case BLARG_TROOPER_FOLLOW_WAYPOINTS: {
        int *path;
        if (!fast_dec_timer((int *)(data + 0x258)))
            break;
        if (moby->prev_seq != 8 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 8, 0, scale_game_frames(11), 5);
        path = (int *)l12_paths[TROOPER_I(0x254)];
        if (vector_distance_xy(&moby->pos, path + (TROOPER_I(0x260) * 4 + 4)) < 3.0f)
            ++TROOPER_I(0x260);
        if (*path - 1 < TROOPER_I(0x260))
            TROOPER_I(0x260) = *path - 1;
        steer_blarg_trooper_toward_point(
            moby, (float *)(path + (TROOPER_I(0x260) * 4 + 4)), 0.0f, D_0015ED6C * 8.0f);
        if (TROOPER_I(0x260) == *path - 1)
            goto state3;
        else if (vector_distance_xy(&moby->pos, &target->pos) < TROOPER_F(0x264))
            moby->state = 3;
        break;
    }
    case BLARG_TROOPER_APPROACH_TARGET: {
        float angle;
        if (TROOPER_B(0x268) == 1) {
            int hit;
            qcopy(query_position_or_projectile_origin.f, &moby->pos);
            query_position_or_projectile_origin.f[0] += fast_cos(moby->rot.z);
            query_position_or_projectile_origin.f[1] += fast_sin(moby->rot.z);
            hit = collect_mobies_in_sphere(&query_position_or_projectile_origin, 0, moby, 0, 1.5f);
            if (hit) {
                qcopy(delta.f, ((char *)&D_L12_0015F580));
                if (hit > 0) {
                    struct Moby **iterator = D_L12_00178380;
                    int           count = hit;
                    do {
                        if (is_moby_in_class_range_500_to_540(*iterator) &&
                            absolute_float(moby->pos.z - (*iterator)->pos.z) < 0.7f)
                            call_for_help_e81e0(
                                *iterator, (int)hero.moby, 0x10000, &moby->pos, delta.f, 1.0f);
                        --count;
                        ++iterator;
                    } while (count != 0);
                }
            }
        }
        angle = fast_subtract_rotations(
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), moby->rot.z);
        if (angle > 0.9599311f)
            angle = 0.9599311f;
        else if (angle < -0.9599311f)
            angle = -0.9599311f;
        TROOPER_F(0x188) = angle * 0.5f;
        TROOPER_F(0x208) = angle * 0.5f;
        steer_blarg_trooper_toward_point(moby, (float *)&target->pos, TROOPER_F(0x244), 0.0f);
        if (vector_distance_xy(&moby->pos, &target->pos) < 5.5f || TROOPER_B(0x242)) {
            angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            if (fast_difference_between_rotations(moby->rot.z, angle) < 0.08726646f)
                goto state5;
        }
        if (moby->prev_seq != 8 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 8, 0, scale_game_frames(11), 5);
        break;
    }
    case BLARG_TROOPER_PREPARE_SHOT: {
        if (TROOPER_F(0x28c) < 8.0f)
            TROOPER_F(0x28c) = 8.0f;
        set_blarg_trooper_aim_yaw_offsets(moby, (float *)&target->pos);
        ease_blarg_trooper_height_to_ground((char *)moby);
        if (moby->prev_seq != 2 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 2, 0, scale_game_frames(8), 5);
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 10 && moby->seq == moby->prev_seq)
                blend_moby_animation_ex(moby, 10, 0, scale_game_frames(5), 5);
            moby->state = 6;
        }
        break;
    }
    case BLARG_TROOPER_FIRE_PROJECTILE: {
        int   count;
        float angle;
        float other;
        float value;
        set_blarg_trooper_aim_yaw_offsets(moby, (float *)&target->pos);
        ease_blarg_trooper_height_to_ground((char *)moby);
        if (TROOPER_CHILD && is_value_within_interpolated_window(moby, 0.0f)) {
            moby_point_position(TROOPER_CHILD, 0, &query_position_or_projectile_origin);
            fast_vec_sub(&delta, work.v20, &query_position_or_projectile_origin);
            angle = fast_arc_tan(delta.f[0], delta.f[1]);
            other = fast_add_rotations(moby->rot.z, TROOPER_F(0x208));
            angle = 0.2268928f < fast_difference_between_rotations(angle, other) ? other : angle;
            value = vector_length_xy(&delta);
            value = fast_arc_tan(value, delta.f[2]);
            build_spherical_offset(&delta, D_0015ED6C * 15.0f, angle, value);
            allocate_voice_for_target_entry(0, 0, (int)moby);
            count = truncate_float_to_s32(18.0f / (D_0015ED6C * 15.0f));
            spawn_blarg_trooper_projectile(
                moby, &query_position_or_projectile_origin, &delta, count);
        }
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 5 && moby->seq == moby->prev_seq)
                blend_moby_animation_ex(moby, 5, 0, scale_game_frames(8), 5);
            moby->state = 7;
        }
        break;
    }
    case BLARG_TROOPER_WAIT_BETWEEN_SHOTS: {
        float angle;
        set_blarg_trooper_aim_yaw_offsets(moby, (float *)&target->pos);
        ease_blarg_trooper_height_to_ground((char *)moby);
        if ((moby->prev_seq != 5 || (moby->anim_flags & 2)) && moby->prev_seq != 0 &&
            moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 0, 0, scale_game_frames(8), 5);
        if (!random_integer_below(9))
            goto state5;
        if (vector_distance_xy(&moby->pos, &target->pos) > 10.5f && !TROOPER_B(0x242))
            goto state3;
        angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        if (fast_difference_between_rotations(moby->rot.z, angle) > 0.4886922f)
            moby->state = 3;
        break;
    }
    case BLARG_TROOPER_RETURN_HOME: {
        float angle;
        steer_blarg_trooper_toward_point(moby, (float *)(data + 0x220), 0.0f, 0.0f);
        angle = TROOPER_F(0x23c) * 30.0f;
        if (angle > 0.7853982f)
            angle = 0.7853982f;
        else if (angle < -0.7853982f)
            angle = -0.7853982f;
        TROOPER_F(0x188) = angle * 0.7f;
        TROOPER_F(0x208) = angle * 0.4f;
        if (vector_distance_xy(&moby->pos, data + 0x220) < 1.0f)
            moby->state = 2;
        break;
    }
    case BLARG_TROOPER_KNOCKBACK_RECOVERY: {
        float angle;
        moby->flags &= 0xffbe;
        if (step_moby_motion(moby, data + 0x70) & 0x60) {
            if (vector_distance_xy(&moby->pos, &target->pos) < 6.0f || TROOPER_B(0x242)) {
                angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
                if (fast_difference_between_rotations(moby->rot.z, angle) < 0.12217305f) {
                state5:
                    moby->state = 5;
                    break;
                }
            }
        state3:
            moby->state = 3;
            break;
        }
        if (!(moby->pos.z < 2.0f))
            goto finish;
        goto leave;
    }
    case BLARG_TROOPER_DEATH_FALL: {
        if (step_moby_motion(moby, data + 0x70) & 0x60) {
            FUN_L00_00257470(moby, 0, -1);
            spawn_blarg_trooper_death_effects(moby, &moby->pos);
            goto leave;
        }
        if (moby->pos.z < 2.0f) {
            FUN_L00_00257470(moby, 0, -1);
        leave:
            remove_blarg_trooper_and_attachment(moby);
            return;
        }
        break;
    }
    }
finish:
    if (moby->visible || (TROOPER_CHILD && TROOPER_CHILD->visible)) {
        update_blarg_trooper_attachment(moby);
        if (moby->flags & 0x8000) {
            TROOPER_F(0x188) = -TROOPER_F(0x188);
            TROOPER_F(0x208) = -TROOPER_F(0x208);
        }
        update_joint_animation(D_0015ED64 * 0.018f, D_0015ED64 * 0.3f, moby, data + 0x120, 5);
        update_joint_animation(D_0015ED64 * 0.024f, D_0015ED64 * 0.3f, moby, data + 0x1a0, 6);
    }
    return;
}
#undef TROOPER_I
#undef TROOPER_F
#undef TROOPER_B
#undef TROOPER_H
#undef TROOPER_CHILD
/* Variables of the Hoven blarg trooper (moby->pvars): the l00 ElectroidVars
   damage layout, with the alert and call-for-help state at 0x230.. */
typedef struct {
    u8  pad0[0x20];
    f32 health; /* 0x20 */
    u8  pad24[2];
    s16 stun_timer; /* 0x26 */
    u8  pad28[0x10];
    s32 reset_alert; /* 0x38 */
    u8  pad3C[0x24];
    u8  anim[7]; /* 0x60: animation blend state */
    u8  unk67;   /* 0x67 */
    u8  pad68[8];
    u8  fall[0x10]; /* 0x70 */
    f32 unk80;      /* 0x80 */
    u8  pad84[4];
    f32 unk88; /* 0x88 */
    f32 unk8C; /* 0x8C */
    u8  pad90[4];
    s32 unk94; /* 0x94 */
    u8  pad98[8];
    f32 unkA0; /* 0xA0 */
    f32 unkA4; /* 0xA4 */
    f32 unkA8; /* 0xA8 */
    u8  padAC;
    u8  unkAD; /* 0xAD */
    u8  padAE[0x12];
    f32 unkC0; /* 0xC0 */
    f32 unkC4; /* 0xC4 */
    u8  padC8[0x168];
    f32 speed; /* 0x230 */
    u8  pad234[0xC];
    s16 alert_timer; /* 0x240 */
    u8  pad242[0x32];
    s32 call_enabled; /* 0x274 */
    s32 groups[4];    /* 0x278: group ids counted before calling for help (-1: none) */
    s32 call_timer;   /* 0x288 */
    f32 alert_speed;  /* 0x28C */
    s32 volume;       /* 0x290: clip volume that hands the trooper to FUN_L12_002e86d0 (-1: none) */
} BlargTrooperVars;

extern void alert_group_e81e0(s32, s32) __asm__("FUN_L01_0026e090");

/* What find_moby_hit reports: where the hit landed and the moby that hit. */
typedef struct {
    u8           pad0[0x10];
    Vec4         pos;
    struct Moby *moby; /* 0x20 */
} BlargTrooperHit;

extern BlargTrooperHit *find_hit_e81e0(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void start_fall_e81e0(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void anim_reset_e81e0(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_e81e0(struct Moby *, void *) __asm__("FUN_L00_0025d538");

/* Damage and alert step of the Hoven blarg trooper (cf. FUN_L12_002e3528): refreshes its
   speed while alerted, calls for help behind itself once its groups have 9 or
   more members, hands off inside its clip volume, and takes hits from other
   classes (dying, or staggering back). */
void process_blarg_trooper_damage_and_alerts(struct Moby *moby) {
    BlargTrooperVars *vars = (BlargTrooperVars *)moby->pvars;
    Vec4              help_direction_or_hit_position;
    s32               hit;
    f32               dmg;
    f32               angle;
    f32               unused;
    BlargTrooperHit  *coll;

    if (vars->reset_alert != 0) {
        vars->reset_alert = 0;
        vars->alert_timer = scale_game_frames(0xF0);
    }
    if (moby->state != 0) {
        vars->speed = fast_dec_timer_s16(&vars->alert_timer) ? vars->alert_speed : 20.0f;
    }
    if (vars->call_enabled != 0 && moby->state != 2) {
        vars->call_timer++;
        if (scale_game_frames(0x3C) * 17 < vars->call_timer) {
            s32 count = 0;
            if (vars->groups[0] != -1)
                count += count_active_moby_group_members(vars->groups[0], 2);
            if (vars->groups[1] != -1)
                count += count_active_moby_group_members(vars->groups[1], 2);
            if (vars->groups[2] != -1)
                count += count_active_moby_group_members(vars->groups[2], 2);
            if (vars->groups[3] != -1)
                count += count_active_moby_group_members(vars->groups[3], 2);
            if (count >= 9 && random_integer_below(0x27) == 0) {
                help_direction_or_hit_position.f[0] =
                    fast_cos(fast_add_rotations(moby->rot.z, 3.1415927f));
                help_direction_or_hit_position.f[1] =
                    fast_sin(fast_add_rotations(moby->rot.z, 3.1415927f));
                help_direction_or_hit_position.f[2] = 0.0f;
                call_for_help_e81e0(moby,
                                    D_001413D0_e81e0,
                                    0x10000,
                                    &moby->pos,
                                    help_direction_or_hit_position.f,
                                    1.0f);
            }
        }
    }
    if (vars->volume != -1 && is_point_inside_clip_volume(&hero.motion.pos, vars->volume)) {
        remove_blarg_trooper_and_attachment(moby);
        return;
    }
    coll = find_hit_e81e0(moby, 0x330000, 0);
    dmg = 0.0f;
    resolve_moby_hit_response(moby, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (coll != 0 && coll->moby->oclass != moby->oclass && coll->moby->oclass != 0xB8 &&
        moby->state != 9 && dmg != 0.0f) {
        if (moby->group != 0xFF) {
            alert_group_e81e0(moby->group, 1);
        }
        vars->health -= dmg;
        vars->unk80 = frame_time_sq * 30.0f;
        vars->unkAD = 0;
        vars->unk94 = 9;
        vars->unkA0 = 1.0f;
        vars->unkA4 = 1.0f;
        vars->unkA8 = 1.0f;
        if (vars->health <= 0.0f) {
            moby->flags &= ~0x1000;
            vars->unk88 = frame_time * 12.0f;
            vars->unk8C = frame_time * 9.0f;
            help_direction_or_hit_position.q = coll->pos.q;
            trooper_knockback_arc(
                &help_direction_or_hit_position, &angle, &vars->unk88, &vars->unk8C);
            start_fall_e81e0(moby, vars->fall, 9, 1, 0, angle);
            vars->unkC0 = 10.0f;
            vars->unkC4 = 19.0f;
            moby->state = 9;
            vars->unk67 = 0x78;
            anim_reset_e81e0(moby, vars->anim);
        } else {
            vars->unk88 = frame_time * 4.7f;
            vars->unk8C = frame_time * 7.7f;
            help_direction_or_hit_position.q = coll->pos.q;
            trooper_knockback_arc(
                &help_direction_or_hit_position, &unused, &vars->unk88, &vars->unk8C);
            start_fall_e81e0(moby, vars->fall, 6, 1, 0, unused);
            vars->unkC0 = 4.0f;
            vars->unkC4 = 8.0f;
            moby->state = 4;
            vars->unk67 = 0xFA;
            vars->stun_timer = scale_game_frames(0x3C);
            anim_reset_e81e0(moby, vars->anim);
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_e81e0(moby, vars->anim);
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9988), where it is exact; names translated to the US level program. */

void set_blarg_trooper_aim_yaw_offsets(struct Moby *moby, float *pos) {
    char *data = (char *)moby->pvars;
    float target_yaw = fast_arc_tan(pos[0] - moby->pos.x, pos[1] - moby->pos.y);
    float yaw_offset = fast_subtract_rotations(target_yaw, moby->rot.z);
    if (yaw_offset > 1.2217305f) {
        yaw_offset = 1.2217305f;
    } else if (yaw_offset < -1.2217305f) {
        yaw_offset = -1.2217305f;
    }
    *(float *)(data + 0x188) = yaw_offset * 0.35f;
    *(float *)(data + 0x208) = yaw_offset * 0.8f;
}

/* Release the vendor moby and any child it owns. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9A50), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x248];
    void *child;
} Level12VendorData;

void remove_blarg_trooper_and_attachment(struct Moby *moby) {
    moby->unkB4 = 0;
    FUN_L00_00257470(moby, 0, -1);
    if (((Level12VendorData *)moby->pvars)->child != 0) {
        mark_moby_for_removal(((Level12VendorData *)moby->pvars)->child);
    }
    mark_moby_for_removal(moby);
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9AA0), where it is exact; names translated to the US level program. */

extern void *spawn_moby_attachment(void *, void *) __asm__("FUN_L04_002ce080");

void update_blarg_trooper_attachment(struct Moby *moby) {
    char          *data = (char *)moby->pvars;
    unsigned char *target = *(unsigned char **)(data + 0x248);
    if (target != 0) {
        unsigned char *current;
        update_moby_bone_attachment(moby, target, 1);
        current = *(unsigned char **)(data + 0x248);
        if (moby->flags & 1) {
            *(unsigned short *)(current + 0x34) |= 0x41;
        } else {
            *(unsigned short *)(current + 0x34) &= 0xFFBE;
        }
    } else {
        char vector[16];
        moby_point_position(moby, 1, vector);
        *(void **)(data + 0x248) = spawn_moby_attachment(moby, vector);
    }
}
/* Initialize two vendor vectors using the global scale. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9B30), where it is exact; names translated to the US level program. */

extern void approach_value(void *, f32, f32) __asm__("FUN_00213ed8");

/* Eases the moby's height toward the ground below it. */
void ease_blarg_trooper_height_to_ground(char *moby) {
    float value = probe_ground_height(moby + 0x10, 0, 0.5f);
    approach_value(moby + 0x18, value, 27.0f * frame_time_sq);
}

extern void approach_angle(void *, void *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern int  path_8808(int, void *, void *, void *) __asm__("FUN_L00_00261b48");

void steer_blarg_trooper_toward_point(struct Moby *moby,
                                      float       *target,
                                      float        max_angle,

                                      float approach_speed) {
    char *data = (char *)moby->pvars;
    float distance;
    float angle;
    float old[4] __attribute__((aligned(16)));
    float result[4] __attribute__((aligned(16)));
    float next[4] __attribute__((aligned(16)));
    float delta[4] __attribute__((aligned(16)));
    int   count;

    if (*(unsigned char *)(data + 0x242)) {
        float *rotation = &moby->rot.z;
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        approach_angle(rotation,
                       data + 0x23c,
                       angle,
                       frame_time_sq * 15.707963f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
        return;
    }
    distance = vector_distance_xy(&moby->pos, target);
    if (distance > 6.5f && max_angle != 0.0f) {
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        angle = fast_add_rotations(angle, max_angle);
        approach_angle(&moby->rot.z,
                       data + 0x23c,
                       angle,
                       frame_time_sq * 15.707963f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    } else {
        float *rotation = &moby->rot.z;
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        approach_angle(rotation,
                       data + 0x23c,
                       angle,
                       frame_time_sq * 15.707963f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    }
    if (approach_speed != 0.0f) {
        approach_value(data + 0xf4, approach_speed, frame_time_sq * 7.0f);
    } else {
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        if (fast_difference_between_rotations(moby->rot.z, angle) < 1.5707964f && 6.0f < distance) {
            approach_value(data + 0xf4, frame_time * 7.0f, frame_time_sq * 7.0f);
        } else {
            approach_value(data + 0xf4, frame_time, frame_time_sq * 7.0f);
        }
    }
    *(OvlQuad *)old = *(OvlQuad *)&moby->pos;
    move_moby_along_heading(moby, data + 0xd0);
    if (*(int *)(data + 0x26c) != -1) {
        fast_vec_sub(delta, old, &moby->pos);
        fast_vec_normalize(delta, delta, 0.05f);
        fast_vec_add(next, old, delta);
        if (path_8808(*(int *)(data + 0x26c), &moby->pos, next, result)) {
            char *path = ((char *)l12_paths[*(int *)(data + 0x26c)]);
            if (!point_in_path_xy(&moby->pos, path + 0x10, *(int *)path))
                *(OvlQuad *)&moby->pos = *(OvlQuad *)result;
            if (fast_vec_dist(&moby->pos, old) < frame_time * 0.5f) {
                ++*(unsigned char *)(data + 0x243);
                count = *(unsigned char *)(data + 0x243);
                if (scale_game_frames(15) < count)
                    *(unsigned char *)(data + 0x242) = 1;
            } else {
                *(unsigned char *)(data + 0x243) = 0;
            }
        }
    }
}

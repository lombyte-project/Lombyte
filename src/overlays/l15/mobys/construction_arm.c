/* Level 15 construction arm moby class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/l15/mobys/construction_arm_globals.h"

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern char D_L15_001673C0[];

typedef struct {
    char  pad00[0x68];
    float facing;
    char  pad6c[0x14];
    short timer;
    short side;
    float yaw;
    float yaw_velocity;
    float target_yaw;
    float direction;
} UpdateVars_2938;

int D_L15_001619EC = 0x40808080;

int construction_arm_effect_duration_min_frames_gp __asm__("D_L15_001619F4") = 0x3c;

int construction_arm_effect_duration_max_frames_gp __asm__("D_L15_001619F8") = 0x78;

float construction_arm_effect_speed_max_gp __asm__("D_L15_001619FC") = 1e+01f;

float construction_arm_wait_time_min_gp __asm__("D_L15_00161A00") = 2.0f;

float construction_arm_wait_time_max_gp __asm__("D_L15_00161A04") = 2e+01f;

extern unsigned short *l15_moby_group_member_lists[] __asm__("D_L15_001ABE40");

extern float D_L15_0015F580[] MACRO_ADDR;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

extern float FUN_L15_001f8648(float);

extern int FUN_L00_00257b90(int, int);

extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern void cart_burst(float, float, void *, void *, long long) __asm__("FUN_L00_0025f090");

extern void *FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

extern void spawn_burst(float, void *, void *, int, int, int, int) __asm__("FUN_L00_0026cbb0");

void construction_arm_update(struct Moby *moby) __asm__("FUN_L15_002c2938");

/* Moby class 221 */
void construction_arm_update(struct Moby *moby) {
    enum {
        CONSTRUCTION_ARM_INITIALIZE = 0,
        CONSTRUCTION_ARM_WAIT_TO_TURN = 1,
        CONSTRUCTION_ARM_TURN_TO_TARGET_YAW = 2,
        CONSTRUCTION_ARM_WAIT_FOR_ENTRY_ANIMATION = 3,
        CONSTRUCTION_ARM_WAIT_TO_START_EXIT_ANIMATION = 4,
        CONSTRUCTION_ARM_WAIT_FOR_EXIT_ANIMATION = 5,
        CONSTRUCTION_ARM_DESTROY = 6
    };
    UpdateVars_2938 *arm_vars = (UpdateVars_2938 *)moby->pvars;
    int              hit = 0;
    char            *slot;
    int              ready, count;
    unsigned short  *list;
    struct Moby     *group_member_moby;
    float            minimum, maximum, speed, spread, direction;
    int              animation, duration, minimum_effect_duration_frames;
    Vec4             origin, velocity, offset, sum;

    if (moby->state != 6) {
        slot = find_moby_hit(moby, 0x10000, 0);
        if (slot != 0 && *(float *)(slot + 0x2c) > 0.0f)
            hit = 1;
        if (hit)
            moby->state = 6;
    }
    if (moby->state != 0) {
        arm_vars->facing = arm_vars->yaw;
        update_joint_animation(frame_scale_sq * 0.03f, frame_scale_sq * 0.3f, moby, arm_vars, 0);
    }
    switch (moby->state) {
    case CONSTRUCTION_ARM_INITIALIZE:
        arm_vars->timer = truncate_float_to_s32(
            random_float_between(FUN_L15_001f8648(construction_arm_wait_time_min_gp * 60.0f),
                                 FUN_L15_001f8648(construction_arm_wait_time_max_gp * 60.0f)));
        *(int *)&arm_vars->yaw_velocity = arm_vars->side = *(int *)&arm_vars->yaw = 0;
        moby->update_dist = 0xff;
        moby->state = 1;
        break;
    case CONSTRUCTION_ARM_WAIT_TO_TURN:
        if (fast_dec_timer_s16(&arm_vars->timer) &&
            fast_vec_dist(D_L15_001673C0, &moby->pos) < 42.0f) {
            ready = 1;
            if (moby->group != 0xff) {
                slot = (char *)l15_moby_group_member_lists[moby->group];
                count = 0;
                if (slot == 0)
                    return;
                list = (unsigned short *)slot;
                do {
                    group_member_moby =
                        (struct Moby *)((*list & 0x7fff) * 0x100 + (int)l15_moby_array);
                    if ((signed char)group_member_moby->state >= 0 &&
                        group_member_moby->oclass == moby->oclass && group_member_moby->state != 1)
                        count++;
                } while ((short)*list++ >= 0);
                if (count != 0) {
                    arm_vars->timer = scale_game_frames(60);
                    ready = 0;
                }
            }
            if (ready) {
                arm_vars->target_yaw = random_float_between(-0.20943952f, 0.20943952f);
                moby->state = 2;
            }
        }
        break;
    case CONSTRUCTION_ARM_TURN_TO_TARGET_YAW:
        arm_vars->side = FUN_L00_00257b90(0, 1);
        if (fast_difference_between_rotations(arm_vars->yaw, arm_vars->target_yaw) < 0.008726646f) {
            animation = arm_vars->side;
            animation *= 2;
            blend_moby_animation(moby, animation + 1, 0, scale_game_frames(6));
            moby->state = 3;
        }
        arm_vars->yaw = spring_turn(&arm_vars->yaw_velocity,
                                    arm_vars->yaw,
                                    arm_vars->target_yaw,
                                    frame_time_sq * 1.5707964f,
                                    frame_time_sq * 0.7853982f,
                                    frame_time * 0.7853982f);
        break;
    case CONSTRUCTION_ARM_WAIT_FOR_ENTRY_ANIMATION:
        if (moby->prev_seq == arm_vars->side * 2 + 1 && (moby->anim_flags & 2)) {
            arm_vars->timer = scale_game_frames(FUN_L00_00257b90(15, 45));
            animation = arm_vars->side;
            animation *= 2;
            blend_moby_animation(moby, animation + 2, 0, scale_game_frames(3));
            arm_vars->direction = random_angle_radians();
            moby->state = 4;
        }
        break;
    case CONSTRUCTION_ARM_WAIT_TO_START_EXIT_ANIMATION:
        if (fast_vec_dist(&moby->pos, D_L15_001673C0) < 20.0f && random_integer_below(2) == 0 &&
            moby->visible != 0) {
            speed = random_float_between(construction_arm_effect_speed_max_gp / 5.0f,
                                         construction_arm_effect_speed_max_gp) *
                    frame_time;
            spread = random_float_between(0.0f, construction_arm_effect_speed_max_gp / 5.0f) *
                     frame_time;
            direction = random_angle_radians();
            velocity.f[0] = fast_cos(arm_vars->direction) * speed;
            velocity.f[1] = fast_sin(arm_vars->direction) * speed;
            velocity.f[2] = 0.0f;
            offset.f[0] = fast_cos(direction) * spread;
            offset.f[1] = fast_sin(direction) * spread;
            offset.f[2] = 0.0f;
            /* Retail computes the sum but passes velocity to the effect helper. */
            add_vectors(&sum, &velocity, &offset);
            moby_point_position(moby, 1, &origin);
            minimum_effect_duration_frames =
                scale_game_frames(construction_arm_effect_duration_min_frames_gp);
            duration = scale_game_frames(construction_arm_effect_duration_max_frames_gp);
            duration = FUN_L00_00257b90(minimum_effect_duration_frames, duration);
            spawn_burst(40000.0f, &origin, &velocity, D_L15_001619EC, D_L15_001619EC, duration, 1);
        }
        if (fast_dec_timer_s16(&arm_vars->timer)) {
            animation = arm_vars->side;
            animation *= 2;
            blend_moby_animation(moby, animation + 3, 0, scale_game_frames(6));
            moby->state = 5;
        }
        break;
    case CONSTRUCTION_ARM_WAIT_FOR_EXIT_ANIMATION:
        if (moby->prev_seq == arm_vars->side * 2 + 3 && (moby->anim_flags & 2)) {
            minimum = FUN_L15_001f8648(construction_arm_wait_time_min_gp * 60.0f);
            maximum = FUN_L15_001f8648(construction_arm_wait_time_max_gp * 60.0f);
            arm_vars->timer = truncate_float_to_s32(random_float_between(minimum, maximum));
            duration = scale_game_frames(6);
            blend_moby_animation(moby, 0, 0, duration);
            moby->state = 1;
        }
        break;
    case CONSTRUCTION_ARM_DESTROY:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        cart_burst(1.5f, 13.0f, moby, &moby->pos, -1);
        spawn_proximity_gated_bolts(moby);
        FUN_L00_00263fd8(moby,
                         0x74b,
                         &moby->pos,
                         &moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L15_0015F580,
                         D_L15_0015F580,
                         D_L15_0015F580);
        FUN_L00_00263fd8(moby,
                         0x74c,
                         &moby->pos,
                         &moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L15_0015F580,
                         D_L15_0015F580,
                         D_L15_0015F580);
        FUN_L00_00263fd8(moby,
                         0x74d,
                         &moby->pos,
                         &moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L15_0015F580,
                         D_L15_0015F580,
                         D_L15_0015F580);
        mark_moby_for_removal(moby);
        break;
    }
}

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

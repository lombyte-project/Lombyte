/* Shared moby class blarg_elite_commando. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_globals.h"
#include "rnc/overlay/quad.h"
#include "rnc/game/options.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_data/blarg_elite_commando_update.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_rocket.h"

typedef struct {
    char          pad00[0x20];
    float         health;
    char          pad24[0x14];
    int           reset_timer;
    char          pad3C[0x24];
    short         damage[3];
    unsigned char pad66;
    unsigned char animation;
    char          pad68[8];
    float         knockback[4];
    float         impulse_time;
    float         vertical_impulse;
    float         forward_impulse;
    float         lateral_impulse;
    int           sound_timer;
    int           active;
    float         damping;
    char          pad9C[0x11];
    unsigned char pending;
    char          padAE[0x12];
    float         blend_start;
    float         blend_end;
    char          padC8[8];
    float         target[4];
    char          padE0[0x30];
    int           owner;
    int           pursuit_mode;
    char          pad118[0xC];
    float         pursuit_range;
    char          pad128[4];
    int           path_index;
    char          pad130[0x1B0];
    int           pursuit_timer;
    char          pad2E4[0x24];
    int           reward_hits;
} L16DamageData;

typedef struct {
    OvlQuad vector;
    int     status;
    float   amount;
    float   angle;
    float   fatal_angle;
} L16DamageScratch;

typedef struct {
    char          pad00[0x2080];
    int           owner;
    char          pad2084[8];
    int           mode;
    char          pad2090[0x14];
    unsigned char difficulty;
} L16DamageResident;

typedef struct {
    unsigned char pad00[0x1D];
    unsigned char awarded;
} L16DamageReward;

typedef struct {
    char pad00[0x2E0];
    int  timer;
} L16GroupTimer;

typedef struct {
    char           pad00[0x20];
    signed char    state;
    unsigned char  group;
    char           pad22[0x56];
    L16GroupTimer *data;
} L16GroupMoby;

typedef struct {
    int values[6];
} L16ParticleChoices;

void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float fast_add_rotations(float, float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern int   scale_game_frames(int);
extern int   truncate_float_to_s32(float);
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void
     update_blarg_elite_commando_damage_and_pursuit(struct Moby *) __asm__("FUN_L16_002d38b0");
void select_blarg_elite_commando_movement_animation(char *moby) __asm__("FUN_L16_002d4070");
extern void transform_vector_by_basis(void *, void *, void *);
extern void FUN_L16_002d41f8_2(void *, void *) __asm__("FUN_L16_002d41f8");
void        update_blarg_elite_commando_joint_aim(char *commando_moby) __asm__("FUN_L16_002d4590");
void        emit_blarg_elite_commando_joint_sparks(char *commando_moby) __asm__("FUN_L16_002d4908");
extern float func_L16_002D5280_update(char *, void *, int) __asm__("FUN_L16_002d3eb8");
extern int   func_0022ED80_update(int, int, void *) __asm__("FUN_0022da68");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int  select_moby_combat_target(float, char *, void *) __asm__("FUN_L00_0025fcb8");
extern int  FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void
randomize_blarg_elite_commando_group_pursuit_timers(void *, int, int) __asm__("FUN_L16_002d3f78");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern float func_001F9878_timer(float) __asm__("FUN_001f96b0");
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void  spawn_particle(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern void spawn_glow_particle(void *, void *, void *, float, int, int, int, int, int) __asm__("FUN_L00_002ac910");
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

extern char               D_L16_001671C0[];
extern char              *D_L16_001B0930[];
extern char               D_L16_001671C0[], D_0013E633[];
extern int                D_L16_0015F5C4;
extern int D_L16_0016CC90 NOT_SDA;
extern int D_001413DC     NOT_SDA;
extern int    blarg_elite_commando_reward_kill_count_gp __asm__("D_0015EE10") __attribute__((sda));
extern char  *D_L16_0015FFD8;
extern short *D_L16_001ABCC0[];
extern L16ParticleChoices D_L16_001E8300, D_L16_001E8618;
extern L16ParticleChoices D_L16_001E8600, D_L16_001E8318;
extern char               D_L16_00167240[], D_0013F420[];

L16ParticleChoices D_L18_001F2710 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

/* Update the floating enemy's patrol, attack and death states. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D40B8), where it is exact; names translated to the US level program. */
void blarg_elite_commando_update(char *m) __asm__("FUN_L16_002d2cf0");

/* Moby class 638 */
void blarg_elite_commando_update(char *commando_moby) {
    enum {
        COMMANDO_STATE_INITIALIZE = 0,
        COMMANDO_STATE_RETURN_TO_INITIAL_POSITION = 1,
        COMMANDO_STATE_IDLE = 2,
        COMMANDO_STATE_MOVE_TO_PATROL_NODE = 3,
        COMMANDO_STATE_PREPARE_ROCKET_FIRE = 4,
        COMMANDO_STATE_FIRE_ROCKETS = 5,
        COMMANDO_STATE_MOVE_TO_ATTACK_NODE = 7,
        COMMANDO_STATE_KNOCKBACK_RECOVERY = 8,
        COMMANDO_STATE_FATAL_FALL = 9,
        COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM = 10
    };
    float  target[4], scratch_vector[4], delta[4];
    char  *commando_data = ((char *)MOBY(commando_moby)->pvars);
    char  *source, *aim_target_moby;
    float  distance, angle;
    float *heading, *movement;
    int    ticks;
    int    owner_moby_class;
    update_blarg_elite_commando_damage_and_pursuit(commando_moby);
    aim_target_moby = *(char **)(commando_data + 0x110);
    owner_moby_class = *(short *)(aim_target_moby + 0xA6);
    if (owner_moby_class == 0 || owner_moby_class == 0x1A3) {
        qcopy(target, &hero.motion.near_ground_point);
    } else {
        qcopy(target, aim_target_moby + 0x10);
    }
    if ((&big_head_cheat_enabled)[7])
        manipulator_set_scale(commando_data + 0x130, 2.5f);
    switch (((unsigned char *)commando_moby)[0x20]) {
    case COMMANDO_STATE_INITIALIZE:
        *(int *)(commando_data + 0x2EC) = 0;
        commando_data[0x29] = 1;
        *(short *)(commando_data + 0x24) = 3;
        commando_data[0x28] = 2;
        *(float *)(commando_data + 0x30) = 1.5f;
        *(float *)(commando_data + 0x20) = 3.0f;
        *(float *)(commando_data + 0x2F4) = random_angle_radians();
        *(float *)(commando_data + 0x2FC) = random_angle_radians();
        *(float *)(commando_data + 0x2E8) = random_angle_radians();
        *(float *)(commando_data + 0x2F0) =
            random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        *(float *)(commando_data + 0x2F8) =
            random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        *(float *)(commando_data + 0x2E4) =
            random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        *(short *)(commando_data + 0x302) =
            truncate_float_to_s32(scale_time(random_float_between(180.0f, 360.0f)));
        if (random_integer_below(2))
            MOBY(commando_moby)->flags |= 0x8000;
        else
            MOBY(commando_moby)->flags &= 0x7FFF;
        {
            char *position = commando_moby + 0x10;
            if (*(int *)(commando_data + 0x120) >= 0) {
                int node = truncate_float_to_s32(random_float_between(
                    0.0f, (float)(*(int *)D_L16_001B0930[*(int *)(commando_data + 0x120)] - 1)));
                *(short *)(commando_data + 0x300) = node;
                qcopy(position,
                      D_L16_001B0930[*(int *)(commando_data + 0x120)] + (short)node * 16 + 0x10);
            }
            *(int *)(commando_data + 0x308) = 1;
            qcopy(commando_data + 0x2C0, position);
            qcopy(commando_data + 0x2B0, position);
        }
        if (*(int *)(commando_data + 0x128)) {
            commando_moby[0x20] = 10;
            commando_moby[0x31] = 0;
            MOBY(commando_moby)->flags = (MOBY(commando_moby)->flags | 1) & 0xEFFF;
            MOBY(commando_moby)->collision = 0;
            if (((unsigned char *)commando_moby)[0x53] != 1) {
                blend_moby_animation(commando_moby, 1, 0, scale_game_frames(20));
            }
        } else
            goto idle;
        break;
    case COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM:
        if (*(char **)(commando_data + 0x2D0)) {
            commando_moby[0x31] = 1;
            MOBY(commando_moby)->flags &= 0xFFFE;
            movement = scratch_vector;
            transform_vector_by_basis(
                movement, commando_data + 0x2C0, *(char **)(commando_data + 0x2D0) + 0xC0);
            fast_vec_add(commando_moby + 0x10, movement, *(char **)(commando_data + 0x2D0) + 0x10);
            MOBY(commando_moby)->rot.z =
                fast_add_rotations(*(float *)(*(char **)(commando_data + 0x2D0) + 0x48),
                                   *(float *)(commando_data + 0x2CC));
        }
        break;
    case COMMANDO_STATE_RETURN_TO_INITIAL_POSITION:
        if (!fast_dec_timer_s16(commando_data + 0x2DC)) {
            fast_vec_normalize(
                scratch_vector, commando_moby + 0xC0, *(short *)(commando_data + 0x2DC) * 0.1f);
            fast_vec_add(scratch_vector, scratch_vector, commando_moby + 0x10);
            func_L16_002D5280_update(commando_moby, scratch_vector, 1);
        } else {
            heading = (float *)&MOBY(commando_moby)->rot.z;
            angle = fast_arc_tan(*(float *)(commando_data + 0x2B0) - MOBY(commando_moby)->pos.x,
                                 *(float *)(commando_data + 0x2B4) - MOBY(commando_moby)->pos.y);
            approach_angle(heading,
                           (float *)(commando_data + 0x2D8),
                           angle,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
            distance = func_L16_002D5280_update(commando_moby, commando_data + 0x2B0, 1);
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            if (distance < 1.0f)
                commando_moby[0x20] = 2;
        }
        break;
    case COMMANDO_STATE_IDLE:
        fast_dec_timer_s16(commando_data + 0x2DC);
        if (*(int *)(commando_data + 0x114) != 2) {
            if (*(short *)(commando_data + 0x2DC) == 0) {
                if (*(int *)(commando_data + 0x120) >= 0) {
                    commando_moby[0x20] = 7;
                    *(short *)(commando_data + 0x2DC) =
                        truncate_float_to_s32(scale_time(random_float_between(30.0f, 90.0f)));
                    *(short *)(commando_data + 0x300) = (*(short *)(commando_data + 0x300) + 1) % 2;
                    select_blarg_elite_commando_movement_animation(commando_moby);
                } else if (!random_integer_below(20)) {
                    commando_moby[0x20] = 4;
                    if (((unsigned char *)commando_moby)[0x53] != 2) {
                        blend_moby_animation(commando_moby, 2, 0, scale_game_frames(10));
                    }
                }
            }
        } else if (fast_dec_timer_s16(commando_data + 0x302)) {
            if (*(int *)(commando_data + 0x120) >= 0) {
                *(short *)(commando_data + 0x300) = (*(short *)(commando_data + 0x300) + 1) % 2;
                commando_moby[0x20] = 3;
                *(short *)(commando_data + 0x302) = scale_game_frames(180);
                select_blarg_elite_commando_movement_animation(commando_moby);
            }
        }
        break;
    case COMMANDO_STATE_MOVE_TO_PATROL_NODE: {
        float path_distance =
            func_L16_002D5280_update(commando_moby,
                                     D_L16_001B0930[*(int *)(commando_data + 0x120)] +
                                         (*(short *)(commando_data + 0x300) * 16 + 0x10),
                                     1);
        if (path_distance < 0.01f) {
            commando_moby[0x20] = 2;
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            *(short *)(commando_data + 0x302) =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 360.0f)));
        } else if (fast_dec_timer_s16(commando_data + 0x302)) {
            commando_moby[0x20] = 2;
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            *(short *)(commando_data + 0x302) =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 360.0f)));
        }
        if (*(float *)(commando_data + 0x2D4) < 2.0f * frame_time && path_distance < 0.5f) {
            if (((unsigned char *)commando_moby)[0x53] != 0) {
                ticks = 30;
                goto animate_idle;
            }
        }
        break;
    }
    case COMMANDO_STATE_PREPARE_ROCKET_FIRE:
        heading = (float *)&MOBY(commando_moby)->rot.z;
        angle = fast_arc_tan(target[0] - MOBY(commando_moby)->pos.x,
                             target[1] - MOBY(commando_moby)->pos.y);
        approach_angle(heading,
                       (float *)(commando_data + 0x2D8),
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (((unsigned char *)commando_moby)[0x70] & 2) {
            commando_moby[0x20] = 5;
            if (((unsigned char *)commando_moby)[0x53] != 3) {
                blend_moby_animation(commando_moby, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case COMMANDO_STATE_FIRE_ROCKETS:
        heading = (float *)&MOBY(commando_moby)->rot.z;
        angle = fast_arc_tan(target[0] - MOBY(commando_moby)->pos.x,
                             target[1] - MOBY(commando_moby)->pos.y);
        approach_angle(heading,
                       (float *)(commando_data + 0x2D8),
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (is_value_within_interpolated_window(commando_moby, 2.0f) ||
            is_value_within_interpolated_window(commando_moby, 17.0f) ||
            is_value_within_interpolated_window(commando_moby, 32.0f)) {
            moby_point_position(commando_moby, 0, scratch_vector);
            fast_vec_sub(delta, target, scratch_vector);
            fast_vec_normalize(delta, delta, frame_time * 20.0f);
            spawn_blarg_elite_commando_rocket(commando_moby, delta, (char *)scratch_vector);
        } else if (is_value_within_interpolated_window(commando_moby, 10.0f)) {
            int count = *(unsigned short *)(commando_data + 0x2DE) + 1;
            *(unsigned short *)(commando_data + 0x2DE) = count;
            if ((short)count >= 3) {
                commando_moby[0x20] = 2;
                if (((unsigned char *)commando_moby)[0x53] != 0)
                    blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
                *(short *)(commando_data + 0x2DC) = scale_game_frames(60);
                *(unsigned short *)(commando_data + 0x2DE) = 0;
            }
        }
        break;
    case COMMANDO_STATE_MOVE_TO_ATTACK_NODE:
        distance = func_L16_002D5280_update(commando_moby,
                                            D_L16_001B0930[*(int *)(commando_data + 0x120)] +
                                                (*(short *)(commando_data + 0x300) * 16 + 0x10),
                                            1);
        fast_dec_timer_s16(commando_data + 0x2DC);
        if (distance < 0.01f || *(short *)(commando_data + 0x2DC) == 0) {
            commando_moby[0x20] = 4;
            if (((unsigned char *)commando_moby)[0x53] != 2)
                blend_moby_animation(commando_moby, 2, 0, scale_game_frames(30));
        }
        if (distance < 0.5f && *(float *)(commando_data + 0x2D4) < 2.0f * frame_time) {
            if (((unsigned char *)commando_moby)[0x53] != 2) {
                blend_moby_animation(commando_moby, 2, 0, scale_game_frames(30));
            }
        }
        break;
    case COMMANDO_STATE_KNOCKBACK_RECOVERY:
        if (((unsigned char *)commando_moby)[0x70] & 2) {
        idle:
            commando_moby[0x20] = 2;
            if (((unsigned char *)commando_moby)[0x53] != 0) {
                ticks = 10;
            animate_idle:
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(ticks));
            }
        } else {
            fast_vec_normalize(scratch_vector, commando_moby + 0xC0, -frame_time);
            fast_vec_add(commando_moby + 0x10, commando_moby + 0x10, scratch_vector);
        }
        break;
    case COMMANDO_STATE_FATAL_FALL: {
        char *position;
        movement = scratch_vector;
        position = commando_moby + 0x10;
        qcopy(movement, position);
        if (step_moby_motion(commando_moby, commando_data + 0x70) & 0x160) {
            FUN_L16_002d41f8_2(commando_moby, position);
            func_0022ED80_update(0, 0, commando_moby);
            mark_moby_for_removal(commando_moby);
            return;
        }
        fast_vec_sub(movement, position, movement);
        fast_vec_add(commando_data + 0x2C0, commando_data + 0x2C0, movement);
    } break;
    case 6:
    default:
        break;
    }
    if (blarg_elite_commando_bobbing_and_voice_enabled_gp) {
        *(float *)(commando_data + 0x2BC) = MOBY(commando_moby)->pos.z;
        if (((unsigned char *)commando_moby)[0x20] != 0 &&
            ((unsigned char *)commando_moby)[0x20] != 10) {
            approach_value((float *)(commando_data + 0x2EC), 0.25f, frame_time * 0.25f);
            *(float *)(commando_data + 0x2F4) = fast_add_rotations(
                *(float *)(commando_data + 0x2F4), *(float *)(commando_data + 0x2F0));
            *(float *)(commando_data + 0x2FC) = fast_add_rotations(
                *(float *)(commando_data + 0x2FC), *(float *)(commando_data + 0x2F8));
            *(float *)(commando_data + 0x2E8) = fast_add_rotations(
                *(float *)(commando_data + 0x2E8), *(float *)(commando_data + 0x2E4));
            MOBY(commando_moby)->pos.x =
                *(float *)(commando_data + 0x2C0) +
                *(float *)(commando_data + 0x2EC) * fast_sin(*(float *)(commando_data + 0x2F4));
            MOBY(commando_moby)->pos.y =
                *(float *)(commando_data + 0x2C4) +
                *(float *)(commando_data + 0x2EC) * fast_sin(*(float *)(commando_data + 0x2FC));
            MOBY(commando_moby)->pos.z =
                *(float *)(commando_data + 0x2C8) +
                *(float *)(commando_data + 0x2EC) * fast_sin(*(float *)(commando_data + 0x2E8));
        }
        if (fast_vec_dist(commando_moby + 0x10, D_L16_001671C0) < 32.0f &&
            !voice_is_playing(commando_moby, *(int *)(commando_data + 0x304)))
            *(int *)(commando_data + 0x304) = func_0022ED80_update(7, 4, commando_moby);
    }
    if (blarg_elite_commando_joint_aim_enabled_gp)
        update_blarg_elite_commando_joint_aim(commando_moby);
    if (blarg_elite_commando_joint_sparks_enabled_gp && D_001413DC != 22)
        emit_blarg_elite_commando_joint_sparks(commando_moby);
    if (current_level_index == 18 && D_L16_0015F5C4 == 2 && (unsigned int)(D_L16_0016CC90 - 3) < 3U)
        mark_moby_for_removal(commando_moby);
}

/* Handle damage and knockback, then update the pursuit target and timer. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D4C78), where it is exact; names translated to the US level program. */
void update_blarg_elite_commando_damage_and_pursuit(struct Moby *commando_moby) {
    enum {
        HIT_RESPONSE_UNAVAILABLE = 0,
        HIT_RESPONSE_FATAL = 1,
        HIT_RESPONSE_FATAL_FALL = 2,
        HIT_RESPONSE_KNOCKBACK_RECOVERY = 3,
        HIT_RESPONSE_RECOVERABLE_KNOCKBACK = 4,
        HIT_RESPONSE_NONFATAL_KNOCKBACK = 5,
        HIT_RESPONSE_KNOCKBACK_TO_IDLE = 6,
        HIT_RESPONSE_ENTER_KNOCKBACK_RECOVERY = 7,
        HIT_RESPONSE_KNOCKBACK_AND_RECOVER = 8,
        HIT_RESPONSE_ANIMATION_ONLY = 9,
        HIT_RESPONSE_DAMAGE_ANIMATION_ONLY = 10,
        HIT_RESPONSE_NO_REACTION = 11
    };
    L16DamageScratch damage_reaction;
    L16DamageData   *commando_data = (L16DamageData *)commando_moby->pvars;
    char            *hit;
    unsigned int     response;
    float            range;
    float            time_scale;
    commando_moby->scale = commando_moby->pclass->scale;
    damage_reaction.amount = 0.0f;
    hit = find_moby_hit(commando_moby, 0x330000, 0);
    if (hit) {
        if (*(char **)(hit + 0x20)) {
            char *source = *(char **)(hit + 0x20);
            hit = *(short *)(source + 0xA6) != 0x31 ? hit : 0;
        }
    }
    response = hit_reaction(commando_moby,
                            hit,
                            &commando_data->health,
                            0,
                            &damage_reaction.status,
                            &damage_reaction.amount,
                            0,
                            4);
    if (damage_reaction.status != 1 && commando_moby->state != 9) {
        randomize_blarg_elite_commando_group_pursuit_timers(commando_moby, 0x12C, 0x168);
        if (hit && *(char **)(hit + 0x20) && *(short *)(*(char **)(hit + 0x20) + 0xA6) != 0x47) {
            commando_data->reward_hits = 0;
        }
        commando_data->health -= damage_reaction.amount;
        if (commando_data->health <= 0.0f)
            response = 1;
        commando_data->sound_timer = truncate_float_to_s32(512.0f);
        commando_data->impulse_time = frame_time_sq * 20.0f;
        commando_data->damping = 0.5f;
        commando_data->vertical_impulse = 0.0f;
        commando_data->active = 1;
        commando_data->pending = 0;
        switch (response) {
        case HIT_RESPONSE_ANIMATION_ONLY:
        case HIT_RESPONSE_DAMAGE_ANIMATION_ONLY:
            commando_data->animation = 0xFA;
            break;
        case HIT_RESPONSE_KNOCKBACK_RECOVERY:
        case HIT_RESPONSE_RECOVERABLE_KNOCKBACK:
        case HIT_RESPONSE_NONFATAL_KNOCKBACK:
        case HIT_RESPONSE_KNOCKBACK_TO_IDLE:
        case HIT_RESPONSE_ENTER_KNOCKBACK_RECOVERY:
        case HIT_RESPONSE_KNOCKBACK_AND_RECOVER: {
            float impulse = frame_time * 6.0f;
            char *source;
            commando_data->blend_start = -1.0f;
            commando_data->blend_end = -1.0f;
            commando_data->forward_impulse = impulse;
            commando_data->lateral_impulse = impulse;
            source = *(char **)(hit + 0x20);
            {
                float heading = fast_arc_tan(commando_moby->pos.x - *(float *)(source + 0x10),
                                             commando_moby->pos.y - *(float *)(source + 0x14));
                damage_reaction.vector = *(OvlQuad *)(hit + 0x10);
                damage_reaction.angle = heading;
            }
            trooper_knockback_arc(&damage_reaction.vector,
                                  &damage_reaction.angle,
                                  &commando_data->forward_impulse,
                                  &commando_data->lateral_impulse);
            trooper_start_knockback(
                damage_reaction.angle, commando_moby, commando_data->knockback, 6, 1, 0);
            commando_moby->state = 8;
            commando_data->animation = 0x78;
            break;
        }
        case HIT_RESPONSE_FATAL:
        case HIT_RESPONSE_FATAL_FALL: {
            char *source;
            if (current_level_index == 18 && commando_data->reward_hits) {
                ++*(int *)&blarg_elite_commando_reward_kill_count_gp;
                if (*(int *)&blarg_elite_commando_reward_kill_count_gp >= 10) {
                    if (item_unlocked[0x1D] == 0) {
                        item_unlocked[0x1D] = 1;
                        allocate_voice_for_bank_entry(1, 0, 0);
                        queue_dialogue_message(0x53D6, -1);
                    }
                }
            }
            time_scale = frame_time_sq * 20.0f;
            commando_moby->flags &= 0xEFFF;
            commando_data->impulse_time = time_scale;
            if (((L16DamageResident *)((char *)&hero))->difficulty == 2) {
                float impulse = frame_time;
                commando_moby->anim_speed = 0.5f;
                commando_data->forward_impulse = impulse * 24.0f;
                commando_data->lateral_impulse = impulse * 8.0f;
            } else {
                float impulse = frame_time * 8.0f;
                commando_data->forward_impulse = impulse;
                commando_data->lateral_impulse = impulse;
            }
            commando_data->blend_start = -1.0f;
            commando_data->blend_end = -1.0f;
            source = *(char **)(hit + 0x20);
            {
                float heading = fast_arc_tan(commando_moby->pos.x - *(float *)(source + 0x10),
                                             commando_moby->pos.y - *(float *)(source + 0x14));
                damage_reaction.vector = *(OvlQuad *)(hit + 0x10);
                damage_reaction.fatal_angle = heading;
            }
            trooper_knockback_arc(&damage_reaction.vector,
                                  &damage_reaction.fatal_angle,
                                  &commando_data->forward_impulse,
                                  &commando_data->lateral_impulse);
            trooper_start_knockback(
                damage_reaction.fatal_angle, commando_moby, commando_data->knockback, 7, 1, 0);
            commando_moby->state = 9;
            commando_data->animation = 0xF0;
            FUN_L00_00257470(commando_moby, 0, -1);
            break;
        }
        case HIT_RESPONSE_UNAVAILABLE:
        case HIT_RESPONSE_NO_REACTION:
        default:
            break;
        }
        trooper_anim_update(commando_moby, commando_data->damage);
    }
    commando_moby->hit_slot = 0xFF;
    trooper_anim_finish(commando_moby, commando_data->damage);
    fast_dec_timer(&commando_data->pursuit_timer);
    if (commando_data->reset_timer) {
        commando_data->pursuit_timer =
            truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
    }
    commando_data->reset_timer = 0;
    range = 255.0f;
    if (commando_data->pursuit_timer == 0)
        range = commando_data->pursuit_range;
    if (((L16DamageResident *)((char *)&hero))->mode == 22) {
        commando_data->pursuit_mode = 2;
    } else if (commando_data->path_index >= 0 && commando_data->pursuit_timer == 0) {
        char *path = D_L16_001B0930[commando_data->path_index];
        FUN_L00_0025ff38(
            24.0f, commando_moby, commando_data->target, 0, 0, path + 0x10, *(int *)path);
    } else {
        select_moby_combat_target(24.0f, commando_moby, commando_data->target);
    }
    if (range < vector_distance_xy(&commando_moby->pos, commando_data->target))
        commando_data->pursuit_mode = 2;
    if (commando_data->owner == 0)
        commando_data->owner = ((L16DamageResident *)((char *)&hero))->owner;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5188), where it is exact; names translated to the US level program. */
void set_blarg_elite_commando_parent_transform(char *moby,
                                               void *owner,
                                               void *position,
                                               char *vector) __asm__("FUN_L16_002d3dc0");

void set_blarg_elite_commando_parent_transform(char *moby,
                                               void *parent_moby,
                                               void *position,

                                               char *vector) {
    char *commando_data = ((char *)MOBY(moby)->pvars);
    *(void **)(commando_data + 0x2D0) = parent_moby;
    qcopy(commando_data + 0x2C0, position);
    *(float *)(commando_data + 0x2CC) = *(float *)(vector + 8);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D51A8), where it is exact; names translated to the US level program. */
void start_blarg_elite_commando_return_to_initial_position(
    struct Moby *moby, int return_countdown_ticks) __asm__("FUN_L16_002d3de0");

void start_blarg_elite_commando_return_to_initial_position(struct Moby *moby,

                                                           int return_countdown_ticks) {
    char *commando_data = (char *)moby->pvars;
    if (moby->state == 10) {
        moby->state = 1;
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
        }
        moby->visible = 1;
        moby->flags &= 0xFFFE;
        moby->collision = moby->pclass->collision;
        moby->flags |= 0x1000;
        *(short *)(commando_data + 0x2DC) = return_countdown_ticks;
        *(float *)(commando_data + 0x2BC) = moby->pos.z;
        qcopy(commando_data + 0x2C0, &moby->pos);
        *(float *)(commando_data + 0x2D4) = frame_time * 5.0f;
    }
}

/* steers the moby's vector toward the target and returns the distance */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5280), where it is exact; names translated to the US level program. */
float advance_blarg_elite_commando_base_position(char  *moby,
                                                 float *target) __asm__("FUN_L16_002d3eb8");

float advance_blarg_elite_commando_base_position(char *moby, float *target) {
    char *commando_motion_data = ((char *)MOBY(moby)->pvars);
    char *base_position = commando_motion_data + 0x2C0;
    float distance = fast_vec_dist(base_position, target);
    float acceleration_braking_step = frame_time_sq * 4.0f;
    int   zero[1];
    float movement_step[4];
    zero[0] = 0;
    advance_accelerated_scalar((float *)zero,
                               (float *)(commando_motion_data + 0x2D4),
                               distance,
                               acceleration_braking_step,
                               acceleration_braking_step,
                               frame_time * 5.0f);
    fast_vec_sub(movement_step, target, base_position);
    fast_vec_normalize(movement_step, movement_step, *(float *)(commando_motion_data + 0x2D4));
    fast_vec_add(base_position, base_position, movement_step);
    return distance;
}

/* Randomize the timers of active mobys in the selected linked group. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5340), where it is exact; names translated to the US level program. */
void randomize_blarg_elite_commando_group_pursuit_timers(void *group_source_moby,
                                                         int   lower,

                                                         int upper) {
    L16GroupMoby  *source_moby = group_source_moby;
    L16GroupTimer *timer_data;
    short         *group_entry;
    if (source_moby->group == 255) {
        timer_data = source_moby->data;
        timer_data->timer = truncate_float_to_s32(
            func_001F9878_timer(random_float_between((float)lower, (float)upper)));
    } else {
        L16GroupMoby *group_member_moby;
        group_entry = D_L16_001ABCC0[source_moby->group];
    next:
        group_member_moby = (L16GroupMoby *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) +
                                             (int)D_L16_0015FFD8);
        if (group_member_moby->state >= 0) {
            timer_data = group_member_moby->data;
            timer_data->timer = truncate_float_to_s32(
                func_001F9878_timer(random_float_between((float)lower, (float)upper)));
        }
        if (*group_entry++ >= 0)
            goto next;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5438), where it is exact; names translated to the US level program. */
void select_blarg_elite_commando_movement_animation(char *moby) {
    char *commando_path_state = ((char *)MOBY(moby)->pvars);
    char *target = D_L16_001B0930[*(int *)(commando_path_state + 0x120)] +
                   (*(short *)(commando_path_state + 0x300) << 4);
    float angle = fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                               *(float *)(target + 0x14) - MOBY(moby)->pos.y);
    float delta = fast_subtract_rotations(angle, MOBY(moby)->rot.z);
    if (delta < 0.78539819f && -0.78539819f < delta) {
        if ((unsigned char)moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
        }
    } else if (delta < 0.0f) {
        if (MOBY(moby)->flags & 0x8000) {
            if ((unsigned char)moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
        } else {
            if ((unsigned char)moby[0x53] != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(0x14));
        }
    } else if (MOBY(moby)->flags & 0x8000) {
        if ((unsigned char)moby[0x53] != 5)
            blend_moby_animation(moby, 5, 0, scale_game_frames(0x14));
    } else {
        if ((unsigned char)moby[0x53] != 4)
            blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
    }
}

/* Spawn randomized hit particles at joint two, with extra bursts for distant hits. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D55C0), where it is exact; names translated to the US level program. */
void emit_blarg_elite_commando_death_particles(void *moby) __asm__("FUN_L16_002d41f8");

void emit_blarg_elite_commando_death_particles(void *moby) {
    float              delta[4], velocity[4], position[4];
    L16ParticleChoices first_particle_choices, second_particle_choices;
    float              distance, range, delay;
    int                count;
    void              *origin;
    float             *joint = position;
    void              *movement;
    moby_point_position(moby, 2, joint);
    fast_vec_sub(delta, D_L16_001671C0, joint);
    clear_u64_value(velocity);
    distance = fast_vec_length(delta);
    if (distance < 8.0f)
        count = truncate_float_to_s32(distance) + 2;
    else
        count = 10;
    range = 0.0f;
    if (distance < 7.0f)
        range = 7.0f - distance;
    origin = position;
    movement = velocity;
    if (count > 0) {
        int remaining = count;
        do {
            float sample;
            int  *first_choice, *second_choice, duration1, duration2;
            remaining--;
            sample = random_float_between(8.0f, 10.0f);
            first_particle_choices = D_L16_001E8300;
            second_particle_choices = D_L16_001E8318;
            delay = sample * frame_time - range * frame_time;
            first_choice = &first_particle_choices.values[random_integer_below(6)];
            second_choice = &second_particle_choices.values[random_integer_below(6)];
            {
                int lo = scale_game_frames(15), hi = scale_game_frames(20);
                duration1 = random_int_between(lo, hi);
            }
            {
                int lo = scale_game_frames(25), hi = scale_game_frames(30);
                duration2 = random_int_between(lo, hi);
            }
            spawn_particle(origin,
                           movement,
                           *first_choice,
                           *second_choice,
                           duration1,
                           duration2,
                           0,
                           0,
                           400000.0f,
                           delay);
        } while (remaining != 0);
    }
    if (blarg_elite_commando_extra_glow_gate < 0.95f && distance > 9.0f) {
        spawn_glow_particle(moby, origin, movement, 4.0f, scale_game_frames(15), 127, 127, 127, 32);
        spawn_glow_particle(moby, origin, movement, 4.0f, scale_game_frames(24), 127, 32, 0, 32);
    }
    spawn_glow_particle(moby, origin, movement, 4.0f, scale_game_frames(20), 127, 64, 0, 48);
    spawn_glow_particle(moby, origin, movement, 3.5f, scale_game_frames(27), 96, 16, 0, 64);
    spawn_glow_particle(moby, origin, movement, 3.0f, scale_game_frames(29), 32, 0, 0, 32);
}

/* Aim paired joints toward the owner or player, then smooth their rotations. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5958), where it is exact; names translated to the US level program. */
void update_blarg_elite_commando_joint_aim(char *commando_moby) {
    float position[4], target[4], matrix[16], delta[4], aim[4], aimDelta[4], velocity[4];
    char *commando_data = ((char *)MOBY(commando_moby)->pvars);
    if (*(int *)(commando_data + 0x114) != 2) {
        float *savedTarget, *savedMatrix;
        float  yaw, pitch;
        qcopy(position, commando_moby + 0x10);
        position[2] += 2.0f;
        if (*(short *)(*(char **)(commando_data + 0x110) + 0xA6) == 0)
            qcopy(target, D_0013F420);
        else
            qcopy(target, commando_data + 0xD0);
        savedTarget = target;
        transpose_rotation_matrix(matrix, commando_moby + 0xC0);
        fast_vec_sub(delta, savedTarget, position);
        transform_vector_by_basis(delta, delta, matrix);
        yaw = fast_arc_tan(delta[0], delta[1]);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        if (yaw > 0.7853982f)
            yaw = 0.7853982f;
        else if (yaw < -0.7853982f)
            yaw = -0.7853982f;
        if (pitch > 0.7853982f)
            pitch = 0.7853982f;
        else if (pitch < -0.7853982f)
            pitch = -0.7853982f;
        {
            float halfYaw = yaw * 0.5f, halfPitch = pitch * 0.5f;
            *(float *)(commando_data + 0x198) = halfYaw;
            *(float *)(commando_data + 0x194) = halfPitch;
            *(float *)(commando_data + 0x218) = halfYaw;
            *(float *)(commando_data + 0x214) = halfPitch;
        }
        if ((unsigned int)(MOBY(commando_moby)->state - 4) < 2) {
            float yaw2, pitch2, bound;
            fast_vec_normalize(aim, commando_moby + 0xC0, 1.0f);
            fast_vec_normalize(velocity, commando_moby + 0xD0, 0.5f);
            fast_vec_add(aim, aim, position);
            fast_vec_add(aim, aim, velocity);
            fast_vec_sub(aimDelta, savedTarget, aim);
            savedMatrix = matrix;
            transform_vector_by_basis(aimDelta, aimDelta, savedMatrix);
            pitch2 = -fast_arc_tan(vector_length_xy(aimDelta), aimDelta[2]);
            yaw2 = -fast_arc_tan(aimDelta[0], aimDelta[1]);
            bound = 0.7853982f;
            if (pitch2 > bound || pitch2 < (bound = -0.7853982f))
                pitch2 = bound;
            if (yaw2 > 0.34906584f)
                yaw2 = 0.34906584f;
            else if (yaw2 < -0.34906584f)
                yaw2 = -0.34906584f;
            *(float *)(commando_data + 0x294) = pitch2;
            *(float *)(commando_data + 0x298) = yaw2;
        }
    }
    update_joint_animation(
        commando_moby, commando_data + 0x130, 1, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    update_joint_animation(
        commando_moby, commando_data + 0x1B0, 2, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    update_joint_animation(
        commando_moby, commando_data + 0x230, 3, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
}

/* Emit randomized joint particles while the moby is active. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5CD0), where it is exact; names translated to the US level program. */
void emit_blarg_elite_commando_joint_sparks(char *commando_moby) {
    float matrix[16], velocity[4], acceleration[4], scatter[4];
    int   i;
    if (MOBY(commando_moby)->visible == 0 || MOBY(commando_moby)->state == 10)
        return;
    for (i = 0; i < 2; i++) {
        float speed, z;
        int   color1, color2, time1, time2, time3;
        if (random_float_between(0.0f, 1.0f) > 0.75f)
            continue;
        build_moby_bone_transform(commando_moby, i + 4, matrix);
        speed = (random_float_between(-0.2f, 0.2f) + 1.0f) * -3.0f * frame_time;
        fast_vec_normalize(velocity, matrix + 8, speed);
        {
            float component = (random_float_between(-0.2f, 0.2f) + 1.0f) * -2.0f;
            z = component * frame_time;
        }
        clear_u64_value(acceleration);
        acceleration[2] = z;
        z = random_float_between(-1.5f, 1.5f) * frame_time;
        random_vector(scatter, z, z);
        fast_vec_add(acceleration, acceleration, scatter);
        velocity[3] = random_float_between(0.2f, 0.2f);
        acceleration[3] = random_float_between(0.4f, 0.4f);
        color1 = fast_tween_color(0x801010FF, 0x801080FF, random_float_between(0.0f, 1.0f));
        color2 = fast_tween_color(0x400000C0, 0x4030FFFF, random_float_between(0.0f, 1.0f));
        time1 =
            truncate_float_to_s32(scale_time((random_float_between(-0.1f, 0.1f) + 1.0f) * 5.0f));
        time2 =
            truncate_float_to_s32(scale_time((random_float_between(-0.1f, 0.1f) + 1.0f) * 20.0f));
        time3 =
            truncate_float_to_s32(scale_time((random_float_between(-0.1f, 0.1f) + 1.0f) * 20.0f));
        spawn_spark(matrix + 12, velocity, acceleration, color1, color2, time1, time2, time3, -1);
    }
}

L16ParticleChoices D_L16_001E8300 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

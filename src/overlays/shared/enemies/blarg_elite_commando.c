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
#include "rnc/overlay/shared/math/quad.h"
#include "rnc/game/options.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_data/blarg_elite_commando_update.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_rocket.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"

enum {
    COMMANDO_PURSUIT_MODE_DISABLED = 2
};

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

typedef struct L16DamageData {
    u8            pad0[0x20];
    float         health;
    s16           unk24;
    u8            pad26[0x2];
    s8            unk28;
    s8            unk29;
    u8            pad2A[0x6];
    f32           unk30;
    u8            pad34[0x4];
    int           reset_timer;
    u8            pad3C[0x24];
    short         damage[3];
    u8            pad66[0x1];
    unsigned char animation;
    u8            pad68[0x8];
    float         knockback[4];
    float         impulse_time;
    float         vertical_impulse;
    float         forward_impulse;
    float         lateral_impulse;
    int           sound_timer;
    int           active;
    float         damping;
    u8            pad9C[0x11];
    unsigned char pending;
    u8            padAE[0x12];
    float         blend_start;
    float         blend_end;
    u8            padC8[0x8];
    float         target[4];
    u8            padE0[0x30];
    int           owner;
    int           pursuit_mode;
    u8            pad118[0x8];
    s32           unk120;
    float         pursuit_range;
    s32           unk128;
    int           path_index;
    u8            unk130[0x64];
    f32           joint_1_target_pitch;
    f32           joint_1_target_yaw;
    u8            pad19C[0x14];
    u8            unk1B0[0x64];
    f32           joint_2_target_pitch;
    f32           joint_2_target_yaw;
    u8            pad21C[0x14];
    u8            unk230[0x64];
    f32           unk294;
    f32           joint_3_target_yaw;
    u8            pad29C[0x14];
    f32           initial_position_x;
    f32           initial_position_y;
    u8            pad2B8[0x4];
    f32           cached_position_z;
    f32           unk2C0;
    f32           base_position_y;
    f32           unk2C8;
    f32           parent_heading_offset;
    char         *unk2D0;
    f32           movement_step_length;
    u8            heading_angular_step[0x4];
    s16           state_countdown_ticks;
    u16           rocket_fire_cycle_count;
    int           pursuit_timer;
    f32           bobbing_z_phase_step;
    f32           unk2E8;
    f32           bobbing_amplitude;
    f32           bobbing_x_phase_step;
    f32           unk2F4;
    f32           unk2F8;
    f32           bobbing_y_phase;
    s16           selected_path_node_index;
    s16           patrol_countdown_ticks;
    s32           voice_playback_handle;
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
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
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
extern void
randomize_blarg_elite_commando_group_pursuit_timers(void *, int, int) __asm__("FUN_L16_002d3f78");
extern float func_001F9878_timer(float) __asm__("FUN_001f96b0");
extern void  spawn_glow_particle(void *, void *, void *, float, int, int, int, int, int) __asm__("FUN_L00_002ac910");
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
    float          target[4], scratch_vector[4], delta[4];
    L16DamageData *commando_data = (L16DamageData *)MOBY(commando_moby)->pvars;
    char          *source, *aim_target_moby;
    float          distance, angle;
    float         *heading, *movement;
    int            ticks;
    int            owner_moby_class;
    update_blarg_elite_commando_damage_and_pursuit(commando_moby);
    aim_target_moby = *(char **)(&commando_data->owner);
    owner_moby_class = MOBY(aim_target_moby)->oclass;
    if (owner_moby_class == 0 || owner_moby_class == 0x1A3) {
        qcopy(target, &hero.motion.near_ground_point);
    } else {
        qcopy(target, &MOBY(aim_target_moby)->pos);
    }
    if ((&big_head_cheat_enabled)[7])
        manipulator_set_scale(&commando_data->unk130, 2.5f);
    switch (((unsigned char *)commando_moby)[0x20]) {
    case COMMANDO_STATE_INITIALIZE:
        *(int *)&commando_data->bobbing_amplitude = 0;
        commando_data->unk29 = 1;
        commando_data->unk24 = 3;
        commando_data->unk28 = 2;
        commando_data->unk30 = 1.5f;
        commando_data->health = 3.0f;
        commando_data->unk2F4 = random_angle_radians();
        commando_data->bobbing_y_phase = random_angle_radians();
        commando_data->unk2E8 = random_angle_radians();
        commando_data->bobbing_x_phase_step =
            random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        commando_data->unk2F8 = random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        commando_data->bobbing_z_phase_step =
            random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        commando_data->patrol_countdown_ticks =
            truncate_float_to_s32(scale_time(random_float_between(180.0f, 360.0f)));
        if (random_integer_below(2))
            MOBY(commando_moby)->flags |= 0x8000;
        else
            MOBY(commando_moby)->flags &= 0x7FFF;
        {
            char *position = &MOBY(commando_moby)->pos;
            if (commando_data->unk120 >= 0) {
                int node = truncate_float_to_s32(random_float_between(
                    0.0f, (float)(*(int *)D_L16_001B0930[commando_data->unk120] - 1)));
                commando_data->selected_path_node_index = node;
                qcopy(position, D_L16_001B0930[commando_data->unk120] + (short)node * 16 + 0x10);
            }
            commando_data->reward_hits = 1;
            qcopy(&commando_data->unk2C0, position);
            qcopy(&commando_data->initial_position_x, position);
        }
        if (commando_data->unk128) {
            MOBY(commando_moby)->state = COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM;
            MOBY(commando_moby)->visible = 0;
            MOBY(commando_moby)->flags = (MOBY(commando_moby)->flags | 1) & 0xEFFF;
            MOBY(commando_moby)->collision = 0;
            if (((unsigned char *)commando_moby)[0x53] != 1) {
                blend_moby_animation(commando_moby, 1, 0, scale_game_frames(20));
            }
        } else
            goto idle;
        break;
    case COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM:
        if (commando_data->unk2D0) {
            MOBY(commando_moby)->visible = 1;
            MOBY(commando_moby)->flags &= 0xFFFE;
            movement = scratch_vector;
            transform_vector_by_basis(
                movement, &commando_data->unk2C0, commando_data->unk2D0 + 0xC0);
            fast_vec_add(&MOBY(commando_moby)->pos, movement, commando_data->unk2D0 + 0x10);
            MOBY(commando_moby)->rot.z = fast_add_rotations(
                *(float *)(commando_data->unk2D0 + 0x48), commando_data->parent_heading_offset);
        }
        break;
    case COMMANDO_STATE_RETURN_TO_INITIAL_POSITION:
        if (!fast_dec_timer_s16(&commando_data->state_countdown_ticks)) {
            fast_vec_normalize(
                scratch_vector, commando_moby + 0xC0, commando_data->state_countdown_ticks * 0.1f);
            fast_vec_add(scratch_vector, scratch_vector, &MOBY(commando_moby)->pos);
            func_L16_002D5280_update(commando_moby, scratch_vector, 1);
        } else {
            heading = (float *)&MOBY(commando_moby)->rot.z;
            angle = fast_arc_tan(commando_data->initial_position_x - MOBY(commando_moby)->pos.x,
                                 commando_data->initial_position_y - MOBY(commando_moby)->pos.y);
            approach_angle(heading,
                           (float *)(&commando_data->heading_angular_step),
                           angle,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
            distance =
                func_L16_002D5280_update(commando_moby, &commando_data->initial_position_x, 1);
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            if (distance < 1.0f)
                MOBY(commando_moby)->state = COMMANDO_STATE_IDLE;
        }
        break;
    case COMMANDO_STATE_IDLE:
        fast_dec_timer_s16(&commando_data->state_countdown_ticks);
        if (commando_data->pursuit_mode != COMMANDO_PURSUIT_MODE_DISABLED) {
            if (commando_data->state_countdown_ticks == 0) {
                if (commando_data->unk120 >= 0) {
                    MOBY(commando_moby)->state = COMMANDO_STATE_MOVE_TO_ATTACK_NODE;
                    commando_data->state_countdown_ticks =
                        truncate_float_to_s32(scale_time(random_float_between(30.0f, 90.0f)));
                    commando_data->selected_path_node_index =
                        (commando_data->selected_path_node_index + 1) % 2;
                    select_blarg_elite_commando_movement_animation(commando_moby);
                } else if (!random_integer_below(20)) {
                    MOBY(commando_moby)->state = COMMANDO_STATE_PREPARE_ROCKET_FIRE;
                    if (((unsigned char *)commando_moby)[0x53] != 2) {
                        blend_moby_animation(commando_moby, 2, 0, scale_game_frames(10));
                    }
                }
            }
        } else if (fast_dec_timer_s16(&commando_data->patrol_countdown_ticks)) {
            if (commando_data->unk120 >= 0) {
                commando_data->selected_path_node_index =
                    (commando_data->selected_path_node_index + 1) % 2;
                MOBY(commando_moby)->state = COMMANDO_STATE_MOVE_TO_PATROL_NODE;
                commando_data->patrol_countdown_ticks = scale_game_frames(180);
                select_blarg_elite_commando_movement_animation(commando_moby);
            }
        }
        break;
    case COMMANDO_STATE_MOVE_TO_PATROL_NODE: {
        float path_distance =
            func_L16_002D5280_update(commando_moby,
                                     D_L16_001B0930[commando_data->unk120] +
                                         (commando_data->selected_path_node_index * 16 + 0x10),
                                     1);
        if (path_distance < 0.01f) {
            MOBY(commando_moby)->state = COMMANDO_STATE_IDLE;
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            commando_data->patrol_countdown_ticks =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 360.0f)));
        } else if (fast_dec_timer_s16(&commando_data->patrol_countdown_ticks)) {
            MOBY(commando_moby)->state = COMMANDO_STATE_IDLE;
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            commando_data->patrol_countdown_ticks =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 360.0f)));
        }
        if (commando_data->movement_step_length < 2.0f * frame_time && path_distance < 0.5f) {
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
                       (float *)(&commando_data->heading_angular_step),
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (((unsigned char *)commando_moby)[0x70] & 2) {
            MOBY(commando_moby)->state = COMMANDO_STATE_FIRE_ROCKETS;
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
                       (float *)(&commando_data->heading_angular_step),
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
            int count = commando_data->rocket_fire_cycle_count + 1;
            commando_data->rocket_fire_cycle_count = count;
            if ((short)count >= 3) {
                MOBY(commando_moby)->state = COMMANDO_STATE_IDLE;
                if (((unsigned char *)commando_moby)[0x53] != 0)
                    blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
                commando_data->state_countdown_ticks = scale_game_frames(60);
                commando_data->rocket_fire_cycle_count = 0;
            }
        }
        break;
    case COMMANDO_STATE_MOVE_TO_ATTACK_NODE:
        distance =
            func_L16_002D5280_update(commando_moby,
                                     D_L16_001B0930[commando_data->unk120] +
                                         (commando_data->selected_path_node_index * 16 + 0x10),
                                     1);
        fast_dec_timer_s16(&commando_data->state_countdown_ticks);
        if (distance < 0.01f || commando_data->state_countdown_ticks == 0) {
            MOBY(commando_moby)->state = COMMANDO_STATE_PREPARE_ROCKET_FIRE;
            if (((unsigned char *)commando_moby)[0x53] != 2)
                blend_moby_animation(commando_moby, 2, 0, scale_game_frames(30));
        }
        if (distance < 0.5f && commando_data->movement_step_length < 2.0f * frame_time) {
            if (((unsigned char *)commando_moby)[0x53] != 2) {
                blend_moby_animation(commando_moby, 2, 0, scale_game_frames(30));
            }
        }
        break;
    case COMMANDO_STATE_KNOCKBACK_RECOVERY:
        if (((unsigned char *)commando_moby)[0x70] & 2) {
        idle:
            MOBY(commando_moby)->state = COMMANDO_STATE_IDLE;
            if (((unsigned char *)commando_moby)[0x53] != 0) {
                ticks = 10;
            animate_idle:
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(ticks));
            }
        } else {
            fast_vec_normalize(scratch_vector, commando_moby + 0xC0, -frame_time);
            fast_vec_add(&MOBY(commando_moby)->pos, &MOBY(commando_moby)->pos, scratch_vector);
        }
        break;
    case COMMANDO_STATE_FATAL_FALL: {
        char *position;
        movement = scratch_vector;
        position = &MOBY(commando_moby)->pos;
        qcopy(movement, position);
        if (step_moby_motion(commando_moby, &commando_data->knockback) & 0x160) {
            FUN_L16_002d41f8_2(commando_moby, position);
            func_0022ED80_update(0, 0, commando_moby);
            mark_moby_for_removal(commando_moby);
            return;
        }
        fast_vec_sub(movement, position, movement);
        fast_vec_add(&commando_data->unk2C0, &commando_data->unk2C0, movement);
    } break;
    case 6:
    default:
        break;
    }
    if (blarg_elite_commando_bobbing_and_voice_enabled_gp) {
        commando_data->cached_position_z = MOBY(commando_moby)->pos.z;
        if (((unsigned char *)commando_moby)[0x20] != 0 &&
            ((unsigned char *)commando_moby)[0x20] != 10) {
            approach_value((float *)(&commando_data->bobbing_amplitude), 0.25f, frame_time * 0.25f);
            commando_data->unk2F4 =
                fast_add_rotations(commando_data->unk2F4, commando_data->bobbing_x_phase_step);
            commando_data->bobbing_y_phase =
                fast_add_rotations(commando_data->bobbing_y_phase, commando_data->unk2F8);
            commando_data->unk2E8 =
                fast_add_rotations(commando_data->unk2E8, commando_data->bobbing_z_phase_step);
            MOBY(commando_moby)->pos.x =
                commando_data->unk2C0 +
                commando_data->bobbing_amplitude * fast_sin(commando_data->unk2F4);
            MOBY(commando_moby)->pos.y =
                commando_data->base_position_y +
                commando_data->bobbing_amplitude * fast_sin(commando_data->bobbing_y_phase);
            MOBY(commando_moby)->pos.z =
                commando_data->unk2C8 +
                commando_data->bobbing_amplitude * fast_sin(commando_data->unk2E8);
        }
        if (fast_vec_dist(&MOBY(commando_moby)->pos, D_L16_001671C0) < 32.0f &&
            !voice_is_playing(commando_moby, commando_data->voice_playback_handle))
            commando_data->voice_playback_handle = func_0022ED80_update(7, 4, commando_moby);
    }
    if (blarg_elite_commando_joint_aim_enabled_gp)
        update_blarg_elite_commando_joint_aim(commando_moby);
    if (blarg_elite_commando_joint_sparks_enabled_gp && D_001413DC != 22)
        emit_blarg_elite_commando_joint_sparks(commando_moby);
    if (current_level_index == LEVEL_VELDIN2 && D_L16_0015F5C4 == 2 &&
        (unsigned int)(D_L16_0016CC90 - 3) < 3U)
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
            if (current_level_index == LEVEL_VELDIN2 && commando_data->reward_hits) {
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
    L16DamageData *commando_data = (L16DamageData *)MOBY(moby)->pvars;
    commando_data->unk2D0 = parent_moby;
    qcopy(&commando_data->unk2C0, position);
    commando_data->parent_heading_offset = *(float *)(vector + 8);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D51A8), where it is exact; names translated to the US level program. */
void start_blarg_elite_commando_return_to_initial_position(
    struct Moby *moby, int return_countdown_ticks) __asm__("FUN_L16_002d3de0");

void start_blarg_elite_commando_return_to_initial_position(struct Moby *moby,

                                                           int return_countdown_ticks) {
    L16DamageData *commando_data = (L16DamageData *)moby->pvars;
    if (moby->state == COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM) {
        moby->state = COMMANDO_STATE_RETURN_TO_INITIAL_POSITION;
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
        }
        moby->visible = 1;
        moby->flags &= 0xFFFE;
        moby->collision = moby->pclass->collision;
        moby->flags |= 0x1000;
        commando_data->state_countdown_ticks = return_countdown_ticks;
        commando_data->cached_position_z = moby->pos.z;
        qcopy(&commando_data->unk2C0, &moby->pos);
        commando_data->movement_step_length = frame_time * 5.0f;
    }
}

/* steers the moby's vector toward the target and returns the distance */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5280), where it is exact; names translated to the US level program. */
float advance_blarg_elite_commando_base_position(char  *moby,
                                                 float *target) __asm__("FUN_L16_002d3eb8");

float advance_blarg_elite_commando_base_position(char *moby, float *target) {
    L16DamageData *commando_motion_data = (L16DamageData *)MOBY(moby)->pvars;
    char          *base_position = &commando_motion_data->unk2C0;
    float          distance = fast_vec_dist(base_position, target);
    float          acceleration_braking_step = frame_time_sq * 4.0f;
    int            zero[1];
    float          movement_step[4];
    zero[0] = 0;
    advance_accelerated_scalar((float *)zero,
                               (float *)(&commando_motion_data->movement_step_length),
                               distance,
                               acceleration_braking_step,
                               acceleration_braking_step,
                               frame_time * 5.0f);
    fast_vec_sub(movement_step, target, base_position);
    fast_vec_normalize(movement_step, movement_step, commando_motion_data->movement_step_length);
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
    L16DamageData *commando_path_state = (L16DamageData *)MOBY(moby)->pvars;
    char          *target = D_L16_001B0930[commando_path_state->unk120] +
                   (commando_path_state->selected_path_node_index << 4);
    float angle = fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                               *(float *)(target + 0x14) - MOBY(moby)->pos.y);
    float delta = fast_subtract_rotations(angle, MOBY(moby)->rot.z);
    if (delta < 0.78539819f && -0.78539819f < delta) {
        if ((unsigned char)MOBY(moby)->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
        }
    } else if (delta < 0.0f) {
        if (MOBY(moby)->flags & 0x8000) {
            if ((unsigned char)MOBY(moby)->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
        } else {
            if ((unsigned char)MOBY(moby)->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(0x14));
        }
    } else if (MOBY(moby)->flags & 0x8000) {
        if ((unsigned char)MOBY(moby)->prev_seq != 5)
            blend_moby_animation(moby, 5, 0, scale_game_frames(0x14));
    } else {
        if ((unsigned char)MOBY(moby)->prev_seq != 4)
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
            FUN_L00_0026a9f0(origin,
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
    float          position[4], target[4], matrix[16], delta[4], aim[4], aimDelta[4], velocity[4];
    L16DamageData *commando_data = (L16DamageData *)MOBY(commando_moby)->pvars;
    if (commando_data->pursuit_mode != COMMANDO_PURSUIT_MODE_DISABLED) {
        float *savedTarget, *savedMatrix;
        float  yaw, pitch;
        qcopy(position, &MOBY(commando_moby)->pos);
        position[2] += 2.0f;
        if (*(short *)(*(char **)(&commando_data->owner) + 0xA6) == 0)
            qcopy(target, D_0013F420);
        else
            qcopy(target, &commando_data->target);
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
            commando_data->joint_1_target_yaw = halfYaw;
            commando_data->joint_1_target_pitch = halfPitch;
            commando_data->joint_2_target_yaw = halfYaw;
            commando_data->joint_2_target_pitch = halfPitch;
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
            commando_data->unk294 = pitch2;
            commando_data->joint_3_target_yaw = yaw2;
        }
    }
    update_joint_animation(
        commando_moby, &commando_data->unk130, 1, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    update_joint_animation(
        commando_moby, &commando_data->unk1B0, 2, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    update_joint_animation(
        commando_moby, &commando_data->unk230, 3, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
}

/* Emit randomized joint particles while the moby is active. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5CD0), where it is exact; names translated to the US level program. */
void emit_blarg_elite_commando_joint_sparks(char *commando_moby) {
    float matrix[16], velocity[4], acceleration[4], scatter[4];
    int   i;
    if (MOBY(commando_moby)->visible == 0 ||
        MOBY(commando_moby)->state == COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM)
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

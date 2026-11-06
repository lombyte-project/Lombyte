/* Level 04 moby class big_axebot: the large axe-swinging robot, and the spawn of its axe. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/normalize_vector_triplet.h"
#include "rnc/overlay/shared/enemies/trooper_set_knockback.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/math/quad.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB670), where it is exact; names translated to the US level program. */

void blend_big_axebot_and_axe_animation(char *moby,
                                        void *new_seq,
                                        void *frame_index,
                                        void *blend_frames) __asm__("FUN_L04_002ba3e0");

typedef struct BigAxebotVars {
    u8    pad_0[0x424];
    char *axe_moby;
} BigAxebotVars;

void blend_big_axebot_and_axe_animation(char *moby,
                                        void *new_seq,
                                        void *frame_index,

                                        void *blend_frames) {
    BigAxebotVars *axebot_pvars = (BigAxebotVars *)MOBY(moby)->pvars;
    void          *linked_axe_moby;
    blend_moby_animation(moby, new_seq, frame_index, blend_frames);
    linked_axe_moby = (void *)axebot_pvars->axe_moby;
    if (linked_axe_moby != 0) {
        blend_moby_animation(linked_axe_moby, new_seq, frame_index, blend_frames);
    }
}
/* Sets the float at 0x58 on the moby and on the linked moby at data+0x424, if any. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB6D8), where it is exact; names translated to the US level program. */

void set_big_axebot_and_axe_animation_speed(struct Moby *moby,
                                            float animation_speed) __asm__("FUN_L04_002ba448");

void set_big_axebot_and_axe_animation_speed(struct Moby *moby, float animation_speed) {
    BigAxebotVars *big_axebot_pvars = (BigAxebotVars *)moby->pvars;
    moby->anim_speed = animation_speed;
    if (big_axebot_pvars->axe_moby != 0) {
        *(float *)(big_axebot_pvars->axe_moby + 0x58) = animation_speed;
    }
}
/* Spawns an effect on the moby at its marker vector. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB700), where it is exact; names translated to the US level program. */

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void spawn_explosion(void *,
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

void FUN_L04_002ba470(char *moby) {
    float explosion_position[4];
    float unused_explosion_argument[4];
    clear_u64_value(unused_explosion_argument);
    moby_point_position(moby, 6, explosion_position);
    spawn_explosion(moby,
                    unused_explosion_argument,
                    explosion_position,
                    0.0f,
                    0.0f,
                    10,
                    3,
                    4,
                    2.0f,
                    1.0f,
                    100000.0f,
                    -1,
                    1.5f,
                    15.0f,
                    1,
                    1,
                    -1,
                    0);
}

extern void select_moby_combat_target(char *, float *, float) __asm__("FUN_L00_0025fcb8");

/* Update of the big axebot: hit reaction, state machine (idle, alert, path, chase, return home, swing, stagger, turn, death), knockback, gravity, and its axe moby. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB7B0), where it is exact; names translated to the US level program. */

typedef float V4_B7B0[4] __attribute__((aligned(16)));

typedef struct Manip_B7B0 {
    unsigned char id;
    unsigned char on;
    char          pad02[0xE];
    V4_B7B0       q;
    char          pad20[0x20];
} Manip_B7B0;

typedef struct D_B7B0 {
    char                   pad00[0x20];
    float                  hp;
    short                  max_health;
    char                   pad26[2];
    unsigned char          hit_response_class;
    unsigned char          f29;
    char                   pad2A[4];
    unsigned char          f2E;
    char                   pad2F;
    float                  f30;
    char                   pad34[4];
    int                    extended_detection_request;
    char                   pad3C[0x1C];
    unsigned char          f58;
    char                   pad59;
    unsigned char          f5A;
    char                   pad5B[0x55];
    char                   hit_transition_state[7];
    unsigned char          fB7;
    char                   padB8[8];
    struct KnockbackMotion knockback; /* 0xC0 */
    Manip_B7B0             recoil_manipulator;
    Manip_B7B0             aim_manipulator;
    char                   leg_rig_state[0x250];
    V4_B7B0                home;
    unsigned char          start_waiting_for_target;
    unsigned char          last_non_hit_recovery_state;
    short                  idle_action_delay_timer;
    short                  hit_response_cooldown_timer;
    unsigned char          extended_detection_timer;
    unsigned char          alert_voice_cooldown_timer;
    float                  recoil_pitch_angle;
    char                   pad40C[4];
    float                  yaw_turn_velocity;
    float                  movement_yaw;
    float                  hit_recovery_displacement;
    int                    movement_boundary_path_index;
    float                  fall_displacement;
    struct M_B7B0         *attached_axe_moby;
    float                  tracked_aim_yaw;
    float                  recoil_displacement;
    int                    path_trigger_volume_index;
    int                    approach_path_index;
    short                  path_waypoint_index;
    short                  path_follow_started;
    float                  detection_range;
    char                   pad440[8];
    int                    recoil_pose_cooldown_timer;
} D_B7B0;

typedef struct M_B7B0 {
    char           pad00[0x10];
    V4_B7B0        pos;
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  proximity_ground_probe_enabled;
    char           pad32[2];
    unsigned short f34;
    char           pad36[0xA];
    V4_B7B0        rot;
    char           pad50[2];
    unsigned char  animation_source_sequence;
    unsigned char  animation_target_sequence;
    char           pad54[4];
    float          f58;
    char           pad5C[0x14];
    unsigned char  f70;
    char           pad71[7];
    D_B7B0        *data;
    char           pad7C[3];
    char           f7F;
    char           pad80[0x24];
    unsigned char  fA4;
    char           padA5;
    short          cls;
    char           padA8[0x14];
    unsigned char  behavior_flags;
    char           padBD[3];
    V4_B7B0        fC0;
} M_B7B0;

typedef struct {
    V4_B7B0 pos;
    char    pad10[0x30];
    char   *f40;
    char    pad44[0xC];
} T_B7B0;

typedef struct {
    char    pad00[0x10];
    V4_B7B0 pos;
    M_B7B0 *att;
    char    pad24[8];
    float   damage_amount;
} Hit_B7B0;

typedef struct {
    int     count;
    int     pad[3];
    V4_B7B0 pts[1];
} P_B7B0;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float D_L04_0015F580[] MACRO_ADDR;
extern float D_L04_00166F40[4];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float spring_turn(float cur, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   D_L04_0015F5C4;
extern int   FUN_L00_001fefc8(void *);
extern int   is_point_inside_clip_volume(void *point, int volume_index) __asm__("FUN_00214720");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int move_moby_xy_with_collision_constraints(
    void *, void *, void *, int, float, float, float, float) __asm__("FUN_L00_00258490");
extern int step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int allocate_voice_for_target_entry(int, int, char *);
extern int mark_moby_for_removal_cf() __asm__("FUN_L04_002ba470");
extern void       *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
void *spawn_big_axebot_axe(char *source_moby, char *spawn_position) __asm__("FUN_L04_002d34d8");
extern void *random_integer_below_cf(void *, int, unsigned int) __asm__("FUN_001153fc");
extern void  build_axis_rotation_quaternion(void *, int, float) __asm__("FUN_L00_001fff28");
extern void  FUN_L00_00257470(void *, int, int);
extern void  leg_walker_update(void *, void *, float, float, float) __asm__("FUN_L04_00295778");
extern void  FUN_L00_0025a120(void *);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void start_hit_color_flash(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void reset_leg_rig_from_animation(void *, void *) __asm__("FUN_L04_00295de8");
extern void advance_moby_animation(void *);
extern void build_moby_bone_transform(char *, int, char *);
extern void spawn_nearby_effect(char *m, float t, int a, int b, void *pos, void *vel) __asm__("FUN_L00_00259bc8");
extern void copy_matrix3x4(void *, void *);
extern void refresh_moby_spatial_bounds_from_basis(void *);
void        blend_big_axebot_and_axe_animation(char *arg,
                                               void *a,
                                               void *b,
                                               void *c) __asm__("FUN_L04_002ba3e0");

void         big_axebot_update(M_B7B0 *moby) __asm__("FUN_L04_002ba520");
extern float fast_cos(float);
extern float fast_sin(float);

/* Moby class 217 */
void big_axebot_update(M_B7B0 *moby) {
    enum {
        BIG_AXEBOT_STATE_INITIALIZE = 0,
        BIG_AXEBOT_STATE_IDLE = 1,
        BIG_AXEBOT_STATE_ALERT = 2,
        BIG_AXEBOT_STATE_WAIT_FOR_TARGET = 3,
        BIG_AXEBOT_STATE_FOLLOW_PATH = 4,
        BIG_AXEBOT_STATE_CHASE_TARGET = 5,
        BIG_AXEBOT_STATE_RETURN_HOME = 6,
        BIG_AXEBOT_STATE_MELEE_ATTACK = 7,
        BIG_AXEBOT_STATE_HIT_RECOVERY = 8,
        BIG_AXEBOT_STATE_TURN = 9,
        BIG_AXEBOT_STATE_DYING = 10
    };
    enum {
        BIG_AXEBOT_HIT_REACTION_NONE = 0,
        BIG_AXEBOT_HIT_REACTION_FATAL = 1,
        BIG_AXEBOT_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE = 2,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_HIT_RECOVERY = 3,
        BIG_AXEBOT_HIT_REACTION_RETURN_TO_IDLE = 4,
        BIG_AXEBOT_HIT_REACTION_IDLE_RECOVERY = 5,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOIL_RECOVERY = 6,
        BIG_AXEBOT_HIT_REACTION_KNOCKBACK = 7,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOVERY = 8,
        BIG_AXEBOT_HIT_REACTION_NO_ADDITIONAL_RESPONSE = 9,
        BIG_AXEBOT_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        BIG_AXEBOT_HIT_REACTION_IGNORED = 11
    };
    D_B7B0 *data = moby->data;
    T_B7B0  tg;
    V4_B7B0 initial_position_snapshot;
    V4_B7B0 dest;
    V4_B7B0 tpos;
    V4_B7B0 vector_quaternion_scratch;
    int     flag = 0;
    float   rot;
    char   *target_moby;
    float   target_distance;
    float   target_yaw;
    float   hd;

    if (data->extended_detection_request != 0 || data->extended_detection_timer != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8(2.5f, (int)moby, 0, (unsigned char *)&data->recoil_manipulator);
    if (moby->proximity_ground_probe_enabled != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x15;
        }
    }
    select_moby_combat_target(
        (char *)moby, (float *)&tg, flag ? data->detection_range * 2.0f : data->detection_range);
    target_moby = tg.f40;
    rot = moby->rot[2];
    target_distance = vector_distance_xy(moby->pos, (float *)&tg);
    target_yaw = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
    hd = compute_interpolated_record_value(moby);
    if (D_L04_0015F5C4 == 2) {
        moby->f34 |= 0x41;
        if (data->attached_axe_moby != 0) {
            data->attached_axe_moby->f34 |= 0x41;
        }
    } else if (moby->f34 & 1) {
        moby->f34 &= ~0x41;
        if (data->attached_axe_moby != 0) {
            data->attached_axe_moby->f34 &= ~0x41;
        }
    }
    qcopy(initial_position_snapshot, moby->pos);
    qcopy(dest, moby->pos);
    qcopy(tpos, tg.pos);
    if (moby->proximity_ground_probe_enabled != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 32.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x1A;
        }
    }
    fast_dec_timer_s16(&data->hit_response_cooldown_timer);
    if (moby->state != BIG_AXEBOT_STATE_DYING && moby->state != BIG_AXEBOT_STATE_INITIALIZE) {
        int       unused_hit_reaction_output;
        float     dmg;
        Hit_B7B0 *hit;
        int       hit_reaction_result;
        float     knockback_yaw;

        dmg = 0.0f;
        hit = (Hit_B7B0 *)find_moby_hit(moby, 0x210000, 0);
        hit_reaction_result =
            hit_reaction(moby, hit, &data->hp, 0, &unused_hit_reaction_output, &dmg, 0, 4);
        if (hit != 0 && dmg != 0.0f && moby->state != BIG_AXEBOT_STATE_INITIALIZE) {
            if (hit->att != 0) {
                struct Hero *h = &hero;
                if (hit->att != h->items[0].moby) {
                    knockback_yaw = fast_arc_tan(moby->pos[0] - hit->att->pos[0],
                                                 moby->pos[1] - hit->att->pos[1]);
                } else {
                    knockback_yaw = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                 moby->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                knockback_yaw = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                             moby->pos[1] - h->motion.pos.f[1]);
            }
            data->knockback.speed_approach_step = frame_time_sq * 12.0f;
            data->knockback.gravity = frame_time_sq * 35.0f;
            data->knockback.speed = frame_time * 7.0f;
            data->knockback.flags = 9;
            data->knockback.unk3D = 0;
            data->hp -= dmg;
            if (data->hp <= 0.0f) {
                hit_reaction_result = 1;
            }
            if (data->hit_response_cooldown_timer != 0 && hit_reaction_result != 1) {
                if (data->recoil_pose_cooldown_timer <= 0) {
                    data->recoil_pitch_angle = -0.5235988f;
                }
                data->recoil_pose_cooldown_timer = scale_game_frames(0x1E);
                data->fB7 = 0x78;
                start_hit_color_flash(moby, (short *)data->hit_transition_state);
            } else {
                switch (hit_reaction_result) {
                case BIG_AXEBOT_HIT_REACTION_NONE:
                    break;
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_HIT_RECOVERY:
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOIL_RECOVERY:
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOVERY:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    if (data->recoil_pose_cooldown_timer <= 0) {
                        data->recoil_pitch_angle = -0.5235988f;
                    }
                    data->recoil_pose_cooldown_timer = scale_game_frames(0x1E);
                    if ((hit->att->cls == 0 || hit->damage_amount > 1.0f) &&
                        (moby->animation_source_sequence != 4 || hd < 17.0f || hd > 20.0f)) {
                        moby->behavior_flags |= 5;
                        blend_big_axebot_and_axe_animation(
                            (char *)moby, (void *)5, 0, (void *)scale_game_frames(6));
                        set_big_axebot_and_axe_animation_speed(moby, 2.0f);
                        moby->state = BIG_AXEBOT_STATE_HIT_RECOVERY;
                    }
                    data->recoil_displacement = 3.4f / scale_time(7.0f) * 0.5f;
                    data->fB7 = 0x78;
                    start_hit_color_flash(moby, (short *)data->hit_transition_state);
                    data->hit_response_cooldown_timer = scale_game_frames(0x46);
                    data->extended_detection_timer = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_KNOCKBACK:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    if (data->recoil_pose_cooldown_timer <= 0) {
                        data->recoil_pitch_angle = -0.5235988f;
                    }
                    data->recoil_pose_cooldown_timer = scale_game_frames(0x1E);
                    moby->behavior_flags |= 5;
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)5, 0, (void *)scale_game_frames(6));
                    set_big_axebot_and_axe_animation_speed(moby, 2.0f);
                    moby->state = BIG_AXEBOT_STATE_HIT_RECOVERY;
                    trooper_start_knockback(knockback_yaw, moby, &data->knockback, 5, 1, 0);
                    data->knockback.apex_animation_frame = 5.0f;
                    data->knockback.landing_animation_frame = 10.0f;
                    data->recoil_displacement = 3.4f / scale_time(7.0f);
                    data->fB7 = 0x78;
                    start_hit_color_flash(moby, (short *)data->hit_transition_state);
                    data->hit_response_cooldown_timer = scale_game_frames(0x46);
                    data->extended_detection_timer = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_RETURN_TO_IDLE:
                case BIG_AXEBOT_HIT_REACTION_IDLE_RECOVERY:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)6, (void *)2, (void *)scale_game_frames(4));
                    data->idle_action_delay_timer = scale_game_frames(0x28);
                    moby->state = BIG_AXEBOT_STATE_IDLE;
                    data->recoil_displacement = 3.4f / scale_time(7.0f);
                    data->fB7 = 0x78;
                    start_hit_color_flash(moby, (short *)data->hit_transition_state);
                    data->hit_response_cooldown_timer = scale_game_frames(0x46);
                    data->extended_detection_timer = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_FATAL:
                    data->knockback.collision_z_offset = 1.0f;
                    data->knockback.collision_radius_1024ths = 0x400;
                    trooper_set_knockback(&data->knockback, 5.7f, 4.0f);
                    *(OvlQuad *)vector_quaternion_scratch = *(OvlQuad *)hit->pos;
                    trooper_knockback_arc(vector_quaternion_scratch,
                                          &knockback_yaw,
                                          &data->knockback.speed,
                                          &data->knockback.vertical_speed);
                    trooper_start_knockback(knockback_yaw, moby, &data->knockback, 7, 1, 0);
                    data->knockback.apex_animation_frame = 12.0f;
                    data->knockback.landing_animation_frame = 25.0f;
                    moby->state = BIG_AXEBOT_STATE_DYING;
                    data->fB7 = 0x78;
                    start_hit_color_flash(moby, (short *)data->hit_transition_state);
                    FUN_L00_00257470(moby, 0, -1);
                    break;
                case BIG_AXEBOT_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_NO_ADDITIONAL_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_NO_FURTHER_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_IGNORED:
                    break;
                }
            }
        }
    }
    moby->fA4 = 0xFF;
    if (data->extended_detection_request != 0) {
        data->extended_detection_timer = scale_game_frames(0xF0);
    }
    data->extended_detection_request = 0;
    FUN_L00_001fefc8(&data->extended_detection_timer);
    if (moby->state != BIG_AXEBOT_STATE_INITIALIZE && data->f2E != 1) {
        if (data->attached_axe_moby != 0) {
            mark_moby_for_removal(data->attached_axe_moby);
        }
        mark_moby_for_removal(moby);
        return;
    }
    fast_dec_timer_s16(&data->idle_action_delay_timer);
    switch (moby->state) {
    case BIG_AXEBOT_STATE_INITIALIZE:
        data->hp = 3.0f;
        data->max_health = 3;
        data->hit_response_class = 2;
        data->f2E = 1;
        data->f29 = 0;
        data->f30 = 1.5f;
        moby->behavior_flags = 0;
        data->fall_displacement = 0.0f;
        data->attached_axe_moby = 0;
        data->alert_voice_cooldown_timer = 0;
        qcopy(data->home, moby->pos);
        if (data->recoil_manipulator.on == 0) {
            attach_manipulator((char *)moby, 0, &data->recoil_manipulator);
        }
        attach_manipulator((char *)moby, 0xC, &data->aim_manipulator);
        data->tracked_aim_yaw = moby->rot[2];
        data->f58 = 0xA;
        data->f5A = 0xC;
        if (data->start_waiting_for_target != 0) {
            moby->state = BIG_AXEBOT_STATE_WAIT_FOR_TARGET;
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)0xA, 0, (void *)scale_game_frames(0xC));
        } else {
            moby->state = BIG_AXEBOT_STATE_IDLE;
        }
        break;
    case BIG_AXEBOT_STATE_IDLE: {
        if (moby->animation_target_sequence == 6 &&
            data->idle_action_delay_timer == scale_game_frames(0x25)) {
            allocate_voice_for_target_entry(2, 0, (char *)moby);
        }
        if ((moby->f70 & 2) && (flag ? data->detection_range * 2.0f < target_distance
                                     : data->detection_range < target_distance)) {
            int arr[6];

            random_integer_below_cf(arr, 0, 0x18);
            arr[5] = 1;
            blend_big_axebot_and_axe_animation((char *)moby,
                                               (void *)arr[random_integer_below(6)],
                                               0,
                                               (void *)scale_game_frames(0x18));
        } else if (moby->animation_target_sequence != 0xD &&
                   (moby->animation_target_sequence != 6 || (moby->f70 & 2))) {
            if (flag ? target_distance < data->detection_range * 2.0f
                     : target_distance < data->detection_range) {
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)0xD, 0, (void *)scale_game_frames(0x18));
            }
        }
        if (data->path_trigger_volume_index != -1 && data->approach_path_index != -1 &&
            data->path_follow_started == 0 &&
            (flag ||
             is_point_inside_clip_volume(&hero.motion.pos, data->path_trigger_volume_index) != 0)) {
            moby->state = BIG_AXEBOT_STATE_FOLLOW_PATH;
            moby->behavior_flags |= 1;
            data->path_follow_started = 1;
        } else {
            if (moby->animation_source_sequence == 0xD &&
                FUN_L00_001fefc8(&data->alert_voice_cooldown_timer) != 0) {
                allocate_voice_for_target_entry(0xA, 0, (char *)moby);
                data->alert_voice_cooldown_timer = scale_game_frames(0xB4);
            }
            if (data->idle_action_delay_timer == 0) {
                if (target_distance < 4.5f) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
                    moby->state = BIG_AXEBOT_STATE_MELEE_ATTACK;
                } else if (flag ? target_distance < data->detection_range * 2.0f
                                : target_distance < data->detection_range) {
                    int nf = (moby->behavior_flags & 1) == 0;
                    if (nf) {
                        blend_big_axebot_and_axe_animation(
                            (char *)moby, (void *)2, 0, (void *)scale_game_frames(0xC));
                        moby->state = BIG_AXEBOT_STATE_ALERT;
                        moby->behavior_flags |= 1;
                    }
                }
            }
        }
        break;
    }
    case BIG_AXEBOT_STATE_ALERT:
        if (moby->animation_source_sequence == 2 && (moby->f70 & 2)) {
            blend_big_axebot_and_axe_animation(
                (char *)moby,
                (void *)((flag ? target_distance < data->detection_range * 2.0f
                               : target_distance < data->detection_range)
                             ? 0xD
                             : 0),
                0,
                (void *)scale_game_frames(0xC));
            moby->state = BIG_AXEBOT_STATE_IDLE;
        }
        break;
    case BIG_AXEBOT_STATE_WAIT_FOR_TARGET:
        if (moby->animation_source_sequence != 0 && (moby->f70 & 2)) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)0xB, 0, (void *)scale_game_frames(0xC));
        }
        if (data->idle_action_delay_timer == 0) {
            if (target_distance < 4.5f) {
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
                moby->state = BIG_AXEBOT_STATE_MELEE_ATTACK;
            } else if (flag ? target_distance < data->detection_range * 2.0f
                            : target_distance < data->detection_range) {
                int nf = (moby->behavior_flags & 1) == 0;
                if (nf) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)2, 0, (void *)scale_game_frames(0xC));
                    moby->state = BIG_AXEBOT_STATE_ALERT;
                    moby->behavior_flags |= 1;
                }
            }
        }
        break;
    case BIG_AXEBOT_STATE_FOLLOW_PATH: {
        P_B7B0 *path = ((P_B7B0 *)l04_paths[data->approach_path_index]);
        float   a;

        if (moby->animation_target_sequence != 3) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)3, 0, (void *)scale_game_frames(0xC));
        }
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = BIG_AXEBOT_STATE_MELEE_ATTACK;
        }
        if (vector_distance_xy(path->pts[data->path_waypoint_index], moby->pos) < 1.0f) {
            data->path_waypoint_index++;
            if (data->path_waypoint_index == path->count) {
                if (flag ? data->detection_range * 2.0f < target_distance
                         : data->detection_range < target_distance) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, 0, 0, (void *)scale_game_frames(0x18));
                } else {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)0xD, 0, (void *)scale_game_frames(0x18));
                }
                moby->state = BIG_AXEBOT_STATE_IDLE;
                break;
            }
        }
        a = fast_arc_tan(path->pts[data->path_waypoint_index][0] - moby->pos[0],
                         path->pts[data->path_waypoint_index][1] - moby->pos[1]);
        moby->rot[2] = spring_turn(moby->rot[2],
                                   a,
                                   &data->yaw_turn_velocity,
                                   frame_time_sq * 12.566371f,
                                   frame_time_sq * 6.2831855f,
                                   frame_time * 3.1415927f);
        vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * (frame_time * 6.0f);
        vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * (frame_time * 6.0f);
        vector_quaternion_scratch[2] = 0.0f;
        fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
        move_moby_xy_with_collision_constraints(
            moby, moby->pos, vector_quaternion_scratch, 1, 0.25f, 1.5f, 0.75f, 0.87266463f);
        break;
    }
    case BIG_AXEBOT_STATE_CHASE_TARGET:
        if (moby->behavior_flags & 8) {
            moby->behavior_flags &= ~8;
            reset_leg_rig_from_animation(moby, data->leg_rig_state);
        }
        data->movement_yaw = spring_turn(moby->rot[2],
                                         target_yaw,
                                         &data->yaw_turn_velocity,
                                         frame_time_sq * 6.2831855f,
                                         frame_time_sq * 125.663704f,
                                         frame_time * 12.566371f);
        leg_walker_update(moby,
                          data->leg_rig_state,
                          frame_time * 4.0f,
                          data->movement_yaw,
                          frame_time_sq * 20.0f);
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = BIG_AXEBOT_STATE_MELEE_ATTACK;
        } else if (data->movement_boundary_path_index != -1) {
            vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * 2.0f;
            vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * 2.0f;
            vector_quaternion_scratch[2] = 0.0f;
            fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
            if (point_in_path_xy(
                    vector_quaternion_scratch,
                    ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->pts,
                    ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->count) == 0) {
                data->yaw_turn_velocity = 0.0f;
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)8, 0, (void *)scale_game_frames(0xC));
                moby->state = BIG_AXEBOT_STATE_TURN;
            }
            if (point_in_path_xy(
                    tpos,
                    ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->pts,
                    ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->count) == 0) {
                moby->state = BIG_AXEBOT_STATE_RETURN_HOME;
            }
        }
        break;
    case BIG_AXEBOT_STATE_RETURN_HOME:
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = BIG_AXEBOT_STATE_MELEE_ATTACK;
        } else {
            float a;

            if (moby->behavior_flags & 8) {
                moby->behavior_flags &= ~8;
                reset_leg_rig_from_animation(moby, data->leg_rig_state);
            }
            a = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);
            data->movement_yaw = spring_turn(moby->rot[2],
                                             a,
                                             &data->yaw_turn_velocity,
                                             frame_time_sq * 6.2831855f,
                                             frame_time_sq * 125.663704f,
                                             frame_time * 12.566371f);
            leg_walker_update(moby,
                              data->leg_rig_state,
                              frame_time * 4.0f,
                              data->movement_yaw,
                              frame_time_sq * 20.0f);
            if (vector_distance_xy(moby->pos, data->home) < 2.0f) {
                moby->state = BIG_AXEBOT_STATE_IDLE;
            }
            if (data->movement_boundary_path_index != -1) {
                vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * 2.0f;
                vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * 2.0f;
                vector_quaternion_scratch[2] = 0.0f;
                fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
                if (point_in_path_xy(
                        vector_quaternion_scratch,
                        ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->pts,
                        ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->count) == 0) {
                    data->yaw_turn_velocity = 0.0f;
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)8, 0, (void *)scale_game_frames(0xC));
                    moby->state = BIG_AXEBOT_STATE_TURN;
                    moby->behavior_flags |= 0x10;
                }
                if (point_in_path_xy(
                        tpos,
                        ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->pts,
                        ((P_B7B0 *)l04_paths[data->movement_boundary_path_index])->count) != 0) {
                    moby->state = BIG_AXEBOT_STATE_IDLE;
                }
            }
        }
        break;
    case BIG_AXEBOT_STATE_MELEE_ATTACK:
        if (moby->animation_source_sequence == moby->animation_target_sequence &&
            compute_interpolated_record_value(moby) >= 17.0f &&
            compute_interpolated_record_value(moby) < 20.0f &&
            absolute_float(moby->pos[2] - tg.pos[2]) < 2.0f &&
            vector_distance_xy(moby->pos, (float *)&tg) < 4.5f) {
            float p[4];

            vector_quaternion_scratch[0] = fast_cos(target_yaw) * 0.2f;
            vector_quaternion_scratch[1] = fast_sin(target_yaw) * 0.2f;
            vector_quaternion_scratch[2] = 0.0f;
            qcopy(p, tg.pos);
            p[2] += 0.75f;
            if (target_moby != 0) {
                spawn_nearby_effect(target_moby, 1.0f, (int)moby, 1, p, vector_quaternion_scratch);
            }
        }
        if (hd > 20.0f) {
            if (moby->f58 > 1.0f) {
                float animation_speed = moby->f58 - frame_time * 3.0f;

                if (animation_speed < 1.0f) {
                    animation_speed = 1.0f;
                }
                set_big_axebot_and_axe_animation_speed(moby, animation_speed);
            }
        }
        if (moby->animation_source_sequence == 4 && (moby->f70 & 2)) {
            data->idle_action_delay_timer = scale_game_frames(0x5A);
            blend_big_axebot_and_axe_animation(
                (char *)moby,
                (void *)((flag ? target_distance < data->detection_range * 2.0f
                               : target_distance < data->detection_range)
                             ? 0xD
                             : 0),
                0,
                (void *)scale_game_frames(0xC));
            moby->state = BIG_AXEBOT_STATE_IDLE;
        }
        break;
    case BIG_AXEBOT_STATE_HIT_RECOVERY:
        if (!(moby->animation_target_sequence == 5 &&
              (moby->animation_source_sequence != moby->animation_target_sequence ||
               hd <= 13.0f))) {
            if (moby->behavior_flags & 4) {
                moby->behavior_flags &= ~4;
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(6));
                set_big_axebot_and_axe_animation_speed(moby, 1.5f);
                moby->state = BIG_AXEBOT_STATE_MELEE_ATTACK;
            } else if (moby->animation_target_sequence == 8 &&
                       (hd < 28.0f ||
                        moby->animation_source_sequence != moby->animation_target_sequence)) {
                float hit_recovery_displacement_limit = frame_scale * 0.11111111f;

                if (data->hit_recovery_displacement < hit_recovery_displacement_limit) {
                    data->hit_recovery_displacement += frame_time_sq * 20.0f;
                } else {
                    data->hit_recovery_displacement = hit_recovery_displacement_limit;
                }
                build_spherical_offset(vector_quaternion_scratch,
                                       data->hit_recovery_displacement,
                                       data->movement_yaw,
                                       moby->rot[1]);
                fast_vec_add(moby->pos, moby->pos, vector_quaternion_scratch);
                moby->rot[2] = spring_turn(moby->rot[2],
                                           target_yaw,
                                           &data->yaw_turn_velocity,
                                           frame_time_sq * 3.1415927f,
                                           frame_time_sq * 6.2831855f,
                                           frame_time * 4.1887903f);
            } else if (data->hit_recovery_displacement > 0.0f) {
                data->hit_recovery_displacement -= frame_time_sq * 200.0f;
                build_spherical_offset(vector_quaternion_scratch,
                                       data->hit_recovery_displacement,
                                       data->movement_yaw,
                                       moby->rot[1]);
                fast_vec_add(moby->pos, moby->pos, vector_quaternion_scratch);
                moby->rot[2] = spring_turn(moby->rot[2],
                                           target_yaw,
                                           &data->yaw_turn_velocity,
                                           frame_time_sq * 3.1415927f,
                                           frame_time_sq * 6.2831855f,
                                           frame_time * 4.1887903f);
            } else {
                moby->state = BIG_AXEBOT_STATE_IDLE;
                data->idle_action_delay_timer = 0;
            }
        }
        break;
    case BIG_AXEBOT_STATE_TURN:
        if (moby->animation_source_sequence == 8 && (moby->f70 & 2)) {
            moby->state = BIG_AXEBOT_STATE_CHASE_TARGET;
            moby->behavior_flags = (moby->behavior_flags | 8) & ~0x10;
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)9, 0, (void *)scale_game_frames(0xC));
        } else if (moby->behavior_flags & 0x10) {
            float a = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);

            moby->rot[2] = spring_turn(moby->rot[2],
                                       a,
                                       &data->yaw_turn_velocity,
                                       frame_time_sq * 3.1415927f,
                                       frame_time_sq * 6.2831855f,
                                       frame_time * 4.1887903f);
        } else {
            moby->rot[2] = spring_turn(moby->rot[2],
                                       target_yaw,
                                       &data->yaw_turn_velocity,
                                       frame_time_sq * 3.1415927f,
                                       frame_time_sq * 6.2831855f,
                                       frame_time * 4.1887903f);
        }
        break;
    case BIG_AXEBOT_STATE_DYING:
        if ((step_moby_motion(moby, &data->knockback) & 0x40) || moby->pos[2] < 1.0f) {
            mark_moby_for_removal_cf((char *)moby);
            if (data->attached_axe_moby != 0) {
                mark_moby_for_removal(data->attached_axe_moby);
            }
            FUN_L00_00257470(moby, 0, -1);
            FUN_L00_00263fd8((char *)moby,
                             0x798,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x799,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x79A,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    update_hit_color_flash(moby, data->hit_transition_state);
    if (data->recoil_displacement > 0.0f) {
        struct Hero *h = &hero;

        vector_quaternion_scratch[0] = fast_cos(fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                             moby->pos[1] - h->motion.pos.f[1])) *
                                       data->recoil_displacement;
        vector_quaternion_scratch[1] = fast_sin(fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                             moby->pos[1] - h->motion.pos.f[1])) *
                                       data->recoil_displacement;
        vector_quaternion_scratch[2] = 0.0f;
        fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
        if (data->fall_displacement == 0.0f) {
            data->recoil_displacement += -3.4f / (scale_time(7.0f) * scale_time(7.0f));
        }
        move_moby_xy_with_collision_constraints(
            moby, moby->pos, vector_quaternion_scratch, 1, 0.25f, 1.5f, 0.75f, 0.87266463f);
    }
    data->recoil_pitch_angle *= 0.96f;
    fast_dec_timer(&data->recoil_pose_cooldown_timer);
    build_axis_rotation_quaternion(vector_quaternion_scratch, 1, data->recoil_pitch_angle);
    build_axis_rotation_quaternion(data->recoil_manipulator.q, 2, 0.0f);
    multiply_quaternions(
        data->recoil_manipulator.q, data->recoil_manipulator.q, vector_quaternion_scratch);
    if (moby->state != BIG_AXEBOT_STATE_HIT_RECOVERY) {
        data->last_non_hit_recovery_state = moby->state;
    }
    data->fall_displacement += frame_time_sq * 29.8f;
    moby->pos[2] -= data->fall_displacement - 2.0f;
    {
        float ground_height = probe_ground_height(moby->pos, 0, 0.5f);

        moby->pos[2] -= 2.0f;
        if (moby->pos[2] < ground_height) {
            moby->pos[2] = ground_height;
            data->fall_displacement = 0.0f;
        }
    }
    if (moby->pos[2] < data->home[2] - 8.0f) {
        mark_moby_for_removal_cf((char *)moby);
        if (data->attached_axe_moby != 0) {
            mark_moby_for_removal(data->attached_axe_moby);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (moby->state != BIG_AXEBOT_STATE_INITIALIZE) {
        float q[4];
        float bone_transform[4][4];
        float bone_euler_angles[4];
        {
            data->tracked_aim_yaw = fast_add_rotations(data->tracked_aim_yaw,
                                                       fast_subtract_rotations(moby->rot[2], rot));
            build_moby_bone_transform((char *)moby, 0xC, (char *)bone_transform);
            matrix_to_euler(bone_transform, bone_euler_angles);
            build_axis_rotation_quaternion(
                q, 2, fast_subtract_rotations(target_yaw, data->tracked_aim_yaw));
            data->tracked_aim_yaw = target_yaw;
            multiply_quaternions(data->aim_manipulator.q, data->aim_manipulator.q, q);
        }
    }
    if (data->attached_axe_moby != 0) {
        float axe_bone_transform[4][4];
        {
            M_B7B0 *axe;

            build_moby_bone_transform((char *)moby, 0xD, (char *)axe_bone_transform);
            axe = data->attached_axe_moby;
            qcopy(axe->pos, axe_bone_transform[3]);
            advance_moby_animation(axe);
            update_entity(data->attached_axe_moby);
            copy_matrix3x4(data->attached_axe_moby->fC0, axe_bone_transform);
            normalize_vector_triplet(data->attached_axe_moby->fC0);
            refresh_moby_spatial_bounds_from_basis(data->attached_axe_moby);
            data->attached_axe_moby->f34 |= 6;
        }
    } else {
        float p[4];

        moby_point_position(moby, 0xB, p);
        data->attached_axe_moby = spawn_big_axebot_axe(moby, p);
    }
}

/* Creates the big axebot's axe (class 0x23A) at spawn_position, with the axebot's draw distance, yaw and spawn frame. */
void *spawn_big_axebot_axe(char *source_moby, char *spawn_position) {
    char  saved_position[16];
    char *saved_position_ptr = saved_position;
    char *created_moby;
    *(OvlQuad *)saved_position = *(OvlQuad *)spawn_position;
    created_moby = (char *)create_moby(0x23a);
    if (created_moby != 0) {
        MOBY(created_moby)->update_dist = 0xFF;
        MOBY(created_moby)->draw_dist = ((unsigned short)MOBY(source_moby)->draw_dist);
        created_moby[0x31] = 1;
        *(int *)(created_moby + 0x40) = 0;
        *(int *)(created_moby + 0x44) = 0;
        MOBY(created_moby)->rot.z = MOBY(source_moby)->rot.z;
        MOBY(created_moby)->spawn_frame = MOBY(source_moby)->spawn_frame;
        qcopy(created_moby + 0x10, saved_position_ptr);
        update_entity(created_moby);
    }
    return created_moby;
}

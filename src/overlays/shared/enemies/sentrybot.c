/* Shared moby class sentrybot. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/enemies/sentrybot_data/sentrybot_update.h"
#include "sda.h"
#include "rnc/overlay/shared/enemies/sentrybot_data/update_sentrybot_hit_response_and_target_visibility.h"

typedef struct {
    char  pad0[0x2084];
    int   f2084;
    char  pad1[0x258];
    short f22E0;
} Hero98BB8;

typedef struct {
    char pad0[0x180];
    int  f180[4];
} Sentry98BB8;

typedef struct {
    char  p[0x156];
    short h156;
} D_a0d0;

typedef struct {
    u8 pad[0x454];
    u8 killed[1]; /* per spawn id: spawn slot + 2 */
} KillFlags;

typedef struct {
    u8           pad0[0x74];
    u8           active;
    u8           pad75[0x13];
    struct Moby *owner;
} VoiceSlot;

extern char *
spawn_sentrybot_projectile(char *, float *, char *, char *) __asm__("FUN_L15_002e4580");
extern float        D_L15_001673C0[4];
extern float        sentrybot_positive_heading_delta_offsets_degrees[] __asm__("D_L15_001CBFE0");
extern float        sentrybot_negative_heading_delta_offsets_degrees[] __asm__("D_L15_001CC060");
extern float        sentrybot_turn_around_heading_offsets_degrees[] __asm__("D_L15_001CC0E0");
extern float        animation_end(int a, unsigned int b) __asm__("FUN_L04_00241910");
extern float        fast_sin(float);
extern float        vector_length_xy(void *);
extern int         *D_L15_001B0AB0[];
extern int          D_L15_0015F5C4;
extern int          D_L15_0015F5CC;
extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");
extern int          fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int          step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int begin_sentrybot_turn_toward_heading(unsigned char *, char *, int, int, float) __asm__("FUN_L15_00299880");
int move_sentrybot_toward_point(char  *moby,
                                float *point,
                                float  target_forward_step) __asm__("FUN_L15_00299ae0");

extern int
move_sentrybot_toward_point(char *moby, float *point, float arg) __asm__("FUN_L15_00299ae0");
extern int can_sentrybot_approach_linked_moby(void *) __asm__("FUN_L15_00299ca0");
int        find_sentrybot_attention_target(char *moby) __asm__("FUN_L15_00299dd8");

extern int find_sentrybot_attention_target(char *m) __asm__("FUN_L15_00299dd8");
int        can_sentrybot_advance_patrol(void *input_moby) __asm__("FUN_L15_00299ff8");

extern int can_sentrybot_advance_patrol(void *) __asm__("FUN_L15_00299ff8");
extern int random_integer_below(int);
extern int scale_game_frames(int);

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void
compute_selected_world_matrices(void *, int, void *, void *) __asm__("FUN_L00_0024f8f0");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025a120(void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void manipulator_set_scale(void *, float) __asm__("FUN_L00_00263b38");
void        begin_sentrybot_turn_toward_point(char  *moby,
                                              void  *moby_data,
                                              float *point,
                                              void  *next_state,
                                              void  *next_animation) __asm__("FUN_L15_00299a68");

extern void begin_sentrybot_turn_toward_point(
    char *moby, void *a, float *point, void *b, void *c) __asm__("FUN_L15_00299a68");
void alert_visible_group_sentrybots(void *mp) __asm__("FUN_L15_0029a0d0");

extern void update_sentrybot_joint_aim(void *) __asm__("FUN_L15_0029a2a8");
extern void update_sentrybot_hit_response_and_target_visibility(void *,
                                                                int) __asm__("FUN_L15_0029a570");
void        draw_sentrybot_effect_quads(char *moby) __asm__("FUN_L15_0029aba8");

extern void draw_sentrybot_effect_quads(char *m) __asm__("FUN_L15_0029aba8");
extern void mark_moby_killed_and_release_owned_voice(int) __asm__("FUN_L15_002a35d8");
extern void set_sentrybot_alarm_duration(char *m, int a) __asm__("FUN_L15_002cbac0");
void        prepare_sentrybot_alarm(unsigned char *moby) __asm__("FUN_L15_002cbba0");

extern void prepare_sentrybot_alarm(unsigned char *moby) __asm__("FUN_L15_002cbba0");
extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern void
transform_scaled_vertex_batch(char *arg0, int arg1, void *arg2, char *arg3) __asm__("FUN_0020cd48");

extern void FUN_L15_00299880_u(void *, void *, void *, void *, float) __asm__("FUN_L15_00299880");

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float
approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern void FUN_L00_00258830(int, int, float, float, float, int);

extern int D_L15_001ABE40[];

int can_sentrybot_approach_linked_moby(void *mm) __asm__("FUN_L15_00299ca0");

extern short *D_L15_001ABE40_a0[] __asm__("D_L15_001ABE40");
extern int    FUN_001efa68_a0(void *, void *, int, int, int) __asm__("FUN_001efa68");
void          set_sentrybot_attention_target(char        *source_moby,
                                             struct Moby *recipient_moby) __asm__("FUN_L15_0029a248");

extern void set_sentrybot_attention_target(char *, struct Moby *) __asm__("FUN_L15_0029a248");

extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");

float sentrybot_class_scale_multiplier_gp __asm__("D_L15_001613EC") = 1.0f;

extern char  D_001413F4[];
extern short sentrybot_target_visibility_height_check_gate[] __asm__("D_0013F658");
extern s32   D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;
extern char  D_0013F420[];
extern char *FUN_L00_0025a420(void *, int, int);
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   update_float_countdown(void *) __asm__("FUN_L08_00211b08");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int  FUN_L00_0025ff38(void *, void *, float, int, int, void *, int);

extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

extern KillFlags D_L15_001BABD0 NOT_SDA;
extern u8                       secondary_kill_record_gate_by_mission[8] __asm__("D_L15_0015FC88");
extern u8                       D_0014C050[][16] NOT_SDA;
extern u8                       D_0013E550[] NOT_SDA;
extern short                    D_L15_00161ABC_gp __asm__("D_L15_00161ABC") __attribute__((sda));

extern int D_L15_001ABE40_lui[] __asm__("D_L15_001ABE40") NOT_SDA;

/* Sentrybot update: runs the patrol, alert and attack state machine, then sets the alert level and the eye colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_00298BB8), where it is exact; names translated to the US level program. */

void sentrybot_update(unsigned char *moby) __asm__("FUN_L15_002979d8");

/* Moby class 44 */
void sentrybot_update(unsigned char *moby) {
    enum {
        SENTRYBOT_INITIALIZE = 0,
        SENTRYBOT_WAIT_FOR_LINKED_MOBY = 1,
        SENTRYBOT_PATROL_IDLE = 2,
        SENTRYBOT_PATROL_PAUSE = 3,
        SENTRYBOT_PATROL_MOVE = 4,
        SENTRYBOT_DEACTIVATE_LINKED_MOBYS_AFTER_ANIMATION = 5,
        SENTRYBOT_TURN_NEGATIVE_HEADING_DELTA = 6,
        SENTRYBOT_TURN_POSITIVE_HEADING_DELTA = 7,
        SENTRYBOT_TURN_AROUND = 8,
        SENTRYBOT_RETURN_HOME = 9,
        SENTRYBOT_WAIT_FOR_ANIMATION_THEN_PATROL_IDLE = 10,
        SENTRYBOT_WAIT_FOR_TRANSITION_ANIMATION = 11,
        SENTRYBOT_ALERT_IDLE = 12,
        SENTRYBOT_PATROL_MOVE_FAST = 13,
        SENTRYBOT_APPROACH_LINKED_MOBY = 14,
        SENTRYBOT_ACTIVATE_LINKED_MOBY = 15,
        SENTRYBOT_WAIT_FOR_ANIMATION_THEN_EMIT_WITH_NEGATIVE_JOINT_AIM = 16,
        SENTRYBOT_EMIT_WITH_NEGATIVE_JOINT_AIM = 17,
        SENTRYBOT_WAIT_FOR_ANIMATION_THEN_EMIT_WITH_POSITIVE_JOINT_AIM = 18,
        SENTRYBOT_EMIT_WITH_POSITIVE_JOINT_AIM = 19,
        SENTRYBOT_RETURN_HOME_FAST = 20,
        SENTRYBOT_RECOVERABLE_KNOCKBACK = 21,
        SENTRYBOT_DEATH_KNOCKBACK = 23
    };
    enum {
        SENTRYBOT_ANIM_TURN_NEGATIVE_HEADING_DELTA = 10,
        SENTRYBOT_ANIM_TURN_POSITIVE_HEADING_DELTA = 11,
        SENTRYBOT_ANIM_TURN_AROUND = 12
    };
    float          va[4];
    float          vb[4];
    float          emission_origin[4];
    float          selected_joint_world_matrix[4][4];
    int            j2;
    int            j3;
    unsigned char *sentrybot_instance_data;
    char          *target;
    int            flag;
    int            clip_volume_index;
    int            lvl;
    int            tick;
    int            alpha;
    int            color_pulse_period_frames;
    int            color;
    float          color_pulse_sine;

    if (sentrybot_update_disabled_gp != 0)
        return;
    sentrybot_instance_data = ((unsigned char *)MOBY(moby)->pvars);
    if (D_L15_0015F5C4 == 2) {
        MOBY(moby)->flags |= 0x41;
        return;
    }
    clip_volume_index = *(int *)(sentrybot_instance_data + 0x2D0);
    flag = 0;
    if (clip_volume_index != -1) {
        flag = is_point_inside_clip_volume(D_L15_001673C0, clip_volume_index) == 0;
        if (*(int *)(sentrybot_instance_data + 0x2D4) != -1) {
            flag &= is_point_inside_clip_volume(D_L15_001673C0,
                                                *(int *)(sentrybot_instance_data + 0x2D4)) == 0;
        }
    } else if (*(int *)(sentrybot_instance_data + 0x2D8) != -1) {
        flag = is_point_inside_clip_volume(D_L15_001673C0, clip_volume_index);
        if (*(int *)(sentrybot_instance_data + 0x2DC) != -1) {
            flag |= is_point_inside_clip_volume(D_L15_001673C0,
                                                *(int *)(sentrybot_instance_data + 0x2DC));
        }
    }
    update_sentrybot_hit_response_and_target_visibility(moby, flag);
    if (flag != 0 && moby[0x20] != 0x15 && moby[0x20] != 0x17) {
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags = (MOBY(moby)->flags | 0x41) & 0xEFFF;
        return;
    }
    MOBY(moby)->flags = (MOBY(moby)->flags & 0xFFBE) | 0x1000;
    MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
    target = *(char **)(sentrybot_instance_data + 0x110);
    manipulator_set_scale(sentrybot_instance_data + 0x1A0, 2.5f);
    if (moby[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L15_001673C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0x16;
    }
    if (hero.state.current == 0x65)
        *(short *)(sentrybot_instance_data + 0x156) = 0;
    fast_dec_timer_s16(sentrybot_instance_data + 0x154);
    fast_dec_timer_s16(sentrybot_instance_data + 0x156);
    fast_dec_timer_s16(sentrybot_instance_data + 0x19C);
    if (*(short *)(sentrybot_instance_data + 0x156) != 0 &&
        *(int *)(sentrybot_instance_data + 0x128) != -1) {
        char *o = (char *)((*(int *)(sentrybot_instance_data + 0x128) << 8) + (int)l15_moby_array);
        if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
            set_sentrybot_alarm_duration(o, 0xB4);
    }
    if (moby[0xBC] == 0)
        sentrybot_instance_data[0x15F] = 0;
    switch (moby[0x20]) {
    case SENTRYBOT_INITIALIZE:
        if (*(int *)(sentrybot_instance_data + 0x120) == -1 ||
            *D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x120)] == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        *(short *)(sentrybot_instance_data + 0x24) = 3;
        *(float *)(sentrybot_instance_data + 0x20) = 3.0f;
        sentrybot_instance_data[0x28] = 2;
        sentrybot_instance_data[0x29] = 0;
        qcopy(sentrybot_instance_data + 0x160, moby + 0x10);
        *(float *)(sentrybot_instance_data + 0x16C) = MOBY(moby)->rot.z;
        if (*(int *)(sentrybot_instance_data + 0x148) < 5)
            *(int *)(sentrybot_instance_data + 0x148) = 5;
        *(int *)(sentrybot_instance_data + 0x144) = 1;
        moby[0x20] = 2;
        MOBY(moby)->flags |= 0x1000;
        {
            int i;
            for (i = 0; i < 4; i++) {
                if (((Sentry98BB8 *)sentrybot_instance_data)->f180[i] != -1) {
                    moby[0x20] = 1;
                    break;
                }
            }
        }
        moby[0xBC] = 0;
        break;
    case SENTRYBOT_WAIT_FOR_LINKED_MOBY: {
        int i;
        for (i = 0; i < 4; i++) {
            if (((unsigned char *)((((Sentry98BB8 *)sentrybot_instance_data)->f180[i] << 8) +
                                   (int)l15_moby_array))[0x20] >= 2) {
                moby[0x20] = 2;
                break;
            }
        }
        break;
    }
    case SENTRYBOT_PATROL_IDLE:
    case SENTRYBOT_PATROL_PAUSE:
        if (*(int *)(sentrybot_instance_data + 0x114) != 2) {
            moby[0xBC] = 1;
            if (sentrybot_instance_data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                sentrybot_instance_data[0x15F] = 1;
                moby[0x20] = 0xB;
                sentrybot_instance_data[0x15C] = 0x10;
                sentrybot_instance_data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (*(short *)(sentrybot_instance_data + 0x156) != 0 &&
                   sentrybot_instance_data[0x15B] == 0) {
            if (sentrybot_instance_data[0x15A] != 0 ||
                *(int *)(sentrybot_instance_data + 0x124) == -1) {
                begin_sentrybot_turn_toward_point((char *)moby,
                                                  sentrybot_instance_data,
                                                  (float *)(target + 0x10),
                                                  (void *)0x10,
                                                  (void *)3);
            } else {
                if (*(int *)(sentrybot_instance_data + 0x144) != 0) {
                    *(int *)(sentrybot_instance_data + 0x13C) =
                        (*(int *)(sentrybot_instance_data + 0x13C) + 1) %
                        *D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)];
                }
                *(int *)(sentrybot_instance_data + 0x144) = 0;
                moby[0xBC] = 1;
                if (sentrybot_instance_data[0x15F] != 0) {
                    moby[0x20] = 0xD;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(10));
                } else {
                    sentrybot_instance_data[0x15F] = 1;
                    moby[0x20] = 0xB;
                    sentrybot_instance_data[0x15C] = 0xD;
                    sentrybot_instance_data[0x15D] = 0xE;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            }
        } else if (*(short *)(sentrybot_instance_data + 0x19C) != 0) {
            begin_sentrybot_turn_toward_point(
                (char *)moby,
                sentrybot_instance_data,
                (float *)(*(char **)(sentrybot_instance_data + 0x198) + 0x10),
                (void *)0xA,
                (void *)0xF);
        } else if (fast_dec_timer_s16(sentrybot_instance_data + 0x196) != 0 &&
                   (*(int *)(sentrybot_instance_data + 0x190) =
                        find_sentrybot_attention_target((char *)moby)) != 0 &&
                   (*(short *)(sentrybot_instance_data + 0x194) != 0 ||
                    random_integer_below(scale_game_frames(0x96)) == 0)) {
            begin_sentrybot_turn_toward_point(
                (char *)moby,
                sentrybot_instance_data,
                (float *)(*(char **)(sentrybot_instance_data + 0x190) + 0x10),
                (void *)0xA,
                (void *)0xF);
        } else if (moby[0x20] == 2) {
            if (*(int *)(sentrybot_instance_data + 0x124) != -1) {
                if (*(int *)(sentrybot_instance_data + 0x144) == 0 ||
                    can_sentrybot_advance_patrol(moby) != 0) {
                    *(int *)(sentrybot_instance_data + 0x14C) =
                        scale_game_frames(*(int *)(sentrybot_instance_data + 0x148));
                    moby[0x20] = 3;
                }
            } else if (*(short *)(sentrybot_instance_data + 0x158) != 0 &&
                       hero.state.current == 0x59) {
                if (*(int *)(sentrybot_instance_data + 0x150) != -1 ||
                    *(int *)(sentrybot_instance_data + 0x12C) != -1 ||
                    (*(short *)(sentrybot_instance_data + 0x154) == 0 &&
                     random_integer_below(0x78) == 0)) {
                    sentrybot_instance_data[0x15D] = 0;
                    sentrybot_instance_data[0x15C] = 2;
                    moby[0x20] = 5;
                    if (moby[0x53] != 2)
                        blend_moby_animation(moby, 2, 0, scale_game_frames(10));
                }
            } else if (*(int *)(sentrybot_instance_data + 0x124) == -1) {
                begin_sentrybot_turn_toward_heading(moby,
                                                    sentrybot_instance_data,
                                                    2,
                                                    0,
                                                    *(float *)(sentrybot_instance_data + 0x16C));
            }
        } else if (moby[0x20] == 3) {
            if (fast_dec_timer(sentrybot_instance_data + 0x14C) != 0) {
                if (*(int *)(sentrybot_instance_data + 0x144) != 0) {
                    *(int *)(sentrybot_instance_data + 0x13C) =
                        (*(int *)(sentrybot_instance_data + 0x13C) + 1) %
                        *D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)];
                }
                *(int *)(sentrybot_instance_data + 0x144) = 0;
                begin_sentrybot_turn_toward_point(
                    (char *)moby,
                    sentrybot_instance_data,
                    (float *)((char *)D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)] +
                              (*(int *)(sentrybot_instance_data + 0x13C) * 0x10 + 0x10)),
                    (void *)4,
                    (void *)0xD);
            }
        }
        break;
    case SENTRYBOT_WAIT_FOR_ANIMATION_THEN_PATROL_IDLE:
        if (*(int *)(sentrybot_instance_data + 0x114) != 2) {
            moby[0xBC] = 1;
            if (sentrybot_instance_data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                sentrybot_instance_data[0x15F] = 1;
                moby[0x20] = 0xB;
                sentrybot_instance_data[0x15C] = 0x10;
                sentrybot_instance_data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (moby[0x70] & 2) {
            moby[0x20] = 2;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            *(short *)(sentrybot_instance_data + 0x196) = scale_game_frames(0x168);
            *(short *)(sentrybot_instance_data + 0x194) = 0;
        }
        break;
    case SENTRYBOT_DEACTIVATE_LINKED_MOBYS_AFTER_ANIMATION:
        if (moby[0x70] & 2) {
            *(short *)(sentrybot_instance_data + 0x154) = scale_game_frames(0xF0);
            if (*(int *)(sentrybot_instance_data + 0x150) != -1) {
                mark_moby_killed_and_release_owned_voice(
                    (int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x150) << 8));
                *(int *)(sentrybot_instance_data + 0x150) = -1;
            }
            if (*(int *)(sentrybot_instance_data + 0x12C) != -1) {
                mark_moby_killed_and_release_owned_voice(
                    (int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x12C) << 8));
                *(int *)(sentrybot_instance_data + 0x12C) = -1;
            }
            moby[0x20] = sentrybot_instance_data[0x15C];
            switch (moby[0x20]) {
            case SENTRYBOT_PATROL_IDLE:
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                break;
            case SENTRYBOT_PATROL_MOVE:
                if (moby[0x53] != 0xD)
                    blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
                break;
            }
        }
        break;
    case SENTRYBOT_TURN_NEGATIVE_HEADING_DELTA:
    case SENTRYBOT_TURN_POSITIVE_HEADING_DELTA:
    case SENTRYBOT_TURN_AROUND:
        if (*(int *)(sentrybot_instance_data + 0x114) != 2 &&
            fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(*(float *)(sentrybot_instance_data + 0xD0) - MOBY(moby)->pos.x,
                             *(float *)(sentrybot_instance_data + 0xD4) - MOBY(moby)->pos.y)) <
                0.5235988f) {
            moby[0xBC] = 1;
            if (sentrybot_instance_data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                sentrybot_instance_data[0x15F] = 1;
                moby[0x20] = 0xB;
                sentrybot_instance_data[0x15C] = 0x10;
                sentrybot_instance_data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else {
            int    anim = moby[0x52];
            float *turn_heading_offsets_degrees = 0;
            switch (anim) {
            case SENTRYBOT_ANIM_TURN_AROUND:
                turn_heading_offsets_degrees = sentrybot_turn_around_heading_offsets_degrees;
                break;
            case SENTRYBOT_ANIM_TURN_NEGATIVE_HEADING_DELTA:
                turn_heading_offsets_degrees = sentrybot_negative_heading_delta_offsets_degrees;
                break;
            case SENTRYBOT_ANIM_TURN_POSITIVE_HEADING_DELTA:
                turn_heading_offsets_degrees = sentrybot_positive_heading_delta_offsets_degrees;
                break;
            }
            if (moby[0x70] & 2) {
                int k = truncate_float_to_s32(animation_end(moby[0x22], anim));
                if (turn_heading_offsets_degrees != 0) {
                    MOBY(moby)->rot.z =
                        fast_add_rotations(*(float *)(sentrybot_instance_data + 0x170),
                                           turn_heading_offsets_degrees[k] * DEG_TO_RAD);
                }
                moby[0x20] = sentrybot_instance_data[0x15C];
                if (moby[0x53] != sentrybot_instance_data[0x15D])
                    blend_moby_animation(
                        moby, sentrybot_instance_data[0x15D], 0, scale_game_frames(10));
            } else if (moby[0x52] == moby[0x53] && turn_heading_offsets_degrees != 0) {
                float t = compute_interpolated_record_value(moby);
                int   k = truncate_float_to_s32(t);
                t -= convert_integer_to_float(k);
                MOBY(moby)->rot.z = fast_add_rotations(
                    *(float *)(sentrybot_instance_data + 0x170),
                    (turn_heading_offsets_degrees[k] +
                     (turn_heading_offsets_degrees[k + 1] - turn_heading_offsets_degrees[k]) * t) *
                        DEG_TO_RAD);
            }
        }
        break;
    case SENTRYBOT_PATROL_MOVE: {
        float sp = frame_time * 2.0f;
        if (move_sentrybot_toward_point(
                (char *)moby,
                (float *)((char *)D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)] +
                          (*(int *)(sentrybot_instance_data + 0x13C) * 0x10 + 0x10)),
                sp)) {
            moby[0x20] = 2;
            *(int *)(sentrybot_instance_data + 0x144) = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (*(int *)(sentrybot_instance_data + 0x114) != 2) {
            moby[0xBC] = 1;
            if (sentrybot_instance_data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                sentrybot_instance_data[0x15F] = 1;
                moby[0x20] = 0xB;
                sentrybot_instance_data[0x15C] = 0x10;
                sentrybot_instance_data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (*(short *)(sentrybot_instance_data + 0x156) != 0 &&
                   sentrybot_instance_data[0x15B] == 0) {
            if (sentrybot_instance_data[0x15A] != 0) {
                begin_sentrybot_turn_toward_point((char *)moby,
                                                  sentrybot_instance_data,
                                                  (float *)(target + 0x10),
                                                  (void *)0x10,
                                                  (void *)3);
            } else {
                *(int *)(sentrybot_instance_data + 0x14C) = scale_game_frames(0x3C);
                moby[0xBC] = 1;
                if (sentrybot_instance_data[0x15F] != 0) {
                    moby[0x20] = 0xD;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(10));
                } else {
                    sentrybot_instance_data[0x15F] = 1;
                    moby[0x20] = 0xB;
                    sentrybot_instance_data[0x15C] = 0xD;
                    sentrybot_instance_data[0x15D] = 0xE;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            }
        } else if (*(short *)(sentrybot_instance_data + 0x19C) != 0) {
            begin_sentrybot_turn_toward_point(
                (char *)moby,
                sentrybot_instance_data,
                (float *)(*(char **)(sentrybot_instance_data + 0x198) + 0x10),
                (void *)0xA,
                (void *)0xF);
        } else if (*(short *)(sentrybot_instance_data + 0x158) != 0 && hero.state.current == 0x59) {
            if (*(int *)(sentrybot_instance_data + 0x150) != -1 ||
                *(int *)(sentrybot_instance_data + 0x12C) != -1 ||
                *(short *)(sentrybot_instance_data + 0x154) == 0) {
                sentrybot_instance_data[0x15C] = moby[0x20];
                sentrybot_instance_data[0x15D] = 0xD;
                moby[0x20] = 5;
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        } else if (fast_dec_timer_s16(sentrybot_instance_data + 0x196) != 0 &&
                   (*(int *)(sentrybot_instance_data + 0x190) =
                        find_sentrybot_attention_target((char *)moby)) != 0 &&
                   (*(short *)(sentrybot_instance_data + 0x194) != 0 ||
                    random_integer_below(scale_game_frames(0x96)) == 0)) {
            begin_sentrybot_turn_toward_point(
                (char *)moby,
                sentrybot_instance_data,
                (float *)(*(char **)(sentrybot_instance_data + 0x190) + 0x10),
                (void *)0xA,
                (void *)0xF);
        }
        break;
    }
    case SENTRYBOT_PATROL_MOVE_FAST: {
        float sp = frame_time * 3.5f;
        if (move_sentrybot_toward_point(
                (char *)moby,
                (float *)((char *)D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)] +
                          (*(int *)(sentrybot_instance_data + 0x13C) * 0x10 + 0x10)),
                sp)) {
            moby[0x20] = 0xC;
            *(int *)(sentrybot_instance_data + 0x144) = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (*(int *)(sentrybot_instance_data + 0x114) != 2 &&
                   sentrybot_instance_data[0x15B] == 0) {
            moby[0xBC] = 1;
            if (sentrybot_instance_data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                sentrybot_instance_data[0x15F] = 1;
                moby[0x20] = 0xB;
                sentrybot_instance_data[0x15C] = 0x10;
                sentrybot_instance_data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (*(short *)(sentrybot_instance_data + 0x156) != 0) {
            *(int *)(sentrybot_instance_data + 0x14C) = scale_game_frames(0x3C);
        } else {
            moby[0x20] = 9;
            if (moby[0x53] != 0xD)
                blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
            if (*(int *)(sentrybot_instance_data + 0x128) != -1) {
                char *o = (char *)((*(int *)(sentrybot_instance_data + 0x128) << 8) +
                                   (int)l15_moby_array);
                if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
                    set_sentrybot_alarm_duration(o, scale_game_frames(0x1E));
            }
        }
        break;
    }
    case SENTRYBOT_RETURN_HOME: {
        unsigned char *home = sentrybot_instance_data + 0x160;
        if (fast_vec_dist(moby + 0x10, home) > 0.2f) {
            move_sentrybot_toward_point((char *)moby, (float *)home, frame_time * 2.0f);
        } else if (*(int *)(sentrybot_instance_data + 0x124) != -1) {
            moby[0xBC] = 0;
            *(int *)(sentrybot_instance_data + 0x144) = 0;
            begin_sentrybot_turn_toward_point(
                (char *)moby,
                sentrybot_instance_data,
                (float *)((char *)D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)] +
                          (*(int *)(sentrybot_instance_data + 0x13C) * 0x10 + 0x10)),
                (void *)2,
                (void *)0);
        } else {
            begin_sentrybot_turn_toward_heading(
                moby, sentrybot_instance_data, 2, 0, *(float *)(sentrybot_instance_data + 0x16C));
        }
        break;
    }
    case SENTRYBOT_RETURN_HOME_FAST: {
        unsigned char *home = sentrybot_instance_data + 0x160;
        if (fast_vec_dist(moby + 0x10, home) > 0.2f) {
            move_sentrybot_toward_point((char *)moby, (float *)home, frame_time * 3.0f);
        } else if (*(short *)(sentrybot_instance_data + 0x156) == 0) {
            moby[0xBC] = 0;
            *(int *)(sentrybot_instance_data + 0x144) = 0;
            if (*(int *)(sentrybot_instance_data + 0x124) != -1) {
                begin_sentrybot_turn_toward_point(
                    (char *)moby,
                    sentrybot_instance_data,
                    (float *)((char *)D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)] +
                              (*(int *)(sentrybot_instance_data + 0x13C) * 0x10 + 0x10)),
                    (void *)2,
                    (void *)0);
            } else {
                begin_sentrybot_turn_toward_heading(moby,
                                                    sentrybot_instance_data,
                                                    2,
                                                    0,
                                                    *(float *)(sentrybot_instance_data + 0x16C));
            }
        } else if (fast_difference_between_rotations(
                       MOBY(moby)->rot.z,
                       fast_arc_tan(*(float *)(sentrybot_instance_data + 0xD0) - MOBY(moby)->pos.x,
                                    *(float *)(sentrybot_instance_data + 0xD4) -
                                        MOBY(moby)->pos.y)) > 0.7853982f &&
                   sentrybot_instance_data[0x15B] == 0) {
            begin_sentrybot_turn_toward_point((char *)moby,
                                              sentrybot_instance_data,
                                              (float *)(sentrybot_instance_data + 0xD0),
                                              (void *)0x10,
                                              (void *)3);
        } else {
            moby[0x20] = 0xC;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    case SENTRYBOT_WAIT_FOR_TRANSITION_ANIMATION:
        if (*(int *)(sentrybot_instance_data + 0x128) != -1) {
            prepare_sentrybot_alarm(
                (unsigned char *)((int)l15_moby_array +
                                  (*(int *)(sentrybot_instance_data + 0x128) << 8)));
        }
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                alert_visible_group_sentrybots(moby);
                if (*(int *)(sentrybot_instance_data + 0x128) != -1 &&
                    *(int *)(*(char **)((char *)((*(int *)(sentrybot_instance_data + 0x128) << 8) +
                                                 (int)l15_moby_array) +
                                        0x78) +
                             8) == 0 &&
                    can_sentrybot_approach_linked_moby(moby) != 0) {
                    moby[0x20] = 0xE;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(0x14));
                } else if (sentrybot_instance_data[0x15C] == 0x10) {
                    begin_sentrybot_turn_toward_point((char *)moby,
                                                      sentrybot_instance_data,
                                                      (float *)(target + 0x10),
                                                      (void *)0x10,
                                                      (void *)3);
                } else {
                    if (sentrybot_instance_data[0x15C] != 0xFF)
                        moby[0x20] = sentrybot_instance_data[0x15C];
                    if (sentrybot_instance_data[0x15D] != 0xFF &&
                        moby[0x53] != sentrybot_instance_data[0x15D])
                        blend_moby_animation(
                            moby, sentrybot_instance_data[0x15D], 0, scale_game_frames(10));
                }
            }
        }
        break;
    case SENTRYBOT_APPROACH_LINKED_MOBY:
        prepare_sentrybot_alarm(
            (unsigned char *)((int)l15_moby_array +
                              (*(int *)(sentrybot_instance_data + 0x128) << 8)));
        fast_vec_sub(
            va,
            (char *)((int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x128) << 8)) + 0x10,
            moby + 0x10);
        fast_vec_normalize(va, va, fast_vec_length(va) - 1.5f);
        fast_vec_add(va, va, moby + 0x10);
        if (move_sentrybot_toward_point((char *)moby, va, frame_time * 4.0f)) {
            moby[0x20] = 0xF;
            if (moby[0x53] != 9)
                blend_moby_animation(moby, 9, 0, scale_game_frames(10));
        }
        break;
    case SENTRYBOT_ACTIVATE_LINKED_MOBY:
        if (moby[0x52] == moby[0x53] && compute_interpolated_record_value(moby) >= 12.0f) {
            set_sentrybot_alarm_duration(
                (char *)((int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x128) << 8)),
                0xB4);
        }
        if (moby[0x70] & 2) {
            set_sentrybot_alarm_duration(
                (char *)((int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x128) << 8)),
                0xB4);
            begin_sentrybot_turn_toward_point((char *)moby,
                                              sentrybot_instance_data,
                                              (float *)(target + 0x10),
                                              (void *)0x14,
                                              (void *)0xE);
        }
        break;
    case SENTRYBOT_WAIT_FOR_ANIMATION_THEN_EMIT_WITH_POSITIVE_JOINT_AIM:
        *(short *)(sentrybot_instance_data + 0x19C) = 0;
        *(short *)(sentrybot_instance_data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x13;
                sentrybot_instance_data[0x19E] = 0;
                if (moby[0x53] != 6)
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case SENTRYBOT_WAIT_FOR_ANIMATION_THEN_EMIT_WITH_NEGATIVE_JOINT_AIM:
        *(short *)(sentrybot_instance_data + 0x19C) = 0;
        *(short *)(sentrybot_instance_data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x11;
                sentrybot_instance_data[0x19E] = 0;
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
        }
        break;
    case SENTRYBOT_EMIT_WITH_POSITIVE_JOINT_AIM:
        *(short *)(sentrybot_instance_data + 0x19C) = 0;
        *(short *)(sentrybot_instance_data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (sentrybot_instance_data[0x19E] == 0) {
            float t = compute_interpolated_record_value(moby);
            if (t >= 0.85f && t < 3.0f) {
                j2 = 2;
                compute_selected_world_matrices(moby, 1, &j2, selected_joint_world_matrix);
                transform_scaled_vertex_batch((char *)moby, 1, &j2, (char *)emission_origin);
                fast_vec_normalize(va,
                                   selected_joint_world_matrix[1],
                                   sentrybot_emission_vector_speed_gp * frame_time);
                fast_vec_sub(vb, sentrybot_instance_data + 0x100, emission_origin);
                fast_vec_normalize(vb, vb, sentrybot_emission_vector_speed_gp * frame_time);
                if (fast_difference_between_rotations(fast_arc_tan(va[0], va[1]),
                                                      fast_arc_tan(vb[0], vb[1])) > 0.7853982f ||
                    fast_difference_between_rotations(fast_arc_tan(vector_length_xy(va), va[2]),
                                                      fast_arc_tan(vector_length_xy(vb), vb[2])) >
                        0.7853982f) {
                    spawn_sentrybot_projectile((char *)moby,
                                               va,
                                               (char *)emission_origin,
                                               (char *)(sentrybot_instance_data + 0x100));
                } else {
                    spawn_sentrybot_projectile((char *)moby,
                                               vb,
                                               (char *)emission_origin,
                                               (char *)(sentrybot_instance_data + 0x100));
                }
                sentrybot_instance_data[0x19E] = 1;
            }
        }
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else if (*(int *)(sentrybot_instance_data + 0x128) != -1 &&
                       *(int *)(*(char **)((char *)((*(int *)(sentrybot_instance_data + 0x128)
                                                     << 8) +
                                                    (int)l15_moby_array) +
                                           0x78) +
                                8) == 0 &&
                       can_sentrybot_approach_linked_moby(moby) != 0) {
                moby[0x20] = 0xE;
                if (moby[0x53] != 0xE)
                    blend_moby_animation(moby, 0xE, 0, scale_game_frames(0x14));
            } else if (fast_difference_between_rotations(
                           MOBY(moby)->rot.z,
                           fast_arc_tan(
                               *(float *)(sentrybot_instance_data + 0xD0) - MOBY(moby)->pos.x,
                               *(float *)(sentrybot_instance_data + 0xD4) - MOBY(moby)->pos.y)) >
                       0.7853982f) {
                begin_sentrybot_turn_toward_point((char *)moby,
                                                  sentrybot_instance_data,
                                                  (float *)(sentrybot_instance_data + 0xD0),
                                                  (void *)0x10,
                                                  (void *)3);
            } else if (*(int *)(sentrybot_instance_data + 0x114) == 2) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
            sentrybot_instance_data[0x19E] = 0;
        }
        break;
    case SENTRYBOT_EMIT_WITH_NEGATIVE_JOINT_AIM:
        *(short *)(sentrybot_instance_data + 0x19C) = 0;
        *(short *)(sentrybot_instance_data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (sentrybot_instance_data[0x19E] == 0) {
            float t = compute_interpolated_record_value(moby);
            if (t >= 0.85f && t < 3.0f) {
                j3 = 3;
                compute_selected_world_matrices(moby, 1, &j3, selected_joint_world_matrix);
                transform_scaled_vertex_batch((char *)moby, 1, &j3, (char *)emission_origin);
                fast_vec_normalize(va,
                                   selected_joint_world_matrix[1],
                                   -(sentrybot_emission_vector_speed_gp * frame_time));
                fast_vec_sub(vb, sentrybot_instance_data + 0x100, emission_origin);
                fast_vec_normalize(vb, vb, sentrybot_emission_vector_speed_gp * frame_time);
                if (fast_difference_between_rotations(fast_arc_tan(va[0], va[1]),
                                                      fast_arc_tan(vb[0], vb[1])) > 0.7853982f ||
                    fast_difference_between_rotations(fast_arc_tan(vector_length_xy(va), va[2]),
                                                      fast_arc_tan(vector_length_xy(vb), vb[2])) >
                        0.7853982f) {
                    spawn_sentrybot_projectile((char *)moby,
                                               va,
                                               (char *)emission_origin,
                                               (char *)(sentrybot_instance_data + 0x100));
                } else {
                    spawn_sentrybot_projectile((char *)moby,
                                               vb,
                                               (char *)emission_origin,
                                               (char *)(sentrybot_instance_data + 0x100));
                }
                sentrybot_instance_data[0x19E] = 1;
            }
        }
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else if (*(int *)(sentrybot_instance_data + 0x128) != -1 &&
                       *(int *)(*(char **)((char *)((*(int *)(sentrybot_instance_data + 0x128)
                                                     << 8) +
                                                    (int)l15_moby_array) +
                                           0x78) +
                                8) == 0 &&
                       can_sentrybot_approach_linked_moby(moby) != 0) {
                moby[0x20] = 0xE;
                if (moby[0x53] != 0xE)
                    blend_moby_animation(moby, 0xE, 0, scale_game_frames(0x14));
            } else if (fast_difference_between_rotations(
                           MOBY(moby)->rot.z,
                           fast_arc_tan(
                               *(float *)(sentrybot_instance_data + 0xD0) - MOBY(moby)->pos.x,
                               *(float *)(sentrybot_instance_data + 0xD4) - MOBY(moby)->pos.y)) >
                       0.7853982f) {
                begin_sentrybot_turn_toward_point((char *)moby,
                                                  sentrybot_instance_data,
                                                  (float *)(sentrybot_instance_data + 0xD0),
                                                  (void *)0x12,
                                                  (void *)5);
            } else if (*(int *)(sentrybot_instance_data + 0x114) == 2) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x12;
                if (moby[0x53] != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
            sentrybot_instance_data[0x19E] = 0;
        }
        break;
    case SENTRYBOT_ALERT_IDLE:
        if (hero.state.current == 0x65) {
            moby[0x20] = 9;
            if (moby[0x53] != 0xD)
                blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
            if (*(int *)(sentrybot_instance_data + 0x128) != -1) {
                char *o = (char *)((*(int *)(sentrybot_instance_data + 0x128) << 8) +
                                   (int)l15_moby_array);
                if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
                    set_sentrybot_alarm_duration(o, -1);
            }
        } else {
            int mode = *(int *)(sentrybot_instance_data + 0x114);
            if (mode != 2 && sentrybot_instance_data[0x15B] == 0) {
                begin_sentrybot_turn_toward_point((char *)moby,
                                                  sentrybot_instance_data,
                                                  (float *)(sentrybot_instance_data + 0xD0),
                                                  (void *)0x10,
                                                  (void *)3);
            } else if (*(short *)(sentrybot_instance_data + 0x156) == 0) {
                moby[0x20] = 9;
                if (moby[0x53] != 0xD)
                    blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
                if (*(int *)(sentrybot_instance_data + 0x128) != -1) {
                    char *o = (char *)((*(int *)(sentrybot_instance_data + 0x128) << 8) +
                                       (int)l15_moby_array);
                    if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
                        set_sentrybot_alarm_duration(o, scale_game_frames(0x1E));
                }
            } else if (sentrybot_instance_data[0x15A] != 0) {
                if (mode != 2 && sentrybot_instance_data[0x15B] == 0) {
                    begin_sentrybot_turn_toward_point((char *)moby,
                                                      sentrybot_instance_data,
                                                      (float *)(target + 0x10),
                                                      (void *)0x10,
                                                      (void *)3);
                } else {
                    begin_sentrybot_turn_toward_point((char *)moby,
                                                      sentrybot_instance_data,
                                                      (float *)(target + 0x10),
                                                      (void *)0xC,
                                                      (void *)0);
                }
            } else if (*(int *)(sentrybot_instance_data + 0x124) != -1) {
                if (*(int *)(sentrybot_instance_data + 0x144) != 0) {
                    *(int *)(sentrybot_instance_data + 0x13C) =
                        (*(int *)(sentrybot_instance_data + 0x13C) + 1) %
                        *D_L15_001B0AB0[*(int *)(sentrybot_instance_data + 0x124)];
                }
                *(int *)(sentrybot_instance_data + 0x144) = 0;
                *(int *)(sentrybot_instance_data + 0x14C) = scale_game_frames(0x3C);
                moby[0xBC] = 1;
                if (sentrybot_instance_data[0x15F] != 0) {
                    moby[0x20] = 0xD;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(10));
                } else {
                    sentrybot_instance_data[0x15F] = 1;
                    moby[0x20] = 0xB;
                    sentrybot_instance_data[0x15C] = 0xD;
                    sentrybot_instance_data[0x15D] = 0xE;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case SENTRYBOT_RECOVERABLE_KNOCKBACK: {
        int knockback_fall_flags = step_moby_motion(moby, sentrybot_instance_data + 0x70);
        if (knockback_fall_flags & 0x40) {
            probe_ground_height(moby + 0x10, 0, 0.5f);
            if (collision_hit_material() == -1 &&
                *(float *)(sentrybot_instance_data + 0x168) - 5.0f < MOBY(moby)->pos.z) {
                if (hero.state.current == 0x65) {
                    moby[0x20] = 0xC;
                    if (moby[0x53] != 0)
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                } else {
                    moby[0xBC] = 1;
                    if (sentrybot_instance_data[0x15F] != 0) {
                        moby[0x20] = 0x10;
                        if (moby[0x53] != 3)
                            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                    } else {
                        sentrybot_instance_data[0x15F] = 1;
                        moby[0x20] = 0xB;
                        sentrybot_instance_data[0x15C] = 0x10;
                        sentrybot_instance_data[0x15D] = 3;
                        if (moby[0x53] != 1)
                            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    }
                }
                return;
            }
            if (*(int *)(sentrybot_instance_data + 0x12C) != -1) {
                mark_moby_killed_and_release_owned_voice(
                    (int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x12C) << 8));
                *(int *)(sentrybot_instance_data + 0x12C) = -1;
            }
            if (*(int *)(sentrybot_instance_data + 0x150) != -1) {
                mark_moby_killed_and_release_owned_voice(
                    (int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x150) << 8));
                *(int *)(sentrybot_instance_data + 0x150) = -1;
            }
            allocate_voice_for_target_entry(0xD, 0, (int)moby);
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        } else if (knockback_fall_flags & 0x120) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case SENTRYBOT_DEATH_KNOCKBACK:
        if (step_moby_motion(moby, sentrybot_instance_data + 0x70) & 0x160) {
            if (*(int *)(sentrybot_instance_data + 0x12C) != -1) {
                mark_moby_killed_and_release_owned_voice(
                    (int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x12C) << 8));
                *(int *)(sentrybot_instance_data + 0x12C) = -1;
            }
            if (*(int *)(sentrybot_instance_data + 0x150) != -1) {
                mark_moby_killed_and_release_owned_voice(
                    (int)l15_moby_array + (*(int *)(sentrybot_instance_data + 0x150) << 8));
                *(int *)(sentrybot_instance_data + 0x150) = -1;
            }
            allocate_voice_for_target_entry(0xD, 0, (int)moby);
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    update_sentrybot_joint_aim(moby);
    if (*(short *)(sentrybot_instance_data + 0x156) != 0) {
        lvl = 2;
    } else if (*(int *)(sentrybot_instance_data + 0x114) != 2) {
        lvl = 2;
    } else {
        lvl = *(short *)(sentrybot_instance_data + 0x158) != 0;
    }
    tick = D_L15_0015F5CC;
    if (tick != sentrybot_alert_level_frame_stamp_gp || hero.glow_pulse_level < lvl) {
        sentrybot_alert_level_frame_stamp_gp = tick;
        hero.glow_pulse_level = lvl;
    }
    alpha = 0x80;
    color_pulse_period_frames = scale_game_frames(0xAA);
    if (lvl == 2) {
        color_pulse_period_frames = scale_game_frames(0x32);
    } else if (lvl == 1) {
        color_pulse_period_frames = scale_game_frames(0x5A);
    }
    color_pulse_sine =
        fast_sin((float)(D_L15_0015F5CC % color_pulse_period_frames) /
                     convert_integer_to_float(color_pulse_period_frames) * 2.0f * 3.1415927f +
                 -3.1415927f);
    {
        int color_pulse_red_offset = truncate_float_to_s32(color_pulse_sine * 20.0f);
        int color_pulse_green_offset = truncate_float_to_s32(color_pulse_sine * 70.0f);
        int color_pulse_blue_offset = truncate_float_to_s32(color_pulse_sine * 10.0f);
        int cr = color_pulse_red_offset + 0x32;
        int cb = color_pulse_blue_offset + 0x14;
        int cg = color_pulse_green_offset + 0xB4;
        if (cr > 0xFF)
            cr = 0xFF;
        if (cg > 0xFF)
            cg = 0xFF;
        if (lvl == 2) {
            color = (alpha << 24) | (cb << 16) | ((cr / 2) << 8) | cg;
        } else {
            color = (alpha << 24) | (cb << 16) | (cg << 8) | cr;
        }
    }
    MOBY(moby)->glow_color = fast_tween_color(MOBY(moby)->glow_color, color, 0.1f);
    {
        unsigned char *effect_position = sentrybot_instance_data + 0x2A0;
        int            i;
        for (i = 0; i < 3; i++) {
            moby_point_position(moby, i + 4, effect_position);
            effect_position += 0x10;
        }
    }
    add_draw_callback((void (*)(void))draw_sentrybot_effect_quads, moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AA60), where it is exact; names translated to the US level program. */

int begin_sentrybot_turn_toward_heading(

    unsigned char *moby, char *moby_data, int next_state, int next_animation, float angle) {
    float heading_delta = -fast_subtract_rotations(MOBY(moby)->rot.z, angle);
    float stored_heading_delta;
    int   turn_required = 1;
    *(int *)(moby_data + 0x144) = 0;
    *(float *)(moby_data + 0x170) = MOBY(moby)->rot.z;
    *(float *)(moby_data + 0x174) = angle;
    *(float *)(moby_data + 0x178) = heading_delta;
    moby_data[0x15C] = next_state;
    moby_data[0x15D] = next_animation;
    if (heading_delta > 2.0943952f || heading_delta < -2.0943952f) {
        moby[0x20] = 8;
        stored_heading_delta = *(float *)(moby_data + 0x178);
        if (stored_heading_delta > 0.0f) {
            *(float *)(moby_data + 0x178) = stored_heading_delta + -6.2831855f;
        }
        if (moby[0x53] != 0xC) {
            blend_moby_animation(moby, 0xC, 0, scale_game_frames(10));
        }

    } else if (heading_delta < -0.7853982f) {
        moby[0x20] = 6;
        if (moby[0x53] != 0xA) {
            blend_moby_animation(moby, 0xA, 0, scale_game_frames(10));
        }

    } else if (heading_delta > 0.7853982f) {
        moby[0x20] = 7;
        if (moby[0x53] != 0xB) {
            blend_moby_animation(moby, 0xB, 0, scale_game_frames(10));
        }

    } else {
        moby[0x20] = next_state;
        if (moby[0x53] != next_animation) {
            blend_moby_animation(moby, next_animation, 0, scale_game_frames(10));
        }
        turn_required = 0;
    }
    return turn_required;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AC48), where it is exact; names translated to the US level program. */

/* Aims at point (heading from the moby) and passes it on to FUN_L15_00299880. */

void begin_sentrybot_turn_toward_point(

    char *moby, void *moby_data, float *point, void *next_state, void *next_animation) {
    float angle = fast_arc_tan(point[0] - MOBY(moby)->pos.x, point[1] - MOBY(moby)->pos.y);
    FUN_L15_00299880_u(moby, moby_data, next_state, next_animation, angle);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029ACC0), where it is exact; names translated to the US level program. */

int move_sentrybot_toward_point(char *moby, float *point, float target_forward_step) {
    float *fp = (float *)&MOBY(moby)->rot.z;
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    float  movement_delta[4];
    float  target_heading;
    float  movement_target_heading;
    target_heading = fast_arc_tan(point[0] - MOBY(moby)->pos.x, point[1] - MOBY(moby)->pos.y);
    approach_angle(fp,
                   (float *)(moby_data + 0x138),
                   target_heading,
                   frame_time_sq * 12.566371f,
                   frame_time_sq * 12.566371f,
                   frame_time * 6.981317f);
    movement_target_heading =
        fast_arc_tan(point[0] - MOBY(moby)->pos.x, point[1] - MOBY(moby)->pos.y);
    if (fast_difference_between_rotations(MOBY(moby)->rot.z, movement_target_heading) <
        0.7853982f) {
        approach_value(moby_data + 0x134, target_forward_step, frame_time_sq * 9.0f);
    } else {
        approach_value(moby_data + 0x134, 0.0f, frame_time_sq * 9.0f);
    }
    movement_delta[0] = fast_cos(MOBY(moby)->rot.z) * *(float *)(moby_data + 0x134);
    movement_delta[1] = fast_sin(MOBY(moby)->rot.z) * *(float *)(moby_data + 0x134);
    movement_delta[2] = -(frame_time * 0.5f);
    FUN_L00_00258830((int)moby, (int)movement_delta, 0.5f, 0.5f, 0.0f, 0);
    return vector_distance_xy((float *)&MOBY(moby)->pos, point) < 0.2f;
}

/* checks that no closer live object blocks the line to the target */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AE80), where it is exact; names translated to the US level program. */

int can_sentrybot_approach_linked_moby(void *mm) {
    char  *moby = mm;
    int    linked_moby_index = *(int *)(((char *)MOBY(moby)->pvars) + 0x128);
    char  *linked_moby;
    float  linked_moby_distance_xy;
    short *group_moby_entry;
    if (linked_moby_index == -1)
        return 0;
    linked_moby = (char *)l15_moby_array + (linked_moby_index << 8);
    if (*(int *)(((char *)MOBY(linked_moby)->pvars) + 8) != 0)
        return 0;
    if (((unsigned char *)moby)[0x21] == 0xFF)
        return 1;
    linked_moby_distance_xy =
        vector_distance_xy((float *)&MOBY(moby)->pos, (float *)&MOBY(linked_moby)->pos);
    group_moby_entry = (short *)D_L15_001ABE40[((unsigned char *)moby)[0x21]];
    if (group_moby_entry == 0)
        return 1;
    do {
        char *listed_moby =
            (char *)(((*(unsigned short *)group_moby_entry & 0x7FFF) << 8) + (int)l15_moby_array);
        if (((unsigned char *)listed_moby)[0x20] == 0xE)
            return 0;
        if (((unsigned char *)listed_moby)[0xBC] == 1) {
            int j = *(int *)(((char *)MOBY(listed_moby)->pvars) + 0x128);
            if (j != -1) {
                if (vector_distance_xy((float *)&MOBY(listed_moby)->pos,
                                       (float *)((char *)l15_moby_array + (j << 8) + 0x10)) <
                    linked_moby_distance_xy)
                    return 0;
            }
        }
    } while (*group_moby_entry++ >= 0);
    return 1;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AFB8), where it is exact; names translated to the US level program. */

int find_sentrybot_attention_target(char *moby) {
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    int    best = 0;
    float  bd;
    float  line_start[4];
    float  candidate_line_endpoint[4];
    short *group_moby_entry;
    int    moby_group_index = *(int *)(moby_data + 0x17C);
    if (moby_group_index == -1)
        return 0;
    group_moby_entry = (short *)D_L15_001ABE40[moby_group_index];
    if (group_moby_entry == 0)
        return 0;
    qcopy(line_start, moby + 0x10);
    line_start[2] += 1.0f;
    bd = 0.0f;
    do {
        int   id = *(unsigned short *)group_moby_entry & 0x7FFF;
        char *candidate_moby = (char *)((id << 8) + (int)l15_moby_array);
        if (MOBY(candidate_moby)->oclass == 0x27B && MOBY(candidate_moby)->state >= 6 &&
            MOBY(candidate_moby)->state < 0x10) {
            float f = fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(MOBY(candidate_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(candidate_moby)->pos.y - MOBY(moby)->pos.y));
            unsigned char candidate_state = ((char *)((id << 8) + (int)l15_moby_array))[0x20];
            if (candidate_state == 7 || candidate_state == 0xF)
                *(short *)(moby_data + 0x194) = 1;
            if (*(short *)(moby_data + 0x194) != 0 || f < 1.2217305f) {
                qcopy(candidate_line_endpoint, (char *)((id << 8) + (int)l15_moby_array) + 0x10);
                line_start[2] += 1.0f;
                if (collision_line(line_start, candidate_line_endpoint, 2, 0, 0) == 0) {
                    float candidate_line_distance =
                        fast_vec_dist(line_start, candidate_line_endpoint);
                    if (best == 0 || candidate_line_distance < bd) {
                        bd = candidate_line_distance;
                        best = (int)l15_moby_array + (id << 8);
                    }
                }
            }
        }
    } while (*group_moby_entry++ >= 0);
    if (*(short *)(moby_data + 0x194) == 0) {
        if (bd > 8.0f)
            best = 0;
    }
    return best;
}

/* Returns 0 if a live listed moby of class 0x2C has a lower rank than this one, else 1. */

int can_sentrybot_advance_patrol(void *input_moby) {
    struct Moby    *moby = input_moby;
    unsigned short *group_moby_entry;
    char           *moby_data;
    if (moby->group == 0xFF)
        return 1;
    group_moby_entry = (unsigned short *)D_L15_001ABE40[moby->group];
    moby_data = ((char *)moby->pvars);
    if (group_moby_entry == 0)
        return 1;
    do {
        unsigned char *listed_moby =
            (unsigned char *)(((*group_moby_entry & 0x7FFF) << 8) + (int)l15_moby_array);
        if (MOBY(listed_moby)->oclass == 0x2C && listed_moby != 0 && listed_moby[0x20] != 0xFE &&
            listed_moby[0x20] != 0xFD && listed_moby[0xBC] == 0) {
            char *od = ((char *)MOBY(listed_moby)->pvars);
            if (*(int *)(od + 0x144) == 0 && *(int *)(od + 0x124) != -1 &&
                *(int *)(od + 0x13C) >= *(int *)(moby_data + 0x13C))
                return 0;
        }
    } while ((short)*group_moby_entry++ >= 0);
    return 1;
}

/* Wakes every linked switch that can see this moby, or rearms the timer when there is no list. */

void alert_visible_group_sentrybots(void *mp) {
    unsigned char *source_moby = mp;
    char          *moby_data = ((char *)MOBY(source_moby)->pvars);
    float          source_line_endpoint[4];
    float          listed_line_endpoint[4];
    short         *group_moby_entry;
    int            listed_moby_byte_offset;
    char          *listed_moby;
    if (camera_secondary_mode[0] == 0x65) {
        *(short *)(moby_data + 0x156) = 0;
        return;
    }
    if (source_moby[0x21] == 0xFF) {
        *(short *)(moby_data + 0x156) = scale_game_frames(0xB4);
        return;
    }
    group_moby_entry = D_L15_001ABE40_a0[source_moby[0x21]];
    if (group_moby_entry == 0) {
        return;
    }
    qcopy(source_line_endpoint, source_moby + 0x10);
    source_line_endpoint[2] += 1.0f;
    do {
        listed_moby_byte_offset = (*(unsigned short *)group_moby_entry & 0x7FFF) << 8;
        listed_moby = (char *)(listed_moby_byte_offset + (int)l15_moby_array);
        if (listed_moby[0x20] >= 0 && MOBY(listed_moby)->oclass == 0x2C) {
            qcopy(listed_line_endpoint, listed_moby + 0x10);
            listed_line_endpoint[2] += 1.0f;
            if (FUN_001efa68_a0(listed_line_endpoint, source_line_endpoint, 2, 0, 0) == 0) {
                moby_data =
                    *(char **)((char *)(listed_moby_byte_offset + (int)l15_moby_array) + 0x78);
                ((D_a0d0 *)moby_data)->h156 = scale_game_frames(300);
                moby_data[0x15B] = 0;
                set_sentrybot_attention_target(
                    (char *)source_moby, (char *)((int)l15_moby_array + listed_moby_byte_offset));
            }
        }
    } while (*group_moby_entry++ >= 0);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029B428), where it is exact; names translated to the US level program. */

void set_sentrybot_attention_target(char *source_moby, struct Moby *recipient_moby) {
    char *recipient_moby_data;
    if (source_moby != recipient_moby) {
        recipient_moby_data = ((char *)recipient_moby->pvars);
        if (*(short *)(recipient_moby_data + 0x19C) == 0 || random_integer_below(0x10) == 0) {
            *(char **)(recipient_moby_data + 0x198) = source_moby;
            *(short *)(recipient_moby_data + 0x19C) = scale_game_frames(0xB4);
        }
    }
}

/* Turns the turret head toward its current target (or remembered point), clamping yaw and pitch, and eases both joints. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029B488), where it is exact; names translated to the US level program. */

void update_sentrybot_joint_aim(void *mv) {
    char *moby = mv;
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float target_position[4];
    float aim_origin[4];
    float target_displacement[4];
    float k1 = 0.02f;
    float k2 = 0.3f;
    float yaw, pitch;
    int   have = 0;
    if (*(short *)(moby_data + 0x194) != 0 && *(char **)(moby_data + 0x190) != 0) {
        qcopy(target_position, *(char **)(moby_data + 0x190) + 0x10);
        have = 1;
    } else if (*(short *)(moby_data + 0x19C) != 0 && *(char **)(moby_data + 0x198) != 0) {
        qcopy(target_position, *(char **)(moby_data + 0x198) + 0x10);
        have = 1;
    } else if (((unsigned char *)moby_data)[0x19F] != 0) {
        qcopy(target_position, moby_data + 0x100);
        have = 1;
    }
    if (have) {
        qcopy(aim_origin, moby + 0x10);
        aim_origin[2] += 1.0f;
        fast_vec_sub(target_displacement, target_position, aim_origin);
        yaw = fast_subtract_rotations(fast_arc_tan(target_displacement[0], target_displacement[1]),
                                      MOBY(moby)->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(target_displacement), target_displacement[2]);
        if ((unsigned int)(((unsigned char *)moby)[0x20] - 0x10) < 2) {
            if (1.5707964f < yaw)
                yaw = 1.5707964f;
            else if (yaw < -1.5707964f)
                yaw = -1.5707964f;
        } else {
            if (1.0471976f < yaw)
                yaw = 1.0471976f;
            else if (yaw < -1.0471976f)
                yaw = -1.0471976f;
        }
        if (0.5235988f < pitch)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
    } else {
        pitch = 0.0f;
        yaw = pitch;
    }
    if ((unsigned int)(((unsigned char *)moby)[0x20] - 0x10) < 2) {
        *(float *)(moby_data + 0x288) = yaw;
        if (1.0471976f < yaw)
            *(float *)(moby_data + 0x288) = 1.0471976f;
        else if (yaw < -1.0471976f)
            *(float *)(moby_data + 0x288) = -1.0471976f;
        *(float *)(moby_data + 0x208) = yaw - *(float *)(moby_data + 0x288);
    } else {
        *(float *)(moby_data + 0x208) = yaw * 0.7f;
        *(float *)(moby_data + 0x288) = yaw * 0.3f;
    }
    {
        float cached_frame_scale_sq = frame_scale_sq;
        *(float *)(moby_data + 0x204) = pitch;
        update_joint_animation(
            moby, moby_data + 0x1A0, 1, k1 * cached_frame_scale_sq, k2 * cached_frame_scale_sq);
    }
    update_joint_animation(moby, moby_data + 0x220, 0, k1 * frame_scale_sq, k2 * frame_scale_sq);
}

void update_sentrybot_hit_response_and_target_visibility(void *sentrybot_moby, int flag) {
    enum {
        SENTRYBOT_NO_KNOCKBACK_SETUP_RESPONSE = 0,
        LETHAL_HIT_RESPONSE = 1,
        DEATH_KNOCKBACK_HIT_RESPONSE = 2,
        NONLETHAL_KNOCKBACK_HIT_RESPONSE = 3,
        NONLETHAL_ARC_HIT_RESPONSE = 4,
        NONLETHAL_ARC_KNOCKBACK_HIT_RESPONSE = 5,
        NONLETHAL_FALL_HIT_RESPONSE = 6,
        SENTRYBOT_RECOVERABLE_KNOCKBACK_RESPONSE = 7,
        SENTRYBOT_RECOVERABLE_ARC_KNOCKBACK_RESPONSE = 8,
        SENTRYBOT_FIRST_CHANNEL_BLEND_HIT_RESPONSE = 9,
        SENTRYBOT_FIRST_CHANNEL_BLEND_TARGET_HIT_RESPONSE = 10,
        SENTRYBOT_NO_KNOCKBACK_START_RESPONSE = 11
    };
    unsigned char *moby = sentrybot_moby;
    unsigned char *sentrybot_data = ((unsigned char *)MOBY(moby)->pvars);
    char          *hit;
    float          impact;
    int            kind;
    int            code;
    float          angle;
    float          fall_angle;
    float          speed;
    int            amount;
    float          hit_vector_or_sight_line_origin[4];
    int           *path;
    int            group;
    unsigned char  seen;

    if (moby[0x20] == 0)
        return;
    MOBY(moby)->scale =
        *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * sentrybot_class_scale_multiplier_gp;
    impact = 0.0f;
    hit = FUN_L00_0025a420(moby, 0x330000, 0);
    code = resolve_moby_hit_response(moby, hit, sentrybot_data + 0x20, 0, &kind, &impact, 0, 4);
    if (kind != 1 && moby[0x20] != 0x17) {
        if ((unsigned char)D_001413F4[0] == 0)
            alert_visible_group_sentrybots(moby);
        *(float *)(sentrybot_data + 0x20) -= impact;
        if (*(short *)(*(char **)(hit + 0x20) + 0xA6) == 0x47)
            code = 3;
        if (*(float *)(sentrybot_data + 0x20) <= 0.0f)
            code = 1;
        amount = truncate_float_to_s32(512.0f);
        speed = sentrybot_knockback_gravity_gp * frame_time_sq;
        *(int *)(sentrybot_data + 0x90) = amount;
        *(float *)(sentrybot_data + 0x98) = 0.5f;
        *(int *)(sentrybot_data + 0x94) = 9;
        *(float *)(sentrybot_data + 0x80) = speed;
        sentrybot_data[0xAD] = 0;
        switch (code) {
        case SENTRYBOT_NO_KNOCKBACK_SETUP_RESPONSE:
        case SENTRYBOT_NO_KNOCKBACK_START_RESPONSE:
            break;
        case SENTRYBOT_FIRST_CHANNEL_BLEND_HIT_RESPONSE:
        case SENTRYBOT_FIRST_CHANNEL_BLEND_TARGET_HIT_RESPONSE:
            sentrybot_data[0x67] = 0xFA;
            break;
        case NONLETHAL_KNOCKBACK_HIT_RESPONSE:
        case NONLETHAL_ARC_HIT_RESPONSE:
        case NONLETHAL_ARC_KNOCKBACK_HIT_RESPONSE:
        case NONLETHAL_FALL_HIT_RESPONSE:
        case SENTRYBOT_RECOVERABLE_KNOCKBACK_RESPONSE:
        case SENTRYBOT_RECOVERABLE_ARC_KNOCKBACK_RESPONSE: {
            float step;
            float step_sq;
            float forward;
            float lateral;
            float vz;
            float fall_speed;
            step = frame_time;
            fall_speed = sentrybot_knockback_gravity_gp;
            step_sq = frame_time_sq;
            fall_speed = fall_speed * step_sq;
            forward = sentrybot_recoverable_knockback_horizontal_speed_gp * step;
            lateral = sentrybot_recoverable_knockback_vertical_speed_gp * step;
            vz = sentrybot_recoverable_knockback_horizontal_speed_approach_rate_gp * step;
            *(float *)(sentrybot_data + 0x80) = fall_speed;
            *(float *)(sentrybot_data + 0x88) = forward;
            *(float *)(sentrybot_data + 0x8C) = lateral;
            *(float *)(sentrybot_data + 0x84) = vz;
            *(float *)(sentrybot_data + 0xC0) = 5.0f;
            *(float *)(sentrybot_data + 0xC4) = 10.0f;
        }
            {
                float heading =
                    fast_arc_tan(MOBY(moby)->pos.x - *(float *)(*(char **)(hit + 0x20) + 0x10),
                                 MOBY(moby)->pos.y - *(float *)(*(char **)(hit + 0x20) + 0x14));
                *(u128 *)hit_vector_or_sight_line_origin = *(u128 *)(hit + 0x10);
                angle = heading;
            }
            trooper_knockback_arc(hit_vector_or_sight_line_origin,
                                  &angle,
                                  sentrybot_data + 0x88,
                                  sentrybot_data + 0x8C);
            trooper_start_knockback(moby, sentrybot_data + 0x70, 7, 1, 0, angle);
            moby[0x20] = 0x15;
            moby[0xBC] = 2;
            sentrybot_data[0x67] = 0x78;
            break;
        case LETHAL_HIT_RESPONSE:
        case DEATH_KNOCKBACK_HIT_RESPONSE: {
            float step;
            float step_sq;
            float forward;
            float lateral;
            float vz;
            float fall_speed;
            step = frame_time;
            fall_speed = sentrybot_death_knockback_gravity_gp;
            step_sq = frame_time_sq;
            fall_speed = fall_speed * step_sq;
            forward = sentrybot_death_knockback_horizontal_speed_gp * step;
            lateral = sentrybot_death_knockback_vertical_speed_gp * step;
            vz = sentrybot_death_knockback_horizontal_speed_approach_rate_gp * step;
            MOBY(moby)->flags &= 0xEFFF;
            *(float *)(sentrybot_data + 0x80) = fall_speed;
            *(float *)(sentrybot_data + 0x88) = forward;
            *(float *)(sentrybot_data + 0x8C) = lateral;
            *(float *)(sentrybot_data + 0x84) = vz;
            *(float *)(sentrybot_data + 0xC0) = 8.0f;
            *(float *)(sentrybot_data + 0xC4) = 16.0f;
        }
            {
                float heading =
                    fast_arc_tan(MOBY(moby)->pos.x - *(float *)(*(char **)(hit + 0x20) + 0x10),
                                 MOBY(moby)->pos.y - *(float *)(*(char **)(hit + 0x20) + 0x14));
                *(u128 *)hit_vector_or_sight_line_origin = *(u128 *)(hit + 0x10);
                fall_angle = heading;
            }
            trooper_knockback_arc(hit_vector_or_sight_line_origin,
                                  &fall_angle,
                                  sentrybot_data + 0x88,
                                  sentrybot_data + 0x8C);
            trooper_start_knockback(moby, sentrybot_data + 0x70, 8, 1, 0, fall_angle);
            moby[0x20] = 0x17;
            moby[0xBC] = 2;
            sentrybot_data[0x67] = 0xF0;
            break;
        }
        trooper_anim_update(moby, sentrybot_data + 0x60);
    }
    moby[0xA4] = 0xFF;
    trooper_anim_finish(moby, sentrybot_data + 0x60);
    if (flag != 0)
        return;
    if (*(int *)(sentrybot_data + 0x38) != 0) {
        float random_target_countdown_duration;
        int   rounded;
        random_target_countdown_duration = random_float_between(180.0f, 240.0f);
        rounded = truncate_float_to_s32(scale_time(random_target_countdown_duration));
        *(int *)(sentrybot_data + 0x38) = 0;
        *(float *)(sentrybot_data + 0x130) = (float)rounded;
    }
    update_float_countdown(sentrybot_data + 0x130);
    if (fast_vec_dist(moby + 0x10, D_0013F420) < 64.0f) {
        group = 0;
        qcopy(hit_vector_or_sight_line_origin, moby + 0x10);
        hit_vector_or_sight_line_origin[2] += 1.0f;
        if (*(int *)(sentrybot_data + 0x12C) != -1)
            group = (int)l15_moby_array + (*(int *)(sentrybot_data + 0x12C) << 8);
        sentrybot_data[0x15A] =
            collision_line(hit_vector_or_sight_line_origin, D_0013F420, 2, (void *)group, 0) == 0;
    } else {
        sentrybot_data[0x15A] = 0;
    }
    path = D_L15_001B0AB0[*(int *)(sentrybot_data + 0x120)];
    if (FUN_L00_0025ff38(moby, sentrybot_data + 0xD0, 128.0f, 0, 0, path + 4, *path) != 2) {
        float target_angle;
        seen = 0;
        target_angle = fast_arc_tan(*(float *)(sentrybot_data + 0xD0) - MOBY(moby)->pos.x,
                                    *(float *)(sentrybot_data + 0xD4) - MOBY(moby)->pos.y);
        if ((fast_difference_between_rotations(MOBY(moby)->rot.z, target_angle) < 1.5707964f &&
             (sentrybot_target_visibility_height_check_gate[0] == 0 ||
              absolute_float(MOBY(moby)->pos.z - *(float *)(sentrybot_data + 0xD8)) < 4.0f)) ||
            moby[0x20] == 0x10 || moby[0x20] == 0x11) {
            seen = sentrybot_data[0x15A];
        }
        if (*(int *)(sentrybot_data + 0x114) == 1)
            sentrybot_data[0x15E] = sentrybot_data[0x15B] + 1;
        else if ((unsigned char)D_001413F4[0] != 3)
            sentrybot_data[0x15E] = 0;
        else if (sentrybot_data[0x15E] >= 2)
            sentrybot_data[0x15B] = 1;
        sentrybot_data[0x19F] = 1;
        if (seen == 0) {
            if (sentrybot_data[0x15A] == 0)
                sentrybot_data[0x15B] = 1;
            sentrybot_data[0x19F] = 0;
            *(int *)(sentrybot_data + 0x114) = 2;
            *(short *)(sentrybot_data + 0x158) = 0;
        } else if (((*(short *)(sentrybot_data + 0x156) == 0 || sentrybot_data[0x15B] != 0) &&
                    (unsigned char)D_001413F4[0] == 3) ||
                   (sentrybot_data[0x15E] != 0 && *(int *)(sentrybot_data + 0x114) != 1)) {
            *(int *)(sentrybot_data + 0x114) = 2;
            *(short *)(sentrybot_data + 0x158) = 1;
        } else if ((unsigned char)D_001413F4[0] != 3 && *(int *)(sentrybot_data + 0x114) != 2) {
            sentrybot_data[0x15B] = 0;
        }
        if (D_L15_0015F5C4 == 2)
            *(int *)(sentrybot_data + 0x114) = 2;
    } else {
        sentrybot_data[0x19F] = 0;
        sentrybot_data[0x15B] = 1;
        *(short *)(sentrybot_data + 0x158) = 0;
    }
    if (*(int *)(sentrybot_data + 0x110) == 0)
        *(int *)(sentrybot_data + 0x110) = (*(int *)&D_001413D0_lui);
}

/* Applies the same effect to three consecutive 16-byte slots of a moby's data, tagged with its colour word. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029BD88), where it is exact; names translated to the US level program. */

void draw_sentrybot_effect_quads(char *moby) {
    int   effect_color = (MOBY(moby)->glow_color & 0xFFFFFF) | 0x30000000;
    char *effect_position = ((char *)MOBY(moby)->pvars) + 0x2A0;
    int   i;
    for (i = 2; i >= 0; i--) {
        draw_offset_textured_effect_quad(effect_position, effect_color, 0.2f, 0.08f);
        effect_position += 0x10;
    }
}

/* Marks the moby as killed in the level's spawn tables and releases the voice slot it still owns. */

void mark_moby_killed_and_release_owned_voice(int addr) {
    struct Moby *moby = (struct Moby *)addr;
    s32          slot;
    moby->collision = 0;
    D_L15_001BABD0.killed[(s16)moby->save_id] = moby->mission + 2;
    if (moby->mission == 0xFF || (secondary_kill_record_gate_by_mission[moby->mission] != 0xFF &&
                                  D_0014C050[current_level_index][moby->mission] == 0xFF)) {
        l15_save_state.cleared[(s16)moby->save_id] = moby->mission + 2;
    }
    if (moby->command != 0xFF) {
        VoiceSlot *voice_slot;
        slot = moby->command;
        voice_slot = (VoiceSlot *)(D_0013E550 + slot * 0x70);
        if (voice_slot->owner == moby && voice_slot->active != 0) {
            release_voice_slot(slot);
        }
    }
    moby->command = 0xFF;
    moby->state = 2;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCE50), where it is exact; names translated to the US level program. */

void set_sentrybot_alarm_duration(char *moby, int duration_frames) {
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    int    flag;
    short *group_moby_entry;
    if (duration_frames == -1) {
        *(int *)&D_L15_00161ABC_gp = 0;
        duration_frames = 0;
    }
    flag = duration_frames != 0;
    if (((unsigned char *)moby)[0x21] == 0xFF) {
        *(int *)(moby_data + 0x18) = scale_game_frames(duration_frames);
        *(int *)(moby_data + 8) = flag;
        return;
    }
    group_moby_entry = (short *)D_L15_001ABE40_lui[((unsigned char *)moby)[0x21]];
    if (group_moby_entry) {
        do {
            char *listed_moby = (char *)(((*(unsigned short *)group_moby_entry & 0x7FFF) << 8) +
                                         (int)l15_moby_array);
            if (MOBY(listed_moby)->oclass == 0x198) {
                char *listed_moby_data = ((char *)MOBY(listed_moby)->pvars);
                *(int *)(listed_moby_data + 8) = flag;
                *(int *)(listed_moby_data + 0x18) = scale_game_frames(duration_frames);
            }
        } while (*group_moby_entry++ >= 0);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCF30), where it is exact; names translated to the US level program. */

void prepare_sentrybot_alarm(unsigned char *moby) {
    char *alarm_moby_data = ((char *)MOBY(moby)->pvars);
    if (moby[0x20] == 1) {
        moby[0x20] = 2;
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
    }
    if (moby[0x20] == 3) {
        if (*(int *)(alarm_moby_data + 8) == 0) {
            *(int *)(alarm_moby_data + 0x18) = scale_game_frames(0x78);
        }
    }
}

float D_L17_0016138C = 1.0f;
short D_L17_00161990 = 0;

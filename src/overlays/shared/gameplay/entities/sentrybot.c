/* Shared moby classes: sentrybot, rocket, mine, spiderbot, door, sentrybot_alarm. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"

/* Sentrybot update: runs the patrol, alert and attack state machine, then sets the alert level and the eye colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_00298BB8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x2084];
    int f2084;
    char pad1[0x258];
    short f22E0;
} Hero98BB8;

typedef struct {
    char pad0[0x180];
    int f180[4];
} Sentry98BB8;

extern char *spawn_class_3a6_moby(char *, float *, char *, char *) __asm__("FUN_L15_002e4580");
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern float D_L15_001673C0[4];
extern float D_L15_001CBFE0[];
extern float D_L15_001CC060[];
extern float D_L15_001CC0E0[];
extern float FUN_001f9b48(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float animation_end(int a, unsigned int b) __asm__("FUN_L04_00241910");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_sin(float);
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *);
extern int *D_L15_001B0AB0[];
extern int D_L15_0015F5C4;
extern int D_L15_0015F5CC;
extern int D_L15_0015FFD8_m __asm__("D_L15_0015FFD8");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int begin_sentrybot_turn_toward_heading(unsigned char *, char *, int, int, float) __asm__("FUN_L15_00299880");
int move_sentrybot_toward_point(char *moby, float *point, float target_forward_step) __asm__("FUN_L15_00299ae0");

extern int move_sentrybot_toward_point(char *moby, float *point, float arg) __asm__("FUN_L15_00299ae0");
extern int can_sentrybot_approach_linked_moby(void *) __asm__("FUN_L15_00299ca0");
int find_sentrybot_attention_target(char *moby) __asm__("FUN_L15_00299dd8");

extern int find_sentrybot_attention_target(char *m) __asm__("FUN_L15_00299dd8");
int can_sentrybot_advance_patrol(void *input_moby) __asm__("FUN_L15_00299ff8");

extern int can_sentrybot_advance_patrol(void *) __asm__("FUN_L15_00299ff8");
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int random_integer_below(int);
extern int scale_game_frames(int);
extern int tick_countdown_32(void *);
extern int truncate_float_to_s32() __asm__("FUN_001fa6d0");
#include "rnc/overlay/shared/gameplay/entities/sentrybot_data/sentrybot_update.h"
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void compute_selected_world_matrices(void *, int, void *, void *) __asm__("FUN_L00_0024f8f0");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025a120(void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void manipulator_set_scale(void *, float) __asm__("FUN_L00_00263b38");
void begin_sentrybot_turn_toward_point(char *moby, void *moby_data, float *point, void *next_state, void *next_animation) __asm__("FUN_L15_00299a68");

extern void begin_sentrybot_turn_toward_point(char *moby, void *a, float *point, void *b, void *c) __asm__("FUN_L15_00299a68");
void alert_visible_group_sentrybots(void *mp) __asm__("FUN_L15_0029a0d0");

void alert_visible_group_sentrybots(void *mp) __asm__("FUN_L15_0029a0d0");
extern void update_sentrybot_joint_aim(void *) __asm__("FUN_L15_0029a2a8");
extern void update_sentrybot_hit_response_and_target_visibility(void *, int) __asm__("FUN_L15_0029a570");
void draw_sentrybot_effect_quads(char *moby) __asm__("FUN_L15_0029aba8");

extern void draw_sentrybot_effect_quads(char *m) __asm__("FUN_L15_0029aba8");
extern void mark_moby_killed_and_release_owned_voice(int) __asm__("FUN_L15_002a35d8");
extern void set_sentrybot_alarm_duration(char *m, int a) __asm__("FUN_L15_002cbac0");
void prepare_sentrybot_alarm(unsigned char *moby) __asm__("FUN_L15_002cbba0");

extern void prepare_sentrybot_alarm(unsigned char *moby) __asm__("FUN_L15_002cbba0");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void transform_scaled_vertex_batch(char *arg0, int arg1, void *arg2,
                                          char *arg3) __asm__("FUN_0020cd48");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void sentrybot_update(unsigned char *moby) __asm__("FUN_L15_002979d8");
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
    float va[4];
    float vb[4];
    float emission_origin[4];
    float selected_joint_world_matrix[4][4];
    int j2;
    int j3;
    unsigned char *data;
    char *target;
    int flag;
    int clip_volume_index;
    int lvl;
    int tick;
    int alpha;
    int color_pulse_period_frames;
    int color;
    float color_pulse_sine;

    if (sentrybot_update_disabled_gp != 0)
        return;
    data = *(unsigned char **)(moby + 0x78);
    if (D_L15_0015F5C4 == 2) {
        *(unsigned short *)(moby + 0x34) |= 0x41;
        return;
    }
    clip_volume_index = *(int *)(data + 0x2D0);
    flag = 0;
    if (clip_volume_index != -1) {
        flag = is_point_inside_clip_volume(D_L15_001673C0, clip_volume_index) == 0;
        if (*(int *)(data + 0x2D4) != -1) {
            flag &= is_point_inside_clip_volume(D_L15_001673C0, *(int *)(data + 0x2D4)) == 0;
        }
    } else if (*(int *)(data + 0x2D8) != -1) {
        flag = is_point_inside_clip_volume(D_L15_001673C0, clip_volume_index);
        if (*(int *)(data + 0x2DC) != -1) {
            flag |= is_point_inside_clip_volume(D_L15_001673C0, *(int *)(data + 0x2DC));
        }
    }
    update_sentrybot_hit_response_and_target_visibility(moby, flag);
    if (flag != 0 && moby[0x20] != 0x15 && moby[0x20] != 0x17) {
        *(int *)(moby + 0x94) = 0;
        *(unsigned short *)(moby + 0x34) = (*(unsigned short *)(moby + 0x34) | 0x41) & 0xEFFF;
        return;
    }
    *(unsigned short *)(moby + 0x34) = (*(unsigned short *)(moby + 0x34) & 0xFFBE) | 0x1000;
    *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
    target = *(char **)(data + 0x110);
    manipulator_set_scale(data + 0x1A0, 2.5f);
    if (moby[0x31] != 0 && FUN_001f9b48(moby + 0x10, D_L15_001673C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0x16;
    }
    if (hero.state.current == 0x65)
        *(short *)(data + 0x156) = 0;
    fast_dec_timer_s16(data + 0x154);
    fast_dec_timer_s16(data + 0x156);
    fast_dec_timer_s16(data + 0x19C);
    if (*(short *)(data + 0x156) != 0 && *(int *)(data + 0x128) != -1) {
        char *o = (char *)((*(int *)(data + 0x128) << 8) + D_L15_0015FFD8_m);
        if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
            set_sentrybot_alarm_duration(o, 0xB4);
    }
    if (moby[0xBC] == 0)
        data[0x15F] = 0;
    switch (moby[0x20]) {
    case SENTRYBOT_INITIALIZE:
        if (*(int *)(data + 0x120) == -1 || *D_L15_001B0AB0[*(int *)(data + 0x120)] == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        *(short *)(data + 0x24) = 3;
        *(float *)(data + 0x20) = 3.0f;
        data[0x28] = 2;
        data[0x29] = 0;
        qcopy(data + 0x160, moby + 0x10);
        *(float *)(data + 0x16C) = *(float *)(moby + 0x48);
        if (*(int *)(data + 0x148) < 5)
            *(int *)(data + 0x148) = 5;
        *(int *)(data + 0x144) = 1;
        moby[0x20] = 2;
        *(unsigned short *)(moby + 0x34) |= 0x1000;
        {
            int i;
            for (i = 0; i < 4; i++) {
                if (((Sentry98BB8 *)data)->f180[i] != -1) {
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
            if (((unsigned char *)((((Sentry98BB8 *)data)->f180[i] << 8) +
                                   D_L15_0015FFD8_m))[0x20] >= 2) {
                moby[0x20] = 2;
                break;
            }
        }
        break;
    }
    case SENTRYBOT_PATROL_IDLE:
    case SENTRYBOT_PATROL_PAUSE:
        if (*(int *)(data + 0x114) != 2) {
            moby[0xBC] = 1;
            if (data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                data[0x15F] = 1;
                moby[0x20] = 0xB;
                data[0x15C] = 0x10;
                data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (*(short *)(data + 0x156) != 0 && data[0x15B] == 0) {
            if (data[0x15A] != 0 || *(int *)(data + 0x124) == -1) {
                begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(target + 0x10), (void *)0x10,
                                 (void *)3);
            } else {
                if (*(int *)(data + 0x144) != 0) {
                    *(int *)(data + 0x13C) =
                        (*(int *)(data + 0x13C) + 1) % *D_L15_001B0AB0[*(int *)(data + 0x124)];
                }
                *(int *)(data + 0x144) = 0;
                moby[0xBC] = 1;
                if (data[0x15F] != 0) {
                    moby[0x20] = 0xD;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(10));
                } else {
                    data[0x15F] = 1;
                    moby[0x20] = 0xB;
                    data[0x15C] = 0xD;
                    data[0x15D] = 0xE;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            }
        } else if (*(short *)(data + 0x19C) != 0) {
            begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(*(char **)(data + 0x198) + 0x10),
                             (void *)0xA, (void *)0xF);
        } else if (fast_dec_timer_s16(data + 0x196) != 0 &&
                   (*(int *)(data + 0x190) = find_sentrybot_attention_target((char *)moby)) != 0 &&
                   (*(short *)(data + 0x194) != 0 ||
                    random_integer_below(scale_game_frames(0x96)) == 0)) {
            begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(*(char **)(data + 0x190) + 0x10),
                             (void *)0xA, (void *)0xF);
        } else if (moby[0x20] == 2) {
            if (*(int *)(data + 0x124) != -1) {
                if (*(int *)(data + 0x144) == 0 || can_sentrybot_advance_patrol(moby) != 0) {
                    *(int *)(data + 0x14C) = scale_game_frames(*(int *)(data + 0x148));
                    moby[0x20] = 3;
                }
            } else if (*(short *)(data + 0x158) != 0 &&
                       hero.state.current == 0x59) {
                if (*(int *)(data + 0x150) != -1 || *(int *)(data + 0x12C) != -1 ||
                    (*(short *)(data + 0x154) == 0 && random_integer_below(0x78) == 0)) {
                    data[0x15D] = 0;
                    data[0x15C] = 2;
                    moby[0x20] = 5;
                    if (moby[0x53] != 2)
                        blend_moby_animation(moby, 2, 0, scale_game_frames(10));
                }
            } else if (*(int *)(data + 0x124) == -1) {
                begin_sentrybot_turn_toward_heading(moby, data, 2, 0, *(float *)(data + 0x16C));
            }
        } else if (moby[0x20] == 3) {
            if (tick_countdown_32(data + 0x14C) != 0) {
                if (*(int *)(data + 0x144) != 0) {
                    *(int *)(data + 0x13C) =
                        (*(int *)(data + 0x13C) + 1) % *D_L15_001B0AB0[*(int *)(data + 0x124)];
                }
                *(int *)(data + 0x144) = 0;
                begin_sentrybot_turn_toward_point((char *)moby, data,
                                 (float *)((char *)D_L15_001B0AB0[*(int *)(data + 0x124)] +
                                           (*(int *)(data + 0x13C) * 0x10 + 0x10)),
                                 (void *)4, (void *)0xD);
            }
        }
        break;
    case SENTRYBOT_WAIT_FOR_ANIMATION_THEN_PATROL_IDLE:
        if (*(int *)(data + 0x114) != 2) {
            moby[0xBC] = 1;
            if (data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                data[0x15F] = 1;
                moby[0x20] = 0xB;
                data[0x15C] = 0x10;
                data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (moby[0x70] & 2) {
            moby[0x20] = 2;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            *(short *)(data + 0x196) = scale_game_frames(0x168);
            *(short *)(data + 0x194) = 0;
        }
        break;
    case SENTRYBOT_DEACTIVATE_LINKED_MOBYS_AFTER_ANIMATION:
        if (moby[0x70] & 2) {
            *(short *)(data + 0x154) = scale_game_frames(0xF0);
            if (*(int *)(data + 0x150) != -1) {
                mark_moby_killed_and_release_owned_voice(D_L15_0015FFD8_m + (*(int *)(data + 0x150) << 8));
                *(int *)(data + 0x150) = -1;
            }
            if (*(int *)(data + 0x12C) != -1) {
                mark_moby_killed_and_release_owned_voice(D_L15_0015FFD8_m + (*(int *)(data + 0x12C) << 8));
                *(int *)(data + 0x12C) = -1;
            }
            moby[0x20] = data[0x15C];
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
        if (*(int *)(data + 0x114) != 2 &&
            fast_difference_between_rotations(
                *(float *)(moby + 0x48),
                fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(moby + 0x10),
                             *(float *)(data + 0xD4) - *(float *)(moby + 0x14))) < 0.5235988f) {
            moby[0xBC] = 1;
            if (data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                data[0x15F] = 1;
                moby[0x20] = 0xB;
                data[0x15C] = 0x10;
                data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else {
            int anim = moby[0x52];
            float *turn_heading_offsets_degrees = 0;
            switch (anim) {
            case SENTRYBOT_ANIM_TURN_AROUND:
                turn_heading_offsets_degrees = D_L15_001CC0E0;
                break;
            case SENTRYBOT_ANIM_TURN_NEGATIVE_HEADING_DELTA:
                turn_heading_offsets_degrees = D_L15_001CC060;
                break;
            case SENTRYBOT_ANIM_TURN_POSITIVE_HEADING_DELTA:
                turn_heading_offsets_degrees = D_L15_001CBFE0;
                break;
            }
            if (moby[0x70] & 2) {
                int k = func_001FA898_r(animation_end(moby[0x22], anim));
                if (turn_heading_offsets_degrees != 0) {
                    *(float *)(moby + 0x48) =
                        fast_add_rotations(*(float *)(data + 0x170), turn_heading_offsets_degrees[k] * DEG_TO_RAD);
                }
                moby[0x20] = data[0x15C];
                if (moby[0x53] != data[0x15D])
                    blend_moby_animation(moby, data[0x15D], 0, scale_game_frames(10));
            } else if (moby[0x52] == moby[0x53] && turn_heading_offsets_degrees != 0) {
                float t = compute_interpolated_record_value(moby);
                int k = func_001FA898_r(t);
                t -= convert_integer_to_float(k);
                *(float *)(moby + 0x48) = fast_add_rotations(
                    *(float *)(data + 0x170), (turn_heading_offsets_degrees[k] + (turn_heading_offsets_degrees[k + 1] - turn_heading_offsets_degrees[k]) * t) * DEG_TO_RAD);
            }
        }
        break;
    case SENTRYBOT_PATROL_MOVE: {
        float sp = frame_time * 2.0f;
        if (move_sentrybot_toward_point((char *)moby,
                             (float *)((char *)D_L15_001B0AB0[*(int *)(data + 0x124)] +
                                       (*(int *)(data + 0x13C) * 0x10 + 0x10)),
                             sp)) {
            moby[0x20] = 2;
            *(int *)(data + 0x144) = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (*(int *)(data + 0x114) != 2) {
            moby[0xBC] = 1;
            if (data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                data[0x15F] = 1;
                moby[0x20] = 0xB;
                data[0x15C] = 0x10;
                data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (*(short *)(data + 0x156) != 0 && data[0x15B] == 0) {
            if (data[0x15A] != 0) {
                begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(target + 0x10), (void *)0x10,
                                 (void *)3);
            } else {
                *(int *)(data + 0x14C) = scale_game_frames(0x3C);
                moby[0xBC] = 1;
                if (data[0x15F] != 0) {
                    moby[0x20] = 0xD;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(10));
                } else {
                    data[0x15F] = 1;
                    moby[0x20] = 0xB;
                    data[0x15C] = 0xD;
                    data[0x15D] = 0xE;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            }
        } else if (*(short *)(data + 0x19C) != 0) {
            begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(*(char **)(data + 0x198) + 0x10),
                             (void *)0xA, (void *)0xF);
        } else if (*(short *)(data + 0x158) != 0 && hero.state.current == 0x59) {
            if (*(int *)(data + 0x150) != -1 || *(int *)(data + 0x12C) != -1 ||
                *(short *)(data + 0x154) == 0) {
                data[0x15C] = moby[0x20];
                data[0x15D] = 0xD;
                moby[0x20] = 5;
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        } else if (fast_dec_timer_s16(data + 0x196) != 0 &&
                   (*(int *)(data + 0x190) = find_sentrybot_attention_target((char *)moby)) != 0 &&
                   (*(short *)(data + 0x194) != 0 ||
                    random_integer_below(scale_game_frames(0x96)) == 0)) {
            begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(*(char **)(data + 0x190) + 0x10),
                             (void *)0xA, (void *)0xF);
        }
        break;
    }
    case SENTRYBOT_PATROL_MOVE_FAST: {
        float sp = frame_time * 3.5f;
        if (move_sentrybot_toward_point((char *)moby,
                             (float *)((char *)D_L15_001B0AB0[*(int *)(data + 0x124)] +
                                       (*(int *)(data + 0x13C) * 0x10 + 0x10)),
                             sp)) {
            moby[0x20] = 0xC;
            *(int *)(data + 0x144) = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (*(int *)(data + 0x114) != 2 && data[0x15B] == 0) {
            moby[0xBC] = 1;
            if (data[0x15F] != 0) {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                data[0x15F] = 1;
                moby[0x20] = 0xB;
                data[0x15C] = 0x10;
                data[0x15D] = 3;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (*(short *)(data + 0x156) != 0) {
            *(int *)(data + 0x14C) = scale_game_frames(0x3C);
        } else {
            moby[0x20] = 9;
            if (moby[0x53] != 0xD)
                blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
            if (*(int *)(data + 0x128) != -1) {
                char *o = (char *)((*(int *)(data + 0x128) << 8) + D_L15_0015FFD8_m);
                if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
                    set_sentrybot_alarm_duration(o, scale_game_frames(0x1E));
            }
        }
        break;
    }
    case SENTRYBOT_RETURN_HOME: {
        unsigned char *home = data + 0x160;
        if (FUN_001f9b48(moby + 0x10, home) > 0.2f) {
            move_sentrybot_toward_point((char *)moby, (float *)home, frame_time * 2.0f);
        } else if (*(int *)(data + 0x124) != -1) {
            moby[0xBC] = 0;
            *(int *)(data + 0x144) = 0;
            begin_sentrybot_turn_toward_point((char *)moby, data,
                             (float *)((char *)D_L15_001B0AB0[*(int *)(data + 0x124)] +
                                       (*(int *)(data + 0x13C) * 0x10 + 0x10)),
                             (void *)2, (void *)0);
        } else {
            begin_sentrybot_turn_toward_heading(moby, data, 2, 0, *(float *)(data + 0x16C));
        }
        break;
    }
    case SENTRYBOT_RETURN_HOME_FAST: {
        unsigned char *home = data + 0x160;
        if (FUN_001f9b48(moby + 0x10, home) > 0.2f) {
            move_sentrybot_toward_point((char *)moby, (float *)home, frame_time * 3.0f);
        } else if (*(short *)(data + 0x156) == 0) {
            moby[0xBC] = 0;
            *(int *)(data + 0x144) = 0;
            if (*(int *)(data + 0x124) != -1) {
                begin_sentrybot_turn_toward_point((char *)moby, data,
                                 (float *)((char *)D_L15_001B0AB0[*(int *)(data + 0x124)] +
                                           (*(int *)(data + 0x13C) * 0x10 + 0x10)),
                                 (void *)2, (void *)0);
            } else {
                begin_sentrybot_turn_toward_heading(moby, data, 2, 0, *(float *)(data + 0x16C));
            }
        } else if (fast_difference_between_rotations(
                       *(float *)(moby + 0x48),
                       fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(moby + 0x10),
                                    *(float *)(data + 0xD4) - *(float *)(moby + 0x14))) >
                       0.7853982f &&
                   data[0x15B] == 0) {
            begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(data + 0xD0), (void *)0x10, (void *)3);
        } else {
            moby[0x20] = 0xC;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    case SENTRYBOT_WAIT_FOR_TRANSITION_ANIMATION:
        if (*(int *)(data + 0x128) != -1) {
            prepare_sentrybot_alarm((unsigned char *)(D_L15_0015FFD8_m + (*(int *)(data + 0x128) << 8)));
        }
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                alert_visible_group_sentrybots(moby);
                if (*(int *)(data + 0x128) != -1 &&
                    *(int *)(*(char **)((char *)((*(int *)(data + 0x128) << 8) + D_L15_0015FFD8_m) +
                                        0x78) +
                             8) == 0 &&
                    can_sentrybot_approach_linked_moby(moby) != 0) {
                    moby[0x20] = 0xE;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(0x14));
                } else if (data[0x15C] == 0x10) {
                    begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(target + 0x10), (void *)0x10,
                                     (void *)3);
                } else {
                    if (data[0x15C] != 0xFF)
                        moby[0x20] = data[0x15C];
                    if (data[0x15D] != 0xFF && moby[0x53] != data[0x15D])
                        blend_moby_animation(moby, data[0x15D], 0, scale_game_frames(10));
                }
            }
        }
        break;
    case SENTRYBOT_APPROACH_LINKED_MOBY:
        prepare_sentrybot_alarm((unsigned char *)(D_L15_0015FFD8_m + (*(int *)(data + 0x128) << 8)));
        subtract_vector_xyz(va, (char *)(D_L15_0015FFD8_m + (*(int *)(data + 0x128) << 8)) + 0x10,
                            moby + 0x10);
        normalize_vector_xyz(va, va, vector_length_xyz(va) - 1.5f);
        add_vector_xyz(va, va, moby + 0x10);
        if (move_sentrybot_toward_point((char *)moby, va, frame_time * 4.0f)) {
            moby[0x20] = 0xF;
            if (moby[0x53] != 9)
                blend_moby_animation(moby, 9, 0, scale_game_frames(10));
        }
        break;
    case SENTRYBOT_ACTIVATE_LINKED_MOBY:
        if (moby[0x52] == moby[0x53] && compute_interpolated_record_value(moby) >= 12.0f) {
            set_sentrybot_alarm_duration((char *)(D_L15_0015FFD8_m + (*(int *)(data + 0x128) << 8)), 0xB4);
        }
        if (moby[0x70] & 2) {
            set_sentrybot_alarm_duration((char *)(D_L15_0015FFD8_m + (*(int *)(data + 0x128) << 8)), 0xB4);
            begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(target + 0x10), (void *)0x14,
                             (void *)0xE);
        }
        break;
    case SENTRYBOT_WAIT_FOR_ANIMATION_THEN_EMIT_WITH_POSITIVE_JOINT_AIM:
        *(short *)(data + 0x19C) = 0;
        *(short *)(data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x13;
                data[0x19E] = 0;
                if (moby[0x53] != 6)
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case SENTRYBOT_WAIT_FOR_ANIMATION_THEN_EMIT_WITH_NEGATIVE_JOINT_AIM:
        *(short *)(data + 0x19C) = 0;
        *(short *)(data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x11;
                data[0x19E] = 0;
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
        }
        break;
    case SENTRYBOT_EMIT_WITH_POSITIVE_JOINT_AIM:
        *(short *)(data + 0x19C) = 0;
        *(short *)(data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (data[0x19E] == 0) {
            float t = compute_interpolated_record_value(moby);
            if (t >= 0.85f && t < 3.0f) {
                j2 = 2;
                compute_selected_world_matrices(moby, 1, &j2, selected_joint_world_matrix);
                transform_scaled_vertex_batch((char *)moby, 1, &j2, (char *)emission_origin);
                normalize_vector_xyz(va, selected_joint_world_matrix[1], sentrybot_emission_vector_speed_gp * frame_time);
                subtract_vector_xyz(vb, data + 0x100, emission_origin);
                normalize_vector_xyz(vb, vb, sentrybot_emission_vector_speed_gp * frame_time);
                if (fast_difference_between_rotations(fast_arc_tan(va[0], va[1]),
                                                      fast_arc_tan(vb[0], vb[1])) > 0.7853982f ||
                    fast_difference_between_rotations(fast_arc_tan(vector_length_xy(va), va[2]),
                                                      fast_arc_tan(vector_length_xy(vb), vb[2])) >
                        0.7853982f) {
                    spawn_class_3a6_moby((char *)moby, va, (char *)emission_origin, (char *)(data + 0x100));
                } else {
                    spawn_class_3a6_moby((char *)moby, vb, (char *)emission_origin, (char *)(data + 0x100));
                }
                data[0x19E] = 1;
            }
        }
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else if (*(int *)(data + 0x128) != -1 &&
                       *(int *)(*(char **)((char *)((*(int *)(data + 0x128) << 8) +
                                                    D_L15_0015FFD8_m) +
                                           0x78) +
                                8) == 0 &&
                       can_sentrybot_approach_linked_moby(moby) != 0) {
                moby[0x20] = 0xE;
                if (moby[0x53] != 0xE)
                    blend_moby_animation(moby, 0xE, 0, scale_game_frames(0x14));
            } else if (fast_difference_between_rotations(
                           *(float *)(moby + 0x48),
                           fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(moby + 0x10),
                                        *(float *)(data + 0xD4) - *(float *)(moby + 0x14))) >
                       0.7853982f) {
                begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(data + 0xD0), (void *)0x10,
                                 (void *)3);
            } else if (*(int *)(data + 0x114) == 2) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x10;
                if (moby[0x53] != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
            data[0x19E] = 0;
        }
        break;
    case SENTRYBOT_EMIT_WITH_NEGATIVE_JOINT_AIM:
        *(short *)(data + 0x19C) = 0;
        *(short *)(data + 0x194) = 0;
        alert_visible_group_sentrybots(moby);
        if (data[0x19E] == 0) {
            float t = compute_interpolated_record_value(moby);
            if (t >= 0.85f && t < 3.0f) {
                j3 = 3;
                compute_selected_world_matrices(moby, 1, &j3, selected_joint_world_matrix);
                transform_scaled_vertex_batch((char *)moby, 1, &j3, (char *)emission_origin);
                normalize_vector_xyz(va, selected_joint_world_matrix[1], -(sentrybot_emission_vector_speed_gp * frame_time));
                subtract_vector_xyz(vb, data + 0x100, emission_origin);
                normalize_vector_xyz(vb, vb, sentrybot_emission_vector_speed_gp * frame_time);
                if (fast_difference_between_rotations(fast_arc_tan(va[0], va[1]),
                                                      fast_arc_tan(vb[0], vb[1])) > 0.7853982f ||
                    fast_difference_between_rotations(fast_arc_tan(vector_length_xy(va), va[2]),
                                                      fast_arc_tan(vector_length_xy(vb), vb[2])) >
                        0.7853982f) {
                    spawn_class_3a6_moby((char *)moby, va, (char *)emission_origin, (char *)(data + 0x100));
                } else {
                    spawn_class_3a6_moby((char *)moby, vb, (char *)emission_origin, (char *)(data + 0x100));
                }
                data[0x19E] = 1;
            }
        }
        if (moby[0x70] & 2) {
            if (hero.state.current == 0x65) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else if (*(int *)(data + 0x128) != -1 &&
                       *(int *)(*(char **)((char *)((*(int *)(data + 0x128) << 8) +
                                                    D_L15_0015FFD8_m) +
                                           0x78) +
                                8) == 0 &&
                       can_sentrybot_approach_linked_moby(moby) != 0) {
                moby[0x20] = 0xE;
                if (moby[0x53] != 0xE)
                    blend_moby_animation(moby, 0xE, 0, scale_game_frames(0x14));
            } else if (fast_difference_between_rotations(
                           *(float *)(moby + 0x48),
                           fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(moby + 0x10),
                                        *(float *)(data + 0xD4) - *(float *)(moby + 0x14))) >
                       0.7853982f) {
                begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(data + 0xD0), (void *)0x12,
                                 (void *)5);
            } else if (*(int *)(data + 0x114) == 2) {
                moby[0x20] = 0xC;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 0x12;
                if (moby[0x53] != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
            data[0x19E] = 0;
        }
        break;
    case SENTRYBOT_ALERT_IDLE:
        if (hero.state.current == 0x65) {
            moby[0x20] = 9;
            if (moby[0x53] != 0xD)
                blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
            if (*(int *)(data + 0x128) != -1) {
                char *o = (char *)((*(int *)(data + 0x128) << 8) + D_L15_0015FFD8_m);
                if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
                    set_sentrybot_alarm_duration(o, -1);
            }
        } else {
            int mode = *(int *)(data + 0x114);
            if (mode != 2 && data[0x15B] == 0) {
                begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(data + 0xD0), (void *)0x10,
                                 (void *)3);
            } else if (*(short *)(data + 0x156) == 0) {
                moby[0x20] = 9;
                if (moby[0x53] != 0xD)
                    blend_moby_animation(moby, 0xD, 0, scale_game_frames(10));
                if (*(int *)(data + 0x128) != -1) {
                    char *o = (char *)((*(int *)(data + 0x128) << 8) + D_L15_0015FFD8_m);
                    if (*(int *)(*(char **)(o + 0x78) + 8) != 0)
                        set_sentrybot_alarm_duration(o, scale_game_frames(0x1E));
                }
            } else if (data[0x15A] != 0) {
                if (mode != 2 && data[0x15B] == 0) {
                    begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(target + 0x10), (void *)0x10,
                                     (void *)3);
                } else {
                    begin_sentrybot_turn_toward_point((char *)moby, data, (float *)(target + 0x10), (void *)0xC,
                                     (void *)0);
                }
            } else if (*(int *)(data + 0x124) != -1) {
                if (*(int *)(data + 0x144) != 0) {
                    *(int *)(data + 0x13C) =
                        (*(int *)(data + 0x13C) + 1) % *D_L15_001B0AB0[*(int *)(data + 0x124)];
                }
                *(int *)(data + 0x144) = 0;
                *(int *)(data + 0x14C) = scale_game_frames(0x3C);
                moby[0xBC] = 1;
                if (data[0x15F] != 0) {
                    moby[0x20] = 0xD;
                    if (moby[0x53] != 0xE)
                        blend_moby_animation(moby, 0xE, 0, scale_game_frames(10));
                } else {
                    data[0x15F] = 1;
                    moby[0x20] = 0xB;
                    data[0x15C] = 0xD;
                    data[0x15D] = 0xE;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case SENTRYBOT_RECOVERABLE_KNOCKBACK: {
        int knockback_fall_flags = mine_fall(moby, data + 0x70);
        if (knockback_fall_flags & 0x40) {
            probe_ground_height(moby + 0x10, 0, 0.5f);
            if (collision_hit_material() == -1 && *(float *)(data + 0x168) - 5.0f < *(float *)(moby + 0x18)) {
                if (hero.state.current == 0x65) {
                    moby[0x20] = 0xC;
                    if (moby[0x53] != 0)
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                } else {
                    moby[0xBC] = 1;
                    if (data[0x15F] != 0) {
                        moby[0x20] = 0x10;
                        if (moby[0x53] != 3)
                            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                    } else {
                        data[0x15F] = 1;
                        moby[0x20] = 0xB;
                        data[0x15C] = 0x10;
                        data[0x15D] = 3;
                        if (moby[0x53] != 1)
                            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    }
                }
                return;
            }
            if (*(int *)(data + 0x12C) != -1) {
                mark_moby_killed_and_release_owned_voice(D_L15_0015FFD8_m + (*(int *)(data + 0x12C) << 8));
                *(int *)(data + 0x12C) = -1;
            }
            if (*(int *)(data + 0x150) != -1) {
                mark_moby_killed_and_release_owned_voice(D_L15_0015FFD8_m + (*(int *)(data + 0x150) << 8));
                *(int *)(data + 0x150) = -1;
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
        if (mine_fall(moby, data + 0x70) & 0x160) {
            if (*(int *)(data + 0x12C) != -1) {
                mark_moby_killed_and_release_owned_voice(D_L15_0015FFD8_m + (*(int *)(data + 0x12C) << 8));
                *(int *)(data + 0x12C) = -1;
            }
            if (*(int *)(data + 0x150) != -1) {
                mark_moby_killed_and_release_owned_voice(D_L15_0015FFD8_m + (*(int *)(data + 0x150) << 8));
                *(int *)(data + 0x150) = -1;
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
    if (*(short *)(data + 0x156) != 0) {
        lvl = 2;
    } else if (*(int *)(data + 0x114) != 2) {
        lvl = 2;
    } else {
        lvl = *(short *)(data + 0x158) != 0;
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
    color_pulse_sine = fast_sin((float)(D_L15_0015F5CC % color_pulse_period_frames) / convert_integer_to_float(color_pulse_period_frames) * 2.0f * 3.1415927f +
                 -3.1415927f);
    {
        int color_pulse_red_offset = func_001FA898_r(color_pulse_sine * 20.0f);
        int color_pulse_green_offset = func_001FA898_r(color_pulse_sine * 70.0f);
        int color_pulse_blue_offset = func_001FA898_r(color_pulse_sine * 10.0f);
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
    *(int *)(moby + 0x90) = fast_tween_color(*(int *)(moby + 0x90), color, 0.1f);
    {
        unsigned char *effect_position = data + 0x2A0;
        int i;
        for (i = 0; i < 3; i++) {
            moby_point_position(moby, i + 4, effect_position);
            effect_position += 0x10;
        }
    }
    enqueue_callback_list_1((void (*)(void))draw_sentrybot_effect_quads, moby);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AA60), where it is exact; names translated to the US level program. */

extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
void blend_moby_animation_c(void *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

int begin_sentrybot_turn_toward_heading(unsigned char *moby, char *moby_data, int next_state, int next_animation, float angle) {
    float heading_delta = -fast_subtract_rotations(*(float *)(moby + 0x48), angle);
    float stored_heading_delta;
    int turn_required = 1;
    *(int *)(moby_data + 0x144) = 0;
    *(float *)(moby_data + 0x170) = *(float *)(moby + 0x48);
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
            blend_moby_animation_c(moby, 0xC, 0, scale_game_frames(10));
        }

    } else if (heading_delta < -0.7853982f) {
        moby[0x20] = 6;
        if (moby[0x53] != 0xA) {
            blend_moby_animation_c(moby, 0xA, 0, scale_game_frames(10));
        }

    } else if (heading_delta > 0.7853982f) {
        moby[0x20] = 7;
        if (moby[0x53] != 0xB) {
            blend_moby_animation_c(moby, 0xB, 0, scale_game_frames(10));
        }

    } else {
        moby[0x20] = next_state;
        if (moby[0x53] != next_animation) {
            blend_moby_animation_c(moby, next_animation, 0, scale_game_frames(10));
        }
        turn_required = 0;
    }
    return turn_required;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AC48), where it is exact; names translated to the US level program. */

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_L15_00299880_u(void *, void *, void *, void *, float) __asm__("FUN_L15_00299880");

/* Aims at point (heading from the moby) and passes it on to FUN_L15_00299880. */
void begin_sentrybot_turn_toward_point(char *moby, void *moby_data, float *point, void *next_state, void *next_animation) {
    float angle =
        fast_arc_tan(point[0] - *(float *)(moby + 0x10), point[1] - *(float *)(moby + 0x14));
    FUN_L15_00299880_u(moby, moby_data, next_state, next_animation, angle);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029ACC0), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float FUN_001f9b80(float *, float *);
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern void FUN_L00_00258830(int, int, float, float, float, int);

int move_sentrybot_toward_point(char *moby, float *point, float target_forward_step) {
    float *fp = (float *)(moby + 0x48);
    char *moby_data = *(char **)(moby + 0x78);
    float movement_delta[4];
    float target_heading;
    float movement_target_heading;
    target_heading = fast_arc_tan(point[0] - *(float *)(moby + 0x10), point[1] - *(float *)(moby + 0x14));
    approach_angle(fp, (float *)(moby_data + 0x138), target_heading, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f,
                     frame_time * 6.981317f);
    movement_target_heading = fast_arc_tan(point[0] - *(float *)(moby + 0x10), point[1] - *(float *)(moby + 0x14));
    if (fast_difference_between_rotations(*(float *)(moby + 0x48), movement_target_heading) < 0.7853982f) {
        approach_value(moby_data + 0x134, target_forward_step, frame_time_sq * 9.0f);
    } else {
        approach_value(moby_data + 0x134, 0.0f, frame_time_sq * 9.0f);
    }
    movement_delta[0] = fast_cos(*(float *)(moby + 0x48)) * *(float *)(moby_data + 0x134);
    movement_delta[1] = fast_sin(*(float *)(moby + 0x48)) * *(float *)(moby_data + 0x134);
    movement_delta[2] = -(frame_time * 0.5f);
    FUN_L00_00258830((int)moby, (int)movement_delta, 0.5f, 0.5f, 0.0f, 0);
    return FUN_001f9b80((float *)(moby + 0x10), point) < 0.2f;
}

#define NOT_SDA

#define MACRO_ADDR

/* checks that no closer live object blocks the line to the target */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AE80), where it is exact; names translated to the US level program. */

extern float FUN_001f9b80(float *, float *);
extern int D_L15_0015FFD8_m __asm__("D_L15_0015FFD8");
extern int D_L15_001ABE40[];

int can_sentrybot_approach_linked_moby(void *mm) __asm__("FUN_L15_00299ca0");

int can_sentrybot_approach_linked_moby(void *mm) {
    char *moby = mm;
    int linked_moby_index = *(int *)(*(char **)(moby + 0x78) + 0x128);
    char *linked_moby;
    float linked_moby_distance_xy;
    short *group_moby_entry;
    if (linked_moby_index == -1)
        return 0;
    linked_moby = (char *)D_L15_0015FFD8_m + (linked_moby_index << 8);
    if (*(int *)(*(char **)(linked_moby + 0x78) + 8) != 0)
        return 0;
    if (((unsigned char *)moby)[0x21] == 0xFF)
        return 1;
    linked_moby_distance_xy = FUN_001f9b80((float *)(moby + 0x10), (float *)(linked_moby + 0x10));
    group_moby_entry = (short *)D_L15_001ABE40[((unsigned char *)moby)[0x21]];
    if (group_moby_entry == 0)
        return 1;
    do {
        char *listed_moby = (char *)(((*(unsigned short *)group_moby_entry & 0x7FFF) << 8) + D_L15_0015FFD8_m);
        if (((unsigned char *)listed_moby)[0x20] == 0xE)
            return 0;
        if (((unsigned char *)listed_moby)[0xBC] == 1) {
            int j = *(int *)(*(char **)(listed_moby + 0x78) + 0x128);
            if (j != -1) {
                if (FUN_001f9b80((float *)(listed_moby + 0x10),
                                 (float *)((char *)D_L15_0015FFD8_m + (j << 8) + 0x10)) < linked_moby_distance_xy)
                    return 0;
            }
        }
    } while (*group_moby_entry++ >= 0);
    return 1;
}
#include "sda.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029AFB8), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);
extern int D_L15_0015FFD8_m __asm__("D_L15_0015FFD8");
extern int D_L15_001ABE40[];
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");

int find_sentrybot_attention_target(char *moby) {
    char *moby_data = *(char **)(moby + 0x78);
    int best = 0;
    float bd;
    float line_start[4];
    float candidate_line_endpoint[4];
    short *group_moby_entry;
    int moby_group_index = *(int *)(moby_data + 0x17C);
    if (moby_group_index == -1)
        return 0;
    group_moby_entry = (short *)D_L15_001ABE40[moby_group_index];
    if (group_moby_entry == 0)
        return 0;
    qcopy(line_start, moby + 0x10);
    line_start[2] += 1.0f;
    bd = 0.0f;
    do {
        int id = *(unsigned short *)group_moby_entry & 0x7FFF;
        char *candidate_moby = (char *)((id << 8) + D_L15_0015FFD8_m);
        if (*(short *)(candidate_moby + 0xA6) == 0x27B && *(unsigned char *)(candidate_moby + 0x20) >= 6 &&
            *(unsigned char *)(candidate_moby + 0x20) < 0x10) {
            float f = fast_difference_between_rotations(
                *(float *)(moby + 0x48), fast_arc_tan(*(float *)(candidate_moby + 0x10) - *(float *)(moby + 0x10),
                                                   *(float *)(candidate_moby + 0x14) - *(float *)(moby + 0x14)));
            unsigned char candidate_state = ((char *)((id << 8) + D_L15_0015FFD8_m))[0x20];
            if (candidate_state == 7 || candidate_state == 0xF)
                *(short *)(moby_data + 0x194) = 1;
            if (*(short *)(moby_data + 0x194) != 0 || f < 1.2217305f) {
                qcopy(candidate_line_endpoint, (char *)((id << 8) + D_L15_0015FFD8_m) + 0x10);
                line_start[2] += 1.0f;
                if (collision_line(line_start, candidate_line_endpoint, 2, 0, 0) == 0) {
                    float candidate_line_distance = FUN_001f9b48(line_start, candidate_line_endpoint);
                    if (best == 0 || candidate_line_distance < bd) {
                        bd = candidate_line_distance;
                        best = D_L15_0015FFD8_m + (id << 8);
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

extern int D_L15_0015FFD8;
extern int D_L15_001ABE40[];

int can_sentrybot_advance_patrol(void *input_moby) {
    unsigned char *moby = input_moby;
    unsigned short *group_moby_entry;
    char *moby_data;
    if (moby[0x21] == 0xFF)
        return 1;
    group_moby_entry = (unsigned short *)D_L15_001ABE40[moby[0x21]];
    moby_data = *(char **)(moby + 0x78);
    if (group_moby_entry == 0)
        return 1;
    do {
        unsigned char *listed_moby = (unsigned char *)(((*group_moby_entry & 0x7FFF) << 8) + D_L15_0015FFD8);
        if (*(short *)(listed_moby + 0xA6) == 0x2C && listed_moby != 0 && listed_moby[0x20] != 0xFE && listed_moby[0x20] != 0xFD && listed_moby[0xBC] == 0) {
            char *od = *(char **)(listed_moby + 0x78);
            if (*(int *)(od + 0x144) == 0 && *(int *)(od + 0x124) != -1 &&
                *(int *)(od + 0x13C) >= *(int *)(moby_data + 0x13C))
                return 0;
        }
    } while ((short)*group_moby_entry++ >= 0);
    return 1;
}
/* Wakes every linked switch that can see this moby, or rearms the timer when there is no list. */
extern int D_001413D4_a0[] __asm__("D_001413D4");
extern short *D_L15_001ABE40_a0[] __asm__("D_L15_001ABE40");
extern int D_L15_0015FFD8_a0 __asm__("D_L15_0015FFD8");
extern int FUN_001f96f8_a0(int) __asm__("FUN_001f96f8");
extern int FUN_001efa68_a0(void *, void *, int, int, int) __asm__("FUN_001efa68");
void set_sentrybot_attention_target(char *source_moby, char *recipient_moby) __asm__("FUN_L15_0029a248");

extern void set_sentrybot_attention_target(char *, char *) __asm__("FUN_L15_0029a248");

typedef struct { char p[0x156]; short h156; } D_a0d0;

void alert_visible_group_sentrybots(void *mp) {
    unsigned char *source_moby = mp;
    char *moby_data = *(char **)(source_moby + 0x78);
    float source_line_endpoint[4];
    float listed_line_endpoint[4];
    short *group_moby_entry;
    int listed_moby_byte_offset;
    char *listed_moby;
    if (D_001413D4_a0[0] == 0x65) {
        *(short *)(moby_data + 0x156) = 0;
        return;
    }
    if (source_moby[0x21] == 0xFF) {
        *(short *)(moby_data + 0x156) = FUN_001f96f8_a0(0xB4);
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
        listed_moby = (char *)(listed_moby_byte_offset + D_L15_0015FFD8_a0);
        if (listed_moby[0x20] >= 0 && *(short *)(listed_moby + 0xA6) == 0x2C) {
            qcopy(listed_line_endpoint, listed_moby + 0x10);
            listed_line_endpoint[2] += 1.0f;
            if (FUN_001efa68_a0(listed_line_endpoint, source_line_endpoint, 2, 0, 0) == 0) {
                moby_data = *(char **)((char *)(listed_moby_byte_offset + D_L15_0015FFD8_a0) + 0x78);
                ((D_a0d0 *)moby_data)->h156 = FUN_001f96f8_a0(300);
                moby_data[0x15B] = 0;
                set_sentrybot_attention_target((char *)source_moby, (char *)(D_L15_0015FFD8_a0 + listed_moby_byte_offset));
            }
        }
    } while (*group_moby_entry++ >= 0);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029B428), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

void set_sentrybot_attention_target(char *source_moby, char *recipient_moby) {
    char *recipient_moby_data;
    if (source_moby != recipient_moby) {
        recipient_moby_data = *(char **)(recipient_moby + 0x78);
        if (*(short *)(recipient_moby_data + 0x19C) == 0 || random_integer_below(0x10) == 0) {
            *(char **)(recipient_moby_data + 0x198) = source_moby;
            *(short *)(recipient_moby_data + 0x19C) = scale_game_frames(0xB4);
        }
    }
}
/* Turns the turret head toward its current target (or remembered point), clamping yaw and pitch, and eases both joints. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029B488), where it is exact; names translated to the US level program. */

extern f32 vector_length_xy_c(void *) __asm__("FUN_001f9b20");
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");

void update_sentrybot_joint_aim(void *mv) {
    char *moby = mv;
    char *moby_data = *(char **)(moby + 0x78);
    float target_position[4];
    float aim_origin[4];
    float target_displacement[4];
    float k1 = 0.02f;
    float k2 = 0.3f;
    float yaw, pitch;
    int have = 0;
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
        subtract_vector_xyz_c(target_displacement, target_position, aim_origin);
        yaw = fast_subtract_rotations(fast_arc_tan(target_displacement[0], target_displacement[1]), *(float *)(moby + 0x48));
        pitch = -fast_arc_tan(vector_length_xy_c(target_displacement), target_displacement[2]);
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
        update_joint_animation(moby, moby_data + 0x1A0, 1, k1 * cached_frame_scale_sq, k2 * cached_frame_scale_sq);
    }
    update_joint_animation(moby, moby_data + 0x220, 0, k1 * frame_scale_sq, k2 * frame_scale_sq);
}
#include "rnc/overlay/shared/gameplay/entities/sentrybot_globals.h"
extern float sentrybot_class_scale_multiplier_gp __asm__("D_L15_001613EC") __attribute__((sda));
#include "rnc/overlay/shared/gameplay/entities/sentrybot_data/update_sentrybot_hit_response_and_target_visibility.h"
extern char D_001413F4[];
extern short D_0013F658[];
extern s32 D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;
extern char D_0013F420[];
extern char *FUN_L00_0025a420(void *, int, int);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float FUN_001f99c0(float);
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int update_float_countdown(void *) __asm__("FUN_L08_00211b08");
extern int convert_float_to_int_a570(float) __asm__("FUN_001fa6d0");
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int FUN_L00_0025ff38(void *, void *, float, int, int, void *, int);

void update_sentrybot_hit_response_and_target_visibility(void *object, int flag) {
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
    unsigned char *moby = object;
    unsigned char *data = *(unsigned char **)(moby + 0x78);
    char *hit;
    float impact;
    int kind;
    int code;
    float angle;
    float fall_angle;
    float speed;
    int amount;
    float hit_vector_or_sight_line_origin[4];
    int *path;
    int group;
    unsigned char seen;

    if (moby[0x20] == 0)
        return;
    *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * sentrybot_class_scale_multiplier_gp;
    impact = 0.0f;
    hit = FUN_L00_0025a420(moby, 0x330000, 0);
    code = fun_00213928(moby, hit, data + 0x20, 0, &kind, &impact, 0, 4);
    if (kind != 1 && moby[0x20] != 0x17) {
        if ((unsigned char)D_001413F4[0] == 0)
            alert_visible_group_sentrybots(moby);
        *(float *)(data + 0x20) -= impact;
        if (*(short *)(*(char **)(hit + 0x20) + 0xA6) == 0x47)
            code = 3;
        if (*(float *)(data + 0x20) <= 0.0f)
            code = 1;
        amount = convert_float_to_int_a570(512.0f);
        speed = sentrybot_knockback_gravity_gp * frame_time_sq;
        *(int *)(data + 0x90) = amount;
        *(float *)(data + 0x98) = 0.5f;
        *(int *)(data + 0x94) = 9;
        *(float *)(data + 0x80) = speed;
        data[0xAD] = 0;
        switch (code) {
        case SENTRYBOT_NO_KNOCKBACK_SETUP_RESPONSE:
        case SENTRYBOT_NO_KNOCKBACK_START_RESPONSE:
            break;
        case SENTRYBOT_FIRST_CHANNEL_BLEND_HIT_RESPONSE:
        case SENTRYBOT_FIRST_CHANNEL_BLEND_TARGET_HIT_RESPONSE:
            data[0x67] = 0xFA;
            break;
        case NONLETHAL_KNOCKBACK_HIT_RESPONSE:
        case NONLETHAL_ARC_HIT_RESPONSE:
        case NONLETHAL_ARC_KNOCKBACK_HIT_RESPONSE:
        case NONLETHAL_FALL_HIT_RESPONSE:
        case SENTRYBOT_RECOVERABLE_KNOCKBACK_RESPONSE:
        case SENTRYBOT_RECOVERABLE_ARC_KNOCKBACK_RESPONSE:
            {
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
                *(float *)(data + 0x80) = fall_speed;
                *(float *)(data + 0x88) = forward;
                *(float *)(data + 0x8C) = lateral;
                *(float *)(data + 0x84) = vz;
                *(float *)(data + 0xC0) = 5.0f;
                *(float *)(data + 0xC4) = 10.0f;
            }
            {
                float heading = fast_arc_tan(*(float *)(moby + 0x10) - *(float *)(*(char **)(hit + 0x20) + 0x10),
                                              *(float *)(moby + 0x14) - *(float *)(*(char **)(hit + 0x20) + 0x14));
                *(u128 *)hit_vector_or_sight_line_origin = *(u128 *)(hit + 0x10);
                angle = heading;
            }
            trooper_knockback_arc(hit_vector_or_sight_line_origin, &angle, data + 0x88, data + 0x8C);
            trooper_start_knockback(moby, data + 0x70, 7, 1, 0, angle);
            moby[0x20] = 0x15;
            moby[0xBC] = 2;
            data[0x67] = 0x78;
            break;
        case LETHAL_HIT_RESPONSE:
        case DEATH_KNOCKBACK_HIT_RESPONSE:
            {
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
                *(unsigned short *)(moby + 0x34) &= 0xEFFF;
                *(float *)(data + 0x80) = fall_speed;
                *(float *)(data + 0x88) = forward;
                *(float *)(data + 0x8C) = lateral;
                *(float *)(data + 0x84) = vz;
                *(float *)(data + 0xC0) = 8.0f;
                *(float *)(data + 0xC4) = 16.0f;
            }
            {
                float heading = fast_arc_tan(*(float *)(moby + 0x10) - *(float *)(*(char **)(hit + 0x20) + 0x10),
                                              *(float *)(moby + 0x14) - *(float *)(*(char **)(hit + 0x20) + 0x14));
                *(u128 *)hit_vector_or_sight_line_origin = *(u128 *)(hit + 0x10);
                fall_angle = heading;
            }
            trooper_knockback_arc(hit_vector_or_sight_line_origin, &fall_angle, data + 0x88, data + 0x8C);
            trooper_start_knockback(moby, data + 0x70, 8, 1, 0, fall_angle);
            moby[0x20] = 0x17;
            moby[0xBC] = 2;
            data[0x67] = 0xF0;
            break;
        }
        trooper_anim_update(moby, data + 0x60);
    }
    moby[0xA4] = 0xFF;
    trooper_anim_finish(moby, data + 0x60);
    if (flag != 0)
        return;
    if (*(int *)(data + 0x38) != 0) {
        float value;
        int rounded;
        value = random_float_between(180.0f, 240.0f);
        rounded = convert_float_to_int_a570(multiply_global_factor_ed64(value));
        *(int *)(data + 0x38) = 0;
        *(float *)(data + 0x130) = (float)rounded;
    }
    update_float_countdown(data + 0x130);
    if (FUN_001f9b48(moby + 0x10, D_0013F420) < 64.0f) {
        group = 0;
        qcopy(hit_vector_or_sight_line_origin, moby + 0x10);
        hit_vector_or_sight_line_origin[2] += 1.0f;
        if (*(int *)(data + 0x12C) != -1)
            group = D_L15_0015FFD8_m + (*(int *)(data + 0x12C) << 8);
        data[0x15A] = collision_line(hit_vector_or_sight_line_origin, D_0013F420, 2, (void *)group, 0) == 0;
    } else {
        data[0x15A] = 0;
    }
    path = D_L15_001B0AB0[*(int *)(data + 0x120)];
    if (FUN_L00_0025ff38(moby, data + 0xD0, 128.0f, 0, 0, path + 4, *path) != 2) {
        float target_angle;
        seen = 0;
        target_angle = fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(moby + 0x10),
                              *(float *)(data + 0xD4) - *(float *)(moby + 0x14));
        if ((fast_difference_between_rotations(*(float *)(moby + 0x48), target_angle) < 1.5707964f &&
             (D_0013F658[0] == 0 || FUN_001f99c0(*(float *)(moby + 0x18) - *(float *)(data + 0xD8)) < 4.0f)) ||
            moby[0x20] == 0x10 || moby[0x20] == 0x11) {
            seen = data[0x15A];
        }
        if (*(int *)(data + 0x114) == 1)
            data[0x15E] = data[0x15B] + 1;
        else if ((unsigned char)D_001413F4[0] != 3)
            data[0x15E] = 0;
        else if (data[0x15E] >= 2)
            data[0x15B] = 1;
        data[0x19F] = 1;
        if (seen == 0) {
            if (data[0x15A] == 0)
                data[0x15B] = 1;
            data[0x19F] = 0;
            *(int *)(data + 0x114) = 2;
            *(short *)(data + 0x158) = 0;
        } else if (((*(short *)(data + 0x156) == 0 || data[0x15B] != 0) &&
                    (unsigned char)D_001413F4[0] == 3) ||
                   (data[0x15E] != 0 && *(int *)(data + 0x114) != 1)) {
            *(int *)(data + 0x114) = 2;
            *(short *)(data + 0x158) = 1;
        } else if ((unsigned char)D_001413F4[0] != 3 && *(int *)(data + 0x114) != 2) {
            data[0x15B] = 0;
        }
        if (D_L15_0015F5C4 == 2)
            *(int *)(data + 0x114) = 2;
    } else {
        data[0x19F] = 0;
        data[0x15B] = 1;
        *(short *)(data + 0x158) = 0;
    }
    if (*(int *)(data + 0x110) == 0)
        *(int *)(data + 0x110) = (*(int *)&D_001413D0_lui);
}
#define NOT_SDA

#define MACRO_ADDR

/* Applies the same effect to three consecutive 16-byte slots of a moby's data, tagged with its colour word. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029BD88), where it is exact; names translated to the US level program. */

extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_sentrybot_effect_quads(char *moby) {
    int effect_color = (*(int *)(moby + 0x90) & 0xFFFFFF) | 0x30000000;
    char *effect_position = *(char **)(moby + 0x78) + 0x2A0;
    int i;
    for (i = 2; i >= 0; i--) {
        draw_offset_textured_effect_quad(effect_position, effect_color, 0.2f, 0.08f);
        effect_position += 0x10;
    }
}
/* Flies the moby along its velocity, spinning; it dies with a sound when it hits something or times out. */
#include "qcopy.h"
#include "rnc/overlay/quad.h"
extern char D_L15_001673C0_y[] __asm__("D_L15_001673C0");
extern int D_L15_00174458_y[] __asm__("D_L15_00174458");
extern float D_0015ED6C;
extern float dist_y(void *, void *) __asm__("FUN_001f9b48");
extern int sound_y(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern float addrot_y(float, float) __asm__("FUN_001fa580");
extern void add_y(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_y(void *, void *, float) __asm__("FUN_001f9a68");
extern int randbelow_y(int) __asm__("FUN_00213260");
extern void setupcol_y(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern int collide_y(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int tick_y(void *) __asm__("FUN_001f9740");
extern void remove_y(void *) __asm__("FUN_0020c828");
void spawn_rocket_trail_spark(char *moby) __asm__("FUN_L15_0029ae18");

void spawn_rocket_trail_spark(char *moby) __asm__("FUN_L15_0029ae18");
void spawn_rocket_detonation_glow(char *moby) __asm__("FUN_L15_0029af88");

void spawn_rocket_detonation_glow(char *moby) __asm__("FUN_L15_0029af88");

void rocket_update(unsigned char *m) __asm__("FUN_L15_0029ac30");
void rocket_update(unsigned char *moby) {
    char *rocket_data = *(char **)(moby + 0x78);
    float next[4];
    float back[4];
    char col[0x30];
    float collision_setup_value;
    if (*(int *)(rocket_data + 0x10) == 0) {
        if (dist_y(moby + 0x10, D_L15_001673C0_y) < 15.0f) {
            sound_y(9, 0, moby, 0x27E);
            *(int *)(rocket_data + 0x10) = 1;
        }
    }
    if (moby[0x20] != 0)
        return;
    collision_setup_value = 1.0f;
    *(float *)(moby + 0x40) = addrot_y(*(float *)(moby + 0x40), D_0015ED6C * 6.2831855f);
    add_y(next, moby + 0x10, rocket_data);
    scale_y(back, rocket_data, -2.0f);
    add_y(back, back, moby + 0x10);
    if (hero.vehicle_mode == 2 && randbelow_y(5) != 0)
        collision_setup_value = 0.0f;
    setupcol_y(col, moby, 0x10003, collision_setup_value, rocket_data);
    *(short *)(col + 0x1A) = *(short *)(moby + 0xA6);
    if (collide_y(back, next, 0x10, moby, col) != 0 || tick_y(rocket_data + 0x14) != 0) {
        if (D_L15_00174458_y[0] != *(int *)(rocket_data + 0x18)) {
            sound_y(8, 0, moby, 0x27E);
            spawn_rocket_detonation_glow((char *)moby);
            remove_y(moby);
            return;
        }
    }
    qcopy(moby + 0x10, next);
    spawn_rocket_trail_spark((char *)moby);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029BFF8), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float D_0015ED6C;
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern s32 truncate_float_to_s32_u(f32) __asm__("FUN_001fa6d0");
extern int rocket_trail_spark_initial_configuration_word_gp __asm__("D_L15_00161418") __attribute__((sda));
extern float rocket_trail_spark_frame_count_range_start_gp __asm__("D_L15_00161428") __attribute__((sda));
extern float rocket_trail_spark_third_frame_count_range_start_gp __asm__("D_L15_00161438") __attribute__((sda));
extern float rocket_trail_spark_first_velocity_scale_gp __asm__("D_L15_00161448") __attribute__((sda));
extern int rocket_trail_spark_second_configuration_word_gp __asm__("D_L15_0016141C") __attribute__((sda));
#include "rnc/overlay/shared/gameplay/entities/sentrybot_data/spawn_rocket_trail_spark.h"
extern float rocket_trail_spark_frame_count_range_end_gp __asm__("D_L15_0016142C") __attribute__((sda));
extern float rocket_trail_spark_second_frame_count_range_start_gp __asm__("D_L15_00161430") __attribute__((sda));
extern float rocket_trail_spark_second_frame_count_range_end_gp __asm__("D_L15_00161434") __attribute__((sda));
extern float rocket_trail_spark_third_frame_count_range_end_gp __asm__("D_L15_0016143C") __attribute__((sda));
extern float rocket_trail_spark_first_noise_radius_factor_gp __asm__("D_L15_00161440") __attribute__((sda));
extern float rocket_trail_spark_second_noise_radius_factor_gp __asm__("D_L15_00161444") __attribute__((sda));
extern float rocket_trail_spark_second_velocity_scale_gp __asm__("D_L15_0016144C") __attribute__((sda));
extern int rocket_trail_spark_lookup_index_and_flags_gp __asm__("D_L15_00161454") __attribute__((sda));
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void spawn_rocket_trail_spark(char *moby) {
    float noise[4];
    float spark_a[4];
    float spark_b[4];
    char *data = *(char **)(moby + 0x78);
    float noise_radius;
    int frame_count, second_frame_count, third_frame_count;
    noise_radius = rocket_trail_spark_first_noise_radius_factor_gp;
    noise_radius *= D_0015ED6C;
    scale_vector_xyz(spark_a, data, rocket_trail_spark_first_velocity_scale_gp);
    random_vector(noise, noise_radius, noise_radius);
    add_vector_xyz(spark_a, spark_a, noise);
    noise_radius = rocket_trail_spark_second_noise_radius_factor_gp;
    noise_radius *= D_0015ED6C;
    scale_vector_xyz(spark_b, data, rocket_trail_spark_second_velocity_scale_gp);
    random_vector(noise, noise_radius, noise_radius);
    add_vector_xyz(spark_b, spark_b, noise);
    spark_a[3] = rocket_trail_spark_first_vector_fourth_component_gp;
    spark_b[3] = rocket_trail_spark_second_vector_fourth_component_gp;
    frame_count = func_001FA898_r(multiply_global_factor_ed64(
        random_float_between_alt(rocket_trail_spark_frame_count_range_start_gp, rocket_trail_spark_frame_count_range_end_gp)));
    second_frame_count = func_001FA898_r(multiply_global_factor_ed64(
        random_float_between_alt(rocket_trail_spark_second_frame_count_range_start_gp, rocket_trail_spark_second_frame_count_range_end_gp)));
    third_frame_count = func_001FA898_r(multiply_global_factor_ed64(
        random_float_between_alt(rocket_trail_spark_third_frame_count_range_start_gp, rocket_trail_spark_third_frame_count_range_end_gp)));
    spawn_spark(moby + 0x10, spark_a, spark_b, rocket_trail_spark_initial_configuration_word_gp, rocket_trail_spark_second_configuration_word_gp, frame_count, second_frame_count, third_frame_count,
                 rocket_trail_spark_lookup_index_and_flags_gp);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029C168), where it is exact; names translated to the US level program. */

extern float D_L15_0015F580[] __attribute__((section(".sdata")));
extern void spawn_glow_particle(char *, char *, int, int, float, int, int, int, int) __asm__("FUN_L00_002ac910");

extern float rocket_detonation_glow_scale_gp __asm__("D_L15_00161458") __attribute__((sda));
extern int rocket_detonation_glow_lifetime_frames_gp __asm__("D_L15_0016145C") __attribute__((sda));
extern unsigned char D_L15_00161460 __attribute__((sda));
extern unsigned char D_L15_00161464 __attribute__((sda));
extern unsigned char D_L15_00161468 __attribute__((sda));
extern unsigned char D_L15_0016146C __attribute__((sda));
void spawn_rocket_detonation_glow(char *moby) {
    char *glow_spawn_position = moby + 0x10;
    int scaled_frame_count = scale_game_frames(rocket_detonation_glow_lifetime_frames_gp);
    spawn_glow_particle(moby, glow_spawn_position, (int)&D_L15_0015F580, scaled_frame_count, rocket_detonation_glow_scale_gp,
                     D_L15_00161464, D_L15_00161468, D_L15_0016146C, D_L15_00161460);
}
/* update of moby class 256: a homing missile that leads its target, trails smoke and explodes on contact */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029E8A0), where it is exact; names translated to the US level program. */

typedef int u128_29E8A0 __attribute__((mode(TI)));

typedef struct { int w[20]; } __attribute__((aligned(16))) Tbl_29E8A0;

typedef struct { int c[6]; } Cols_29E8A0;

typedef struct { char pad0[2]; unsigned char f2; unsigned char f3; } Part_29E8A0;

typedef struct {
    float v[4];
    char *moby;
    int flags;
    char kind;
    char one;
    unsigned short id;
    float radius;
    int count;
} Coll_29E8A0;

typedef struct {
    float v[4];
    char *owner;
    int timer;
    struct M_29E8A0 *target;
    float zoff;
    float speed;
    float yawvel;
    float pitchvel;
    float yaw0;
    float pitch0;
    float speed_end;
    int f38;
    short f3C;
    short f3E;
} D_29E8A0;

typedef struct M_29E8A0 {
    char pad0[0x10];
    float pos[4];
    signed char state;
    char pad21[0x1F];
    float rot[4];
    char pad50[0x28];
    D_29E8A0 *data;
    char pad7C[0x2A];
    unsigned short id;
    char padA8[0x14];
    unsigned char bc;
    char padBD[3];
    float mtx[16];
} M_29E8A0;

extern Cols_29E8A0 D_L15_001E2660;
extern Cols_29E8A0 D_L15_001E2678;
extern Tbl_29E8A0 D_L15_001E2610;
extern int D_L15_00178500[];
extern char *FUN_L00_002af450(char *, char *, int, int, int);
extern char D_0013E533[];
typedef struct {
    char pad[0x140];
    float position[4];
} LevelView;
extern LevelView D_L15_00167280;
extern char D_L15_00174440[];
extern char D_L15_001B0D70[];
extern f32 convert_integer_to_float_c2(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos_c3(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c3(f32) __asm__("func_001F9DE0");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 vector_length_xy_c2(void *) __asm__("FUN_001f9b20");
extern float D_0015ED64;
extern float D_0015ED6C;
extern float D_0015ED70;
#include "rnc/overlay/shared/gameplay/entities/sentrybot_data/update_target_leading_missile_2.h"
extern float FUN_L00_001ff430(void *);
extern float FUN_L00_00258110(float *vel, float cur, float target, float k, float d, float max);
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(float *, float *) __asm__("FUN_001f9b80");
#include "rnc/overlay/shared/gameplay/entities/sentrybot_data/update_target_leading_missile.h"
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_L00_00257b90(int, int);
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 random_integer_below_c3(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c4(f32) __asm__("func_001FA6D0");
extern unsigned char *D_L15_001B26DC __attribute__((section(".data")));
extern unsigned char *FUN_L00_002712b8(void *pos, float *vec, int s, int a, int col, int n, float x, float y, float z, float w, float pw);
extern void *FUN_L00_0026bb70_c(void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void suck_draw_reticle(void *, int, float, float, void *, int, int, float, int) __asm__("FUN_L00_001edf60");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void FUN_L00_001ff2c0(void *, void *, void *);
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_001ffd48(void *, void *, float);
extern void FUN_L00_001ffe20(void *, void *, void *);
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void FUN_L00_0026a4c8(void *pos, void *vel, int color, int tag, int life, float size);
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, float, int, int);
extern void FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
extern void FUN_L00_002ac910(char *, char *, int, int, float, int, int, int, int);
extern void mine_camera_blast(void *, void *, int, int) __asm__("FUN_L00_002d3838");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void mark_moby_for_removal_c3(void *) __asm__("func_0020C828");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_c(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013F490[];
extern char D_0013F350[];

void update_target_leading_missile(M_29E8A0 *missile_moby) __asm__("FUN_L15_0029d6c0");

void update_target_leading_missile(M_29E8A0 *missile_moby) {
    Tbl_29E8A0 camera_blast_parameters;
    float effect_bias_velocity[4];
    float previous_position[4];
    float burst_drift_velocity[4];
    float burst_rotation_axis[4];
    float trail_offset_or_position[4];
    float missile_movement_delta[4];
    float vB0[4];
    float up[4];
    char *owner;
    D_29E8A0 *missile_data;
    M_29E8A0 *target_moby;
    char *hero;
    float yaw;
    float pitch;
    int k;

    camera_blast_parameters = D_L15_001E2610;
    owner = 0;
    *(u128_29E8A0 *)burst_rotation_axis = 0;
    burst_rotation_axis[2] = D_0015ED6C * 8.0f;
    *(u128_29E8A0 *)burst_drift_velocity = *(u128_29E8A0 *)burst_rotation_axis;
    *(u128_29E8A0 *)burst_rotation_axis = 0;
    burst_rotation_axis[2] = 1.0f;
    missile_data = missile_moby->data;
    target_moby = missile_data->target;
    missile_data->speed = missile_data->speed + (missile_data->speed_end - missile_data->speed) / 10.0f * D_0015ED64;
    fast_dec_timer_s16(&missile_data->f3C);
    {
        float a = convert_integer_to_float_c2(missile_data->f3C);
        float b = convert_integer_to_float_c2(missile_data->f3E);
        fast_vec_scale(up, ((char *)&D_0013F490), a / b);
    }
    {
        float delta[4];
        float dv[4];
        float tp[4];
        float ip[4];
        if (missile_target_reticle_enabled != 0 && target_moby != 0) {
            float scaled_target_distance = fast_vec_dist(D_L15_001673C0, target_moby->pos) / 50.0f;
            qcopy(delta, target_moby->pos);
            delta[2] = delta[2] + missile_data->zoff;
            suck_draw_reticle(missile_moby, 0xFF0FFFFF, absolute_float(0.5f - scaled_target_distance * 0.35f), 0.0f, delta, 0xD, -1, 90.0f, 4);
        }
        if (target_moby != 0 && missile_data->timer < scale_game_frames(0x96) - scale_game_frames(0) && target_moby->state >= 0) {
            float intercept_time = 0.0f;
            qcopy(tp, target_moby->pos);
            tp[2] = tp[2] + missile_data->zoff;
            subtract_vector_xyz_c2(delta, tp, missile_moby->pos);
            if (missile_data->f38 != 0) {
                float a, b, c, intercept_time_plus_root, intercept_time_minus_root, discriminant_sqrt;
                subtract_vector_xyz_c2(dv, tp, missile_data->v);
                FUN_L00_001ff2c0(ip, dv, up);
                qcopy(missile_data->v, tp);
                c = FUN_L00_001ff430(dv);
                b = (dv[0] * delta[0] + dv[1] * delta[1] + dv[2] * delta[2]) * -2.0f;
                a = missile_data->speed * missile_data->speed - c;
                c = -FUN_L00_001ff430(delta);
                discriminant_sqrt = square_root_scalar(b * b - a * 4.0f * c);
                intercept_time_plus_root = (-b + discriminant_sqrt) / (a + a);
                intercept_time_minus_root = (-b - discriminant_sqrt) / (a + a);
                if (intercept_time_plus_root > 0.0f && intercept_time_minus_root > 0.0f) {
                    if (intercept_time_minus_root < intercept_time_plus_root) intercept_time = intercept_time_plus_root;
                    else intercept_time = intercept_time_minus_root;
                } else if (intercept_time_plus_root > 0.0f) {
                    intercept_time = intercept_time_plus_root;
                } else if (intercept_time_minus_root > 0.0f) {
                    intercept_time = intercept_time_minus_root;
                } else {
                    intercept_time = -1.0f;
                }
            }
            if (intercept_time > 0.0f && intercept_time < (float)scale_game_frames(0x96)) {
                fast_vec_scale(ip, dv, intercept_time);
                add_vector_xyz(ip, ip, delta);
                yaw = fast_arc_tan(ip[0], ip[1]);
                pitch = -fast_arc_tan(vector_length_xy_c2(ip), ip[2]);
            } else {
                yaw = fast_arc_tan(delta[0], delta[1]);
                pitch = -fast_arc_tan(vector_length_xy_c2(delta), delta[2]);
            }
        } else {
            if (missile_data->timer < scale_game_frames(0x96) - scale_game_frames(0)) {
                yaw = missile_data->yaw0;
                pitch = missile_data->pitch0;
            } else {
                yaw = missile_moby->rot[2];
                pitch = missile_moby->rot[1];
            }
        }
        if (target_moby != 0) {
            missile_moby->rot[2] = FUN_L00_00258110(&missile_data->yawvel, missile_moby->rot[2], yaw,
                D_0015ED70 * 6.2831855f, D_0015ED70 * 3.1415927f, D_0015ED6C * 6.2831855f);
            missile_moby->rot[1] = FUN_L00_00258110(&missile_data->pitchvel, missile_moby->rot[1], pitch,
                D_0015ED70 * 6.2831855f, D_0015ED70 * 3.1415927f, D_0015ED6C * 6.2831855f);
        } else {
            missile_moby->rot[2] = FUN_L00_00258110(&missile_data->yawvel, missile_moby->rot[2], yaw,
                D_0015ED70 * 6.2831855f, D_0015ED70 * 3.1415927f, D_0015ED6C * 4.712389f);
            missile_moby->rot[1] = FUN_L00_00258110(&missile_data->pitchvel, missile_moby->rot[1], pitch,
                D_0015ED70 * 6.2831855f, D_0015ED70 * 3.1415927f, D_0015ED6C * 4.712389f);
        }
        {
            Part_29E8A0 *trail_particle;
            build_spherical_offset_c(missile_movement_delta, missile_data->speed, missile_moby->rot[2], -missile_moby->rot[1]);
            *(u128_29E8A0 *)dv = 0;
            dv[0] = random_float_between(-1.0f, 1.0f);
            dv[1] = random_float_between(-1.0f, 1.0f);
            dv[2] = random_float_between(-1.0f, 1.0f);
            *(u128_29E8A0 *)delta = *(u128_29E8A0 *)dv;
            fast_vec_normalize(delta, delta, random_float_between(0.1f, 0.2f) * D_0015ED6C);
            fast_vec_normalize(trail_offset_or_position, missile_movement_delta, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
            add_vector_xyz(delta, delta, trail_offset_or_position);
            fast_vec_normalize(trail_offset_or_position, missile_movement_delta, random_float_between(0.0f, 1.0f) * missile_data->speed);
            add_vector_xyz(trail_offset_or_position, trail_offset_or_position, missile_moby->pos);
            trail_particle = (Part_29E8A0 *)FUN_L00_002712b8(trail_offset_or_position, delta, scale_game_frames(0x3C), 0x7F, 0x606060, 3,
                40000.0f, 1000.0f, 1.0f, -0.0002f, 0.0f);
            if (trail_particle != 0) {
                trail_particle->f2 = *D_L15_001B26DC;
                trail_particle->f3 = 0x44;
            }
            FUN_L00_002712b8(missile_moby->pos, delta, scale_game_frames(6), 0x7F, 0xB0B0B0, 3, 40000.0f, 1000.0f, 1.0f, -0.0004f, 0.0f);
            *(u128_29E8A0 *)dv = 0;
            dv[2] = 0.02f;
            transform_vector_by_basis(dv, dv, missile_moby->mtx);
            build_look_at_matrix(dv, dv, missile_moby->mtx, random_angle_radians());
            FUN_L00_0026cbb0(trail_offset_or_position, dv, 0x4F007FFF, 0x1FFFFFFF, scale_game_frames(5), 1, 20000.0f);
            qcopy(previous_position, missile_moby->pos);
            FUN_L00_001ff290(delta, missile_movement_delta, up);
        }
    }
    add_vector_xyz(missile_moby->pos, missile_moby->pos, missile_movement_delta);
    if (missile_moby->pos[0] < 0.0f || missile_moby->pos[1] < 0.0f || missile_moby->pos[2] < 0.0f) {
        mine_spawn_drops(missile_moby, D_L15_001B0D70);
        mark_moby_for_removal_c3(missile_moby);
        return;
    }
    {
        Coll_29E8A0 coll;
        coll.flags = 0x830000;
        coll.radius = 3.0f;
        coll.moby = (char *)missile_moby;
        coll.count = 1;
        qcopy(coll.v, missile_movement_delta);
        set_vector_length_xy(coll.v, coll.v, 1.0f);
        hero = ((char *)&D_0013F350);
        coll.v[3] = 5627.9248f;
        coll.kind = 3;
        coll.id = missile_moby->id;
        coll.v[2] = 1.0f;
        coll.one = 1;
        if (collision_line(previous_position, missile_moby->pos, 0, *(char **)(hero + 0x2080), &coll) != 0) {
            char *hit = D_L15_00174440;
            char *hm = *(char **)(hit + 0x18);
            if (hm != 0) {
                if (hm != *(char **)(hero + 0x2080) && hm != missile_data->owner) {
                    missile_moby->bc = 2;
                    qcopy(missile_moby->pos, hit + 0x20);
                    subtract_vector_xyz_c2(vB0, hit + 0x50, hit + 0x60);
                    owner = (char *)missile_moby;
                    fast_vec_normalize(vB0, vB0, 1.0f);
                    FUN_L00_001ff660(burst_drift_velocity, missile_movement_delta, hit + 0x40);
                    fast_vec_normalize(burst_drift_velocity, burst_drift_velocity, D_0015ED6C + D_0015ED6C);
                    fast_vec_normalize(burst_rotation_axis, hit + 0x40, 1.0f);
                }
            } else if (*(int *)(hit + 0x1C) > 0) {
                missile_moby->bc = 1;
                qcopy(missile_moby->pos, hit + 0x20);
                subtract_vector_xyz_c2(vB0, hit + 0x50, hit + 0x60);
                fast_vec_normalize(vB0, vB0, 1.0f);
                FUN_L00_001ff660(burst_drift_velocity, missile_movement_delta, hit + 0x40);
                fast_vec_normalize(burst_drift_velocity, burst_drift_velocity, D_0015ED6C + D_0015ED6C);
                fast_vec_normalize(burst_rotation_axis, hit + 0x40, 1.0f);
            }
        } else if (tick_countdown_32_alt(&missile_data->timer) != 0 || vector_distance_xy_alt(missile_moby->pos, (float *)(hero + 0x80)) > 260.0f) {
            mine_spawn_drops(missile_moby, D_L15_001B0D70);
            mark_moby_for_removal_c3(missile_moby);
            return;
        }
        if (missile_moby->bc != 0) {
            float burst_position_or_velocity[4];
            float effect_reference_distance;
            char *blast_attenuation_view;
            int i, j;
            int r = collect_mobies_in_sphere(missile_moby->pos, 2.0f, 0x10, missile_moby, 0);
            *(u128_29E8A0 *)burst_position_or_velocity = *(u128_29E8A0 *)missile_moby->pos;
            FUN_L00_0025a9f8(missile_moby, burst_position_or_velocity, ((char * *)D_L15_00178500), r, (int)owner, 0x810000, 3, 1, 3.0f, 1.0f, 1.0f);
            {
            float K[16];
            float K2[16];
            float M3[16];
            float M2[16];
            float M1[16];
            float ny;
            K[4] = -burst_rotation_axis[2];
            K[1] = burst_rotation_axis[2];
            K[0] = 0.0f;
            K[8] = burst_rotation_axis[1];
            ny = -burst_rotation_axis[1];
            K[9] = -burst_rotation_axis[0];
            K[2] = ny;
            K[6] = burst_rotation_axis[0];
            K[12] = 0.0f;
            K[5] = 0.0f;
            K[13] = 0.0f;
            K[10] = 0.0f;
            K[14] = 0.0f;
            K[3] = 0.0f;
            K[7] = 0.0f;
            K[11] = 0.0f;
            K[15] = 0.0f;
            multiply_matrix(K2, K, K);
            for (k = 0; k < 10; k++) {
                if (missile_moby->bc == 1) {
                    float burst_rotation_angle = random_angle_radians();
                    FUN_L00_001ffd48(M1, K2, 1.0f - fast_cos_c3(burst_rotation_angle));
                    FUN_L00_001ffd48(M2, K, fast_sin_c3(burst_rotation_angle));
                    FUN_L00_001ffe20(M2, M2, M1);
                    set_identity_matrix(M1);
                    FUN_L00_001ffe20(M3, M2, M1);
                    transform_vector_c(burst_position_or_velocity, vB0, M3);
                    fast_vec_normalize(burst_position_or_velocity, burst_position_or_velocity, random_float_between(8.5f, 16.5f) * D_0015ED6C);
                } else {
                    float sp = random_float_between(8.5f, 16.5f) * D_0015ED6C;
                    float random_yaw = random_angle_radians();
                    build_spherical_offset_c(burst_position_or_velocity, sp, random_yaw, random_angle_radians());
                }
                burst_position_or_velocity[2] = burst_position_or_velocity[2] + D_0015ED6C * 5.0f;
                add_vector_xyz(burst_position_or_velocity, burst_position_or_velocity, burst_drift_velocity);
                FUN_L00_0026bb70_c(missile_moby->pos, burst_position_or_velocity, 0x4F007FFF, 0x1F00007F, 40000.0f,
                    FUN_L00_00257b90(scale_game_frames(0x3C), scale_game_frames(0x78)), 1, -1, -1);
            }
            {
                float random_bias_direction[4];
                {
                    *(u128_29E8A0 *)random_bias_direction = 0;
                    random_bias_direction[0] = random_float_between(-1.0f, 1.0f);
                    random_bias_direction[1] = random_float_between(-1.0f, 1.0f);
                    random_bias_direction[2] = random_float_between(-1.0f, 1.0f);
                    *(u128_29E8A0 *)burst_position_or_velocity = *(u128_29E8A0 *)random_bias_direction;
                }
            }
            subtract_vector_xyz_c2(effect_bias_velocity, D_L15_001673C0, missile_moby->pos);
            effect_reference_distance = fast_vec_length(effect_bias_velocity);
            effect_bias_velocity[2] = effect_bias_velocity[2] + effect_reference_distance * 0.5f;
            fast_vec_normalize(burst_position_or_velocity, burst_position_or_velocity, effect_reference_distance / 5.0f * D_0015ED6C);
            fast_vec_normalize(effect_bias_velocity, effect_bias_velocity, (effect_reference_distance + effect_reference_distance) * D_0015ED6C);
            add_vector_xyz(burst_position_or_velocity, burst_position_or_velocity, effect_bias_velocity);
            clamp_vector_length(burst_position_or_velocity, burst_position_or_velocity, D_0015ED6C * 10.0f);
            FUN_L00_002af450((char *)missile_moby->pos, (char *)burst_position_or_velocity, FUN_L00_00257b90(scale_game_frames(0x3C), scale_game_frames(0x5A)), 0, 0);
            {
                int burst_effect_pair_count;
                float under;
                if (effect_reference_distance < 6.0f) {
                    burst_effect_pair_count = truncate_float_to_s32_c4(effect_reference_distance) / 2;
                } else {
                    burst_effect_pair_count = 3;
                }
                under = 0.0f;
                if (effect_reference_distance < 7.0f) under = 7.0f - effect_reference_distance;
                i = 0;
                while (i < burst_effect_pair_count) {
                    float sz = random_float_between(8.0f, 10.0f) * D_0015ED6C - under * D_0015ED6C;
                    Cols_29E8A0 c1 = D_L15_001E2660;
                    Cols_29E8A0 c2 = D_L15_001E2678;
                    int *p1 = &c1.c[random_integer_below_c3(6)];
                    int *p2 = &c2.c[random_integer_below_c3(6)];
                    int colored_burst_effect_frame_count = FUN_L00_00257b90(scale_game_frames(0xF), scale_game_frames(0x14));
                    int colored_burst_effect_secondary_frame_count = FUN_L00_00257b90(scale_game_frames(0x19), scale_game_frames(0x1E));
                    int white_burst_effect_frame_count, white_burst_effect_secondary_frame_count;
                    FUN_L00_0026a9f0(missile_moby->pos, burst_drift_velocity, *p1, *p2, 400000.0f, colored_burst_effect_frame_count, colored_burst_effect_secondary_frame_count, sz, 0, 0);
                    white_burst_effect_frame_count = FUN_L00_00257b90(scale_game_frames(5), scale_game_frames(0xA));
                    white_burst_effect_secondary_frame_count = FUN_L00_00257b90(scale_game_frames(0xF), scale_game_frames(0x14));
                    FUN_L00_0026a9f0(missile_moby->pos, burst_drift_velocity, 0x7FFFFFFF, 0xFFFFFF, 400000.0f, white_burst_effect_frame_count, white_burst_effect_secondary_frame_count, sz * 0.5f, 0, 0);
                    i++;
                }
            }
            for (j = 0; j < 10; j++) {
                Cols_29E8A0 c1 = D_L15_001E2660;
                Cols_29E8A0 c2 = D_L15_001E2678;
                float burst_particle_velocity[4];
                {
                    float random_burst_direction[4];
                    *(u128_29E8A0 *)random_burst_direction = 0;
                    random_burst_direction[0] = random_float_between(-1.0f, 1.0f);
                    random_burst_direction[1] = random_float_between(-1.0f, 1.0f);
                    random_burst_direction[2] = random_float_between(-1.0f, 1.0f);
                    *(u128_29E8A0 *)burst_particle_velocity = *(u128_29E8A0 *)random_burst_direction;
                }
                fast_vec_normalize(burst_particle_velocity, burst_particle_velocity, random_float_between(0.0f, 3.0f) * D_0015ED6C);
                {
                    int *p1 = &c1.c[random_integer_below_c3(6)];
                    int *p2 = &c2.c[random_integer_below_c3(6)];
                    FUN_L00_0026a4c8(missile_moby->pos, burst_particle_velocity, *p1, *p2, FUN_L00_00257b90(scale_game_frames(0x14), scale_game_frames(0x23)), 200000.0f);
                }
            }
            if (missile_distant_gray_glow_gate < 0.95f && 9.0f < effect_reference_distance) {
                ((void (*)(void *, void *, void *, float, int, int, int, int, int))FUN_L00_002ac910)(missile_moby, missile_moby->pos, burst_drift_velocity, 4.0f, scale_game_frames(0xF), 0x7F, 0x7F, 0x7F, 0x20);
                ((void (*)(void *, void *, void *, float, int, int, int, int, int))FUN_L00_002ac910)(missile_moby, missile_moby->pos, burst_drift_velocity, 4.0f, scale_game_frames(0x18), 0x7F, 0x7F, 0x7F, 0x20);
            }
            ((void (*)(void *, void *, void *, float, int, int, int, int, int))FUN_L00_002ac910)(missile_moby, missile_moby->pos, burst_drift_velocity, 4.0f, scale_game_frames(0x14), 0x7F, 0x7F, 0, 0x30);
            ((void (*)(void *, void *, void *, float, int, int, int, int, int))FUN_L00_002ac910)(missile_moby, missile_moby->pos, burst_drift_velocity, 2.0f, scale_game_frames(0x13), 0xFF, 0xFF, 0xFF, 0x20);
            missile_movement_delta[0] = 0.0f;
            blast_attenuation_view = (char *)&D_L15_00167280;
            missile_moby->bc = 0;
            if (effect_reference_distance < 20.0f) {
                *(float *)(blast_attenuation_view + 0x160) = 0.4f - effect_reference_distance * 0.0175f;
            } else {
                *(float *)(blast_attenuation_view + 0x160) = 0.050000012f;
            }
            {
                int sh = scale_game_frames(0x19);
                char *blast_timing_view = (char *)&D_L15_00167280;
                *(int *)(blast_timing_view + 0x168) = sh;
            }
            allocate_voice_for_target_entry(0, 0, (int)missile_moby);
            mine_camera_blast(&camera_blast_parameters, missile_moby->pos, 0, 0);
            mine_spawn_drops(missile_moby, D_L15_001B0D70);
            mark_moby_for_removal_c3(missile_moby);
            }
        }
    }
}
/* update: scale and fade a moby, spawn an effect, delete it when done */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029FCB0), where it is exact; names translated to the US level program. */

extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void play_entity_sound(float, char *, char *, float, float, int, int, int, int) __asm__("FUN_L00_002598b0");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");

void update_scaling_fading_hit_sphere(struct Moby *moby) __asm__("FUN_L15_0029ead0");

void update_scaling_fading_hit_sphere(struct Moby *moby) {
    char *moby_class = moby->pclass;
    char *state = (char *)moby->pvars;
    float half_relative_scale = moby->scale / *(float *)(moby_class + 0x24) * 0.5f;
    char *pos;
    struct Hero *globals;
    half_relative_scale = half_relative_scale + *(float *)state;
    moby->scale = half_relative_scale + half_relative_scale;
    moby->scale = moby->scale * *(float *)(moby_class + 0x24);
    if (*(int *)(state + 8) < scale_game_frames(0x28))
        moby->alpha = *(int *)(state + 8) * 127 / scale_game_frames(0x28);
    globals = &hero;
    pos = &moby->pos;
    play_entity_sound(half_relative_scale, ((char *)globals->moby), pos, (float)func_001FA898_r(*(float *)(state + 0xC)), 1.0f, 0x30000, 0, 1, 0);
    if (tick_countdown_32_alt((int *)(state + 8)))
        mark_moby_for_removal_c(moby);
}
/* update of moby class 1523: charges up a screen fade and a light, then bursts into particles and deletes itself */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029FF90), where it is exact; names translated to the US level program. */

typedef int u128_29FF90 __attribute__((mode(TI)));

typedef struct {
    float vel[4];
    int f10;
    int f14;
    char pad18[0x14];
    short light;
    short active;
} Data29FF90;

typedef struct {
    char pad0[0x10];
    float pos[4];
    unsigned char state;
    char pad21[0x57];
    Data29FF90 *data;
} Moby29FF90;

typedef struct {
    char pad0[0x98];
    float f98;
    char pad9C[0x2008];
    unsigned char f20A4;
} Hero29FF90;

typedef struct {
    float f0;
    float f4;
    char pad8[8];
    float f10;
    float f14;
    float f18;
    float f1C;
} Light29FF90;

typedef struct {
    char pad0[0x10];
    float f10;
    short f14;
    short f16;
    int f18;
} PartSub29FF90;

typedef struct {
    char pad0[0xA];
    short fA;
    float fC;
    char pad10[0x10];
    PartSub29FF90 sub;
} Part29FF90;

typedef struct {
    int on;
    union {
        unsigned int rgba;
        unsigned char b[4];
    } col;
    long reg;
} Fade29FF90;

extern Fade29FF90 D_L15_0015F330 __attribute__((section(".sdata")));
extern Fade29FF90 D_L15_0015F320 __attribute__((section(".sdata")));
extern Light29FF90 D_L15_001808C0[];
extern s32 D_L15_00178500[];
extern char *spawn_mode_controlled_rotating_alpha_fade_particle(void *, void *, int, void *) __asm__("FUN_L00_00274948");
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 fast_cos_c2(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C __attribute__((section(".sdata")));
extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") __attribute__((section(".sdata")));
extern int charged_blast_removal_target __asm__("D_L15_00161484"); /* no foreign declaration */
extern int collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int allocate_packed_color_point_light_slot(float *pos, float radius, float intensity, int color) __asm__("FUN_L00_0023e5e0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below_c2(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c3(f32) __asm__("func_001FA6D0");
extern unsigned char *D_L15_001B26AC __attribute__((section(".data")));
extern void FUN_L00_001ff290(void *, void *, void *);
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
extern void send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void spawn_owner_target_particle(int a0, void *a1, char *a2) __asm__("FUN_L15_0025e2c0");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
unsigned char *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(void *a, void *b, int c, int d, float f, int n, int e, int g, int h) __asm__("FUN_L00_0026bb70");
unsigned char *spawn_source_attached_fading_effect_particle(char *src, int color, int n, int h, float f) __asm__("FUN_L00_0026daa0");
void FUN_L00_00269b70(void *pos, unsigned char red, unsigned char green, unsigned char blue, int target, float a, float b);
void random_spherical_offset(void *out, f32 a, f32 b) __asm__("FUN_00213358");
extern char D_001413F4[];
extern void func_L00_0026AA10_29FF90(void *, unsigned char, unsigned char, unsigned char, float, float, int) __asm__("FUN_L00_00269b70");
extern int func_L00_001F2BE8_29FF90(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern char *func_L00_0026E940_29FF90(float, char *, int, int, int) __asm__("FUN_L00_0026daa0");
extern void *func_L00_0026CA10_29FF90(void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");

extern f32 frame_scale_macro __asm__("D_0015ED60") __attribute__((section(".sdata")));
void update_charged_blast(Moby29FF90 *moby) __asm__("FUN_L15_0029edb0");

void update_charged_blast(Moby29FF90 *moby) {
    enum {
        CHARGING = 0,
        EXPANDING_BLAST = 1
    };
    float random_effect_vector[4];
    float effect_vector_scratch[4];
    float temporary_position[4];
    Data29FF90 *moby_data = moby->data;

    switch (moby->state) {
    case CHARGING:
        if (moby_data->active != 0 && ((Hero29FF90 *)&hero)->f20A4 == 2) {
            float charge_emission_factor;
            int i;

            if (moby_data->f10 == 0) {
                D_L15_0015F330.on = 1;
                D_L15_0015F330.col.rgba = 0xFFFF00;
            }
            moby_data->f10++;
            charge_emission_factor = convert_integer_to_float_c(moby_data->f10) / multiply_global_factor_ed64(30.0f);
            if (charge_emission_factor > 1.0f) charge_emission_factor = 1.0f;
            charge_emission_factor = charge_emission_factor * charge_emission_factor;
            for (i = 0; i < func_001FA898_r(charge_emission_factor * 10.0f); i++) {
                int g;
                int r;
                int rc;

                *(u128_29FF90 *)effect_vector_scratch = 0;
                effect_vector_scratch[0] = random_float_between(-1.0f, 1.0f);
                effect_vector_scratch[1] = random_float_between(-1.0f, 1.0f);
                effect_vector_scratch[2] = random_float_between(-1.0f, 1.0f);
                *(u128_29FF90 *)random_effect_vector = *(u128_29FF90 *)effect_vector_scratch;
                g = random_int_between(0x4F, 0xAF);
                r = random_int_between(0x4F, 0xFF);
                rc = r > 0xFF ? 0xFF : r;
                if (g > 0xFF) g = 0xFF;
                normalize_vector_xyz_c(random_effect_vector, random_effect_vector, random_float_between(20.0f, 40.0f));
                FUN_L00_001ff290(effect_vector_scratch, random_effect_vector, moby->pos);
                spawn_owner_target_particle((0x7F << 24) | (rc << 16) | (g << 8) | 0x4F, random_effect_vector, (char *)moby);
            }
            if (moby_data->f10 < 0x32) {
                int fade_alpha = D_L15_0015F330.col.b[3];

                if (D_L15_0015F330.reg == 0 && fade_alpha == 0xFF) fade_alpha = 0x25;
                D_L15_0015F330.reg = 0x1F00000042L;
                if (fade_alpha < 0x25 - scale_game_frames_alt(1)) fade_alpha = fade_alpha + scale_game_frames_alt(1);
                else fade_alpha = 0x25;
                D_L15_0015F330.col.rgba = (D_L15_0015F330.col.rgba & 0xFFFF00) | (fade_alpha << 24);
            } else {
                D_L15_0015F330.col.rgba = D_L15_0015F330.col.rgba | 0xFF000000;
            }
            if (moby_data->light != -1) {
                float light_charge_fraction;
                int n;

                (D_L15_001808C0 + moby_data->light)->f10 = moby->pos[0];
                (D_L15_001808C0 + moby_data->light)->f14 = moby->pos[1];
                (D_L15_001808C0 + moby_data->light)->f18 = moby->pos[2];
                n = moby_data->f10;
                if (n > 0x32) n = 0x32;
                if (n < 0) n = 0;
                (D_L15_001808C0 + moby_data->light)->f1C = convert_integer_to_float_c(n) * 7.0f / 50.0f;
                light_charge_fraction = convert_integer_to_float_c(moby_data->f10) / multiply_global_factor_ed64(30.0f);
                if (light_charge_fraction > 1.0f) light_charge_fraction = 1.0f;
                func_L00_0026AA10_29FF90(&D_L15_001808C0[moby_data->light].f10, random_int_between(0xF, 0x2F), random_int_between(0x2F, 0x3F),
                                         random_int_between(0x3F, 0x7F), light_charge_fraction * 4.0f * 210000.0f, 0.0f, scale_game_frames_alt(0x14));
                (D_L15_001808C0 + moby_data->light)->f0 = random_float_between((D_L15_001808C0 + moby_data->light)->f0 * 0.8f, (D_L15_001808C0 + moby_data->light)->f1C * 2.0f / 7.0f);
                (D_L15_001808C0 + moby_data->light)->f4 = random_float_between(
                    (D_L15_001808C0 + moby_data->light)->f0 < (D_L15_001808C0 + moby_data->light)->f4 * 0.8f ? (D_L15_001808C0 + moby_data->light)->f0 : (D_L15_001808C0 + moby_data->light)->f4 * 0.8f,
                    (D_L15_001808C0 + moby_data->light)->f0);
            } else if ((D_L15_0015F5CC_macro & 3) == 0) {
                moby_data->light = allocate_packed_color_point_light_slot(moby->pos, 0.0f, 0.0f, 0);
            }
        } else {
            D_L15_0015F320.on = 0;
            D_L15_0015F330.on = 0;
            moby_data->f10 = 0;
            moby_data->vel[0] = fast_cos_c2(((Hero29FF90 *)&hero)->f98) * (D_0015ED6C * 40.0f);
            moby_data->vel[1] = fast_sin_c2(((Hero29FF90 *)&hero)->f98) * (D_0015ED6C * 40.0f);
            moby_data->vel[2] = 0.0f;
            moby->state = 1;
            moby_data->f14 = 0;
        }
        break;
    case EXPANDING_BLAST: {
        float r = D_0015ED6C * 40.0f * convert_integer_to_float_c(moby_data->f14) * 0.5f + 3.0f;
        int capped_blast_remaining_frames;
        int n;
        int col;

        moby_data->f14++;
        if (moby_data->light != -1) {
            (D_L15_001808C0 + moby_data->light)->f1C = (D_L15_001808C0 + moby_data->light)->f1C * (frame_scale_macro * -0.6f + 1.0f);
            if ((D_L15_001808C0 + moby_data->light)->f1C < 0.0001f) {
                release_point_light_slot(moby_data->light);
                moby_data->light = -1;
            }
        }
        if (scale_game_frames_alt(0x3C) < moby_data->f14 || *(unsigned char *)(((char *)&D_001413F4)) != 2
            || (FUN_L00_001ff290(temporary_position, moby->pos, moby_data), moby->pos[0] < 2.0f) || moby->pos[0] > 1021.0f
            || moby->pos[1] < 2.0f || moby->pos[1] > 1021.0f
            || moby->pos[2] < 2.0f || moby->pos[2] > 1021.0f) {
            mark_moby_for_removal_c2(charged_blast_removal_target);
            charged_blast_removal_target = 0;
            return;
        }
        n = func_L00_001F2BE8_29FF90(moby->pos, r, 0x10, moby, 0);
        *(u128_29FF90 *)temporary_position = *(u128_29FF90 *)moby->pos;
        send_blast_hits(moby, temporary_position, D_L15_00178500, n, 0, 0x8B0000, 2, 1, 20.0f, 1.0f, 1.0f);
        capped_blast_remaining_frames = scale_game_frames_alt(0x3C) - moby_data->f14 < scale_game_frames_alt(5) ? scale_game_frames_alt(0x3C) - moby_data->f14 : scale_game_frames_alt(5);
        func_L00_0026E940_29FF90(r * 0.5f * 210000.0f, (char *)moby,
                                 (0x4F << 24) | (random_int_between(0x3F, 0x7F) << 16) | (random_int_between(0x2F, 0x3F) << 8) | random_int_between(0xF, 0x2F),
                                 capped_blast_remaining_frames, -1);
        random_spherical_offset(random_effect_vector, r * D_0015ED6C, r * 2.0f * D_0015ED6C);
        col = (0x7F << 24) | (random_int_between(0x3F, 0x7F) << 16) | (random_int_between(0x2F, 0x3F) << 8) | random_int_between(0xF, 0x2F);
        func_L00_0026CA10_29FF90(moby->pos, random_effect_vector, col, 0x4F7F4F2F, 210000.0f, scale_game_frames_alt(0x2D), 1, *D_L15_001B26AC, -1);
        {
            int i;

            for (i = 0; i < 8; i++) {
                Part29FF90 *p;

                random_spherical_offset(random_effect_vector, r * D_0015ED6C, r * 2.0f * D_0015ED6C);
                random_spherical_offset(effect_vector_scratch, 0.0f, r * 0.5f);
                FUN_L00_001ff290(temporary_position, effect_vector_scratch, moby->pos);
                p = (Part29FF90 *)spawn_mode_controlled_rotating_alpha_fade_particle(effect_vector_scratch, random_effect_vector, 0x7F, moby);
                if (p != 0) {
                    p->fC = random_float_between(6000.0f, 32000.0f) * r * 0.4f;
                }
            }
        }
        {
            int i;

            for (i = 0; i < 12; i++) {
                Part29FF90 *p = (Part29FF90 *)spawn_mode_controlled_rotating_alpha_fade_particle(moby->pos, D_L15_0015F580, 0x7F, moby);

                if (p != 0) {
                    PartSub29FF90 *particle_subrecord = &p->sub;

                    if (i == 2 && random_integer_below_c2(8) == 0) {
                        p->fC = 180000.0f;
                    } else {
                        p->fC = random_float_between(80000.0f, 120000.0f);
                    }
                    p->fC = p->fC * r;
                    p->fA = scale_game_frames_alt(2);
                    particle_subrecord->f10 = 1.0f / convert_integer_to_float_c(p->fA);
                    particle_subrecord->f18 = 0x7F7F7F;
                    particle_subrecord->f16 = 3;
                }
            }
        }
        break;
    }
    }
    moby_data->active = 0;
}
/* Shared level 15/17 moby update (class 77): steers the moby toward its target and runs its state machine. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3668), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern float D_0015ED6C __attribute__((section(".sdata")));
extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") __attribute__((section(".sdata")));
extern int D_L15_00161AB8_lui __asm__("D_L15_00161AB8"); /* no foreign declaration */
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern float group_moby_seeking_speed_gp __asm__("D_L15_001614B4") __attribute__((sda));
extern int group_moby_particle_color_gp __asm__("D_L15_001614B8") __attribute__((sda));
extern float group_moby_particle_scale_gp __asm__("D_L15_001614BC") __attribute__((sda));
extern int group_moby_particle_lifetime_frames_gp __asm__("D_L15_001614C0") __attribute__((sda));
extern float group_moby_effect_z_offset_gp __asm__("D_L15_001614C4") __attribute__((sda));
extern void spawn_moving_hero_relative_alpha_fade_particle(float *pos, char *vel, int color, unsigned char life, unsigned char b, int mode, float scale) __asm__("FUN_L00_002730e0");
void draw_group_moby_effect_quads(unsigned char *moby) __asm__("FUN_L15_002a29b8");

extern void draw_group_moby_effect_quads(unsigned char *) __asm__("FUN_L15_002a29b8");
extern void blend_moby_animation_c2(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern char D_0013F420[];
extern void func_L00_0025A8E8_m(void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002598b0");

void update_hero_seeking_group_moby(char *moby) __asm__("FUN_L15_002a2488");

void update_hero_seeking_group_moby(char *moby) {
    float va[4];
    float pb[4];
    char *data;
    char *callback_frame_stamp;
    char *hero_base;
    float fa;
    float fb;
    float yaw_cosine;
    float yaw_sine;
    float reference_distance_xy;
    float yaw;
    float target_z;
    float fd;
    int state;
    int animation_blend_frames;
    int particle_lifetime_frames;
    int converted_particle_lifetime_frames;
    int particle_initial_rotation;
    unsigned char life;

    data = *(char **)(moby + 0x78);
    if (find_moby_hit(moby, 0x330000, 0) != 0) {
        ((unsigned char *)moby)[0x20] = 4;
        ((unsigned char *)moby)[0xA4] = 0xFF;
    }
    if (((unsigned char *)moby)[0x20] != 1) {
        random_vector(va, 0.0f, D_0015ED6C + D_0015ED6C);
        qcopy(pb, moby + 0x10);
        pb[2] = pb[2] + *(float *)&group_moby_effect_z_offset_gp;
        if (D_L15_00161AB8_lui == 0) {
            ((unsigned char *)moby)[0x20] = 1;
            goto tailA;
        }
        particle_lifetime_frames = scale_game_frames_alt(group_moby_particle_lifetime_frames_gp);
        converted_particle_lifetime_frames = func_001FA898_r((float)particle_lifetime_frames);
        life = (unsigned char)converted_particle_lifetime_frames;
        particle_initial_rotation = random_integer_below_c(0xFF);
        spawn_moving_hero_relative_alpha_fade_particle(pb, (char *)va, group_moby_particle_color_gp, life, (unsigned char)particle_initial_rotation, 0, *(float *)&group_moby_particle_scale_gp);
        callback_frame_stamp = *(char **)(data + 0x6C);
        if (callback_frame_stamp == 0 || *(int *)callback_frame_stamp != D_L15_0015F5CC_macro) {
            if (callback_frame_stamp != 0) {
                *(int *)callback_frame_stamp = D_L15_0015F5CC_macro;
            }
            enqueue_callback_list_1_alt(draw_group_moby_effect_quads, moby);
        }
    }
    state = ((unsigned char *)moby)[0x20];
    if (state == 2) goto s2;
    if (state < 3) {
        if (state == 0) goto s0;
        return;
    }
    if (state == 3) goto s3;
    if (state == 4) goto s4;
    return;

s2:
    *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + (D_0015ED6C + D_0015ED6C);
    if (tick_countdown_32_alt(data + 0x68) == 0) return;
    ((unsigned char *)moby)[0x20] = 3;
    if (((unsigned char *)moby)[0x53] != 1) {
        animation_blend_frames = scale_game_frames_alt(10);
        blend_moby_animation_c2(moby, 1, 0, animation_blend_frames);
    }
    *(int *)(data + 0x68) = scale_game_frames_alt(0xF0);
    return;

s3:
    hero_base = (char *)&hero;
    fa = *(float *)(hero_base + 0x80) - *(float *)(moby + 0x10);
    fb = *(float *)(hero_base + 0x84) - *(float *)(moby + 0x14);
    yaw = fast_add_rotations_c(*(float *)(data + 0x64), fast_arc_tan(fa, fb));
    target_z = *(float *)(hero_base + 0xC8);
    yaw_cosine = fast_cos_c(yaw);
    va[0] = yaw_cosine * (*(float *)&group_moby_seeking_speed_gp * D_0015ED6C);
    yaw_sine = fast_sin_c(yaw);
    va[1] = yaw_sine * (*(float *)&group_moby_seeking_speed_gp * D_0015ED6C);
    va[2] = 0.0f;
    fd = *(float *)(moby + 0x18) - target_z;
    if (0.6f < fd) {
        va[2] = -(D_0015ED6C * 3.0f);
    } else if (fd < 0.4f) {
        va[2] = D_0015ED6C * 3.0f;
    }
    FUN_L00_00258830((int)moby, (int)va, 0.25f, 0.25f, 0.0f, 0);
    reference_distance_xy = FUN_001f9b80((float *)((char *)((char *)&D_0013F420)), (float *)(moby + 0x10));
    if (reference_distance_xy < 0.5f || tick_countdown_32_alt(data + 0x68)) {
        ((unsigned char *)moby)[0x20] = 4;
        return;
    }
    if (((unsigned char *)moby)[0x53] != 1) return;
    if ((((unsigned char *)moby)[0x70] & 2) == 0) return;
    animation_blend_frames = scale_game_frames_alt(10);
    blend_moby_animation_c2(moby, 2, 0, animation_blend_frames);
    return;

s4:
    cart_burst(moby, moby + 0x10, -1, 0.25f, 13.0f);
    func_L00_0025A8E8_m(moby, moby + 0x10, 0x10001, 1.0f, 1.0f, 1.0f, 0, 1, 0);

s0:
    ((unsigned char *)moby)[0x20] = 1;
    ((unsigned char *)moby)[0x31] = 0;
    *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) | 1;
tailA:
    *(int *)(moby + 0x94) = 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3A48), where it is exact; names translated to the US level program. */

extern int D_L15_0015FFD8;
void blend_moby_animation_u(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

int activate_first_inactive_group_moby(int moby_group_index, void *pos) __asm__("FUN_L15_002a2868");

int activate_first_inactive_group_moby(int moby_group_index, void *pos) {
    short *group_moby_entry = (short *)D_L15_001ABE40[moby_group_index];
    struct Hero *hero_data = &hero;
    do {
        unsigned char *listed_moby = (unsigned char *)D_L15_0015FFD8 + ((*(unsigned short *)group_moby_entry & 0x7FFF) << 8);
        unsigned char state = listed_moby[0x20];
        if (state == 1) {
            char *data;
            listed_moby[0x20] = 2;
            if (listed_moby[0x53] != 0)
                blend_moby_animation_u(listed_moby, 0, 0, 0);
            *(unsigned short *)(listed_moby + 0x34) |= 1;
            listed_moby[0x31] = 0;
            data = *(char **)(listed_moby + 0x78);
            *(int *)(listed_moby + 0x94) = *(int *)(*(char **)(listed_moby + 0x24) + 0x10);
            *(float *)(data + 0x64) = random_float_between_alt(-15.0f, 15.0f) * DEG_TO_RAD;
            *(int *)(data + 0x68) = scale_game_frames(0x3C);
            qcopy(listed_moby + 0x10, pos);
            *(float *)(listed_moby + 0x18) -= 1.0f;
            *(float *)(listed_moby + 0x48) = fast_arc_tan(hero_data->motion.pos.f[0] - *(float *)(listed_moby + 0x10),
                                                hero_data->motion.pos.f[1] - *(float *)(listed_moby + 0x14));
            return 1;
        }
    } while (*group_moby_entry++ > -1);
    return 0;
}
#include "rnc/gameplay/entities/moby.h"
#define MOBY(p) ((struct Moby *)(p))
typedef struct {
    float v[4][4];
    int col[4];
    float uv[4][2];
    long long r70;
    long long texture; /* get_effect_texture */
    unsigned long long r80;
    unsigned long long alpha; /* GS ALPHA register value (0x8000000044) */
} EffectQuad;

extern float D_L15_001CE110[4][4][4];
extern float D_L15_001CE210[8];
extern int group_moby_effect_first_quad_color_gp __asm__("D_L15_001614C8") __attribute__((sda));
extern long long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern float fast_atan2(float, float) __asm__("FUN_001f9e90");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void rotation_matrix_from_euler(void *, void *) __asm__("FUN_001fa050");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_group_moby_effect_quads(unsigned char *moby) {
    EffectQuad effect_quad;
    float angles[4] __attribute__((aligned(16)));
    float rotation[12] __attribute__((aligned(16)));
    float position[4] __attribute__((aligned(16)));
    unsigned short *group_moby_entry;
    int i, j;

    effect_quad.texture = get_effect_texture(8);
    effect_quad.r80 = 0xff9000000260LL;
    effect_quad.alpha = 0x8000000044LL;
    effect_quad.r70 = 0;
    {
        int count = 3;
        float *source = D_L15_001CE210;
        float *v = &effect_quad.uv[0][1];
        float *u = &effect_quad.uv[0][0];
        do {
            count--;
            *u = source[0];
            *v = source[1];
            source += 2;
            u += 2;
            v += 2;
        } while (count >= 0);
    }
    group_moby_entry = (unsigned short *)D_L15_001ABE40[MOBY(moby)->group];
    do {
        unsigned char *other = (unsigned char *)(D_L15_0015FFD8 + ((*group_moby_entry & 0x7fff) << 8));
        if (MOBY(other)->state != 1) {
            clear_vector(angles);
            angles[2] = fast_atan2(D_L15_00167280.position[0] - MOBY(other)->pos.x,
                                   D_L15_00167280.position[1] - MOBY(other)->pos.y);
            angles[1] = -fast_atan2(vector_distance(&MOBY(other)->pos, D_L15_00167280.position),
                                    D_L15_00167280.position[2] - MOBY(other)->pos.z);
            rotation_matrix_from_euler(rotation, angles);
            qcopy(position, &MOBY(other)->pos);
            position[2] += group_moby_effect_z_offset_gp;
            position[3] = 1.0f;
            for (i = 0; i < 4; i++) {
                int *colors = effect_quad.col;
                int color = (&group_moby_effect_first_quad_color_gp)[i];
                for (j = 0; j < 4; j++) {
                    colors[j] = color;
                    qcopy(effect_quad.v[j], D_L15_001CE110[i][j]);
                }
                draw_geometry_quad(&effect_quad, rotation, 0);
            }
        }
    } while ((short)*group_moby_entry++ >= 0);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3DD0), where it is exact; names translated to the US level program. */

#include "qzero.h"

/* Level 15 moby update (class 78), three states: 0 picks the first state from the kill and save flags,
 * 1 sprays sixteen random sparks around the moby and parks its data vector, 2 counts down. */

typedef struct { unsigned char pad[0x454]; unsigned char killed[1]; } KillFlags_2A3DD0;

typedef struct { unsigned char pad0[0x74]; unsigned char active; unsigned char pad75[0x13]; char *owner; } Voice_2A3DD0;

extern short D_L15_0015FFD8_d __asm__("D_L15_0015FFD8");
extern char D_0013F420[];
extern unsigned char D_0014C050_c[] __asm__("D_0014C050");
extern unsigned char D_0014C190[];
extern int D_0015ED84;
extern int transpose_rotation_matrix() __asm__("FUN_001fa2d8");
extern int draw_spark_emitter_effect_quads() __asm__("FUN_L15_002a3138");
unsigned char *create_type_47_particle(void *pos, float *p, float *c, int d, int e, int f, int g, int h, char *owner) __asm__("FUN_L15_002647f0");
extern KillFlags_2A3DD0 D_L15_001BBB40_2A3DD0 __asm__("D_L15_001BB830");
extern unsigned char D_0013E650_2A3DD0[] __asm__("D_0013E550");
extern char D_0013F420[];
extern float func_002140F8_2A3DD0(float, float) __asm__("FUN_002132a8");
extern int func_002140B0_2A3DD0(int) __asm__("FUN_00213260");
extern int func_001F9850_2A3DD0(int) __asm__("FUN_001f96f8");
extern float func_001F9B88_2A3DD0(float) __asm__("FUN_001f99c0");
extern int func_001FA898_2A3DD0(float) __asm__("FUN_001fa6d0");
extern void func_001F9BF0_2A3DD0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_001F9EE8_2A3DD0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void func_001F9EC0_2A3DD0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void func_001F9BD8_2A3DD0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_L00_001FF4B0_2A3DD0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001FA4A0_2A3DD0(void *, void *) __asm__("FUN_001fa2d8");
extern void func_L15_002A47B8_2A3DD0(void *) __asm__("FUN_L15_002a35d8");
extern int func_0022ED80_2A3DD0(int, int, void *) __asm__("FUN_0022da68");
extern void func_001F49B0_2A3DD0(void (*)(void), void *) __asm__("FUN_001f4600");
extern void func_L15_002A4318_2A3DD0(void) __asm__("FUN_L15_002a3138");
extern unsigned char *func_L15_002655D0_2A3DD0(void *, void *, void *, int, int, int, int, int, void *) __asm__("FUN_L15_002647f0");

void update_spark_emitter_moby(unsigned char *moby) __asm__("FUN_L15_002a2bf0");

void update_spark_emitter_moby(unsigned char *moby) {
    enum {
        INITIALIZE_EFFECT = 0,
        EMIT_SPARKS = 1,
        WAIT_FOR_EFFECT_TIMEOUT = 2
    };
    unsigned char *moby_data = *(unsigned char **)(moby + 0x78);
    float V[4];
    float W[16];
    float A[4];
    float B[4];
    float C[4];

    switch (moby[0x20]) {
    case INITIALIZE_EFFECT: {
        unsigned short id;
        *(unsigned short *)(moby + 0x34) |= 1;
        moby[0x31] = 0;
        id = *(unsigned short *)(moby + 0xB2);
        if (D_L15_001BBB40_2A3DD0.killed[(short)id] == 0 && (((*(int *)(D_0014C190 + (((short)id >> 5) * 4) + (D_0015ED84 << 8)) >> (id & 0x1F)) ^ 1) & 1)) {
            moby[0x20] = 1;
        } else {
            moby[0xBC] = 0xFF;
            *(int *)(moby + 0x94) = 0;
            moby[0x20] = 3;
        }
        break;
    }
    case EMIT_SPARKS: {
        int linked_moby_index = *(int *)(moby_data + 0x10);
        if ((linked_moby_index != -1 && (*(struct Moby **)&D_L15_0015FFD8_d)[linked_moby_index].state == 2)
            || D_0014C050_c[(unsigned char)moby[0xB0] + (D_0015ED84 << 4)] == 0xFF) {
            func_L15_002A47B8_2A3DD0(moby);
        } else {
            Voice_2A3DD0 *e = (Voice_2A3DD0 *)(D_0013E650_2A3DD0 + moby[0xBC] * 0x70);
            float lo, hi, z, one;
            int i;
            if (e->owner != (char *)moby || e->active == 0) {
                moby[0xBC] = func_0022ED80_2A3DD0(0, 4, moby);
            }
            func_001F9BF0_2A3DD0(V, D_0013F420, moby + 0x10);
            func_001FA4A0_2A3DD0(W, moby + 0xC0);
            func_001F9EE8_2A3DD0(V, V, W);
            V[0] = 0.0f;
            z = V[0];
            i = 15;
            do {
                int col, fa, fb, fc;
                lo = -5.0f;
                hi = 5.0f;
                one = 1.0f;
                i--;
                A[0] = func_002140F8_2A3DD0(-0.25f, 0.25f);
                A[1] = func_002140F8_2A3DD0(lo, hi);
                A[2] = func_002140F8_2A3DD0(z, hi);
                B[0] = z;
                B[1] = func_002140F8_2A3DD0(lo, hi);
                B[2] = func_002140F8_2A3DD0(lo, hi);
                C[0] = z;
                C[1] = func_002140F8_2A3DD0(lo, hi);
                C[2] = func_002140F8_2A3DD0(lo, hi);
                func_001F9EC0_2A3DD0(A, A, moby + 0xC0);
                func_001F9EC0_2A3DD0(B, B, moby + 0xC0);
                func_001F9EC0_2A3DD0(C, C, moby + 0xC0);
                func_001F9BD8_2A3DD0(A, A, moby + 0x10);
                func_L00_001FF4B0_2A3DD0(B, B, one / (float)func_001F9850_2A3DD0(30));
                func_L00_001FF4B0_2A3DD0(C, C, one / (float)func_001F9850_2A3DD0(30));
                B[3] = 0.125f;
                {
                    int ra = func_002140B0_2A3DD0(0x10);
                    int rb = func_002140B0_2A3DD0(0x20);
                    int rc = func_002140B0_2A3DD0(0x20);
                    int partial_spark_color = ((rb + 0x60) << 8) | 0x60000000;
                    col = ((ra + 0x20) << 16) | partial_spark_color;
                    col |= rc + 0x60;
                }
                fa = func_001FA898_2A3DD0((float)(func_001F9850_2A3DD0(30) / 4));
                fb = func_001FA898_2A3DD0((float)(func_001F9850_2A3DD0(30) / 2));
                fc = func_001FA898_2A3DD0((float)(func_001F9850_2A3DD0(30) / 4));
                func_L15_002655D0_2A3DD0(A, B, C, col, 0x60808080, fa, fb, fc, moby);
            } while (i >= 0);
            if (func_001F9B88_2A3DD0(V[1]) < hi && z < V[2] && V[2] < hi) {
                func_001F9EC0_2A3DD0(moby_data, V, moby + 0xC0);
                func_001F9BD8_2A3DD0(moby_data, moby_data, moby + 0x10);
            } else {
                qzero(moby_data);
            }
            func_001F49B0_2A3DD0(func_L15_002A4318_2A3DD0, moby);
        }
        break;
    }
    case WAIT_FOR_EFFECT_TIMEOUT:
        if ((*(int *)(moby_data + 0x14))++ >= 0x3D) {
            moby[0x20] = 3;
        } else {
            qzero(moby_data);
            func_001F49B0_2A3DD0(func_L15_002A4318_2A3DD0, moby);
        }
        break;
    }
}
extern void copy_rotation_2a3138(void *, void *) __asm__("FUN_001fa298");
extern int interpolate_color_2a3138(float, int, int) __asm__("FUN_001fa6e0");
extern int truncate_float_2a3138(float) __asm__("FUN_001fa6d0");
extern float integer_to_float_2a3138(int) __asm__("FUN_001fa6c0");
extern float fast_cos_2a3138(float) __asm__("FUN_001f9dc8");
extern float fast_sin_2a3138(float) __asm__("FUN_001f9de0");
#include "rnc/overlay/shared/gameplay/entities/sentrybot_data/draw_spark_emitter_effect_quads.h"
extern int D_L15_001CE230[];
extern float D_L15_001CE250[][2];
extern int D_L15_001CE290[][2];
extern float D_L15_001CE2D0[][2];
extern float D_L15_001CE2F0[4][4];
extern float D_L15_001CE330[4][4];

extern int D_L15_001614D8[2] __attribute__((sda));
extern int D_L15_001614E0[2] __attribute__((sda));
extern int D_L15_001614E8[2] __attribute__((sda));
extern int D_L15_001614F0[2] __attribute__((sda));

int draw_spark_emitter_effect_quads(unsigned char *moby) {
    EffectQuad first, second;
    float transform[12];
    float position[4];
    int *data = *(int **)(moby + 0x78);
    int pass, corner, axis, component;
    float *transformPointer;
    float *firstUv1, *firstUv3, *secondUv0, *secondUv1;
    int nextPass;
    int passOffset;
    float *secondUv2, *secondUv3;
    float (*secondVertices)[4];
    float (*firstUv)[2];

    copy_rotation_2a3138(transform, moby + 0xc0);
    qcopy(position, moby + 0x10);
    position[3] = 1.0f;
    first.texture = get_effect_texture(0x2c);
    first.r80 = 0xff9000000260ULL;
    first.r70 = 0;
    second.texture = get_effect_texture(0x2c);
    secondVertices = second.v;
    firstUv = first.uv;
    secondUv0 = second.uv[0];
    firstUv1 = first.uv[1];
    secondUv1 = second.uv[1];
    secondUv2 = second.uv[2];
    firstUv3 = first.uv[3];
    secondUv3 = second.uv[3];
    second.r70 = 0;
    second.r80 = 0xff9000000260ULL;
    second.col[3] = 0;
    second.col[1] = 0;
    for (corner = 0; corner < 4; corner++) {
        qcopy(first.v[corner], D_L15_001CE330[corner]);
        qcopy(secondVertices[corner], D_L15_001CE2F0[corner]);
    }
    for (pass = 0; pass < 6; pass = nextPass) {
        int parity = pass & 1;
        first.alpha =
            (unsigned long long)D_L15_001614D8[parity] |
            ((unsigned long long)D_L15_001614E0[parity] << 2) |
            ((unsigned long long)D_L15_001614E8[parity] << 4) |
            ((unsigned long long)D_L15_001614F0[parity] << 6) |
            0x8000000000ULL;
        second.alpha =
            (unsigned long long)D_L15_001614D8[parity] |
            ((unsigned long long)D_L15_001614E0[parity] << 2) |
            ((unsigned long long)D_L15_001614E8[parity] << 4) |
            ((unsigned long long)D_L15_001614F0[parity] << 6) |
            0x8000000000ULL;
        if (data[5] != 0) {
            second.col[2] = interpolate_color_2a3138((float)data[5] / 60.0f,
                                                   D_L15_001CE230[pass], 0);
            second.col[0] = second.col[2];
            first.col[3] = second.col[2];
            first.col[2] = second.col[2];
            first.col[1] = second.col[2];
            first.col[0] = second.col[2];
        } else {
            second.col[2] = D_L15_001CE230[pass];
            second.col[0] = second.col[2];
            first.col[3] = second.col[2];
            first.col[2] = second.col[2];
            first.col[1] = second.col[2];
            first.col[0] = second.col[2];
        }
        corner = 0;
        passOffset = pass * 8;
        nextPass = pass + 1;
        for (; corner < 4; corner++) {
            float *uv = (float *)(corner * 8 + (unsigned int)firstUv);
            float *frequency = (float *)((char *)D_L15_001CE250 + passOffset);
            int waveform = passOffset;
            int base = corner * 8;
            axis = 1;
            do {
                int limit, value;
                float angle, delta;
                float step;
                float radians;
                axis--;
                limit = truncate_float_2a3138(*frequency * 360.0f);
                angle = integer_to_float_2a3138(spark_emitter_uv_phase_input % limit)
                    / *frequency - 180.0f;
                frequency++;
                value = *(int *)((char *)D_L15_001CE290 + waveform);
                waveform += 4;
                radians = 0.017453292f;
                step = 0.125f;
                if (value != 0) {
                    angle = fast_cos_2a3138(angle * radians);
                } else {
                    angle = fast_sin_2a3138(angle * radians);
                }
                delta = angle * step;
                *uv++ = *(float *)((char *)D_L15_001CE2D0 + base) + delta;
                base += 4;
            } while (axis >= 0);
        }
        for (component = 0; component < 2; component++) {
            float step = (float)component * 0.125f;
            secondUv0[component] = firstUv1[component];
            secondUv1[component] = firstUv1[component] + step;
            secondUv2[component] = firstUv3[component];
            secondUv3[component] = firstUv3[component] + step;
        }
        transformPointer = transform;
        draw_geometry_quad(&first, transformPointer, 0);
        draw_geometry_quad(secondVertices, transformPointer, 0);
        for (corner = 0; corner < 4; corner++) {
            first.v[corner][0] += 0.1f;
            secondVertices[corner][0] += 0.1f;
        }
    }
}
#include "rnc/gameplay/entities/moby.h"

typedef struct {
    u8 pad[0x454];
    u8 killed[1]; /* per spawn id: spawn slot + 2 */
} KillFlags;
typedef struct {
    u8 pad0[0x74];
    u8 active;
    u8 pad75[0x13];
    struct Moby *owner;
} VoiceSlot;
extern KillFlags D_L15_001BABD0 __attribute__((section(".data")));
extern KillFlags D_L15_001BB830 __attribute__((section(".data")));
extern u8 D_L15_0015FC88[8];
extern u8 D_0014C050[][16] __attribute__((section(".data")));
extern u8 D_0013E550[] __attribute__((section(".data")));
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

/* Marks the moby as killed in the level's spawn tables and releases the voice slot it still owns. */
void mark_moby_killed_and_release_owned_voice(int addr) {
    struct Moby *moby = (struct Moby *)addr;
    s32 slot;
    moby->collision = 0;
    D_L15_001BABD0.killed[(s16)moby->save_id] = moby->mission + 2;
    if (moby->mission == 0xFF || (D_L15_0015FC88[moby->mission] != 0xFF && D_0014C050[current_level_index][moby->mission] == 0xFF)) {
        D_L15_001BB830.killed[(s16)moby->save_id] = moby->mission + 2;
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
/* Spiderbot update: steps the walk/turn state, then probes ahead and sprays sparks along the path. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A8090), where it is exact; names translated to the US level program. */

typedef int u128_2A8090 __attribute__((mode(TI)));

typedef struct {
    float v[2];
    float f8;
    float fC;
    unsigned char *moby;
    int flags;
    unsigned char b18;
    unsigned char b19;
    short h1A;
    float f1C;
    int i20;
    int pad24[3];
} Ray_2A8090;

extern f32 convert_integer_to_float_c3(s32) __asm__("func_001FA6C0");
extern f32 fast_cos_c4(f32) __asm__("func_001F9DC8");
extern f32 fast_normalize_angle(f32) __asm__("func_001FA610");
extern f32 fast_sin_c4(f32) __asm__("func_001F9DE0");
extern float D_0015ED6C;
extern float FUN_L04_001f1978(float);
extern float fast_add_rotations_c2(float, float) __asm__("func_001FA580");
extern s32 rand(void);
extern s32 random_integer_below_c4(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c5(f32) __asm__("func_001FA6D0");
extern int spiderbot_effect_spawn_count_gp __asm__("D_L15_00161558") __attribute__((sda));
extern float spiderbot_effect_scale_gp __asm__("D_L15_0016155C") __attribute__((sda));
extern float spiderbot_effect_lifetime_range_end_gp __asm__("D_L15_00161560") __attribute__((sda));
extern u8 D_L15_00174460[];
extern void FUN_L15_002a7628_c() __asm__("FUN_L15_002a7628");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void subtract_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a28");
void normalize_vector_triplet(f32 *vectors) __asm__("FUN_00214128");
extern unsigned char *func_L00_00273F80_r(float *pos, char *vel, int color, unsigned char life, unsigned char b, int mode, float scale) __asm__("FUN_L00_002730e0");

void spiderbot_update(unsigned char *moby) __asm__("FUN_L15_002a6e68");
void spiderbot_update(unsigned char *moby) {
    enum {
        SPIDERBOT_INITIALIZE = 0,
        SPIDERBOT_CIRCULAR_MOTION = 1,
        SPIDERBOT_ARC_OSCILLATION = 2,
        SPIDERBOT_LINEAR_OSCILLATION = 3
    };
    float old[4];
    float A[4];
    float B[4];
    float C[4];
    unsigned char *moby_data = *(unsigned char **)(moby + 0x78);
    *(u128_2A8090 *)old = *(u128_2A8090 *)(moby + 0x10);
    {
    float D[4];
    float M[16];
    float T[4];
    switch (moby[0x20]) {
    case SPIDERBOT_INITIALIZE:
        *(u128_2A8090 *)moby_data = *(u128_2A8090 *)old;
        *(float *)(moby + 0x58) = *(float *)(moby_data + 0x30) / 1.5f;
        *(float *)(moby_data + 0x28) = *(float *)(moby_data + 0x28) * 0.017453292f;
        if (*(int *)(moby_data + 0x20) == 0) {
            moby[0x20] = 1;
            *(float *)(moby_data + 0x2C) = *(float *)(moby + 0x48);
        } else if (*(int *)(moby_data + 0x20) >= 3) {
            moby[0x20] = 2;
            *(float *)(moby_data + 0x2C) = *(float *)(moby + 0x48);
        } else {
            moby[0x20] = 3;
        }
        break;
    case SPIDERBOT_CIRCULAR_MOTION: {
        *(u128_2A8090 *)C = 0;
        C[2] = *(float *)(moby_data + 0x24);
        *(u128_2A8090 *)D = 0;
        *(u128_2A8090 *)B = *(u128_2A8090 *)C;
        *(float *)(moby_data + 0x28) = fast_add_rotations_c2(*(float *)(moby_data + 0x28),
            360.0f / (*(float *)(moby_data + 0x24) * 6.2831855f / *(float *)(moby_data + 0x30)) * 0.017453292f * D_0015ED6C);
        *(float *)(moby + 0x58) = *(float *)(moby_data + 0x30) / 1.5f;
        C[0] = fast_cos_c4(*(float *)(moby_data + 0x2C));
        C[1] = fast_sin_c4(*(float *)(moby_data + 0x2C));
        C[2] = 0.0f;
        build_look_at_matrix(A, B, C, *(float *)(moby_data + 0x28));
        add_vector_xyz(T, moby_data, A);
        *(u128_2A8090 *)(moby + 0x10) = *(u128_2A8090 *)T;
        D[2] = fast_add_rotations_c2(*(float *)(moby_data + 0x2C), 1.5707964f);
        D[1] = fast_add_rotations_c2(-*(float *)(moby_data + 0x28), 3.1415927f);
        euler_rotation_matrix(M, D);
        copy_matrix3x4(moby + 0xC0, M);
        normalize_vector_triplet(moby + 0xC0);
        *(unsigned short *)(moby + 0x34) |= 0x100;
        break;
    }
    case SPIDERBOT_ARC_OSCILLATION: {
        float previous_arc_angle;
        float current_arc_angle;
        *(u128_2A8090 *)C = 0;
        C[2] = -*(float *)(moby_data + 0x24);
        *(u128_2A8090 *)D = 0;
        *(u128_2A8090 *)B = *(u128_2A8090 *)C;
        previous_arc_angle = fast_add_rotations_c2(fast_normalize_angle(fast_cos_c4(*(float *)(moby_data + 0x28)) * 1.5707964f),
                           *(int *)(moby_data + 0x20) == 4 ? 1.5707964f : 0.0f);
        *(float *)(moby_data + 0x28) = fast_add_rotations_c2(*(float *)(moby_data + 0x28),
            360.0f / (*(float *)(moby_data + 0x24) * 6.2831855f / *(float *)(moby_data + 0x30)) * 0.017453292f * D_0015ED6C);
        current_arc_angle = fast_add_rotations_c2(fast_normalize_angle(fast_cos_c4(*(float *)(moby_data + 0x28)) * 1.5707964f),
                           *(int *)(moby_data + 0x20) == 4 ? 1.5707964f : 0.0f);
        *(float *)(moby + 0x58) = *(float *)(moby_data + 0x24) * fast_subtract_rotations(current_arc_angle, previous_arc_angle) / (D_0015ED6C * 1.5f);
        C[0] = fast_cos_c4(*(float *)(moby_data + 0x2C));
        C[1] = fast_sin_c4(*(float *)(moby_data + 0x2C));
        C[2] = 0.0f;
        build_look_at_matrix(A, B, C, current_arc_angle);
        add_vector_xyz(T, moby_data, A);
        *(u128_2A8090 *)(moby + 0x10) = *(u128_2A8090 *)T;
        D[2] = fast_add_rotations_c2(*(float *)(moby_data + 0x2C), 1.5707964f);
        D[1] = fast_add_rotations_c2(-current_arc_angle, 0.0f);
        euler_rotation_matrix(M, D);
        copy_matrix3x4(moby + 0xC0, M);
        normalize_vector_triplet(moby + 0xC0);
        *(unsigned short *)(moby + 0x34) |= 0x100;
        break;
    }
    case SPIDERBOT_LINEAR_OSCILLATION: {
        float previous_linear_displacement;
        float x;
        previous_linear_displacement = fast_cos_c4(*(float *)(moby_data + 0x28)) * *(float *)(moby_data + 0x24);
        *(float *)(moby_data + 0x28) = fast_add_rotations_c2(*(float *)(moby_data + 0x28),
            360.0f / (*(float *)(moby_data + 0x24) * 6.2831855f / *(float *)(moby_data + 0x30)) * 0.017453292f * D_0015ED6C);
        x = fast_cos_c4(*(float *)(moby_data + 0x28)) * *(float *)(moby_data + 0x24);
        *(float *)(moby + 0x58) = (x - previous_linear_displacement) / (D_0015ED6C * 1.5f);
        build_spherical_offset(A, x, *(int *)(moby_data + 0x20) != 2 ? *(float *)(moby + 0x48) : 0.0f,
                      *(int *)(moby_data + 0x20) == 2 ? 1.5707964f : 0.0f);
        add_vector_xyz(B, moby_data, A);
        *(u128_2A8090 *)(moby + 0x10) = *(u128_2A8090 *)B;
        break;
    }
    }
    }
    {
        Ray_2A8090 R;
        float T2[4];
        int i;
        *(u128_2A8090 *)B = 0;
        B[2] = 12.0f;
        func_001F9EC0_2A3DD0(B, B, moby + 0xC0);
        add_vector_xyz(R.v, moby + 0x10, B);
        *(u128_2A8090 *)A = *(u128_2A8090 *)R.v;
        R.flags = 0x10001;
        R.moby = moby;
        R.f1C = 1.0f;
        R.i20 = 1;
        fast_vec_normalize(R.v, B, 1.0f);
        R.h1A = *(unsigned short *)(moby + 0xA6);
        R.f8 = 1.0f;
        R.fC = 5627.9248f;
        R.b18 = 1;
        R.b19 = 1;
        subtract_vector_xyz_c3(T2, moby + 0x10, old);
        *(u128_2A8090 *)C = *(u128_2A8090 *)T2;
        collision_line(moby + 0x10, A, 9, moby, &R);
        if (collision_line(moby + 0x10, A, 2, 0, 0)) {
            *(u128_2A8090 *)(moby_data + 0x10) = *(u128_2A8090 *)D_L15_00174460;
        } else {
            *(u128_2A8090 *)(moby_data + 0x10) = *(u128_2A8090 *)A;
        }
        for (i = 0; i < spiderbot_effect_spawn_count_gp; i++) {
            float effect_direction_sign;
            float effect_segment_length;
            int k;
            unsigned char life;
            effect_direction_sign = -1.0f;
            if (rand() & 1) effect_direction_sign = 1.0f;
            k = truncate_float_to_s32_c5(FUN_L04_001f1978(random_float_between(spiderbot_effect_lifetime_range_end_gp * 0.5f, spiderbot_effect_lifetime_range_end_gp)));
            subtract_vector_xyz_c3(T2, moby_data + 0x10, moby + 0x10);
            *(u128_2A8090 *)B = *(u128_2A8090 *)T2;
            fast_vec_scale(B, B, effect_direction_sign);
            effect_segment_length = fast_vec_length(B);
            *(u128_2A8090 *)A = effect_direction_sign < 0.0f ? *(u128_2A8090 *)(moby_data + 0x10) : *(u128_2A8090 *)(moby + 0x10);
            life = k;
            fast_vec_scale(B, B, random_float_between(0.0f, 0.9f));
            FUN_L00_001ff290(T2, A, B);
            fast_vec_normalize(B, B, random_float_between(D_0015ED6C * 10.0f, effect_segment_length / convert_integer_to_float_c3(k * 4)));
            func_L00_00273F80_r(A, (char *)B, 0x604040FF, life, random_integer_below_c4(0xFF), 0, spiderbot_effect_scale_gp);
        }
    }
    enqueue_callback_list_1_alt(FUN_L15_002a7628_c, moby);
}
/* Builds a screen-space quad from the moby's offset and draws it. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A8850), where it is exact; names translated to the US level program. */


typedef struct {
    float v[4][4];
    int col[4];
    float uv[4][2];
    long a, b, c, d;
} Quad;

extern float D_L15_001673C0[4];
extern float D_L15_001E27F0[];
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

void draw_spiderbot_effect_quad(char *moby) __asm__("FUN_L15_002a7628");

void draw_spiderbot_effect_quad(char *moby) {
    float B[4];
    float A[4];
    Quad effect_quad;
    float F[4];
    float E[4];
    float C[4];
    float D[4];
    char *d = *(char **)(moby + 0x78);
    int i;
    subtract_vector_xyz(A, d + 0x10, moby + 0x10);
    *(OvlQuad *)B = *(OvlQuad *)A;
    normalize_vector_xyz(B, B, 0.5f);
    *(OvlQuad *)C = *(OvlQuad *)(moby + 0x10);
    FUN_L00_001ff290(D, C, B);
    subtract_vector_xyz(E, C, d + 0x10);
    subtract_vector_xyz(A, C, D_L15_001673C0);
    subtract_vector_xyz(F, d + 0x10, D_L15_001673C0);
    cross_vectors_xyz(A, A, E);
    normalize_vector_xyz(A, A, 0.1f);
    cross_vectors_xyz(F, F, E);
    normalize_vector_xyz(F, F, 0.1f);
    effect_quad.b = get_effect_texture_alt(0xE);
    effect_quad.c = 0xFF9000000260L;
    effect_quad.d = 0x8000000048L;
    effect_quad.a = 0;
    effect_quad.col[3] = 0x80243278;
    effect_quad.col[2] = 0x80243278;
    effect_quad.col[1] = 0x80243278;
    effect_quad.col[0] = 0x80243278;
    for (i = 0; i < 4; i++) {
        effect_quad.uv[i][0] = D_L15_001E27F0[i * 2];
        effect_quad.uv[i][1] = D_L15_001E27F0[i * 2 + 1];
    }
    subtract_vector_xyz(D, C, A);
    *(OvlQuad *)effect_quad.v[0] = *(OvlQuad *)D;
    add_vector_xyz(D, C, A);
    *(OvlQuad *)effect_quad.v[1] = *(OvlQuad *)D;
    subtract_vector_xyz(D, d + 0x10, F);
    *(OvlQuad *)effect_quad.v[2] = *(OvlQuad *)D;
    add_vector_xyz(D, d + 0x10, F);
    *(OvlQuad *)effect_quad.v[3] = *(OvlQuad *)D;
    draw_geometry_quad(&effect_quad, 0, 0);
}
typedef struct {
    Vec4 origin;
    s32 trigger;
    f32 travel;
    s32 start_volume;
    s32 stop_volume;
} MovingMobyVars;

extern f32 FUN_001f99c0(f32);
extern f32 fast_cos_door(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_door(f32) __asm__("FUN_001f9de0");
extern void add_vector_door(void *, void *, void *) __asm__("FUN_001f9a10");

void door_update(struct Moby *moby) __asm__("FUN_L15_002bddb0");
void door_update(struct Moby *moby) {
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_WAIT_FOR_TRIGGER = 1,
        DOOR_MOVE_AWAY_FROM_ORIGIN = 2,
        DOOR_WAIT_FOR_START_VOLUME_ENTRY = 4,
        DOOR_WAIT_FOR_STOP_VOLUME_EXIT = 5,
        DOOR_RETURN_TO_ORIGIN = 6
    };
    MovingMobyVars *vars = (MovingMobyVars *)moby->pvars;
    Vec4 offset;
    Vec4 next;
    f32 step;
    f32 magnitude;
    f32 travel;
    s32 reversed;
    s32 next_state;

    reversed = moby->oclass == 0xc4 || moby->oclass == 0x7a6;
    if (vars->trigger == -1 &&
        (vars->start_volume == -1 || vars->stop_volume == -1)) {
        mark_moby_for_removal(moby);
        return;
    }

    switch (moby->state) {
    case DOOR_INITIALIZE:
        vars->origin.q = *(u128 *)&moby->pos;
        if (vars->trigger != 0) {
            moby->state = 1;
            return;
        }
        moby->state = 4;
        return;
    case DOOR_WAIT_FOR_TRIGGER:
        if (((struct Moby *)(D_L15_0015FFD8_m + vars->trigger * 0x100))->state != 4)
            return;
        if (reversed)
            allocate_voice_for_target_entry(0, 0, (int)moby);
        moby->state = 2;
        break;
    case DOOR_MOVE_AWAY_FROM_ORIGIN:
        {
        union { u32 bits; f32 value; } triple;
        f32 increment;
        f32 scale;
        travel = vars->travel;
        increment = (moby->scale * 3.0f / moby->pclass->scale) / 3.0f * frame_time;
        increment = reversed ? -increment : increment;
        vars->travel = travel + increment;
        offset.f[0] = fast_cos_door(moby->rot.z) * vars->travel;
        offset.f[1] = fast_sin_door(moby->rot.z) * vars->travel;
        offset.f[2] = 0.0f;
        add_vector_door(&next, &vars->origin, &offset);
        *(u128 *)&moby->pos = next.q;
        magnitude = FUN_001f99c0(vars->travel);
        scale = moby->scale;
        triple.bits = 0x40400000;
        __builtin_memcpy(&step, &triple.bits, sizeof(step));
        step = step + 0.0f;
        if (!(scale * step / moby->pclass->scale < magnitude))
            return;
        if (vars->trigger != -1) {
            moby->state = 3;
            return;
        }
        moby->state = 5;
        return;
        }
    case DOOR_WAIT_FOR_START_VOLUME_ENTRY:
        vars->travel = 0.0f;
        if (!is_point_inside_clip_volume(&hero.motion.pos, vars->start_volume))
            return;
        if (reversed)
            allocate_voice_for_target_entry(0, 0, (int)moby);
        moby->state = 2;
        break;
    case DOOR_WAIT_FOR_STOP_VOLUME_EXIT:
        if (is_point_inside_clip_volume(&hero.motion.pos, vars->stop_volume))
            return;
        if (reversed)
            allocate_voice_for_target_entry(0, 0, (int)moby);
        moby->state = 6;
        break;
    case DOOR_RETURN_TO_ORIGIN:
        {
        f32 old_travel;
        f32 new_travel;
        magnitude = FUN_001f99c0(vars->travel);
        step = (moby->scale * 3.0f / moby->pclass->scale) / 3.0f * frame_time;
        if (step < magnitude) {
            old_travel = vars->travel;
            if (reversed) {
                f32 reverse_step = -step;
                new_travel = old_travel - reverse_step;
            } else {
                new_travel = old_travel - step;
            }
            vars->travel = new_travel;
        } else {
            vars->travel = 0.0f;
        }
        offset.f[0] = fast_cos_door(moby->rot.z) * vars->travel;
        offset.f[1] = fast_sin_door(moby->rot.z) * vars->travel;
        offset.f[2] = 0.0f;
        add_vector_door(&next, &vars->origin, &offset);
        *(u128 *)&moby->pos = next.q;
        if (vars->travel != 0.0f)
            return;
        moby->state = 4;
        break;
        }
    }
}
extern int interpolate_color_2a3138(float, int, int) __asm__("FUN_001fa6e0");
extern int D_L15_00161AB4 __attribute__((sda));
extern int D_L15_00161AB8 __attribute__((sda));
extern short D_L15_00161ABC_gp __asm__("D_L15_00161ABC") __attribute__((sda));
void update_sentrybot_alarm_effect(void *moby) __asm__("FUN_L15_002cbc30");

extern void update_sentrybot_alarm_effect(void *) __asm__("FUN_L15_002cbc30");

/* Advances the animation state, pulses the color, and updates the shared effect. */
void sentrybot_alarm_update(unsigned char *moby) __asm__("FUN_L15_002cb4c8");
void sentrybot_alarm_update(unsigned char *moby) {
    float position[4];
    float step;
    int color;
    int mode;
    int *data = *(int **)(moby + 0x78);
    switch (moby[0x20]) {
    case 0:
        mode = data[0];
        D_L15_00161AB4 = 0;
        D_L15_00161AB8 = 0;
        *(int *)&D_L15_00161ABC_gp = 0;
        if (mode == 2) {
            moby[0x20] = 6;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, 0);
        } else {
            moby[0x20] = 3;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, 0);
        }
        break;
    case 1:
        color = interpolate_color_2a3138((fast_sin_door(*(float *)(data + 5)) + 1.0f) * 0.5f, 0x801414bc, 0x801414dc);
        step = frame_time;
        *(int *)(moby + 0x90) = color;
        *(float *)(data + 5) = fast_add_rotations(*(float *)(data + 5), step * 6.2831855f);
        if (data[2] == 0) goto update;
        if (D_L15_0015F5C4 != 2) {
            if (data[0] == 1) {
                moby[0x20] = 6;
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(30));
            } else {
                moby[0x20] = 5;
            }
        }
        break;
    case 2:
        if (moby[0x70] & 2) {
            moby[0x20] = 3;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case 3:
        if (data[2] == 0) goto update;
        if (D_L15_0015F5C4 != 2) {
            moby[0x20] = 4;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        }
        break;
    case 4:
        if (moby[0x70] & 2) {
            if (data[2] == 0 || D_L15_0015F5C4 == 2) goto stop_animation;
            if (data[0] == 1) {
                moby[0x20] = 7;
                if (moby[0x53] != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            } else {
                if (moby[0x53] != 8)
                    blend_moby_animation(moby, 8, 0, scale_game_frames(10));
                moby[0x20] = 5;
            }
        }
        break;
stop_animation:
        if (moby[0x53] != 8)
            blend_moby_animation(moby, 8, 0, scale_game_frames(10));
        moby[0x20] = 10;
        break;
    case 5:
        if ((moby[0x70] & 2) && moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        color = interpolate_color_2a3138((fast_sin_door(*(float *)(data + 5)) + 1.0f) * 0.5f, 0x801414dc, 0x8004048c);
        step = frame_time;
        *(int *)(moby + 0x90) = color;
        *(float *)(data + 5) = fast_add_rotations(*(float *)(data + 5), step * 12.566371f);
        if (tick_countdown_32_alt(data + 6) || D_L15_0015F5C4 == 2) {
            set_sentrybot_alarm_duration((char *)moby, 0);
            data[2] = 0;
            if (moby[0x53] != 8)
                blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            moby[0x20] = 10;
        }
        break;
    case 6:
        if (data[2] == 0) goto update;
        moby[0x20] = 7;
        if (moby[0x53] != 5)
            blend_moby_animation(moby, 5, 0, scale_game_frames(10));
        break;
    case 7:
        if (moby[0x70] & 2) {
            moby[0x20] = 8;
            if (moby[0x53] != 6)
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
        }
        break;
    case 8: {
        float *pos = (float *)(moby + 0x10);
        float *hero_pos = hero.motion.pos.f;
        if (FUN_001f9b80(pos, hero_pos) < 16.0f && D_L15_0015F5C4 != 2 && tick_countdown_32_alt(data + 3)) {
            qcopy(position, pos);
            position[2] += 1.0f;
            if (collision_line(position, hero_pos + 20, 2, 0, 0) == 0 && activate_first_inactive_group_moby(data[1], pos))
                data[3] = data[4];
        }
        if (tick_countdown_32_alt(data + 6) || D_L15_0015F5C4 == 2) {
            set_sentrybot_alarm_duration((char *)moby, 0);
            data[2] = 0;
            moby[0x20] = 9;
            if (moby[0x53] != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(10));
        }
        break;
    }
    case 9:
        if (moby[0x70] & 2) {
            if (data[0] == 2) {
                moby[0x20] = 6;
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 10;
                if (moby[0x53] != 8)
                    blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            }
        }
        break;
    case 10:
        if (moby[0x70] & 2) {
            moby[0x20] = 2;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        break;
    }
    if (data[2] != 0) D_L15_00161AB8 = 1;
update:
    if (D_L15_00161AB4 != D_L15_0015F5CC) {
        D_L15_00161AB4 = D_L15_0015F5CC;
        update_sentrybot_alarm_effect(moby);
        D_L15_00161AB8 = 0;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCE50), where it is exact; names translated to the US level program. */

extern int D_L15_0015FFD8_m __asm__("D_L15_0015FFD8");
extern int D_L15_001ABE40_q[] __asm__("D_L15_001ABE40") __attribute__((section(".data")));
extern s32 scale_game_frames_q(s32) __asm__("FUN_001f96f8");
extern short D_L15_00161ABC_gp __asm__("D_L15_00161ABC") __attribute__((sda));

void set_sentrybot_alarm_duration(char *moby, int duration_frames) {
    char *moby_data = *(char **)(moby + 0x78);
    int flag;
    short *group_moby_entry;
    if (duration_frames == -1) {
        *(int *)&D_L15_00161ABC_gp = 0;
        duration_frames = 0;
    }
    flag = duration_frames != 0;
    if (((unsigned char *)moby)[0x21] == 0xFF) {
        *(int *)(moby_data + 0x18) = scale_game_frames_q(duration_frames);
        *(int *)(moby_data + 8) = flag;
        return;
    }
    group_moby_entry = (short *)D_L15_001ABE40_q[((unsigned char *)moby)[0x21]];
    if (group_moby_entry) {
        do {
            char *listed_moby = (char *)(((*(unsigned short *)group_moby_entry & 0x7FFF) << 8) + D_L15_0015FFD8_m);
            if (*(short *)(listed_moby + 0xA6) == 0x198) {
                char *listed_moby_data = *(char **)(listed_moby + 0x78);
                *(int *)(listed_moby_data + 8) = flag;
                *(int *)(listed_moby_data + 0x18) = scale_game_frames_q(duration_frames);
            }
        } while (*group_moby_entry++ >= 0);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCF30), where it is exact; names translated to the US level program. */

void blend_moby_animation_u(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void prepare_sentrybot_alarm(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    if (moby[0x20] == 1) {
        moby[0x20] = 2;
        if (moby[0x53] != 1) {
            blend_moby_animation_u(moby, 1, 0, scale_game_frames(10));
        }
    }
    if (moby[0x20] == 3) {
        if (*(int *)(data + 8) == 0) {
            *(int *)(data + 0x18) = scale_game_frames(0x78);
        }
    }
}
#include "sda.h"

/* per-frame update of the level's looping effect: ramp the gauge, then start, run or stop the draw callback */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCFC0), where it is exact; names translated to the US level program. */

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L15_002cbac0_u() __asm__("FUN_L15_002cbac0");
extern int D_0015ED84;
extern s32 D_L15_00161AB8 ;
extern short D_L15_00161ABC_gp __asm__("D_L15_00161ABC") __attribute__((sda));
extern s32 D_L15_00161AC0_gp __asm__("D_L15_00161AC0") __attribute__((sda));
extern void emit_sine_modulated_rgba_draw_packet(void) __asm__("FUN_L15_002cbd88");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");

extern int sentrybot_alarm_voice_slot_index_gp __asm__("D_L15_00161AC4") __attribute__((sda));
extern short D_L15_00161AC8 __attribute__((sda));
void update_sentrybot_alarm_effect(void *moby) {
    struct Hero *base = &hero;
    if (base->state.current == 0x65) {
        approach_value(&D_L15_00161ABC_gp, 0.0f, frame_time * 2);
        FUN_L15_002cbac0_u(moby, 0);
    } else if (*(int *)&D_L15_00161AB8 != 0 &&
               (D_0015ED84 != 0xF || 240.0f < base->motion.pos.f[1])) {
        approach_value(&D_L15_00161ABC_gp, 1.0f, frame_time * 2);
    } else {
        approach_value(&D_L15_00161ABC_gp, 0.0f, frame_time * 2);
    }
    if (*(float *)&D_L15_00161ABC_gp != 0.0f) {
        *(int *)&D_L15_00161AC0_gp = 1;
        if (voice_is_playing(*(void **)&D_L15_00161AC8, sentrybot_alarm_voice_slot_index_gp) == 0) {
            sentrybot_alarm_voice_slot_index_gp = allocate_voice_for_target_entry(5, 0x11, (int)moby);
            *(void **)&D_L15_00161AC8 = moby;
        }
        enqueue_callback_list_1_alt(emit_sine_modulated_rgba_draw_packet, moby);
    } else {
        if (voice_is_playing(*(void **)&D_L15_00161AC8, sentrybot_alarm_voice_slot_index_gp) != 0) {
            release_voice_slot(sentrybot_alarm_voice_slot_index_gp);
            *(int *)&D_L15_00161AC8 = 0;
            sentrybot_alarm_voice_slot_index_gp = -1;
        }
        *(int *)&D_L15_00161AC0_gp = 0;
    }
}

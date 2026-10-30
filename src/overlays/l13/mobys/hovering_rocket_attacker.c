/* Level 13 hovering rocket attacker moby class. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/l13/mobys/attached_rocket.h"
#include "rnc/overlay/l13/mobys/hovering_rocket_attacker_globals.h"

extern char D_0013F350[];

extern f32 random_angle_radians(void) __asm__("FUN_00213308");

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");

extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");

extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern void func_L00_0025F4A8_alt(void *,
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
                                  float,
                                  int,
                                  float,
                                  int,
                                  int,
                                  int,
                                  int) __asm__("FUN_L00_0025e450");

extern char *FUN_L00_0025a420(void *, int, int);

void update_hovering_rocket_attacker_common(unsigned char *moby) __asm__("FUN_L13_002c2d30");

extern void update_hovering_rocket_attacker_common(unsigned char *) __asm__("FUN_L13_002c2d30");

extern float hovering_rocket_attacker_launch_vector_speed_gp __asm__("D_L13_0016149C")
    __attribute__((sda));

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

void update_hovering_rocket_attacker(unsigned char *moby) __asm__("FUN_L13_002c25b8");

void update_hovering_rocket_attacker(unsigned char *moby) {
    enum {
        UPDATE_INITIALIZE = 0x0,
        TRACK_TARGET_BEFORE_ATTACK = 0x2,
        UPDATE_ATTACK_SEQUENCE = 0x4,
        WAIT_TO_MOVE_TO_TABLE_POINT = 0x5,
        MOVE_TO_TABLE_POINT = 0x6,
        WAIT_TO_RESUME_ATTACK_SEQUENCE = 7,
        EMIT_EFFECT_AND_REMOVE = 0x8
    };
    enum {
        START_ATTACK_ANIMATION_SEQUENCE = 0,
        WAIT_FOR_ATTACK_START_ANIMATION = 1,
        WAIT_TO_BEGIN_ALTERNATING_FIRE = 2,
        FIRE_ALTERNATING_SHOTS = 0x3,
        WAIT_TO_ADVANCE_ATTACK_EXIT_SEQUENCE = 4,
        SELECT_ATTACK_RESTART_OR_EXIT = 5,
        WAIT_TO_COMPLETE_ATTACK_EXIT_SEQUENCE = 6
    };
    unsigned char *moby_data = ((unsigned char *)MOBY(moby)->pvars);
    unsigned char *target;
    float          scratch_vector[4], shot_direction_vector[4], z[4];
    float          distance_or_target_yaw;
    float         *moby_yaw;
    float         *moby_position;
    int            index;
    update_hovering_rocket_attacker_common(moby);
    target = *(unsigned char **)(moby_data + 0xB0);
    switch (moby[0x20]) {
    case UPDATE_INITIALIZE:
        qcopy(moby_data + 0xC0, moby + 0x10);
        MOBY(moby)->flags |= 0x5000;
        *(float *)(moby_data + 0x20) = 4.0f;
        *(short *)(moby_data + 0x24) = 4;
        moby_data[0x28] = 3;
        *(float *)(moby_data + 0xEC) = 15.0f;
        moby_data[0x58] = 50;
        moby_data[0x5A] = 0;
        MOBY(moby)->draw_dist = 255;
        MOBY(moby)->pos.z += 1.0f;
        if (moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, 10);
        moby[0xBC] = 0;
        *(unsigned char **)(moby_data + 0xFC) = create_attached_rocket(moby, 0);
        *(unsigned char **)(moby_data + 0xF8) = create_attached_rocket(moby, 1);
        if (*(int *)(moby_data + 0xD4) != -1)
            moby[0x20] = 5;
        else
            moby[0x20] = 2;
        *(float *)(moby_data + 0x100) = random_angle_radians();
        *(float *)(moby_data + 0x108) = random_angle_radians();
        *(float *)(moby_data + 0x10C) = random_angle_radians();
        *(int *)(moby_data + 0x110) = -1;
        break;
    case WAIT_TO_MOVE_TO_TABLE_POINT:
        if (*(int *)(moby_data + 0xB4) == 2)
            return;
        moby[0xBC] = 0;
        moby[0x20] = 6;
        break;
    case MOVE_TO_TABLE_POINT:
        index = *(int *)(moby_data + 0xD4) * 128;
        moby_position = (float *)&MOBY(moby)->pos;
        z[0] = 0.0f;
        distance_or_target_yaw = fast_vec_dist(moby_position, (char *)l13_cuboids + index + 0x30);
        fast_vec_sub(scratch_vector,
                     (char *)l13_cuboids + *(int *)(moby_data + 0xD4) * 128 + 0x30,
                     moby_position);
        advance_accelerated_scalar(distance_or_target_yaw,
                                   frame_time_sq * 10.0f,
                                   frame_time_sq * 10.0f,
                                   frame_time * 20.0f,
                                   z,
                                   (float *)(moby_data + 0xF0));
        fast_vec_normalize(scratch_vector, scratch_vector, *(float *)(moby_data + 0xF0));
        fast_vec_add(moby_position, moby_position, scratch_vector);
        if (distance_or_target_yaw < 0.1f && *(float *)(moby_data + 0xF0) < 0.01f) {
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 10);
            moby[0xBC] = 0;
            moby[0x20] = 2;
        }
        break;
    case TRACK_TARGET_BEFORE_ATTACK:
        moby_yaw = (float *)&MOBY(moby)->rot.z;
        distance_or_target_yaw = fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                              *(float *)(target + 0x14) - MOBY(moby)->pos.y);
        approach_angle(moby_yaw,
                       (float *)(moby_data + 0xDC),
                       distance_or_target_yaw,
                       frame_time_sq * 4.712389f,
                       frame_time_sq * 4.712389f,
                       frame_time * 10.12291f);
        if (*(int *)(moby_data + 0xB4) == 2)
            return;
        *(float *)(moby_data + 0xEC) = 15.0f;
        moby[0xBC] = 1;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, 0);
        moby[0x20] = 4;
        break;
    case UPDATE_ATTACK_SEQUENCE:
        moby_yaw = (float *)&MOBY(moby)->rot.z;
        distance_or_target_yaw = fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                              *(float *)(target + 0x14) - MOBY(moby)->pos.y);
        approach_angle(moby_yaw,
                       (float *)(moby_data + 0xDC),
                       distance_or_target_yaw,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 25.132742f,
                       frame_time * 6.2831855f);
        switch (moby[0xBC]) {
        case WAIT_FOR_ATTACK_START_ANIMATION:
            if (!(moby[0x70] & 2))
                return;
            moby[0xBC] = 2;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, 2);
            *(int *)(moby_data + 0xE8) = 0;
            break;
        case WAIT_TO_BEGIN_ALTERNATING_FIRE:
            if (!(moby[0x70] & 2))
                return;
            moby[0xBC] = 3;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, 2);
            break;
        case FIRE_ALTERNATING_SHOTS:
            if (fast_dec_timer(moby_data + 0xE8)) {
                *(int *)(moby_data + 0xE8) = scale_game_frames(60);
                if (*(int *)(moby_data + 0xF4) != 0)
                    moby_point_position(moby, 1, scratch_vector);
                else
                    moby_point_position(moby, 0, scratch_vector);
                fast_vec_sub(shot_direction_vector, target + 0x10, scratch_vector);
                fast_vec_normalize(shot_direction_vector,
                                   shot_direction_vector,
                                   hovering_rocket_attacker_launch_vector_speed_gp * frame_time);
                if (*(int *)(moby_data + 0xF4) != 0) {
                    if (*(void **)(moby_data + 0xF8)) {
                        launch_attached_rocket(*(void **)(moby_data + 0xF8),
                                               shot_direction_vector,
                                               target,
                                               *(float *)(moby_data + 0xEC));
                        allocate_voice_for_target_entry(0, 0, (int)moby);
                    }
                    *(unsigned char **)(moby_data + 0xF8) = create_attached_rocket(moby, 1);
                    *(int *)(moby_data + 0xF4) = 0;
                } else {
                    if (*(void **)(moby_data + 0xFC)) {
                        launch_attached_rocket(*(void **)(moby_data + 0xFC),
                                               shot_direction_vector,
                                               target,
                                               *(float *)(moby_data + 0xEC));
                        allocate_voice_for_target_entry(0, 0, (int)moby);
                    }
                    *(unsigned char **)(moby_data + 0xFC) = create_attached_rocket(moby, 0);
                    *(int *)(moby_data + 0xF4) = 1;
                }

                *(float *)(moby_data + 0xEC) += 5.0f;
                if (vector_distance_xy(moby + 0x10, target + 0x10) < *(float *)(moby_data + 0xEC))
                    *(float *)(moby_data + 0xEC) = vector_distance_xy(moby + 0x10, target + 0x10);
            }
            if (*(int *)(moby_data + 0xB4) != 2)
                return;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, 2);
            moby[0xBC] = 4;
            break;
        case WAIT_TO_ADVANCE_ATTACK_EXIT_SEQUENCE:
            if (!(moby[0x70] & 2))
                return;
            moby[0xBC] = 5;
            if (moby[0x53] != 5)
                blend_moby_animation(moby, 5, 0, 2);
            break;
        case SELECT_ATTACK_RESTART_OR_EXIT:
            if (*(int *)(moby_data + 0xB4) == 2) {
                if (moby[0x53] != 6)
                    blend_moby_animation(moby, 6, 0, 2);
                moby[0xBC] = 6;
            } else {
                moby[0xBC] = 2;
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, 2);
            }
            break;
        case WAIT_TO_COMPLETE_ATTACK_EXIT_SEQUENCE:
            if (!(moby[0x70] & 2))
                return;
            if (*(int *)(moby_data + 0xB4) == 2) {
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, 2);
                moby[0x20] = 2;
                moby[0xBC] = 0;
                break;
            }
        case START_ATTACK_ANIMATION_SEQUENCE:
            moby[0xBC] = 1;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, 2);
            break;
        }
        break;
    case EMIT_EFFECT_AND_REMOVE:
        clear_u64_value(scratch_vector);
        func_L00_0025F4A8_alt(moby,
                              scratch_vector,
                              moby + 0x10,
                              0.0f,
                              0.0f,
                              10,
                              3,
                              16,
                              4.0f,
                              2.0f,
                              9.0f,
                              1.0f,
                              2,
                              15.0f,
                              1,
                              1,
                              -1,
                              0);
        if (*(void **)(moby_data + 0xF8))
            mark_moby_for_removal(*(void **)(moby_data + 0xF8));
        if (*(void **)(moby_data + 0xFC))
            mark_moby_for_removal(*(void **)(moby_data + 0xFC));
        mark_moby_for_removal(moby);
        break;
    case WAIT_TO_RESUME_ATTACK_SEQUENCE:
        if (!(moby[0x70] & 2))
            return;
        if (moby[0x53] != moby[0xBC])
            blend_moby_animation(moby, moby[0xBC], 0, 10);
        moby[0x20] = 4;
        break;
    }
}

extern int FUN_L13_002c2d30_update(void *, int) __asm__("FUN_L00_0028d8c0");

extern int FUN_L13_002c2d30_allocate(int, int, void *) __asm__("FUN_L13_00297fa0");

extern void update_moby_vertical_oscillation(float, float, void *, float *, float *) __asm__("FUN_L00_00262b00");

extern void update_moby_rotation_oscillation(void *, void *, void *, float, float, float) __asm__("FUN_L00_00262b80");

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern void FUN_L00_0024f7c8(void *, int, void *);

extern int FUN_L00_00257b90(int, int);

extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");

extern void FUN_L00_00257470(struct Moby *, int, int);

extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern int FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);

extern char *D_L13_001B07B0[];

#include "rnc/overlay/level_labels.h"

extern float hovering_rocket_attacker_scale_multiplier_gp __asm__("D_L13_00161490")
    __attribute__((sda));

void update_hovering_rocket_attacker_common(unsigned char *moby) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    unsigned char *moby_data;
    char          *hit_record;
    int            hit_reaction_code;
    int            flag;
    int            target_selection_status;
    float          damage_amount;
    float          downward_effect_z;
    float          fl;
    char           point_2_position[16];
    char           point_3_position[16];
    char           downward_effect_vector[16];
    float          z[4];

    moby_data = ((unsigned char *)MOBY(moby)->pvars);
    if (moby[0x20] == 0)
        return;
    if (*(int *)(moby_data + 0x110) == -1 ||
        FUN_L13_002c2d30_update(moby, *(int *)(moby_data + 0x110)) == 0) {
        *(int *)(moby_data + 0x110) = -1;
        *(int *)(moby_data + 0x110) = FUN_L13_002c2d30_allocate(1, 4, moby);
    }
    update_moby_vertical_oscillation(0.5f,
                                     frame_time * 1.39626336f,
                                     moby,
                                     (float *)(moby_data + 0x100),
                                     (float *)(moby_data + 0x104));
    update_moby_rotation_oscillation(moby,
                                     moby_data + 0x108,
                                     moby_data + 0x10C,
                                     0.104719758f,
                                     frame_time * 0.314159274f,
                                     frame_time * 0.366519153f);
    MOBY(moby)->pos.x = *(float *)(moby_data + 0xC0) + fast_sin(*(float *)(moby_data + 0x108));
    MOBY(moby)->pos.y = *(float *)(moby_data + 0xC4) + fast_sin(*(float *)(moby_data + 0x10C));
    *(OvlQuad *)z = 0;
    fl = random_float_between(4.5f, 5.5f);
    downward_effect_z = -(fl * frame_time);
    z[2] = downward_effect_z;
    *(OvlQuad *)downward_effect_vector = *(OvlQuad *)z;
    FUN_L00_0024f7c8(moby, 2, point_2_position);
    FUN_L00_0024f7c8(moby, 3, point_3_position);
    if (frame_count & 1) {
        spawn_effect(point_2_position,
                     downward_effect_vector,
                     0xCF0000FF,
                     0xCF,
                     scale_game_frames(FUN_L00_00257b90(0x12, 0x16)),
                     0x32,
                     -0xA,
                     1);
        spawn_effect(point_3_position,
                     downward_effect_vector,
                     0xCF0000FF,
                     0xCF,
                     scale_game_frames(FUN_L00_00257b90(0x12, 0x16)),
                     0x32,
                     -0xA,
                     1);
    } else {
        spawn_effect(point_2_position,
                     downward_effect_vector,
                     0x6000FFFF,
                     0x80,
                     scale_game_frames(FUN_L00_00257b90(0x14, 0x18)),
                     0x64,
                     -0xA,
                     1);
        spawn_effect(point_3_position,
                     downward_effect_vector,
                     0x6000FFFF,
                     0x80,
                     scale_game_frames(FUN_L00_00257b90(0x14, 0x18)),
                     0x64,
                     -0xA,
                     1);
    }
    {
        int ra = FUN_L00_00257b90(0xA, 0x19);
        int rb = FUN_L00_00257b90(0xA, 0xF);
        spawn_effect(point_2_position,
                     downward_effect_vector,
                     0xEFFF7F4F,
                     0xFF0000,
                     scale_game_frames(rb),
                     ra,
                     -ra,
                     1);
        ra = FUN_L00_00257b90(0xA, 0x19);
        rb = FUN_L00_00257b90(0xA, 0xF);
        spawn_effect(point_3_position,
                     downward_effect_vector,
                     0xEFFF7F4F,
                     0xFF0000,
                     scale_game_frames(rb),
                     ra,
                     -ra,
                     1);
    }
    MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                        hovering_rocket_attacker_scale_multiplier_gp;
    damage_amount = 0.0f;
    hit_record = FUN_L00_0025a420(moby, 0x330000, 0);
    hit_reaction_code = resolve_moby_hit_response(
        moby, hit_record, moby_data + 0x20, 0, &flag, &damage_amount, 0, 4);
    if (hit_record != 0) {
        char *hit_moby = *(char **)(hit_record + 0x20);
        if (hit_moby != 0 && MOBY(hit_moby)->oclass == 0x4D1) {
            flag = 1;
            damage_amount = 0.0f;
        }
    }
    if (flag != 1) {
        *(float *)(moby_data + 0x20) = *(float *)(moby_data + 0x20) - damage_amount;
        if (moby[0x53] != 7)
            blend_moby_animation(moby, 7, 0, 5);
        if (*(float *)(moby_data + 0x20) <= 0.0f) {
            hit_reaction_code = 1;
        } else {
            moby[0x20] = 7;
        }
        switch (hit_reaction_code) {
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
            moby_data[0x67] = 0xFA;
            break;
        case HIT_REACTION_LOW_INTERPOLATION_TARGET:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
            moby_data[0x67] = 0x78;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_REMOVE:
            FUN_L00_00257470(moby, 0, -1);
            moby[0x20] = 8;
            moby_data[0x67] = 0xF0;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_NO_HIT:
        default:
            break;
        }
        trooper_anim_update(moby, moby_data + 0x60);
    }
    moby[0xA4] = 0xFF;
    trooper_anim_finish(moby, moby_data + 0x60);
    if (moby[0x20] == 4) {
        *(float *)(moby_data + 0xD8) = 50.0f;
    } else {
        *(float *)(moby_data + 0xD8) = 40.0f;
    }
    {
        char *target_selection_point_table = D_L13_001B07B0[*(int *)(moby_data + 0xD0)];
        target_selection_status = FUN_L00_0025ff38(*(float *)(moby_data + 0xD8),
                                                   moby,
                                                   moby_data + 0x70,
                                                   0,
                                                   0,
                                                   target_selection_point_table + 0x10,
                                                   *(int *)target_selection_point_table);
    }
    if (target_selection_status != 2) {
        if (*(float *)(moby_data + 0xD8) < vector_distance_xy(moby + 0x10, moby_data + 0x70) ||
            8.0f < absolute_float(MOBY(moby)->pos.z - *(float *)(moby_data + 0x78))) {
            *(int *)(moby_data + 0xB4) = 2;
        } else if (moby[0x20] != 5 &&
                   collision_line(moby + 0x10, D_0013F350 + 0xD0, 2, moby, 0) != 0) {
            *(int *)(moby_data + 0xB4) = 2;
        }
    }
    if (*(int *)(moby_data + 0xB0) == 0) {
        *(int *)(moby_data + 0xB0) = D_001413D0[0];
    }
}

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

float absolute_float(float input) __asm__("func_001F99C0");

extern float random_angle_radians(void) __asm__("FUN_00213308");

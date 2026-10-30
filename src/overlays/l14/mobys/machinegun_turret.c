/* Level 14 machinegun turret moby class: target selection, update and hit reactions. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qzero.h"
#include "qcopy.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/rendering/clip_volume.h"
#include "sda.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/l14/mobys/flying_bolt.h"
#include "rnc/overlay/level_labels.h"

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

extern void FUN_L00_00257470(void *, int, int);

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern int D_L14_0015F5CC MACRO_ADDR;

extern float machinegun_turret_retraction_depth_gp __asm__("D_L14_00161508") __attribute__((sda));

extern int D_L14_0016150C __attribute__((sda));

extern float D_L14_00161510 __attribute__((sda));

extern float machinegun_turret_aiming_phase_increment_gp __asm__("D_L14_00161514")
    __attribute__((sda));

extern float machinegun_turret_bolt_launch_vector_magnitude_gp __asm__("D_L14_0016151C")
    __attribute__((sda));

extern float machinegun_turret_center_bolt_spawn_base_offset_magnitude_gp __asm__("D_L14_00161520")
    __attribute__((sda));

extern float
    machinegun_turret_off_center_bolt_spawn_base_offset_magnitude_gp __asm__("D_L14_00161524")
        __attribute__((sda));

extern float machinegun_turret_bolt_spawn_launch_axis_offset_magnitude_gp __asm__("D_L14_00161528")
    __attribute__((sda));

extern float machinegun_turret_bolt_spawn_symmetric_offset_magnitude_gp __asm__("D_L14_0016152C")
    __attribute__((sda));

extern int machinegun_turret_firing_cadence_divisor_gp __asm__("D_L14_00161530")
    __attribute__((sda));

extern int machinegun_turret_target_wait_timeout_frames_gp __asm__("D_L14_00161534")
    __attribute__((sda));

extern int debug_print() __asm__("FUN_001e93b0");

extern char D_L14_001FBD50[];

extern char D_L14_001FBD78[];

extern void func_L14_002B5590_2B4E40(unsigned char *m) __asm__("FUN_L14_002b4340");

extern float func_001F9FA8_2B4E40(float) __asm__("FUN_001f9de0");

extern void func_L00_00251328_2B4E40(void *, int, int, int) __asm__("FUN_L00_002502f0");

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

extern int func_001F9850_2B4E40(int) __asm__("FUN_001f96f8");

extern float func_001FA888_2B4E40(int) __asm__("FUN_001fa6c0");

extern void func_001F9C30_2B4E40(void *, void *, float) __asm__("FUN_001f9a68");

extern void func_L00_001FF4B0_2B4E40(void *, void *, float) __asm__("FUN_001f9bf8");

extern short       D_L14_001B0F30[] NOT_SDA;
extern char       *D_L14_001DE5C0[] NOT_SDA;
extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
int                select_moby_by_xy_distance(char *selection_reference,
                                              char *selection_output,
                                              float best) __asm__("FUN_L14_00268038");

int select_moby_by_xy_distance(char *selection_reference, char *selection_output, float best) {
    struct Hero *hero = &hero_lui;
    char        *selected_moby;
    char        *candidate_moby;
    int          selection_status;
    int          i;
    int          j;
    float        candidate_metric;

    selected_moby = ((char *)hero->moby);
    selection_status = 0;
    if (hero->state.control_mode == 0x18 || hero->state.current == 0x72) {
        selected_moby = 0;
        selection_status = 2;
    }
    for (i = 1; i <= D_L14_001B0F30[0]; i++) {
        candidate_moby = D_L14_0015FFD8 + (D_L14_001B0F30[i] << 8);
        if (candidate_moby == 0)
            continue;
        if (MOBY(candidate_moby)->oclass != 0xCB && MOBY(candidate_moby)->oclass != 0x76C)
            continue;
        if (MOBY(candidate_moby)->state == 0xFE)
            continue;
        if (MOBY(candidate_moby)->state == 0xFD)
            continue;
        if (MOBY(candidate_moby)->state != 3)
            continue;
        candidate_metric = vector_distance_xy(selection_reference + 0x10, candidate_moby + 0x10);
        if (candidate_metric < best) {
            selected_moby = candidate_moby;
            best = candidate_metric;
            selection_status = 1;
        }
    }
    for (j = 0; j < 20; j++) {
        candidate_moby = D_L14_001DE5C0[j];
        if (candidate_moby == 0)
            continue;
        if (MOBY(candidate_moby)->oclass != 0x10E)
            continue;
        if (MOBY(candidate_moby)->state == 0xFE)
            continue;
        if (MOBY(candidate_moby)->state == 0xFD)
            continue;
        if (MOBY(candidate_moby)->command == 0)
            continue;
        candidate_metric = vector_distance_xy(selection_reference + 0x10, candidate_moby + 0x10);
        if (candidate_metric < best) {
            selected_moby = candidate_moby;
            best = candidate_metric;
            selection_status = 1;
        }
    }
    if (selected_moby != 0) {
        *(char **)(selection_output + 0x40) = selected_moby;
        qcopy(selection_output, selected_moby + 0x10);
        qcopy(selection_output + 0x10, selected_moby + 0x40);
        hero = &hero_lui;
        if (selected_moby == ((char *)hero->moby)) {
            qcopy(selection_output + 0x20, &hero->motion.near_ground_point);
            qcopy(selection_output + 0x30, &hero->motion.body_center);
        } else {
            qcopy(selection_output + 0x30, selection_output);
            *(float *)(selection_output + 0x38) += 0.6f;
            qcopy(selection_output + 0x20, selection_output + 0x30);
        }
    } else {
        *(char **)(selection_output + 0x40) = 0;
        qzero(selection_output);
        qzero(selection_output + 0x10);
        qzero(selection_output + 0x30);
        qzero(selection_output + 0x20);
    }
    *(int *)(selection_output + 0x44) = selection_status;
    return selection_status;
}

void machinegun_turret_update(char *moby) __asm__("FUN_L14_002b3bf0");

void machinegun_turret_update(char *moby) {
    enum {
        TURRET_STATE_INITIALIZE = 0,
        TURRET_STATE_WAIT_FOR_ACTIVATION = 1,
        TURRET_STATE_DEPLOY = 2,
        TURRET_STATE_WAIT_FOR_TARGET = 3,
        TURRET_STATE_AIM_AND_FIRE = 4,
        TURRET_STATE_RETRACT = 5,
        TURRET_STATE_DESTROY = 6
    };
    enum {
        TURRET_SPAWN_CENTER = 0,
        TURRET_SPAWN_UNNEGATED_OFFSET = 1
    };
    char *data;
    float target_point_scratch[4];
    float spawn_position[4];
    float spawn_vector_scratch[4];
    float spawn_position_offset[4];
    float v60[4];
    float target_query_result[4];
    float m80[16];
    float signed_phase_increment;
    float phase_angle, sine_input_angle, sine_scale, target_heading;
    int   scaled_wait_frames_or_target_query_status, rem, sel,
        target_missing_or_outside_waking_cuboid, animation_sequence, aiming_cuboid_index;
    char *aiming_offset_vector;
    char *aiming_base_point;

    data = ((char *)MOBY(moby)->pvars);
    func_L14_002B5590_2B4E40((unsigned char *)moby);
    switch (MOBY(moby)->state) {
    case TURRET_STATE_INITIALIZE:
        *(float *)(data + 0x70) = MOBY(moby)->pos.z;
        *(float *)(data + 0x84) = MOBY(moby)->rot.z;
        *(int *)(data + 0x8C) = 0;
        *(short *)(data + 0x74) = 0;
        if (*(int *)(data + 0x80) == -1) {
            debug_print(D_L14_001FBD50, ((short)MOBY(moby)->save_id));
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)(data + 0x90) == -1) {
            debug_print(D_L14_001FBD78, ((short)MOBY(moby)->save_id));
            mark_moby_for_removal(moby);
            return;
        }
        scaled_wait_frames_or_target_query_status =
            func_001F9850_2B4E40(machinegun_turret_target_wait_timeout_frames_gp);
        *(short *)(data + 0x76) = scaled_wait_frames_or_target_query_status;
        *(unsigned char *)(data + 0x8B) = 0;
        if (*(short *)(data + 0x88) != 0) {
            MOBY(moby)->state = 3;
            *(unsigned char *)(data + 0x8A) = 1;
        } else {
            MOBY(moby)->state = 1;
            MOBY(moby)->pos.z = MOBY(moby)->pos.z - machinegun_turret_retraction_depth_gp;
            MOBY(moby)->collision = 0;
            *(unsigned char *)(data + 0x8A) = 0;
            MOBY(moby)->flags = MOBY(moby)->flags & 0xEFFF;
        }
        *(int *)(data + 0x98) = func_001F9850_2B4E40(*(int *)(data + 0x94));
        return;
    case TURRET_STATE_WAIT_FOR_ACTIVATION:
        if (*(short *)(data + 0x88) == 0) {
            return;
        }
        if (fast_dec_timer(data + 0x98) == 0) {
            return;
        }
        MOBY(moby)->state = 2;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(D_L14_0016150C);
        *(float *)(data + 0x78) = 1.0f / func_001FA888_2B4E40(*(short *)(data + 0x74));
        set_moby_animation(moby, 1, 0);
        MOBY(moby)->anim_speed =
            1.0f / (func_001FA888_2B4E40(*(short *)(data + 0x74)) * D_L14_00161510);
        return;
    case TURRET_STATE_DEPLOY: {
        float t = (float)*(short *)(data + 0x74) * *(float *)(data + 0x78);
        MOBY(moby)->pos.z =
            *(float *)(data + 0x70) - (0.0f + (machinegun_turret_retraction_depth_gp - 0.0f) * t);
    }
        if (MOBY(moby)->anim_flags & 2) {
            if (MOBY(moby)->seq == 1) {
                set_moby_animation(moby, 2, 0);
                MOBY(moby)->anim_speed = 1.0f;
            }
        }
        if (*(short *)(data + 0x74) < func_001F9850_2B4E40(D_L14_0016150C >> 1)) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            MOBY(moby)->flags = MOBY(moby)->flags | 0x1000;
        }
        if (fast_dec_timer_s16(data + 0x74) == 0) {
            return;
        }
        *(short *)(data + 0x76) =
            func_001F9850_2B4E40(machinegun_turret_target_wait_timeout_frames_gp);
        MOBY(moby)->state = 3;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(60);
        MOBY(moby)->pos.z = *(float *)(data + 0x70);
        *(int *)(data + 0x8C) = 0;
        return;
    case TURRET_STATE_WAIT_FOR_TARGET:
        scaled_wait_frames_or_target_query_status =
            select_moby_by_xy_distance(moby, (char *)target_point_scratch, 34.0f);
        if (scaled_wait_frames_or_target_query_status != 2) {
            if (is_point_inside_clip_volume(target_point_scratch, *(int *)(data + 0x90)) != 0) {
                MOBY(moby)->state = 4;
                set_moby_animation(moby, 3, 0);
                *(short *)(data + 0x74) = func_001F9850_2B4E40(120);
                *(short *)(data + 0x9C) = func_001F9850_2B4E40(20);
                return;
            }
        }
        if (*(unsigned char *)(data + 0x8A) != 0) {
            return;
        }
        if (fast_dec_timer_s16(data + 0x76) == 0) {
            return;
        }
        *(unsigned char *)(data + 0x8B) = 1;
        return;
    case TURRET_STATE_AIM_AND_FIRE:
        if (*(short *)(data + 0x9E) != 0) {
            signed_phase_increment = -machinegun_turret_aiming_phase_increment_gp;
        } else {
            signed_phase_increment = machinegun_turret_aiming_phase_increment_gp;
        }
        phase_angle = fast_add_rotations(*(float *)(data + 0x8C), signed_phase_increment);
        *(float *)(data + 0x8C) = phase_angle;
        sine_input_angle =
            fast_add_rotations(phase_angle, *(float *)(data + 0x84) * 0.01745329238474369f);
        aiming_cuboid_index = *(int *)(data + 0x80);
        aiming_offset_vector = (int)l14_cuboids + (aiming_cuboid_index << 7) + 0x10;
        sine_scale = func_001F9FA8_2B4E40(sine_input_angle);
        func_001F9C30_2B4E40(target_point_scratch, aiming_offset_vector, sine_scale);
        aiming_cuboid_index = *(int *)(data + 0x80);
        aiming_base_point = (int)l14_cuboids + (aiming_cuboid_index << 7) + 0x30;
        fast_vec_add(target_point_scratch, target_point_scratch, aiming_base_point);
        target_heading = fast_arc_tan(target_point_scratch[0] - MOBY(moby)->pos.x,
                                      target_point_scratch[1] - MOBY(moby)->pos.y);
        MOBY(moby)->rot.z = target_heading;
        fast_dec_timer_s16(data + 0x9C);
        rem = D_L14_0015F5CC % machinegun_turret_firing_cadence_divisor_gp;
        if (rem == 0) {
            sel = random_integer_below(3);
            switch (sel) {
            case TURRET_SPAWN_CENTER:
                func_L00_001FF4B0_2B4E40(
                    spawn_position,
                    moby + 0xE0,
                    machinegun_turret_center_bolt_spawn_base_offset_magnitude_gp);
                break;
            case TURRET_SPAWN_UNNEGATED_OFFSET:
                func_L00_001FF4B0_2B4E40(
                    spawn_position,
                    moby + 0xE0,
                    machinegun_turret_off_center_bolt_spawn_base_offset_magnitude_gp);
                func_L00_001FF4B0_2B4E40(
                    spawn_position_offset,
                    moby + 0xD0,
                    machinegun_turret_bolt_spawn_symmetric_offset_magnitude_gp);
                fast_vec_add(spawn_position, spawn_position, spawn_position_offset);
                break;
            default:
                func_L00_001FF4B0_2B4E40(
                    spawn_position,
                    moby + 0xE0,
                    machinegun_turret_off_center_bolt_spawn_base_offset_magnitude_gp);
                func_L00_001FF4B0_2B4E40(
                    spawn_position_offset,
                    moby + 0xD0,
                    -machinegun_turret_bolt_spawn_symmetric_offset_magnitude_gp);
                fast_vec_add(spawn_position, spawn_position, spawn_position_offset);
                break;
            }
            func_L00_001FF4B0_2B4E40(spawn_vector_scratch,
                                     moby + 0xC0,
                                     machinegun_turret_bolt_spawn_launch_axis_offset_magnitude_gp);
            fast_vec_add(spawn_position, spawn_position, spawn_vector_scratch);
            fast_vec_add(spawn_position, spawn_position, moby + 0x10);
            func_L00_001FF4B0_2B4E40(spawn_vector_scratch,
                                     moby + 0xC0,
                                     machinegun_turret_bolt_launch_vector_magnitude_gp *
                                         frame_time);
            if (*(short *)(data + 0x9C) == 0) {
                func_L00_00251328_2B4E40(
                    spawn_flying_bolt(spawn_position, spawn_vector_scratch, (int)moby, 1),
                    0xE0,
                    0xE0,
                    0xE0);
            }
        }
        scaled_wait_frames_or_target_query_status =
            select_moby_by_xy_distance(moby, (char *)target_query_result, 34.0f);
        target_missing_or_outside_waking_cuboid =
            (scaled_wait_frames_or_target_query_status == 2 ||
             is_point_inside_clip_volume(target_query_result, *(int *)(data + 0x90)) == 0);
        if (target_missing_or_outside_waking_cuboid == 0) {
            return;
        }
        if (fast_dec_timer_s16(data + 0x74) == 0) {
            return;
        }
        if ((MOBY(moby)->anim_flags & 2) == 0) {
            return;
        }
        animation_sequence = MOBY(moby)->seq;
        if (animation_sequence != 3) {
            return;
        }
        *(short *)(data + 0x76) =
            func_001F9850_2B4E40(machinegun_turret_target_wait_timeout_frames_gp);
        MOBY(moby)->state = animation_sequence;
        set_moby_animation(moby, 2, 0);
        MOBY(moby)->anim_speed = 1.0f;
        return;
    case TURRET_STATE_RETRACT: {
        float t = (float)*(short *)(data + 0x74) * *(float *)(data + 0x78);
        MOBY(moby)->pos.z =
            *(float *)(data + 0x70) - (machinegun_turret_retraction_depth_gp +
                                       (0.0f - machinegun_turret_retraction_depth_gp) * t);
        MOBY(moby)->rot.z =
            *(float *)(data + 0x84) + (*(float *)(data + 0x8C) - *(float *)(data + 0x84)) * t;
    }
        if (MOBY(moby)->anim_flags & 2) {
            if (MOBY(moby)->seq == 4) {
                set_moby_animation(moby, 0, 0);
                MOBY(moby)->anim_speed = 1.0f;
            }
        }
        if (*(short *)(data + 0x74) < func_001F9850_2B4E40(D_L14_0016150C >> 1)) {
            MOBY(moby)->collision = 0;
        }
        if (fast_dec_timer_s16(data + 0x74) == 0) {
            return;
        }
        MOBY(moby)->rot.z = *(float *)(data + 0x84);
        MOBY(moby)->state = 1;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(60);
        *(short *)(data + 0x88) = 0;
        *(int *)(data + 0x98) = func_001F9850_2B4E40(*(int *)(data + 0x94));
        MOBY(moby)->pos.z = *(float *)(data + 0x70) - machinegun_turret_retraction_depth_gp;
        MOBY(moby)->flags = MOBY(moby)->flags & 0xEFFF;
        return;
    case TURRET_STATE_DESTROY:
        spawn_explosion(moby,
                        data + 0x40,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        5,
                        2,
                        4,
                        2.0f,
                        1.0f,
                        9.0f,
                        0,
                        1.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    default:
        return;
    }
}

void process_machinegun_turret_hit_reactions(struct Moby *moby) __asm__("FUN_L14_002b4340");

void process_machinegun_turret_hit_reactions(struct Moby *moby) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DESTROY = 2,
        HIT_REACTION_PLAY_HIT_ANIMATION = 3,
        HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION = 4,
        HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION = 5,
        HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH = 6,
        HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION = 7,
        HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH = 8,
        HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH = 9,
        HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION = 10,
        HIT_REACTION_NONE = 11
    };
    unsigned char *moby_vars = moby->pvars;
    int            loc0;
    float          damage_amount = 0.0f;
    char          *hit_record;
    int            k;
    float          remaining_health;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0) {
        if (*(int *)(hit_record + 0x20) != 0) {
            int hit_object_class = *(short *)(*(int *)(hit_record + 0x20) + 0xA6);
            if (hit_object_class == moby->oclass || hit_object_class == 0x2A9)
                hit_record = 0;
        }
    }
    k = hit_reaction(moby, hit_record, moby_vars + 0x20, 0, &loc0, &damage_amount, 0, 4);
    if (loc0 != 1 && moby->state != 6) {
        remaining_health = *(float *)(moby_vars + 0x20) - damage_amount;
        *(float *)(moby_vars + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            k = 1;
        switch (k) {
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        case HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH:
        case HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION:
            moby_vars[0x67] = 100;
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            moby_vars[0x67] = 200;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DESTROY:
            moby->state = 6;
            moby->flags &= 0xEFFF;
            moby_vars[0x67] = 0xF0;
            FUN_L00_00257470(moby, 0, -1);
            break;
        }
        trooper_anim_update(moby, (short *)(moby_vars + 0x60));
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_vars + 0x60);
}

extern int D_L14_0016150C_gp __asm__("D_L14_0016150C") __attribute__((sda));

extern int random_integer_below(int);

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

char D_L14_001FBD50[] = "Turret with no aiming cuboid. UID: %d\n";

char D_L14_001FBD78[] = "Turret with no waking cuboid. UID: %d\n";

int machinegun_turret_target_wait_timeout_frames_gp __asm__("D_L14_00161534") = 0x12c;

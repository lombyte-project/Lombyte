/* Level 17 class 0x558: the shot dreks_fleet_turret fires at the hero, and its spawner. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_globals.h"
#include "rnc/mobys/moby_class_id.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

extern unsigned char *D_L17_001B2A54 NOT_SDA;
/* Spawns one trail part (type 0x50) at the given position; mode picks the colour and size, step how far along the trail it is. */
extern int FUN_001fa6e0_q(int, int, float) __asm__("FUN_001fa6e0");

unsigned char *create_drone_flight_trail_record(char *position_source,
                                                int   mode,
                                                int   step) __asm__("FUN_L17_0026fdd0");

unsigned char *create_drone_flight_trail_record(char *position_source, int mode, int step) {
    unsigned char *flight_trail_record;
    int            remaining_steps;
    if (mode)
        remaining_steps = 5 - step;
    else
        remaining_steps = 0xF - step;
    if (remaining_steps >= 0) {
        flight_trail_record = create_part(0x50);
        if (flight_trail_record != 0) {
            qcopy(flight_trail_record + 0x10, position_source);
            flight_trail_record[9] = truncate_float_to_s32(1.0f) + 0x70;
            *(int *)(flight_trail_record + 0x20) = mode;
            if (mode == 0) {
                int d = 0xF - step;
                *(int *)(flight_trail_record + 4) = 0x60FF3020;
                flight_trail_record[3] = 0x48;
                *(float *)(flight_trail_record + 0xC) = 630000.0f;
                *(short *)(flight_trail_record + 0xA) = d;
                if (step != 0) {
                    float a = convert_integer_to_float((short)d - 1);
                    *(int *)(flight_trail_record + 4) = FUN_001fa6e0_q(
                        0x603F1008,
                        *(int *)(flight_trail_record + 4),
                        a / convert_integer_to_float(*(short *)(flight_trail_record + 0xA)));
                }
            } else {
                int d = 5 - step;
                *(int *)(flight_trail_record + 4) = 0x80D0A080;
                flight_trail_record[3] = 0x44;
                *(float *)(flight_trail_record + 0xC) = 420000.0f;
                *(short *)(flight_trail_record + 0xA) = d;
                if (step != 0) {
                    float a = convert_integer_to_float((short)d - 1);
                    *(int *)(flight_trail_record + 4) = FUN_001fa6e0_q(
                        0x30FFFFFF,
                        *(int *)(flight_trail_record + 4),
                        a / convert_integer_to_float(*(short *)(flight_trail_record + 0xA)));
                }
            }
            flight_trail_record[1] = 0;
            flight_trail_record[8] = random_integer_below(0xFF);
            flight_trail_record[2] = *D_L17_001B2A54;
        }
        return flight_trail_record;
    }
    return 0;
}

/* Updates moby 1368: a drone that drifts toward the hero, tags what it hits and sprays sparks until its timer runs out. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EA8F8), where it is exact; names translated to the US level program. */

extern char D_L17_00174740_lui[] __asm__("D_L17_00174740") NOT_SDA;
extern int  D_0015EEA4;
extern int  FUN_001efa68_q(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int  FUN_L00_001f0d60_q(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int  random_integer_below(int) __asm__("FUN_00213260");
extern unsigned char                      *
spawn_moving_hero_relative_alpha_fade_particle(float        *pos,
                                                                    char         *vel,
                                                                    int           color,
                                                                    unsigned char life,
                                                                    unsigned char b,
                                                                    int           mode,
                                                                    float         scale) __asm__("FUN_L00_002730e0");
extern unsigned char D_00141968_x_q[] __asm__("D_00141968");
extern void FUN_L00_002598b0_q(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void func_L00_0025A8C0_alt_q(char *arg, int a, int b, float scale, void *src) __asm__("FUN_L00_00259888");

extern char D_L17_00174760_lui[] __asm__("D_L17_00174760") NOT_SDA;
void        fleet_turret_shot_update(struct Moby *moby) __asm__("FUN_L17_002e8e08");

/* Moby class 1368 */
void fleet_turret_shot_update(struct Moby *moby) {
    enum {
        DRONE_STATE_FLIGHT = 0,
        DRONE_STATE_SPARKS_UNTIL_REMOVAL = 1
    };
    float        flight_position_or_spark_velocity[4];
    float        b[4];
    float        flight_offset_scratch[4];
    float        lowered_hero_position[4];
    float        movement_midpoint[4];
    float        hit_descriptor[12];
    char        *drone_vars = (char *)moby->pvars;
    char        *target_position;
    struct Hero *g;
    struct Hero *hero_state;
    char        *r;
    int          step;
    int          hit;
    int          scaled_frame_blocks;

    switch (moby->state) {
    case DRONE_STATE_FLIGHT:
        moby->draw_dist = 0;
        moby->visible = 0;
        target_position = drone_vars + 0x10;
        *(OvlQuad *)lowered_hero_position = *(OvlQuad *)(&hero.motion.body_center);
        lowered_hero_position[2] = lowered_hero_position[2] - 0.5f;
        if (vector_distance_xy(lowered_hero_position, target_position) < 8.0f) {
            lerp_vector(target_position, lowered_hero_position, target_position, 0.99f);
        }
        if (vector_distance_xy(&moby->pos, target_position) > 4.0f) {
            if (fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(*(float *)(drone_vars + 0x10) - moby->pos.x,
                                 *(float *)(drone_vars + 0x14) - moby->pos.y)) < 0.7853982f) {
                fast_vec_sub(flight_offset_scratch, target_position, &moby->pos);
                fast_vec_normalize(
                    drone_vars, flight_offset_scratch, *(float *)(drone_vars + 0x40));
            }
        }
        fast_vec_scale(flight_offset_scratch, drone_vars, -20.0f);
        step = 0;
        fast_vec_add(flight_position_or_spark_velocity, &moby->pos, drone_vars);
        fast_vec_add(b, &moby->pos, flight_offset_scratch);
        lerp_vector(movement_midpoint, flight_position_or_spark_velocity, &moby->pos, 0.5f);
        if (*(int *)(drone_vars + 0x38) < 0x3C) {
            step = (0x3C - *(int *)(drone_vars + 0x38)) >> 2;
        }
        create_drone_flight_trail_record((char *)flight_position_or_spark_velocity, 1, step);
        create_drone_flight_trail_record((char *)movement_midpoint, 1, step);
        create_drone_flight_trail_record((char *)flight_position_or_spark_velocity, 0, step);
        b[2] = b[2] + 0.25f;
        func_L00_0025A8C0_alt_q((char *)hit_descriptor, (int)moby, 1, 6.0f, drone_vars);
        *(short *)((char *)hit_descriptor + 0x1A) = moby->oclass;
        qcopy(&moby->pos, flight_position_or_spark_velocity);
        r = D_L17_00174740_lui;
        *(int *)(r + 0x18) = 0;
        if (FUN_001efa68_q(b, flight_position_or_spark_velocity, 0, moby, hit_descriptor) != 0 ||
            FUN_L00_001f0d60_q(0.5f, &moby->pos, 0, moby) != 0) {
            hit = *(int *)(r + 0x18);
            if (hit != 0) {
                if (hit != *(int *)(drone_vars + 0x3C)) {
                    g = &hero;
                    if (g->unk308 != 0 && hit == ((int)g->moby)) {
                        unsigned char *rec = D_00141968_x_q;
                        if (*(unsigned short *)(rec + 0x458) == 0) {
                            (*(unsigned short *)(rec + 0x458))++;
                            scaled_frame_blocks = scale_game_frames(D_0015EEA4) / 600;
                            if (*(unsigned short *)(rec + 0x45A) < scaled_frame_blocks) {
                                *(unsigned short *)(rec + 0x45A) =
                                    scale_game_frames(D_0015EEA4) / 600;
                            }
                            *(int *)(rec + 0x45C) =
                                *(int *)(rec + 0x45C) | (1 << current_level_index) | 0x80000000;
                        }
                    }
                    qcopy(&moby->pos, D_L17_00174760_lui);
                    FUN_L00_002598b0_q((int)moby, 0.75f, &moby->pos, 1, 6.0f, 0.0f, 0, 1, 0);
                    hero_state = &hero;
                    if (hero_state->unk308 != 0 &&
                        *(int *)(D_L17_00174760_lui - 8) == ((int)hero_state->moby)) {
                        unsigned char *rec = D_00141968_x_q;
                        if (*(unsigned short *)(rec + 0x458) == 0) {
                            (*(unsigned short *)(rec + 0x458))++;
                            scaled_frame_blocks = scale_game_frames(D_0015EEA4) / 600;
                            if (*(unsigned short *)(rec + 0x45A) < scaled_frame_blocks) {
                                *(unsigned short *)(rec + 0x45A) =
                                    scale_game_frames(D_0015EEA4) / 600;
                            }
                            *(int *)(rec + 0x45C) =
                                *(int *)(rec + 0x45C) | (1 << current_level_index) | 0x80000000;
                        }
                    }
                    *(int *)(drone_vars + 0x38) = scale_game_frames(0x1E);
                    moby->state = 1;
                    return;
                }
            } else if (vector_distance_xy(&moby->pos, *(char **)(drone_vars + 0x3C) + 0x10) >
                       10.0f) {
                qcopy(&moby->pos, r + 0x20);
                *(int *)(drone_vars + 0x38) = scale_game_frames(0x1E);
                moby->state = 1;
                return;
            }
        }
        if (scale_game_frames(0x46) < *(int *)(drone_vars + 0x38)) {
            if (moby->pos.x > 890.0f || moby->pos.y > 890.0f || moby->pos.z > 890.0f ||
                moby->pos.x < 80.0f || moby->pos.x < 80.0f || moby->pos.y < 80.0f ||
                moby->pos.z < 80.0f) {
                *(int *)(drone_vars + 0x38) = scale_game_frames(0x46);
            }
        }
        if (fast_dec_timer(drone_vars + 0x38) != 0 || moby->pos.x > 968.0f ||
            moby->pos.y > 968.0f || moby->pos.z > 968.0f || moby->pos.x < 10.0f ||
            moby->pos.x < 10.0f || moby->pos.y < 10.0f || moby->pos.z < 10.0f) {
            mark_moby_for_removal(moby);
        }
        break;
    case DRONE_STATE_SPARKS_UNTIL_REMOVAL: {
        float x = random_float_between(frame_time + frame_time, frame_time * 10.0f);
        float y = random_float_between(-3.1415927f, 3.1415927f);
        build_spherical_offset(
            flight_position_or_spark_velocity, x, y, random_float_between(-3.1415927f, 3.1415927f));
        spawn_moving_hero_relative_alpha_fade_particle(&moby->pos.x,
                                                       (char *)flight_position_or_spark_velocity,
                                                       0x60FF3020,
                                                       0xF,
                                                       random_integer_below(0xFF),
                                                       0,
                                                       3.0f);
        if (fast_dec_timer(drone_vars + 0x38) != 0) {
            mark_moby_for_removal(moby);
        }
        break;
    }
    }
}

/* spawns the class-0x558 object from src at pos, sets its state block and aims it along the vector v */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EAFC8), where it is exact; names translated to the US level program. */

extern char *create_moby(int);

char *spawn_class_558_moby_toward_target(struct Moby *source_moby,
                                         char        *spawn_position,
                                         float       *target_position,
                                         float        movement_speed) __asm__("FUN_L17_002e94d8");

char *spawn_class_558_moby_toward_target(struct Moby *source_moby,
                                         char        *spawn_position,
                                         float       *target_position,

                                         float movement_speed) {
    char *spawned_moby = create_moby(0x558);
    if (spawned_moby != 0) {
        char *spawned_vars;
        char *spawned_position = spawned_moby + 0x10;
        float target_delta[4];
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->spawn_frame = source_moby->spawn_frame;
        qcopy(spawned_position, spawn_position);
        spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(float *)(spawned_vars + 0x40) = movement_speed;
        qcopy(spawned_vars + 0x10, target_position);
        *(int *)(spawned_vars + 0x38) = scale_game_frames(0x258);
        *(char **)(spawned_vars + 0x3C) = source_moby;
        fast_vec_sub(target_delta, spawned_vars + 0x10, spawned_position);
        fast_vec_normalize(spawned_vars, target_delta, *(float *)(spawned_vars + 0x40));
        MOBY(spawned_moby)->rot.z = fast_arc_tan(target_position[0] - MOBY(spawned_moby)->pos.x,
                                                 target_position[1] - MOBY(spawned_moby)->pos.y);
        MOBY(spawned_moby)->rot.y =
            -fast_arc_tan(vector_distance_xy(spawned_position, target_position),
                          target_position[2] - MOBY(spawned_moby)->pos.z);
    }
    return spawned_moby;
}

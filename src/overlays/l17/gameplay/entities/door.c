/* Level 17 moby classes: door, fighter_jet, magnet_elevator, robot_dog, shuttle. */
#include "types.h"
#include "rnc/gameplay/entities/moby_class_id.h"

#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"

/* Shuttle destination selection shared by its route helpers. */
enum ShuttleStop {
    SHUTTLE_STOP_FIRST = 0x0,
    SHUTTLE_STOP_SECOND = 0x1
};

/* Robot-dog state byte (+0x20) shared by its update helpers. */
enum RobotDogState {
    ROBOT_DOG_STATE_INITIALIZE = 0x0,
    ROBOT_DOG_STATE_WAIT_FOR_DETECTION_TRIGGER = 0x1,
    ROBOT_DOG_STATE_SELECT_RESPONSE_ON_ANIMATION_COMPLETION = 0x2,
    ROBOT_DOG_STATE_WAIT_FOR_IDLE_ANIMATION_COMPLETION = 0x3,
    ROBOT_DOG_STATE_FALL_UNTIL_LANDING = 0x4,
    ROBOT_DOG_STATE_TRACK_TARGET_FACING = 0x5,
    ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER = 0x6,
    ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER_FAST_PULSE = 0x7,
    ROBOT_DOG_STATE_TURN_TOWARD_TARGET = 0x8,
    ROBOT_DOG_STATE_APPROACH_TARGET = 0x9,
    ROBOT_DOG_STATE_ATTACK_TARGET = 0xA,
    ROBOT_DOG_STATE_RETURN_HOME = 0xB,
    ROBOT_DOG_STATE_RECOVER_THEN_REINITIALIZE = 0xC,
    ROBOT_DOG_STATE_FALL_THEN_REMOVE = 0xF
};

/* Moves a level-17 platform moby between its stops along its heading, with a looping sound. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EA2C8), where it is exact; names translated to the US level program. */

extern char D_L17_001676C0[];
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern int D_L17_0015F5C4;
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern unsigned char D_0013E633[] __asm__("D_0013F3D0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
float absolute_float_c(float input) __asm__("FUN_001f99c0");
s32 is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void door_update(struct Moby *m) __asm__("FUN_L17_002e87d8");
void door_update(struct Moby *door_moby) {
    enum {
        DOOR_STATE_INITIALIZE = 0,
        DOOR_STATE_WAIT_AT_ORIGIN = 1,
        DOOR_STATE_MOVE_AWAY_FROM_ORIGIN = 2,
        DOOR_STATE_WAIT_DISPLACED = 4,
        DOOR_STATE_RETURN_TO_ORIGIN = 5
    };
    char *door_vars = (char *)door_moby->pvars;
    float position_offset[4];
    float updated_position[4];
    float movement_heading;
    float k;
    float k2;
    float displacement_sample;
    int voice_slot_index;

    if ((*(int *)(door_vars + ((0x18)))) == -1 || (*(int *)(door_vars + ((0x1C)))) == -1) {
        mark_moby_for_removal(door_moby);
        return;
    }
    if (D_L17_0015F5C4 == 2)
        return;
    switch (door_moby->state) {
    case DOOR_STATE_INITIALIZE:
        if ((door_moby->oclass) == 0x550 || (door_moby->oclass) == 0x551 ||
            (door_moby->oclass) == 0x553 || (door_moby->oclass) == 0x557 ||
            (door_moby->oclass) == 0x55D) {
            door_moby->command = 1;
        }
        *(OvlQuad *)door_vars = *(OvlQuad *)&door_moby->pos;
        door_moby->state = 1;
        break;
    case DOOR_STATE_MOVE_AWAY_FROM_ORIGIN:
        if (door_moby->pclass->sound_defs != 0 &&
            voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
            (*(int *)(door_vars + ((0x20)))) = allocate_voice_for_target_entry(0, 4, (int)door_moby);
        }
        k = frame_time * 3.0f;
        displacement_sample = (*(float *)(door_vars + ((0x14))));
        if (door_moby->command == 0)
            k = -k;
        (*(float *)(door_vars + ((0x14)))) = displacement_sample + k;
        if ((door_moby->oclass) == 0x55C || (door_moby->oclass) == 0x55D ||
            (door_moby->oclass) == 0x557 || (door_moby->oclass) == 0x559) {
            movement_heading = fast_add_rotations(1.5707964f, door_moby->rot.z);
            position_offset[0] = fast_cos(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        } else {
            position_offset[0] = fast_cos(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        }
        add_vector_xyz(updated_position, door_vars, position_offset);
        *(OvlQuad *)&door_moby->pos = *(OvlQuad *)updated_position;
        if (absolute_float_c((*(float *)(door_vars + ((0x14))))) > 2.0f) {
            voice_slot_index = (*(int *)(door_vars + ((0x20))));
            if (voice_slot_index != -1) {
                char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
                if (*(char **)(e + 0x88) == door_moby) {
                    if (*(unsigned char *)(e + 0x74) != 0)
                        release_voice_slot(voice_slot_index);
                }
            }
            (*(int *)(door_vars + ((0x20)))) = -1;
            door_moby->state = 4;
        }
        break;
    case DOOR_STATE_WAIT_AT_ORIGIN:
        (*(float *)(door_vars + ((0x14)))) = 0.0f;
        voice_slot_index = (*(int *)(door_vars + ((0x20))));
        if (voice_slot_index != -1) {
            char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
            if (*(char **)(e + 0x88) == door_moby) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(voice_slot_index);
            }
        }
        (*(int *)(door_vars + ((0x20)))) = -1;
        if (absolute_float_c(door_moby->rot.x) < 0.7853982f ||
            hero.unk308 != 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, (*(int *)(door_vars + ((0x18))))) == 0) {
                if ((*(int *)(door_vars + ((0x10)))) == -1)
                    break;
                if (is_point_inside_clip_volume(D_L17_0015FFD8 + (*(int *)(door_vars + ((0x10)))) * 0x100 +
                                                    0x10,
                                                (*(int *)(door_vars + ((0x18))))) == 0)
                    break;
            }
            if (door_moby->pclass->sound_defs != 0 &&
                voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
                (*(int *)(door_vars + ((0x20)))) = allocate_voice_for_target_entry(0, 4, (int)door_moby);
            }
            door_moby->state = 2;
        }
        break;
    case 3:
        break;
    case DOOR_STATE_WAIT_DISPLACED:
        voice_slot_index = (*(int *)(door_vars + ((0x20))));
        if (voice_slot_index != -1) {
            char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
            if (*(char **)(e + 0x88) == door_moby) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(voice_slot_index);
            }
        }
        (*(int *)(door_vars + ((0x20)))) = -1;
        if (is_point_inside_clip_volume(&hero.motion.pos, (*(int *)(door_vars + ((0x1C))))))
            break;
        if (is_point_inside_clip_volume(D_L17_001676C0, (*(int *)(door_vars + ((0x1C))))))
            break;
        if ((*(int *)(door_vars + ((0x10)))) != -1 &&
            is_point_inside_clip_volume(D_L17_0015FFD8 + (*(int *)(door_vars + ((0x10)))) * 0x100 + 0x10,
                                        (*(int *)(door_vars + ((0x18))))))
            break;
        if (door_moby->pclass->sound_defs != 0 &&
            voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
            (*(int *)(door_vars + ((0x20)))) = allocate_voice_for_target_entry(0, 4, (int)door_moby);
        }
        door_moby->state = 5;
        break;
    case DOOR_STATE_RETURN_TO_ORIGIN:
        if (door_moby->pclass->sound_defs != 0 &&
            voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
            (*(int *)(door_vars + ((0x20)))) = allocate_voice_for_target_entry(0, 4, (int)door_moby);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, (*(int *)(door_vars + ((0x18))))) ||
            is_point_inside_clip_volume(D_L17_001676C0, (*(int *)(door_vars + ((0x18))))) ||
            ((*(int *)(door_vars + ((0x10)))) != -1 &&
             is_point_inside_clip_volume(D_L17_0015FFD8 + (*(int *)(door_vars + ((0x10)))) * 0x100 + 0x10,
                                         (*(int *)(door_vars + ((0x18))))))) {
            door_moby->state = 2;
            break;
        }
        displacement_sample = absolute_float_c((*(float *)(door_vars + ((0x14)))));
        k2 = frame_time * 3.0f;
        if (displacement_sample > k2) {
            float previous_displacement = (*(float *)(door_vars + ((0x14))));
            float return_displacement;
            if (door_moby->command == 0) {
                float negative_return_step = -k2;
                return_displacement = previous_displacement - negative_return_step;
            } else {
                return_displacement = previous_displacement - k2;
            }
            (*(float *)(door_vars + ((0x14)))) = return_displacement;
        } else {
            (*(float *)(door_vars + ((0x14)))) = 0.0f;
        }
        if ((door_moby->oclass) == 0x55C || (door_moby->oclass) == 0x55D ||
            (door_moby->oclass) == 0x557 || (door_moby->oclass) == 0x559) {
            movement_heading = fast_add_rotations(1.5707964f, door_moby->rot.z);
            position_offset[0] = fast_cos(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        } else {
            position_offset[0] = fast_cos(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        }
        add_vector_xyz(updated_position, door_vars, position_offset);
        *(OvlQuad *)&door_moby->pos = *(OvlQuad *)updated_position;
        if ((*(float *)(door_vars + ((0x14)))) == 0.0f) {
            voice_slot_index = (*(int *)(door_vars + ((0x20))));
            if (voice_slot_index != -1) {
                char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
                if (*(char **)(e + 0x88) == door_moby) {
                    if (*(unsigned char *)(e + 0x74) != 0)
                        release_voice_slot(voice_slot_index);
                }
            }
            (*(int *)(door_vars + ((0x20)))) = -1;
            door_moby->state = 1;
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Updates moby 1368: a drone that drifts toward the hero, tags what it hits and sprays sparks until its timer runs out. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EA8F8), where it is exact; names translated to the US level program. */

extern char D_L17_00174740_lui[] __asm__("D_L17_00174740") __attribute__((section(".data")));
extern float FUN_001f9b80_q(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_q(float, float) __asm__("FUN_001f9e90");
extern float FUN_001fa688_q(float, float) __asm__("FUN_001fa688");
extern float random_float_between_alt_q(float, float) __asm__("FUN_002132a8");
extern int D_0015EEA4_q __asm__("D_0015EEA4");
extern int FUN_001efa68_q(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int FUN_L00_001f0d60_q(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int random_integer_below_q(int) __asm__("FUN_00213260");
extern s32 scale_game_frames_q(s32) __asm__("FUN_001f96f8");
extern unsigned char *spawn_moving_hero_relative_alpha_fade_particle(float *pos, char *vel, int color, unsigned char life,
                                         unsigned char b, int mode,
                                         float scale) __asm__("FUN_L00_002730e0");
extern unsigned char *create_drone_flight_trail_record(char *pos, int mode, int step) __asm__("FUN_L17_0026fdd0");
extern unsigned char D_00141968_x_q[] __asm__("D_00141968");
extern void FUN_001f9a40_q(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void FUN_L00_002598b0_q(int, float, void *, int, float, float, int, int,
                               int) __asm__("FUN_L00_002598b0");
extern void add_vector_xyz_q(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz_q(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_q(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_q(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern int tick_countdown_32_q(void *) __asm__("FUN_001f9740");
void make_hit_descriptor(char *a, int b, int c, float d, void *q) __asm__("FUN_L00_00259888");
void build_spherical_offset_q(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void mmr_q_q(struct Obj *obj) __asm__("FUN_0020c828");
extern void func_L00_0025A8C0_alt_q(char *arg, int a, int b, float scale,
                                    void *src) __asm__("FUN_L00_00259888");

extern char D_L17_00174760_lui[] __asm__("D_L17_00174760") __attribute__((section(".data")));
void update_drone_flight_and_sparks(struct Moby *moby) __asm__("FUN_L17_002e8e08");

void update_drone_flight_and_sparks(struct Moby *moby) {
    enum {
        DRONE_STATE_FLIGHT = 0,
        DRONE_STATE_SPARKS_UNTIL_REMOVAL = 1
    };
    float a[4];
    float b[4];
    float flight_offset_scratch[4];
    float lowered_hero_position[4];
    float movement_midpoint[4];
    float hit_descriptor[12];
    char *drone_vars = (char *)moby->pvars;
    char *target_position;
    struct Hero *g;
    struct Hero *hero_state;
    char *r;
    int step;
    int hit;
    int scaled_frame_blocks;

    switch (moby->state) {
    case DRONE_STATE_FLIGHT:
        moby->draw_dist = 0;
        moby->visible = 0;
        target_position = drone_vars + 0x10;
        *(OvlQuad *)lowered_hero_position = *(OvlQuad *)(&hero.motion.unkD0);
        lowered_hero_position[2] = lowered_hero_position[2] - 0.5f;
        if (FUN_001f9b80_q(lowered_hero_position, target_position) < 8.0f) {
            FUN_001f9a40_q(target_position, lowered_hero_position, target_position, 0.99f);
        }
        if (FUN_001f9b80_q(&moby->pos, target_position) > 4.0f) {
            if (FUN_001fa688_q(moby->rot.z,
                               FUN_001f9e90_q(*(float *)(drone_vars + 0x10) - moby->pos.x,
                                              *(float *)(drone_vars + 0x14) - moby->pos.y)) <
                0.7853982f) {
                subtract_vector_xyz_q(flight_offset_scratch, target_position, &moby->pos);
                normalize_vector_xyz_q(drone_vars, flight_offset_scratch, *(float *)(drone_vars + 0x40));
            }
        }
        scale_vector_xyz_q(flight_offset_scratch, drone_vars, -20.0f);
        step = 0;
        add_vector_xyz_q(a, &moby->pos, drone_vars);
        add_vector_xyz_q(b, &moby->pos, flight_offset_scratch);
        FUN_001f9a40_q(movement_midpoint, a, &moby->pos, 0.5f);
        if (*(int *)(drone_vars + 0x38) < 0x3C) {
            step = (0x3C - *(int *)(drone_vars + 0x38)) >> 2;
        }
        create_drone_flight_trail_record((char *)a, 1, step);
        create_drone_flight_trail_record((char *)movement_midpoint, 1, step);
        create_drone_flight_trail_record((char *)a, 0, step);
        b[2] = b[2] + 0.25f;
        func_L00_0025A8C0_alt_q((char *)hit_descriptor, (int)moby, 1, 6.0f, drone_vars);
        *(short *)((char *)hit_descriptor + 0x1A) = moby->oclass;
        qcopy(&moby->pos, a);
        r = D_L17_00174740_lui;
        *(int *)(r + 0x18) = 0;
        if (FUN_001efa68_q(b, a, 0, moby, hit_descriptor) != 0 ||
            FUN_L00_001f0d60_q(0.5f, &moby->pos, 0, moby) != 0) {
            hit = *(int *)(r + 0x18);
            if (hit != 0) {
                if (hit != *(int *)(drone_vars + 0x3C)) {
                    g = &hero;
                    if (g->unk308 != 0 && hit == ((int)g->moby)) {
                        unsigned char *rec = D_00141968_x_q;
                        if (*(unsigned short *)(rec + 0x458) == 0) {
                            (*(unsigned short *)(rec + 0x458))++;
                            scaled_frame_blocks = scale_game_frames_q(D_0015EEA4_q) / 600;
                            if (*(unsigned short *)(rec + 0x45A) < scaled_frame_blocks) {
                                *(unsigned short *)(rec + 0x45A) =
                                    scale_game_frames_q(D_0015EEA4_q) / 600;
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
                            scaled_frame_blocks = scale_game_frames_q(D_0015EEA4_q) / 600;
                            if (*(unsigned short *)(rec + 0x45A) < scaled_frame_blocks) {
                                *(unsigned short *)(rec + 0x45A) =
                                    scale_game_frames_q(D_0015EEA4_q) / 600;
                            }
                            *(int *)(rec + 0x45C) =
                                *(int *)(rec + 0x45C) | (1 << current_level_index) | 0x80000000;
                        }
                    }
                    *(int *)(drone_vars + 0x38) = scale_game_frames_q(0x1E);
                    moby->state = 1;
                    return;
                }
            } else if (FUN_001f9b80_q(&moby->pos, *(char **)(drone_vars + 0x3C) + 0x10) > 10.0f) {
                qcopy(&moby->pos, r + 0x20);
                *(int *)(drone_vars + 0x38) = scale_game_frames_q(0x1E);
                moby->state = 1;
                return;
            }
        }
        if (scale_game_frames_q(0x46) < *(int *)(drone_vars + 0x38)) {
            if (moby->pos.x > 890.0f || moby->pos.y > 890.0f ||
                moby->pos.z > 890.0f || moby->pos.x < 80.0f ||
                moby->pos.x < 80.0f || moby->pos.y < 80.0f ||
                moby->pos.z < 80.0f) {
                *(int *)(drone_vars + 0x38) = scale_game_frames_q(0x46);
            }
        }
        if (tick_countdown_32_q(drone_vars + 0x38) != 0 || moby->pos.x > 968.0f ||
            moby->pos.y > 968.0f || moby->pos.z > 968.0f ||
            moby->pos.x < 10.0f || moby->pos.x < 10.0f ||
            moby->pos.y < 10.0f || moby->pos.z < 10.0f) {
            mmr_q_q(moby);
        }
        break;
    case DRONE_STATE_SPARKS_UNTIL_REMOVAL: {
        float x = random_float_between_alt_q(frame_time + frame_time, frame_time * 10.0f);
        float y = random_float_between_alt_q(-3.1415927f, 3.1415927f);
        build_spherical_offset_q(a, x, y, random_float_between_alt_q(-3.1415927f, 3.1415927f));
        spawn_moving_hero_relative_alpha_fade_particle(&moby->pos.x, (char *)a, 0x60FF3020, 0xF,
                           random_integer_below_q(0xFF), 0, 3.0f);
        if (tick_countdown_32_q(drone_vars + 0x38) != 0) {
            mmr_q_q(moby);
        }
        break;
    }
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* spawns the class-0x558 object from src at pos, sets its state block and aims it along the vector v */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EAFC8), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

char *spawn_class_558_moby_toward_target(struct Moby *source_moby, char *spawn_position, float *target_position, float movement_speed) __asm__("FUN_L17_002e94d8");

char *spawn_class_558_moby_toward_target(struct Moby *source_moby, char *spawn_position, float *target_position, float movement_speed) {
    char *spawned_moby = func_0020D348_m(0x558);
    if (spawned_moby != 0) {
        char *spawned_vars;
        char *spawned_position = spawned_moby + 0x10;
        float target_delta[4];
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        *(long *)(spawned_moby + 0x38) = source_moby->spawn_frame;
        qcopy(spawned_position, spawn_position);
        spawned_vars = *(char **)(spawned_moby + 0x78);
        *(float *)(spawned_vars + 0x40) = movement_speed;
        qcopy(spawned_vars + 0x10, target_position);
        *(int *)(spawned_vars + 0x38) = scale_game_frames(0x258);
        *(char **)(spawned_vars + 0x3C) = source_moby;
        fast_vec_sub(target_delta, spawned_vars + 0x10, spawned_position);
        fast_vec_normalize(spawned_vars, target_delta, *(float *)(spawned_vars + 0x40));
        *(float *)(spawned_moby + 0x48) =
            fast_arc_tan(target_position[0] - *(float *)(spawned_moby + 0x10), target_position[1] - *(float *)(spawned_moby + 0x14));
        *(float *)(spawned_moby + 0x44) = -fast_arc_tan(vector_distance(spawned_position, target_position), target_position[2] - *(float *)(spawned_moby + 0x18));
    }
    return spawned_moby;
}

#define NOT_SDA

#define MACRO_ADDR

#include "qzero.h"

/* Draws one moby's icon on the radar: its offset from the viewer turned into the radar's frame, faded near the rim. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EC5D0), where it is exact; names translated to the US level program. */

typedef struct {
    short tex;
    short u;
    short v;
    short w;
    short h;
    short fA;
    short x;
    short y;
} IconEC5D0;

extern IconEC5D0 D_L17_001D9B38[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float vector_length_xyz(void *a);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern unsigned char frame_size[] __asm__("D_0013E504");
extern unsigned char D_0015EDB4[4];
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void draw_fighter_jet_radar_contact_icon(char *radar_rotation, char *from, char *moby, int icon, long color, int tex) __asm__("FUN_L17_002eaae0");

void draw_fighter_jet_radar_contact_icon(char *radar_rotation, char *from, char *moby, int icon, long color, int tex) {
    float turn[4];
    float radar_rotation_matrix[16];
    float delta[4];
    float radar_offset[4];
    float radar_distance;
    float side;
    int x, y;
    IconEC5D0 *radar_icon_definition;

    qzero(turn);
    turn[2] = *(float *)(radar_rotation + 8);
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(radar_rotation_matrix, turn);
    subtract_vector_xyz(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(radar_offset, delta, radar_rotation_matrix);
    scale_vector_xyz(radar_offset, radar_offset, 0.0714285746f);
    radar_distance = vector_length_xyz(radar_offset);
    side = 1.0f;
    if (D_0015EDB4[0] != 0) {
        side = -1.0f;
    }
    x = func_001FA898_r(side * radar_offset[0]);
    y = func_001FA898_r(-radar_offset[1]);
    if (radar_distance < 46.0f) {
        if (radar_distance > 38.0f) {
            float alpha_distance_product;
            radar_distance = 46.0f - radar_distance;
            alpha_distance_product = convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * radar_distance;
            color &= 0xFFFFFF;
            color = (func_001FA898_r(alpha_distance_product * 0.125f) << 24) | color;
        }
        radar_icon_definition = &D_L17_001D9B38[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - radar_icon_definition->x, y - radar_icon_definition->y, radar_icon_definition->w, radar_icon_definition->h, radar_icon_definition->u, radar_icon_definition->v, radar_icon_definition->w, radar_icon_definition->h, color,
                           get_effect_texture(tex + 0x28 + radar_icon_definition->tex));
    }
}
/* Walks the moby list: picks the on-screen target, marks targetable mobys and points an arrow at the nearest pickup. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EC7D8), where it is exact; names translated to the US level program. */

extern char *D_L17_0015FFE4;
extern char D_L17_001678D0[];
extern char D_L17_001D9AC0[];
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern float D_L17_001678E0[];
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_001f9b48(void *, void *);
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern unsigned char D_0013E500[];
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
extern void emit_transformed_2d_vertex_packet(void *, int, int, long, float, float, float, float) __asm__("FUN_L13_002b7d50");
void draw_fighter_jet_radar_contact_icon(char *rot, char *from, char *moby, int icon, long color, int tex) __asm__("FUN_L17_002eaae0");
extern void project_to_screen(f32 *, void *) __asm__("FUN_001f2070");
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

short D_L17_00162188 = 2;
int select_fighter_jet_target_and_draw_contacts(int ship_moby, char *ship_vars, char *radar_origin, char *radar_rotation, float f0, float f1, float f2, char *current_target,
                     char *excluded_target) __asm__("FUN_L17_002eace8");

int select_fighter_jet_target_and_draw_contacts(int ship_moby, char *ship_vars, char *radar_origin, char *radar_rotation, float f0, float f1, float f2, char *current_target,
                     char *excluded_target) {
    enum {
        MOBY_CLASS_RADAR_CONTACT_DRAW_BYPASS = 0x188,
        MOBY_CLASS_CONTACT_DRAW_BYPASS = 0x18F,
        MOBY_CLASS_CONTACT_RENDER_SUPPRESSED = 0x190
    };
    float v[4];
    float arrow_target_position_scratch[4];
    float arrow_direction_rotation_matrix[16];
    float u[4];
    int sx, sy, tx, ty;
    char *scanned_moby;
    char *selected_target_moby;
    int flag;
    char *near;
    int best;
    int *scr;
    float bestd;
    int dx, dy, aim_selection_screen_radius, aim_screen_distance_squared;
    float x, y, ang, dd, ship_to_arrow_target_distance, heading_error_magnitude, k, angular_error_magnitude, arrow_pulse_phase;

    scanned_moby = D_L17_0015FFE4;
    best = 0x7FFFFFFF;
    selected_target_moby = 0;
    *(int *)(ship_vars + 0xE8) = 0;
    flag = 0;
    bestd = 100000.0f;
    near = 0;
    while (scanned_moby != 0) {
        scr = (int *)(D_0013E500);
        if ((*(unsigned short *)(scanned_moby + 0x34) & 0x1000) && *(unsigned char *)(scanned_moby + 0x31) != 0 &&
            scanned_moby != 0 && *(char **)(scanned_moby + 0x24) != 0 && *(short *)(*(char **)(scanned_moby + 0x24) + 0x46) == 5) {
            project_world_point_to_screen_pixels(scanned_moby + 0x10, &sx, &sy, 0);
            dx = sx - *(int *)(ship_vars + 0xE0);
            dy = sy - *(int *)(ship_vars + 0xE4);
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;
            qcopy(v, D_L17_001678E0);
            fast_vec_scale(v, D_L17_001678E0, *(float *)(scanned_moby + 0xC) / 1280.0f);
            fast_vec_add(v, v, scanned_moby + 0x10);
            project_world_point_to_screen_pixels(v, &tx, &ty, 0);
            tx -= sx;
            ty -= sy;
            aim_selection_screen_radius = func_001FA898_r(square_root(convert_integer_to_float(tx * tx + ty * ty))) + 0x1C;
            if (dx < aim_selection_screen_radius && dy < aim_selection_screen_radius) {
                if (current_target != 0) {
                    if (scanned_moby == current_target) {
                        best = 0;
                        selected_target_moby = scanned_moby;
                    }
                } else if (scanned_moby != excluded_target) {
                    aim_screen_distance_squared = dx * dx + dy * dy;
                    if (aim_screen_distance_squared < best) {
                        selected_target_moby = scanned_moby;
                        best = aim_screen_distance_squared;
                    }
                }
            }
        }
        switch (*(short *)(scanned_moby + 0xA6)) {
        case MOBY_CLASS_GRAY_RADAR_CONTACT:
        case MOBY_CLASS_GRAY_RADAR_ICON_TWO_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 2, 0x70808080, *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_GRAY_RADAR_QUAD_CONTACT:
        case MOBY_CLASS_GRAY_RADAR_BLIP_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 2, 0x70808080, *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_ORANGE_RADAR_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 1, 0x70004080, *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_YELLOW_RADAR_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 1, 0x70007080, *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_UNMARKED_COUNTED_TARGET:
        case MOBY_CLASS_UNCOUNTED_MARKED_TARGET:
        case MOBY_CLASS_PRIORITY_LOCK_TARGET:
        case MOBY_CLASS_MARKED_COUNTED_TARGET:
            project_to_screen(v, scanned_moby + 0x10);
            if (*(short *)(scanned_moby + 0xA6) != 0x184) {
                if (*(unsigned char *)(scanned_moby + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(D_L17_001D9AC0, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f,
                                         ang);
                    }
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(D_L17_001D9AC0, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f,
                                         -ang);
                    }
                }
            }
            if (*(short *)(scanned_moby + 0xA6) != 0x191) {
                *(int *)(ship_vars + 0xE8) = *(int *)(ship_vars + 0xE8) + 1;
            }
            draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 0, 0x70808080, *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_OFFSCREEN_ARROW_TARGET:
            qcopy(arrow_target_position_scratch, scanned_moby + 0x10);
            arrow_target_position_scratch[2] += 4.0f;
            project_to_screen(v, arrow_target_position_scratch);
            if (*(unsigned char *)(scanned_moby + 0x31) != 0) {
                flag = 1;
                {
                    float x = (v[0] - scr[4]) * 0.0625f;
                    float y = (v[1] - scr[5]) * 0.0625f;
                    float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                    emit_transformed_2d_vertex_packet(D_L17_001D9AC0, 0x1E, 0xFFFFF3, 0x400000FF, x, y, 2.0f, ang);
                }
                {
                    float x = (v[0] - scr[4]) * 0.0625f;
                    float y = (v[1] - scr[5]) * 0.0625f;
                    float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                    emit_transformed_2d_vertex_packet(D_L17_001D9AC0, 0x1E, 0xFFFFF3, 0x400000FF, x, y, 2.5f, -ang);
                }
            }
            dd = FUN_001f9b48(scanned_moby + 0x10, (&hero.motion.pos));
            if (dd < bestd) {
                near = scanned_moby;
                bestd = dd;
            }
            draw_fighter_jet_radar_contact_icon(radar_rotation, (char *)arrow_target_position_scratch, scanned_moby, 0, 0x70808080, *(int *)(ship_vars + 0x104));
            *(int *)(ship_vars + 0xE8) = *(int *)(ship_vars + 0xE8) + 1;
            break;
        case MOBY_CLASS_FLAG_GATED_MARKED_TARGET:
        case MOBY_CLASS_FLAG_GATED_SCREEN_MARKER_TARGET:
            if (!(*(unsigned short *)(scanned_moby + 0x34) & 1)) {
                project_to_screen(v, scanned_moby + 0x10);
                if (*(unsigned char *)(scanned_moby + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(D_L17_001D9AC0, 0x1E, 0xFFFFF3, 0xFF00FF00, x, y, 1.0f,
                                         -ang);
                    }
                }
                draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 1, 0x70808080, *(int *)(ship_vars + 0x104));
            }
            break;
        case MOBY_CLASS_STATE_GATED_MARKED_TARGET:
            if (!(*(unsigned short *)(scanned_moby + 0x34) & 1) && *(unsigned char *)(scanned_moby + 0x20) >= 2) {
                project_to_screen(v, scanned_moby + 0x10);
                if (*(unsigned char *)(scanned_moby + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(D_L17_001D9AC0, 0x1E, 0xFFFFF3, 0xFF00FF00, x, y, 1.0f,
                                         -ang);
                    }
                }
                draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 1, 0x70808080, *(int *)(ship_vars + 0x104));
            }
            break;
        case MOBY_CLASS_EXCLUDED_FROM_RADAR:
        case MOBY_CLASS_RADAR_RENDER_BYPASS:
        case MOBY_CLASS_RADAR_ICON_SUPPRESSED:
        case MOBY_CLASS_RADAR_CONTACT_DRAW_BYPASS:
        case 0x189:
        case 0x18A:
        case 0x18B:
        case 0x18C:
        case MOBY_CLASS_CONTACT_DRAW_BYPASS:
        case MOBY_CLASS_CONTACT_RENDER_SUPPRESSED:
        case MOBY_CLASS_RADAR_DRAW_SUPPRESSED:
        case 0x535:
        case 0x536:
        case 0x537:
        case 0x538:
        case 0x539:
        case 0x53A:
            break;
        default:
            if (scanned_moby != 0 && *(char **)(scanned_moby + 0x24) != 0 &&
                *(short *)(*(char **)(scanned_moby + 0x24) + 0x46) == 5) {
                draw_fighter_jet_radar_contact_icon(radar_rotation, radar_origin, scanned_moby, 1, 0x70808080, *(int *)(ship_vars + 0x104));
            }
            break;
        }
        scanned_moby = *(char **)(scanned_moby + 0x28);
    }
    if (flag == 0 && near != 0) {
        char *np;
        char *ap;
        qcopy(u, near + 0x10);
        np = near + 0x10;
        u[2] += 4.0f;
        ap = (char *)ship_moby + 0x10;
        project_to_screen(arrow_target_position_scratch, u);
        transpose_rotation_matrix(arrow_direction_rotation_matrix, D_L17_001678D0);
        fast_vec_sub(v, np, (&hero.motion.pos));
        transform_vector(v, v, arrow_direction_rotation_matrix);
        v[0] = -v[1];
        v[1] = -v[2];
        v[2] = 0.0f;
        fast_vec_normalize(v, v, 160.0f);
        ang = fast_add_rotations(fast_arc_tan(v[0], v[1]), 1.5707964f);
        v[0] += 256.0f;
        v[1] += 208.0f;
        ship_to_arrow_target_distance = FUN_001f9b48(ap, np);
        heading_error_magnitude = absolute_float(
            fast_subtract_rotations(fast_arc_tan(*(float *)(near + 0x10) - *(float *)((char *)ship_moby + 0x10),
                                      *(float *)(near + 0x14) - *(float *)((char *)ship_moby + 0x14)),
                         *(float *)(ship_vars + 0x38)));
        k = absolute_float(
            fast_subtract_rotations(-fast_arc_tan(vector_distance(ap, np),
                                       *(float *)(near + 0x18) - *(float *)((char *)ship_moby + 0x18)),
                         *(float *)(ship_vars + 0x34)));
        angular_error_magnitude = square_root(heading_error_magnitude * heading_error_magnitude + k * k);
        arrow_pulse_phase = fast_add_rotations(*(float *)(ship_vars + 0x114), 0.1f / (angular_error_magnitude / 3.1415927f));
        *(float *)(ship_vars + 0x114) = arrow_pulse_phase;
        emit_transformed_2d_vertex_packet(&D_L17_00162188, 3, 0xFFFFF3,
                         (func_001FA898_r((fast_sin(arrow_pulse_phase) + 1.4f) * 53.0f) << 24) | 0x8000, v[0],
                         v[1], 240.0f / ship_to_arrow_target_distance + 1.5f, ang);
    }
    return (int)selected_target_moby;
}
#include "qcopy.h"

/* picks the moby to home in on, keeping or dropping the current target */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002ED258), where it is exact; names translated to the US level program. */

extern unsigned char D_0013C940_u[] __asm__("D_0013C940");
extern int D_L17_0015F5C4;
int D_L17_0016216C = 600;
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int select_fighter_jet_target_and_draw_contacts(int, char *, char *, char *, float, float, float, char *,
                              char *) __asm__("FUN_L17_002eace8");
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(char **)((char *)(p) + (o)))
#define H(p, o) (*(short *)((char *)(p) + (o)))
#define B(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define FLAG    (hero.ship_ammo)

extern char D_L17_001676D0_lui[] __asm__("D_L17_001676D0") __attribute__((section(".data")));
void update_fighter_jet_target_lock(int ship_moby, char *ship_vars) __asm__("FUN_L17_002eb768");

void update_fighter_jet_target_lock(int ship_moby, char *ship_vars) {
    char *excluded_target = 0;
    char *pad;
    char *locked_target;
    char *candidate_target;
    float radar_rotation[4];
    int lock_timer_reload;
    if (!(D_L17_0015F5C4 >= 3 && D_L17_0015F5C4 <= 4)) {
        fast_dec_timer((int *)(ship_vars + 0x8C));
    }
    if (W(ship_vars, 0x8C) != 0 && P(ship_vars, 0x88) != 0 && H(P(ship_vars, 0x88), 0xA6) == W(ship_vars, 0x128) &&
        B(P(ship_vars, 0x88), 0x20) != 0xFE && B(P(ship_vars, 0x88), 0x20) != 0xFD) {
        pad = (char *)D_0013C940_u;
        if (B(P(ship_vars, 0x88), 0x31) != 0) {
            if (W(pad, 0x1B4) & 0x2000) {
                goto take;
            }
            if (FLAG != 0) {
                goto skip;
            }
        }
    }
    pad = (char *)D_0013C940_u;
    if (W(pad, 0x1B4) & 0x2000) {
    take:
        excluded_target = P(ship_vars, 0x88);
    }
    W(ship_vars, 0xEC) = 0;
    W(ship_vars, 0x128) = -1;
    W(ship_vars, 0x88) = 0;
skip:
    qcopy(radar_rotation, D_L17_001676D0_lui);
    candidate_target = (char *)select_fighter_jet_target_and_draw_contacts(ship_moby, ship_vars, D_L17_001676D0_lui - 0x10, (char *)radar_rotation, 0.19634955f, 0.19634955f,
                                   255.0f, P(ship_vars, 0x88), excluded_target);
    if ((candidate_target != P(ship_vars, 0x88) && W(ship_vars, 0x8C) >= 0x1F5) || FLAG == 0) {
        W(ship_vars, 0xEC) = 0;
        W(ship_vars, 0x128) = -1;
        W(ship_vars, 0x88) = 0;
    }
    if (candidate_target != 0 && (P(ship_vars, 0x88) == 0 || (H(P(ship_vars, 0x88), 0xA6) != 0x3EB && H(candidate_target, 0xA6) == 0x3EB)) &&
        FLAG != 0) {
        P(ship_vars, 0x88) = candidate_target;
        lock_timer_reload = D_L17_0016216C;
        W(ship_vars, 0x128) = H(candidate_target, 0xA6);
        W(ship_vars, 0x8C) = lock_timer_reload;
    }
    if (candidate_target != P(ship_vars, 0x88)) {
        W(ship_vars, 0x118) = W(ship_vars, 0x118) + 1;
        if (scale_game_frames(0x3C) < W(ship_vars, 0x118)) {
            W(ship_vars, 0xEC) = 0;
            W(ship_vars, 0x128) = -1;
            W(ship_vars, 0x88) = 0;
        }
    } else {
        W(ship_vars, 0x118) = 0;
    }
    locked_target = P(ship_vars, 0x88);
    if (locked_target != 0 && W(ship_vars, 0x84) == 0) {
        if (H(locked_target, 0xA6) != W(ship_vars, 0x128) || B(locked_target, 0x20) == 0xFE || B(locked_target, 0x20) == 0xFD) {
            W(ship_vars, 0x88) = 0;
            W(ship_vars, 0x128) = -1;
        }
    }
}

/* Draws the fleet HUD: target marker, ammo icons, gauge sprites and warnings, and refreshes the gauge colour table. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002ED498), where it is exact; names translated to the US level program. */

extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern char D_L17_001D99E0[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int D_L17_001D9B68[];
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
int lock_hold_frames __asm__("D_L17_00162170") = 500;
int lock_fade_frames __asm__("D_L17_00162174") = 100;
extern short D_L17_00162180 __attribute__((sda));
extern unsigned char D_0013E500[];
extern void append_rotated_sprite_quad(float, float, float, float, float, int, int, long, int, int,
                                       int, int, float, float) __asm__("FUN_001f5ab0");
extern void append_scaled_screen_triangle_strip(float, float, float, void *, int, int, long) __asm__("FUN_L11_00310ad0");
extern void project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
extern void update_fighter_jet_target_lock(int a, char *p) __asm__("FUN_L17_002eb768");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
float absolute_float(float input) __asm__("func_001F99C0");
s32 font_print_center(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_001f6af0");
void draw_quarter_turn_textured_quads(unsigned char a, unsigned char b, unsigned char c, unsigned char d, float x,
                      float y, float s, float ang) __asm__("FUN_L11_00311048");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern long func_001F4868_l(int) __asm__("FUN_001f44b8");
extern void func_L17_002EBF08_f(float, float, float, float, unsigned char, unsigned char,
                                unsigned char, unsigned char) __asm__("FUN_L11_00311048");
extern void *func_001FE540_id(int) __asm__("FUN_001fdd10");
extern int func_001F6F40_c(int, int, long, void *, int) __asm__("FUN_001f6af0");
extern int func_001F6F40_c_001f6b88(int, int, long, void *, int) __asm__("FUN_001f6b88");

void draw_fighter_jet_hud(struct Moby *moby) __asm__("FUN_L17_002eb9a8");

void draw_fighter_jet_hud(struct Moby *moby) {
    char *ship_vars = (char *)moby->pvars;
    char *screen_dimensions;
    int locked_target_screen_x;
    int locked_target_screen_y;
    long aim_reticle_texture;
    int i;
    unsigned char ammo_capacity;
    unsigned char lock_marker_fade_byte;
    int j;
    int x;
    int y;
    int ex;
    int ey;
    int lock_marker_blink_value;
    int hp_bar_fill_threshold;
    int lo;
    int hi;
    int hp_bar_clut_index;
    float lock_fade_ratio;
    float k;
    float lock_marker_fade_factor;
    float px;
    float py;
    float hp_display_step;
    float hp_display_step_magnitude;

    vu1_add_g_sregister(0x42, 0x8000000044L);
    draw_textured_quad(0x170, (screen_dimensions = (char *)D_0013E500, *(int *)(screen_dimensions + 4)) - 0x90, 0x80, 0x80, 0, 0,
                       0x80, 0x80, 0x70808080, func_001F4868_l(*(int *)(ship_vars + 0x104) + 0x28));
    draw_textured_quad(0x170, *(int *)(screen_dimensions + 4) - 0x90, 0x80, 0x80, 0, 0, 0x80, 0x80, 0x70808080,
                       func_001F4868_l(*(int *)(ship_vars + 0x104) + 0x29));
    vu1_add_g_sregister(0x42, 0x8000000048L);
    update_fighter_jet_target_lock((int)moby, ship_vars);
    if (*(int *)(ship_vars + 0x88) != 0) {
        project_world_point_to_screen_pixels(*(char **)(ship_vars + 0x88) + 0x10, &locked_target_screen_x, &locked_target_screen_y, 0);
        if (*(int *)(ship_vars + 0x8C) > lock_hold_frames) {
            lock_fade_ratio = convert_integer_to_float(*(int *)(ship_vars + 0x8C) - lock_hold_frames);
            lock_fade_ratio = lock_fade_ratio / convert_integer_to_float(lock_fade_frames);
            lock_marker_fade_factor = 2.0f * (1.0f - lock_fade_ratio);
            k = lock_fade_ratio * 5.0f + 1.0f;
            if (lock_marker_fade_factor > 1.0f)
                lock_marker_fade_factor = 1.0f;
            lock_marker_fade_byte = func_001FA898_r(lock_marker_fade_factor * 96.0f);
            func_L17_002EBF08_f(convert_integer_to_float(locked_target_screen_x), convert_integer_to_float(locked_target_screen_y), k,
                                normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 30.0f), 0,
                                0xFF, 0, lock_marker_fade_byte);
            *(int *)(ship_vars + 0xEC) = 0;
        } else {
            lock_marker_blink_value = 0xFF;
            if ((*(int *)(ship_vars + 0x8C) / scale_game_frames(0x14)) & 1)
                lock_marker_blink_value = 0;
            func_L17_002EBF08_f(convert_integer_to_float(locked_target_screen_x), convert_integer_to_float(locked_target_screen_y), 1.0f,
                                0.0f, 0xFF, lock_marker_blink_value, 0, 0x60);
            *(int *)(ship_vars + 0xEC) = *(int *)(ship_vars + 0x88);
        }
    }
    y = 0x40;
    x = 0x18;
    for (i = 0; i < hero.ship_ammo_max; i++) {
        if (i < hero.ship_ammo) {
            append_scaled_screen_triangle_strip(x, y, 0.5f, D_L17_001D99E0, 0x19, 0xFFFFF3, 0x50008F00);
        } else {
            append_scaled_screen_triangle_strip(x, y, 0.5f, D_L17_001D99E0, 0x19, 0xFFFFF3, 0x20004F00);
        }
        ammo_capacity = hero.ship_ammo_max;
        if (i == ammo_capacity >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }
    ex = *(int *)(ship_vars + 0xE0);
    ey = *(int *)(ship_vars + 0xE4);
    aim_reticle_texture = func_001F4868_l(0x11);
    append_rotated_sprite_quad(ex, ey, 40.0f, 40.0f, 0.0f, 0x3F, 0x3F, aim_reticle_texture, 0xFFFFF3, 0xFF20FF20, 0,
                               0, 0.5f, 0.5f);
    append_rotated_sprite_quad(ex, ey, 40.0f, 40.0f, 0.0f, 0x3F, 0x3F, func_001F4868_l(0x12),
                               0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    append_rotated_sprite_quad(ex, ey, 10.0f, 10.0f, 0.0f, 0x1F, 0x1F, func_001F4868_l(8), 0xFFFFF3,
                               0xFF20FF20, 0, 0, 0.5f, 0.5f);
    if (hero.ship_hp < hero.ship_hp_max / 10.0f && (D_L17_0015F5CC / 90) & 1) {
        func_001F6F40_c_001f6b88(0x100, 0x186, 0x80000080L, func_001FE540_id(0x5269), 0x64);
    }
    if (moby->state == 8) {
        func_001F6F40_c_001f6b88(0x100, 0xC8, 0x80005080L, func_001FE540_id(0x523E), 0x64);
    }
    hp_display_step = hero.ship_hp - *(float *)(ship_vars + 0xF0);
    hp_display_step *= 0.2f;
    hp_display_step_magnitude = absolute_float(hp_display_step);
    if (hp_display_step_magnitude > 1.0f)
        hp_display_step /= hp_display_step_magnitude;
    *(float *)(ship_vars + 0xF0) += hp_display_step;
    hp_bar_fill_threshold = func_001FA898_r(*(float *)(ship_vars + 0xF0) * 251.0f / hero.ship_hp_max) + 2;
    if (hp_bar_fill_threshold > 0xFD)
        hp_bar_fill_threshold = 0xFD;
    if (hp_bar_fill_threshold < 2)
        hp_bar_fill_threshold = 2;
    lo = *(int *)(ship_vars + 0xF4) <= hp_bar_fill_threshold ? *(int *)(ship_vars + 0xF4) : hp_bar_fill_threshold;
    hi = *(int *)(ship_vars + 0xF4) < hp_bar_fill_threshold ? hp_bar_fill_threshold : *(int *)(ship_vars + 0xF4);
    for (j = lo; j <= hi; j++) {
        hp_bar_clut_index = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < hp_bar_fill_threshold) {
            (*(int **)&D_L17_00162180)[hp_bar_clut_index] = D_L17_001D9B68[hp_bar_clut_index];
        } else {
            (*(int **)&D_L17_00162180)[hp_bar_clut_index] = 0x80000000;
        }
    }
    *(int *)(ship_vars + 0xF4) = hp_bar_fill_threshold;
}
#define NOT_SDA

#define MACRO_ADDR

/* Builds a rotated offset vector from the moby, flips a facing flag on a pad press, and updates the aim. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EDAF0), where it is exact; names translated to the US level program. */

extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int spawn_class_3f1_moby(void *, void *, void *, float, float) __asm__("FUN_L11_00308848");
extern f32 D_L17_001620A0_gp __asm__("D_L17_001620A0") __attribute__((sda));
extern unsigned char D_0013A4E0_u[] __asm__("D_0013A4E0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

extern short D_L17_0016206C __attribute__((sda));
int D_L17_00162070 = 4;
void update_fighter_jet_aim_and_fire_projectile(char *moby, char *ship_vars, float aim_azimuth, float aim_elevation) __asm__("FUN_L17_002ec000");

void update_fighter_jet_aim_and_fire_projectile(char *moby, char *ship_vars, float aim_azimuth, float aim_elevation) {
    char aim_direction[16];
    char projectile_spawn_position[16];
    char projectile_spawn_offset[16];
    char aim_world_position[16];
    build_spherical_offset(aim_direction, *(float *)&D_L17_0016206C * *(float *)&D_L17_001620A0_gp, aim_azimuth, aim_elevation);
    fast_vec_add(aim_direction, aim_direction, ship_vars);
    if (fast_dec_timer(ship_vars + 0x80) && (*(int *)(D_0013A4E0_u + 0x2610) & 0x84)) {
        allocate_voice_for_target_entry(3, 0, (int)moby);
        fast_vec_normalize(projectile_spawn_offset, aim_direction, 2.2f);
        moby_point_position(moby, 3 + (ship_vars[0x61] & 1), projectile_spawn_position);
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
        spawn_class_3f1_moby(moby, aim_direction, projectile_spawn_position, 200.0f, -1.0f);
        *(int *)(ship_vars + 0x80) = D_L17_00162070;
        ship_vars[0x61] ^= 1;
    }
    fast_vec_normalize(aim_direction, aim_direction, 23.0f);
    fast_vec_add(aim_world_position, aim_direction, moby + 0x10);
    project_world_point_to_screen_pixels(aim_world_position, ship_vars + 0xE0, ship_vars + 0xE4, 0);
}
/* Fires a projectile from the moby when the trigger is ready and the player presses a button. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EDC40), where it is exact; names translated to the US level program. */

extern int spawn_ship_rocket(void *, void *, int, void *, float, int) __asm__("FUN_L13_002e6a58");
extern unsigned char D_0013E533_u[] __asm__("D_0013E533");

extern short D_L17_00162074 __attribute__((sda));
int D_L17_00162078 = 30;
void try_fire_fighter_jet_rocket(char *moby, char *ship_vars) __asm__("FUN_L17_002ec150");

void try_fire_fighter_jet_rocket(char *moby, char *ship_vars) {
    char projectile_spawn_position[16];
    char projectile_spawn_offset[16];
    if (fast_dec_timer(ship_vars + 0x84) && (*(int *)(D_0013A4E0_u + 0x2610) & 0x28)) {
        if (hero.ship_ammo != 0) {
            char *projectile_initial_rotation = moby + 0x40;
            float projectile_scale;
            int scaled_frame_count;
            unsigned short t = (*(unsigned short *)(ship_vars + 0x6A) + 1) & 3;
            *(unsigned short *)(ship_vars + 0x6A) = t;
            moby_point_position(moby, t + 7, projectile_spawn_position);
            fast_vec_normalize(projectile_spawn_offset, ship_vars, 0.3f);
            fast_vec_add(projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
            projectile_scale = *(float *)&D_L17_00162074 * *(float *)&D_L17_001620A0_gp;
            scaled_frame_count = scale_game_frames(300);
            if (spawn_ship_rocket(moby, projectile_spawn_position, *(int *)(ship_vars + 0xEC), projectile_initial_rotation, projectile_scale, scaled_frame_count)) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                hero.ship_ammo -= 1;
                *(int *)(ship_vars + 0x84) = D_L17_00162078;
            }
        }
    }
}

#include "qcopy.h"

/* Flies the player's ship: throttle and stick input, steering toward the target, camera, and collision response. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EDE50), where it is exact; names translated to the US level program. */

typedef float W4[4] __attribute__((aligned(16)));
typedef unsigned int Q_c4 __attribute__((mode(TI), aligned(16)));
typedef union {
    Q_c4 q;
    W4 v;
} UQ_c4;
typedef struct {
    UQ_c4 e[2];
} T2_c4;

typedef struct {
    char pad00[0x10];
    W4 pos;
    unsigned char state;
    char pad21[0x13];
    unsigned short flags;
    char pad36[0xA];
    float r40;
    float r44;
    float r48;
    char pad4C[0x70];
    unsigned char bc;
} Moby;

typedef struct {
    char pad00[0x10];
    W4 v10;
    float f20, f24, f28, f2C;
    float f30, f34, f38, f3C;
    char pad40[0x20];
    unsigned char sel;
    unsigned char flip;
    short s62;
    float speed;
    short count;
    short s6A;
    float f6C;
    float f70;
    char pad74[0x1C];
    float f90, f94, f98, f9C;
    float fA0, fA4, fA8, fAC;
    float fB0, fB4, fB8, fBC;
    float fC0, fC4, fC8, fCC;
    float fD0, fD4, fD8;
    int iDC;
    char padE0[0x2C];
    int i10C;
    int i110;
} Obj;

extern char *D_L17_001600EC;
extern char D_L17_00174740[];
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float D_L17_0016D3F0 __attribute__((section(".data")));
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern float FUN_L00_0025bc98(char *, char *, int, float, float, float, float);
extern float interpolate_value_with_repeated_squaring(int, int, float, float, float, float, float) __asm__("FUN_L13_002ba088");
extern float fast_add_rotations(float, float);
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *a);
extern float D_L17_00161FF8 __attribute__((sda));
extern float D_L17_00161FFC __attribute__((sda));
int D_L17_00162000 = 120;
extern float D_L17_00162004 __attribute__((sda));
extern float D_L17_00162008 __attribute__((sda));
extern float D_L17_0016200C __attribute__((sda));
extern float D_L17_00162010 __attribute__((sda));
int D_L17_00162014 = 5;
extern float D_L17_00162018 __attribute__((sda));
extern float D_L17_0016201C __attribute__((sda));
extern float D_L17_00162024 __attribute__((sda));
extern float D_L17_00162028 __attribute__((sda));
extern float D_L17_0016202C __attribute__((sda));
extern float D_L17_00162030 __attribute__((sda));
extern float D_L17_00162034 __attribute__((sda));
extern float D_L17_00162038 __attribute__((sda));
extern float D_L17_0016203C __attribute__((sda));
extern float D_L17_00162040 __attribute__((sda));
extern float D_L17_00162044 __attribute__((sda));
extern float D_L17_00162048 __attribute__((sda));
extern float D_L17_0016204C __attribute__((sda));
extern float D_L17_0016205C __attribute__((sda));
extern float D_L17_00162060 __attribute__((sda));
extern float D_L17_0016208C __attribute__((sda));
extern float D_L17_00162090 __attribute__((sda));
extern float D_L17_0016209C __attribute__((sda));
extern float D_L17_001620A0_gp __asm__("D_L17_001620A0") __attribute__((sda));
extern float D_L17_001620AC __attribute__((sda));
extern float D_L17_001620B0 __attribute__((sda));
extern float D_L17_001620B4 __attribute__((sda));
extern float D_L17_001620B8 __attribute__((sda));
extern float D_L17_001620BC __attribute__((sda));
extern float D_L17_001620C0 __attribute__((sda));
extern float D_L17_001620C4 __attribute__((sda));
extern float D_L17_001620C8 __attribute__((sda));
extern float D_L17_001620CC __attribute__((sda));
extern float D_L17_001620D0 __attribute__((sda));
extern float D_L17_001620D4 __attribute__((sda));
extern float D_L17_001620F0 __attribute__((sda));
extern float D_L17_001620F4 __attribute__((sda));
extern float D_L17_001620F8 __attribute__((sda));
extern float D_L17_001620FC __attribute__((sda));
extern float D_L17_00162100 __attribute__((sda));
extern float D_L17_00162104 __attribute__((sda));
extern float D_L17_00162108 __attribute__((sda));
extern float D_L17_0016210C __attribute__((sda));
extern float D_L17_00162110 __attribute__((sda));
extern float D_L17_00162114 __attribute__((sda));
extern float D_L17_00162118 __attribute__((sda));
extern float D_L17_0016211C __attribute__((sda));
extern float D_L17_00162120 __attribute__((sda));
extern float D_L17_00162124 __attribute__((sda));
extern float D_L17_00162128 __attribute__((sda));
extern float D_L17_0016212C __attribute__((sda));
extern float D_L17_00162130 __attribute__((sda));
extern float D_L17_00162134 __attribute__((sda));
extern float D_L17_00162138 __attribute__((sda));
extern float D_L17_0016213C __attribute__((sda));
extern float D_L17_00162140 __attribute__((sda));
extern float D_L17_00162144 __attribute__((sda));
extern float D_L17_00162148 __attribute__((sda));
extern float D_L17_0016214C __attribute__((sda));
extern float D_L17_00162150 __attribute__((sda));
extern float D_L17_00162154 __attribute__((sda));
extern float D_L17_00162160 __attribute__((sda));
extern float D_L17_00162164 __attribute__((sda));
extern float D_L17_00162168 __attribute__((sda));
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern s32 allocate_voice_for_target_entry_c4(s32, s32, void *) __asm__("func_0022DA68");
int D_L17_0016215C = 10;
float D_L17_001621F0[2] = {512.0f, 512.0f};
extern unsigned short D_L17_0016217C __attribute__((sda));
extern unsigned char D_0013A4E0_c4[] __asm__("D_0013A4E0");
extern unsigned char D_0013E533_c4[] __asm__("D_0013E533");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void draw_fighter_jet_hud();
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void transform_vector_by_basis(void *, void *, void *);
extern void update_view_context() __asm__("func_001F2D98");
extern void vu_euler_rotation_basis(void *, void *);
float absolute_float(float input) __asm__("func_001F99C0");
void update_fighter_jet_aim_and_fire_projectile(char *moby, char *obj, float p2, float p3) __asm__("FUN_L17_002ec000");
void try_fire_fighter_jet_rocket(char *moby, char *obj) __asm__("FUN_L17_002ec150");
void adjust_fighter_jet_steering_for_flight_bounds(char *moby, float *horizontal_steering_input, float *vertical_steering_input) __asm__("FUN_L17_002edf10");

void adjust_fighter_jet_steering_for_flight_bounds(char *moby, float *a, float *b) __asm__("FUN_L17_002edf10");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");
extern char D_0013C940[];
extern char D_0013E550[];
extern char D_0013CAE0[];
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");

void update_fighter_jet_flight(Moby *ship_moby, Obj *ship_vars) __asm__("FUN_L17_002ec360");

void update_fighter_jet_flight(Moby *ship_moby, Obj *ship_vars) {
    W4 old;
    W4 vel;
    UQ_c4 movement_delta;
    W4 side;
    UQ_c4 up;
    W4 ang;
    W4 target;
    W4 diff;
    float horizontal_steering_input;
    float vertical_steering_input;
    float ka;
    float kb;
    float lenA;
    float lenB;
    float distance_to_default_target;
    float target_height_distance, target_height_distance_for_moby_pitch;
    float x, y;
    float half_fov, tn;
    float ratio;
    float collision_adjusted_pitch;
    float k;
    char *pad;
    struct Hero *steering_input_hero;
    char *e;
    char *collision_result;
    int voice_slot_index;
    int hc;

    movement_delta.q = 0;
    up.q = 0;
    movement_delta.v[2] = frame_time * 8.0f * D_L17_001620A0_gp;
    up.v[2] = 1.0f;
    ship_moby->flags &= 0xFEFF;
    *(Q_c4 *)vel = movement_delta.q;
    if (ship_moby->bc == 0) {
        if (D_L17_00162000 < ship_vars->count) {
            pad = (char *)((char *)&D_0013C940);
            if (*(int *)(pad + 0x1B4) & 0x40) {
                voice_slot_index = ship_vars->i10C;
                if (voice_slot_index != -1) {
                    e = (char *)((char *)&D_0013E550) + voice_slot_index * 0x70;
                    if (*(char **)(e + 0x88) == (char *)ship_moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                ship_vars->i10C = -1;
                ship_vars->i10C = allocate_voice_for_target_entry_c4(1, 4, (int)ship_moby);
            } else if (*(int *)(pad + 0x1B8) & 0x40) {
                voice_slot_index = ship_vars->i10C;
                if (voice_slot_index != -1) {
                    e = (char *)((char *)&D_0013E550) + voice_slot_index * 0x70;
                    if (*(char **)(e + 0x88) == (char *)ship_moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                ship_vars->i10C = -1;
            }
            pad = (char *)((char *)&D_0013C940);
            if (*(int *)(pad + 0x1B0) & 0x40) {
                float accelerated_speed = ship_vars->speed + D_L17_0016200C * D_L17_001620A0_gp;
                float max = D_L17_00162008 * D_L17_001620A0_gp;
                ship_vars->speed = accelerated_speed;
                if (max < accelerated_speed) {
                    ship_vars->speed = max;
                }
            } else {
                float hi = D_L17_00162004 * D_L17_001620A0_gp;
                float step = D_L17_00161FFC * D_L17_001620A0_gp;
                float current_speed = ship_vars->speed;
                if (current_speed < hi - step) {
                    ship_vars->speed = current_speed + step;
                } else if (hi < current_speed) {
                    ship_vars->speed = current_speed + (hi - current_speed) * D_L17_00162010;
                } else {
                    ship_vars->speed = hi;
                }
            }
        } else {
            ship_vars->speed += D_L17_00161FF8 * D_L17_001620A0_gp;
            ship_vars->count += 1;
        }
    } else {
        ship_vars->speed = 0.0001f;
    }

    if (D_L17_00162014 < ship_vars->count) {
        steering_input_hero = &hero;
        x = steering_input_hero->stick_x;
        y = steering_input_hero->stick_y;
        ka = D_L17_001620B4;
        kb = D_L17_001620C4;
        horizontal_steering_input = x;
        vertical_steering_input = y;
        if (*(int *)(((char *)&D_0013CAE0)) & 0x200) {
            horizontal_steering_input = x / D_L17_00162028 * D_L17_00162030;
            vertical_steering_input = y / D_L17_00162024 * D_L17_0016202C;
            ka = D_L17_001620B8;
            kb = D_L17_001620C8;
        }
        adjust_fighter_jet_steering_for_flight_bounds((char *)ship_moby, &horizontal_steering_input, &vertical_steering_input);
        vendor_spring(&ship_vars->f6C, &ship_vars->f90, horizontal_steering_input, D_L17_001620AC, D_L17_001620B0, ka);
        vendor_spring(&ship_vars->f70, &ship_vars->f94, vertical_steering_input, D_L17_001620BC, D_L17_001620C0, kb);
        FUN_L00_0025bc98((char *)&ship_vars->f30, (char *)&ship_vars->f98, 0, ship_vars->f6C * D_L17_0016208C,
                         D_L17_001620CC, D_L17_001620D0, D_L17_001620D4);
        ship_vars->f34 = fast_subtract_rotations(ship_vars->f34, ship_vars->f70 * D_L17_00162024);
        ship_vars->f38 = fast_subtract_rotations(ship_vars->f38, ship_vars->f6C * D_L17_00162028);
        if (ship_vars->f34 > 1.3962634f) {
            ship_vars->f34 = 1.3962634f;
        } else if (ship_vars->f34 < -1.3962634f) {
            ship_vars->f34 = -1.3962634f;
        }
        if (ship_moby->r44 > 1.3962634f) {
            ship_moby->r44 = 1.3962634f;
        } else if (ship_moby->r44 < -1.3962634f) {
            ship_moby->r44 = -1.3962634f;
        }
    }

    lenA = 310.0f;
    lenB = 500.0f;
    if (ship_vars->i110 != -1) {
        int target_record_byte_offset = ship_vars->i110 << 7;
        char *base = D_L17_001600EC;
        qcopy(target, ((char(*)[128])base)[ship_vars->i110] + 0x30);
        lenA = vector_length_xyz(base + target_record_byte_offset + 0x20);
        add_vector_xyz(diff, (ship_vars->i110 << 7) + D_L17_001600EC,
                       (ship_vars->i110 << 7) + D_L17_001600EC + 0x10);
        lenB = vector_length_xyz(diff);
    } else {
        qcopy(target, D_L17_001621F0);
    }

    distance_to_default_target = vector_distance(ship_moby->pos, D_L17_001621F0);
    ship_vars->f38 = interpolate_value_with_repeated_squaring(
        1, 5, distance_to_default_target, ship_vars->f38, fast_arc_tan(target[0] - ship_moby->pos[0], target[1] - ship_moby->pos[1]), 50.0f, lenB);
    ship_moby->r48 = interpolate_value_with_repeated_squaring(
        1, 5, distance_to_default_target, ship_moby->r48, fast_arc_tan(target[0] - ship_moby->pos[0], target[1] - ship_moby->pos[1]), 50.0f, lenB);
    target_height_distance = absolute_float(target[2] - ship_moby->pos[2]);
    ship_vars->f34 = interpolate_value_with_repeated_squaring(1, 5, target_height_distance, ship_vars->f34,
                              -fast_arc_tan(vector_distance(ship_moby->pos, target), target[2] - ship_moby->pos[2]),
                              50.0f, lenA);
    target_height_distance_for_moby_pitch = absolute_float(target[2] - ship_moby->pos[2]);
    ship_moby->r44 = interpolate_value_with_repeated_squaring(1, 5, target_height_distance_for_moby_pitch, ship_moby->r44,
                              -fast_arc_tan(vector_distance(ship_moby->pos, target), target[2] - ship_moby->pos[2]),
                              50.0f, lenA);
    build_spherical_offset(movement_delta.v, ship_vars->speed, ship_vars->f38, -ship_vars->f34);
    normalize_vector_xyz(ship_vars->v10, movement_delta.v, 1.0f);
    qcopy(old, ship_moby->pos);
    if (ship_moby->bc == 0) {
        add_vector_xyz(ship_moby->pos, ship_moby->pos, movement_delta.v);
    }
    {
        float lo = D_L17_00162004;
        float sp = ship_vars->speed - lo;
        float range = D_L17_00162008 - lo;
        if (sp < 0.0f) {
            ratio = 0.0f / range;
        } else {
            ratio = sp / range;
        }
    }
    ship_vars->fC4 = ratio;
    {
        T2_c4 tbl;
        T2_c4 wr;
        float ship_rotation_basis[12];
        wr.e[0].q = 0;
        wr.e[1].q = 0;
        wr.e[0].v[0] =
            (D_L17_00162034 - ship_vars->f70 * D_L17_00162138 - ratio * D_L17_00162140) * D_L17_0016209C;
        wr.e[0].v[1] = ship_vars->f6C * D_L17_00162090 * D_L17_0016209C;
        wr.e[0].v[2] = (D_L17_00162038 + ship_vars->f70 * D_L17_0016213C) * D_L17_0016209C;
        wr.e[1].v[0] = D_L17_0016203C * D_L17_0016209C;
        wr.e[1].v[2] = D_L17_00162040 * D_L17_0016209C;
        tbl.e[0].q = wr.e[0].q;
        tbl.e[1].q = wr.e[1].q;
        vendor_spring(&ship_vars->fB4, &ship_vars->fC8, tbl.e[ship_vars->sel].v[0], D_L17_00162114, D_L17_00162118,
                         D_L17_0016211C);
        vendor_spring(&ship_vars->fB8, &ship_vars->fCC, tbl.e[ship_vars->sel].v[1], D_L17_00162120, D_L17_00162124,
                         D_L17_00162128);
        vendor_spring(&ship_vars->fBC, &ship_vars->fD0, tbl.e[ship_vars->sel].v[2], D_L17_0016212C, D_L17_00162130,
                         D_L17_00162134);
        diff[0] = ship_vars->fB4;
        diff[1] = ship_vars->fB8;
        diff[2] = ship_vars->fBC;
        diff[3] = 0.0f;
        vu_euler_rotation_basis(ship_rotation_basis, (char *)&ship_vars->f30);
        transform_vector_by_basis(target, diff, ship_rotation_basis);
        add_vector_xyz(wr.e[0].v, ship_moby->pos, target);
        FUN_L00_002ea9d8(wr.e[0].v);
        wr.e[1].v[0] = fast_add_rotations(ship_vars->f30 * D_L17_00162048, ship_vars->f6C * D_L17_00162044);
        wr.e[1].v[1] = fast_subtract_rotations(fast_add_rotations(ship_vars->f34, D_L17_0016204C),
                                               ship_vars->f70 * D_L17_00162024);
        wr.e[1].v[2] = ship_vars->f38;
        FUN_L00_0025bc98((char *)&ship_vars->f20, (char *)&ship_vars->fA0, 0, wr.e[1].v[0], D_L17_001620F0,
                         D_L17_001620F4, D_L17_001620F8);
        FUN_L00_0025bc98((char *)&ship_vars->f24, (char *)&ship_vars->fA4, 0, wr.e[1].v[1], D_L17_001620FC,
                         D_L17_00162100, D_L17_00162104);
        FUN_L00_0025bc98((char *)&ship_vars->f28, (char *)&ship_vars->fA8, 0, wr.e[1].v[2], D_L17_00162108,
                         D_L17_0016210C, D_L17_00162110);
    }
    ang[0] = ship_vars->f20;
    ang[2] = ship_vars->f28;
    ang[1] = ship_vars->f24;
    FUN_L00_002eaa30(ang);
    vendor_spring(&ship_vars->fB0, &ship_vars->fC0, D_L17_00162144 + (D_L17_00162148 - D_L17_00162144) * ratio,
                     D_L17_0016214C, D_L17_00162150, D_L17_00162154);
    half_fov = ship_vars->fB0 * 0.5f;
    tn = fast_sin(half_fov) / fast_cos(half_fov);
    D_L17_0016D3F0 = tn;
    update_view_context();
    ship_moby->r40 = fast_add_rotations(ship_vars->f30, ship_vars->f6C * D_L17_00162160);
    ship_moby->r44 = fast_add_rotations(ship_vars->f34, -ship_vars->f70 * D_L17_00162164);
    ship_moby->r48 = fast_add_rotations(ship_vars->f38, -ship_vars->f6C * D_L17_00162168);
    if (coll_sphere(1.2f, ship_moby->pos, 0, ship_moby)) {
        collision_result = D_L17_00174740;
        if (*(char **)(collision_result + 0x18) == 0 ||
            (hc = *(short *)(*(char **)(collision_result + 0x18) + 0xA6),
             hc != 0 && hc != 0x192 && hc != 0x4C2 && hc != 0x4C3 && hc != 0x52 && hc != 0x53)) {
            qcopy(ship_moby->pos, collision_result + 0x30);
            FUN_L00_001ff660(movement_delta.v, movement_delta.v, collision_result + 0x40);
            ship_vars->f38 = fast_add_rotations(
                ship_vars->f38,
                fast_subtract_rotations(fast_arc_tan(movement_delta.v[0], movement_delta.v[1]), ship_vars->f38) * D_L17_00162018);
            collision_adjusted_pitch = fast_add_rotations(
                ship_vars->f34,
                fast_subtract_rotations(-fast_arc_tan(vector_length_xy(movement_delta.v), movement_delta.v[2]), ship_vars->f34) *
                    D_L17_0016201C);
            k = ship_vars->speed * D_L17_0016205C;
            ship_vars->f34 = collision_adjusted_pitch;
            ship_vars->speed = k;
            if (*(char **)(collision_result + 0x18) != 0 && *(short *)(*(char **)(collision_result + 0x18) + 0xA6) != 0x15B) {
                if (ship_vars->iDC == 0) {
                    hero.ship_hp = hero.ship_hp - D_L17_00162060;
                    if (hero.ship_hp < 0.0f) {
                        subtract_vector_xyz(side, collision_result + 0x50, collision_result + 0x60);
                        normalize_vector_xyz(side, side, 1.0f);
                        FUN_L00_001ff660(vel, movement_delta.v, collision_result + 0x40);
                        normalize_vector_xyz(vel, vel, frame_time + frame_time);
                        normalize_vector_xyz(up.v, collision_result + 0x40, 1.0f);
                        func_L00_0025F4A8_alt(ship_moby, vel, 0, 8.0f, 100.0f, 0x1E, 0xA, 0x18, 16.0f, 8.0f,
                                              9.0f, 1.0f, -1, 50.0f, 1, 1, -1, 0);
                        ship_moby->state = 7;
                    }
                    ship_vars->iDC = D_L17_0016215C;
                }
            } else {
                *(int *)&hero.ship_hp = 0;
                func_L00_0025F4A8_alt(ship_moby, vel, 0, 8.0f, 100.0f, 0x1E, 0xA, 0x18, 16.0f, 8.0f, 9.0f,
                                      1.0f, -1, 50.0f, 1, 1, -1, 0);
                ship_vars->s62 = D_L17_0016217C;
                ship_moby->state = 7;
                ship_moby->flags |= 0x41;
            }
        }
    }
    enqueue_callback_list_1(draw_fighter_jet_hud, ship_moby);
    try_fire_fighter_jet_rocket((char *)ship_moby, (char *)ship_vars);
    update_fighter_jet_aim_and_fire_projectile((char *)ship_moby, (char *)ship_vars, fast_add_rotations(ship_vars->f28, -ship_vars->f6C / 7.0f),
                     -fast_add_rotations(ship_vars->f24, -(ship_vars->f70 / 7.0f) - 0.1f));
}
/* Per-frame update for the player's ship moby on Veldin Orbit: takes hits, boards and leaves the ship, and runs the flight states. */

typedef struct {
    char pad00[0x10];
    W4 pos;
    unsigned char state;
    char pad21[3];
    char *pclass;
    char pad28[4];
    float scale;
    unsigned char b30;
    char pad31;
    unsigned short h32;
    unsigned short flags;
    char pad36[0xA];
    W4 rot;
    char pad50[0x28];
    char *vars;
    char pad7C[0x28];
    unsigned char bA4;
    char padA5;
    unsigned short oclass;
    char padA8[8];
    unsigned char bB0;
} ShipMoby;

typedef struct {
    W4 dir;
    W4 v10;
    float f20, f24, f28, f2C;
    W4 rot;
    W4 home_pos;
    W4 home_rot;
    unsigned char b60;
    char pad61;
    short hit_timer;
    float speed;
    short s68;
    short s6A;
    float f6C;
    float f70;
    char pad74[0xC];
    int i80;
    int i84;
    char pad88[0x24];
    float fAC;
    float fB0;
    float fB4;
    float fB8;
    float fBC;
    float fC0;
    char padC4[0x18];
    int timerDC;
    char padE0[8];
    int iE8;
    char padEC[4];
    int iF0;
    int iF4;
    int timerF8;
    int exit_path;
    int exit_path2;
    int i104;
    int voice;
    int voice2;
    char pad110[0xC];
    int i11C;
    int enter_path;
    int i124;
    char pad128[8];
    float fade;
} ShipVars;

typedef struct {
    char pad00[0x2C];
    float damage;
    int flags;
} ShipHit;

extern char D_0013E550_ed[] __asm__("D_0013E550");
extern unsigned char D_0014C050_ed[][16] __asm__("D_0014C050");
extern char D_0013CAE4_ed[] __asm__("D_0013CAE4");
typedef struct {
    char pad0[0xA];
    unsigned short idx;
    char padC[4];
} ShipLevelEntry;
extern char D_L17_001D9B68_ed[] __asm__("D_L17_001D9B68");
extern char D_L17_001E6E00[];
char D_L17_001E6E38[] = "no final place for hero during victory, so just fall\n";
extern float fighter_jet_boarding_distance_limit __asm__("D_L17_00161FE0") __attribute__((sda));
extern float fighter_jet_initial_flight_speed __asm__("D_L17_00161FE4") __attribute__((sda));
extern float D_L17_00162054 __attribute__((sda));
extern float fighter_jet_starting_hp __asm__("D_L17_00162058") __attribute__((sda));
/* tan(fov / 2) of the view (set by FUN_L17_002ec360); read as an array element, which keeps
   the load after the stores to the ship vars like retail. */
extern float D_L17_0016D3F0_a[] __asm__("D_L17_0016D3F0");
extern unsigned char D_L17_00162064 __attribute__((sda));
extern unsigned char D_L17_00162068 __attribute__((sda));
extern float fighter_jet_max_hp __asm__("D_L17_00162084") __attribute__((sda));
extern float D_L17_00162088 __attribute__((sda));
int D_L17_00162178 = 180;
char *hp_bar_clut __asm__("D_L17_00162180") = 0;
int hp_bar_saved __asm__("D_L17_00162184") = 0;
typedef struct {
    char pad[0x160];
    float f160;
    int pad164;
    int i168;
} ShipShake;
typedef struct {
    char pad[0x98];
    int unk98;
} ShipCamera;
typedef struct {
    char b[0x400];
} ShipBlock400;

extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float FUN_00213ed8(float *, float, float);
extern int FUN_00215130(int, int);
extern int force_help_message(int, int) __asm__("FUN_002151d8");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void set_animation_parameter(int, int) __asm__("FUN_001ff570");
extern void debug_print(char *) __asm__("FUN_001e93b0");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");
extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern int remove_help_message_for_selector(int) __asm__("FUN_L00_002644e0");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern void jet_camera_mode(void) __asm__("FUN_L00_00203b18");
extern void clear_jet_camera_mode_latch(void) __asm__("FUN_L00_00203b78");
extern void hero_respawn(void) __asm__("FUN_L00_00211250");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void emit_type_74_particles_from_moby_joints(void *, void *) __asm__("FUN_L11_003112b8");
extern void jet_group_state(int, int, int, int) __asm__("FUN_L13_002650a8");
extern void FUN_L17_0021e530(int, int);
extern void update_moby_class_flags_and_link(int, int, int, int) __asm__("FUN_L17_00253a90");
extern int allocate_voice_ed(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_ed(int) __asm__("FUN_0022d798");
extern void spherical_offset_ed(void *, float, float, float) __asm__("FUN_00214db0");
extern void add_vector_ed(void *, void *, void *) __asm__("FUN_001f9a10");

extern float D_L17_0015F3FC;
extern int D_L17_0015F608;
extern char *D_L17_0015F420;
extern ShipLevelEntry D_L17_0016DA80[];
extern ShipShake D_L17_00167580;
void fighter_jet_update(ShipMoby *moby) __asm__("FUN_L17_002ed018");
void fighter_jet_update(ShipMoby *moby) {
    enum {
        FIGHTER_JET_STATE_INITIALIZE = 0,
        FIGHTER_JET_STATE_WAIT_FOR_BOARDING = 1,
        FIGHTER_JET_STATE_BOARDING_FADE = 2,
        FIGHTER_JET_STATE_PILOTED_FLIGHT = 4,
        FIGHTER_JET_STATE_DISMOUNT = 5,
        FIGHTER_JET_STATE_DEATH_RESPAWN_DELAY = 7,
        FIGHTER_JET_STATE_VICTORY_EXIT = 8,
        FIGHTER_JET_STATE_FADE_THEN_REMOVE = 9
    };
    ShipVars *ship_vars;
    ShipHit *hit;
    W4 dir;
    char *e;
    int idx;
    int board;
    float k;
    float fov;

    if (moby != 0) {
        ship_vars = (ShipVars *)moby->vars;
        fast_dec_timer(&ship_vars->timerDC);
        if (moby->state != 7 && moby->state != 1 && moby->state != 2) {
            hit = (ShipHit *)find_moby_hit(moby, 0x30001, 0);
            if (hit != 0 && moby->state == 4) {
                struct Hero *g = &hero;
                if (hit->damage < g->ship_hp) {
                    g->ship_hp -= hit->damage;
                    if (hit->flags & 1) {
                        ShipShake *s = &D_L17_00167580;
                        s->f160 = 1.0f;
                        s->i168 = scale_game_frames(4);
                        k = hit->damage * D_L17_00162088;
                        spherical_offset_ed(dir, ship_vars->speed, ship_vars->rot[2], -ship_vars->rot[1]);
                        clamp_vector_length((char *)hit + 0x10, (char *)hit + 0x10, frame_time * 3.0f);
                        add_vector_ed(moby->pos, moby->pos, (char *)hit + 0x10);
                        FUN_L00_001ff660(dir, dir, (char *)hit + 0x10);
                        ship_vars->rot[2] = fast_add_rotations(
                            ship_vars->rot[2],
                            fast_subtract_rotations(fast_arc_tan(dir[0], dir[1]), ship_vars->rot[2]) * D_L17_00162018 * k);
                        ship_vars->rot[1] = fast_add_rotations(
                            ship_vars->rot[1],
                            fast_subtract_rotations(fast_arc_tan(vector_length_xy(dir), dir[2]), ship_vars->rot[1]) *
                                D_L17_0016201C * k);
                    }
                } else {
                    spherical_offset_ed(dir, ship_vars->speed, ship_vars->rot[2], -ship_vars->rot[1]);
                    {
                        ShipShake *s = &D_L17_00167580;
                        s->f160 = 0.2f;
                        s->i168 = scale_game_frames(5);
                    }
                    g->ship_hp = 0.0f;
                    moby->state = 7;
                    ship_vars->hit_timer = scale_game_frames(30);
                    g->motion.pos.f[2] += 0.2f;
                    ((ShipCamera *)g->moby)->unk98 = 0;
                    FUN_L17_0021e530(0, 1);
                    g->ship_moby = 0;
                    g->ship_oclass = -1;
                    spawn_explosion(moby, dir, 0, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, -1, 1.0f,
                                     15.0f, 1, 1, -1, 0);
                    moby->flags |= 0x41;
                }
            }
        }
        moby->bA4 = 0xFF;
        {
            struct Hero *g = &hero;
            if (g->ship_moby == (struct Moby *)moby) {
                g->ship_hp_percent =
                    g->ship_hp / fighter_jet_max_hp * 200.0f;
            }
        }
        switch (moby->state) {
        case FIGHTER_JET_STATE_INITIALIZE:
            moby->state = 1;
            ship_vars->b60 = 0;
            moby->scale = *(float *)(moby->pclass + 0x24);
            ship_vars->f20 = ship_vars->rot[0];
            ship_vars->f24 = ship_vars->rot[1];
            fov = fast_arc_tan(1.0f, D_L17_0016D3F0_a[0]) * 2.0f;
            ship_vars->fC0 = 0.0f;
            ship_vars->fAC = fov;
            ship_vars->fB0 = fov;
            qcopy_nc(ship_vars->rot, moby->rot);
            qcopy_nc(ship_vars->home_rot, moby->rot);
            qcopy_nc(ship_vars->home_pos, moby->pos);
            {
                int hp_bar_level_entry_index = ship_vars->i104 + 0x28;
                hp_bar_clut = D_L17_0015F420 + (D_L17_0016DA80 + hp_bar_level_entry_index)->idx * 16;
            }
            if (hp_bar_saved == 0) {
                *(ShipBlock400 *)D_L17_001D9B68_ed = *(ShipBlock400 *)hp_bar_clut;
            }
            hp_bar_saved = 1;
            ship_vars->timerF8 = 1;
            ship_vars->iE8 = 1;
            if (moby->bB0 != 0xFF && D_0014C050_ed[current_level_index][moby->bB0] == 0xFF) {
                moby->state = 9;
            }
            if (ship_vars->i11C > 0) {
                jet_group_state(ship_vars->i11C, 0, 0, 0);
            }
            update_moby_class_flags_and_link(0x325, 1, 1, -1);
            ship_vars->i124 = -1;
            break;
        case FIGHTER_JET_STATE_WAIT_FOR_BOARDING: {
            struct Hero *g = &hero;
            board = jet_distance(moby, (ShipCamera *)g->moby) < fighter_jet_boarding_distance_limit &&
                    FUN_00215130(7, 0x53E4) != 0 && (*(int *)D_0013CAE4_ed & 0x10);
            if (board) {
                remove_help_message_for_selector(7);
                moby->state = 2;
                ship_vars->i124 = -1;
            } else if (ship_vars->fade != 0.0f) {
                FUN_00213ed8(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                D_L17_0015F3FC = ship_vars->fade;
            }
            jet_animate(moby, D_L17_00162054 * D_L17_0016209C);
            break;
        }
        case FIGHTER_JET_STATE_BOARDING_FADE:
            FUN_00213ed8(&ship_vars->fade, 1.0f, frame_time * 4.0f);
            D_L17_0015F3FC = ship_vars->fade;
            if (ship_vars->fade == 1.0f) {
                struct Hero *g = &hero;
                g->ship_moby = (struct Moby *)moby;
                g->ship_oclass = moby->oclass;
                g->ship_ammo = D_L17_00162064;
                g->unk160E = 1;
                g->ship_ammo_max = D_L17_00162068;
                g->unk15F8 = D_L17_00162068;
                g->ship_hp = fighter_jet_starting_hp;
                ship_vars->iF0 = 0;
                ship_vars->iF4 = 0xFF;
                g->ship_hp_max = fighter_jet_max_hp;
                g->unk1604 = 100.0f;
                g->unk15F9 = 3;
                g->unk15FA = 3;
                g->unk15FB = 3;
                g->ship_flags = 0;
                jet_game_state(2, 4);
                if (ship_vars->enter_path != -1) {
                    char *p = D_L17_001600EC + (ship_vars->enter_path << 7);
                    FUN_L00_00216f90(p + 0x30, p + 0x70, 50, 1);
                    qcopy_nc(moby->pos, ((char (*)[128])D_L17_001600EC)[ship_vars->enter_path] + 0x30);
                    qcopy_nc(moby->rot, ((char (*)[128])D_L17_001600EC)[ship_vars->enter_path] + 0x70);
                }
                force_help_message(6, 0);
                FUN_L17_0021e530(50, 1);
                jet_mount(moby->pos, moby->rot, 1, 0, 0);
                ship_vars->s68 = 0;
                ship_vars->speed = fighter_jet_initial_flight_speed;
                ship_vars->f6C = 0.0f;
                ship_vars->f70 = 0.0f;
                ship_vars->i84 = 0;
                ship_vars->i80 = 0;
                moby->h32 = 0xFF;
                moby->b30 = 0xFF;
                ship_vars->f28 = ship_vars->rot[2] = moby->rot[2];
                ship_vars->fB4 = D_L17_00162034 * D_L17_0016209C;
                ship_vars->fBC = D_L17_00162038 * D_L17_0016209C;
                ship_vars->fB8 = 0.0f;
                ((ShipCamera *)g->moby)->unk98 = -1;
                spherical_offset_ed(ship_vars->dir, fighter_jet_initial_flight_speed, ship_vars->rot[2], -ship_vars->rot[1]);
                if (ship_vars->i11C > 0) {
                    jet_group_state(ship_vars->i11C, 1, 1, 1);
                }
                update_moby_class_flags_and_link(0x325, 0, 0, -1);
                moby->pos[2] += 3.0f;
                moby->scale = *(float *)(moby->pclass + 0x24) * D_L17_0016209C;
                g->unk20A5 = 1;
                if (ship_vars->voice == -1) {
                    ship_vars->voice = allocate_voice_ed(0, 4, moby);
                }
                jet_camera_mode();
                D_L17_0015F608 = 2;
                update_fighter_jet_flight((Moby *)moby, (Obj *)ship_vars);
                moby->state = 4;
            }
            break;
        case FIGHTER_JET_STATE_PILOTED_FLIGHT:
            force_help_message(6, 0);
            D_L17_0015F608 = 2;
            e = D_0013E550_ed + ship_vars->voice * 0x70;
            if (*(ShipMoby **)(e + 0x88) != moby || *(unsigned char *)(e + 0x74) == 0) {
                ship_vars->voice = -1;
                ship_vars->voice = allocate_voice_ed(0, 4, moby);
            }
            update_fighter_jet_flight((Moby *)moby, (Obj *)ship_vars);
            if (ship_vars->iE8 <= 0) {
                ship_vars->timerF8 = D_L17_00162178;
                moby->state = 8;
            }
            if (ship_vars->fade != 0.0f) {
                FUN_00213ed8(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                D_L17_0015F3FC = ship_vars->fade;
            }
            {
                struct Hero *g = &hero;
                if (g->ship_flags & 1) {
                    moby->state = 5;
                }
            }
            break;
        case FIGHTER_JET_STATE_DISMOUNT: {
            char *p;
            struct Hero *g = &hero;
            g->unk20A5 = 0;
            ((ShipCamera *)g->moby)->unk98 = 0;
            FUN_L17_0021e530(0, 1);
            FUN_L00_002eac18(0);
            set_animation_parameter(g->unk160C, 0);
            qcopy_nc(moby->pos, ship_vars->home_pos);
            qcopy_nc(moby->rot, ship_vars->home_rot);
            moby->scale = *(float *)(moby->pclass + 0x24);
            refresh_moby_spatial_bounds_from_basis(moby);
            p = D_L17_001600EC + (ship_vars->exit_path << 7);
            FUN_L00_00216f90(p + 0x30, p + 0x70, 0, 1);
            jet_game_state(0, 5);
            idx = ship_vars->voice;
            if (idx != -1) {
                char *e = D_0013E550_ed + idx * 0x70;
                if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_ed(idx);
                }
            }
            ship_vars->voice = -1;
            idx = ship_vars->voice2;
            if (idx != -1) {
                char *e = D_0013E550_ed + idx * 0x70;
                if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_ed(idx);
                }
            }
            ship_vars->voice2 = -1;
            moby->state = 1;
            ship_vars->fade = 0.99f;
            D_L17_0015F3FC = 0.99f;
            break;
        }
        case FIGHTER_JET_STATE_DEATH_RESPAWN_DELAY:
            force_help_message(6, 0);
            D_L17_0015F608 = 2;
            if (fast_dec_timer_s16(&ship_vars->hit_timer)) {
                struct Hero *g = &hero;
                clear_jet_camera_mode_latch();
                ((ShipCamera *)g->moby)->unk98 = 0;
                hero_respawn();
                FUN_L00_002eac18(0);
                jet_game_state(0, 5);
                qcopy_nc(moby->pos, ship_vars->home_pos);
                qcopy_nc(moby->rot, ship_vars->home_rot);
                refresh_moby_spatial_bounds_from_basis(moby);
                idx = ship_vars->voice;
                if (idx != -1) {
                    char *e = D_0013E550_ed + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_ed(idx);
                    }
                }
                ship_vars->voice = -1;
                idx = ship_vars->voice2;
                if (idx != -1) {
                    char *e = D_0013E550_ed + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_ed(idx);
                    }
                }
                ship_vars->voice2 = -1;
                moby->state = 0;
            }
            break;
        case FIGHTER_JET_STATE_VICTORY_EXIT:
            force_help_message(6, 0);
            D_L17_0015F608 = 2;
            update_fighter_jet_flight((Moby *)moby, (Obj *)ship_vars);
            if (fast_dec_timer(&ship_vars->timerF8)) {
                FUN_00213ed8(&ship_vars->fade, 1.0f, frame_time * 4.0f);
                D_L17_0015F3FC = ship_vars->fade;
            }
            if (ship_vars->fade >= 1.0f) {
                char *p;
                struct Hero *g = &hero;
                g->unk20A5 = 0;
                ((ShipCamera *)g->moby)->unk98 = 0;
                FUN_L17_0021e530(0, 1);
                FUN_L00_002eac18(0);
                jet_game_state(0, 5);
                if (ship_vars->exit_path2 == -1) {
                    debug_print(D_L17_001E6E00);
                    qcopy_nc(moby->pos, ship_vars->home_pos);
                    qcopy_nc(moby->rot, ship_vars->home_rot);
                } else {
                    qcopy_nc(moby->pos, ((char (*)[128])D_L17_001600EC)[ship_vars->exit_path2] + 0x30);
                    qcopy_nc(moby->rot, ((char (*)[128])D_L17_001600EC)[ship_vars->exit_path2] + 0x70);
                    moby->pos[2] += 0.57318f;
                }
                moby->scale = *(float *)(moby->pclass + 0x24);
                refresh_moby_spatial_bounds_from_basis(moby);
                if (ship_vars->exit_path == -1) {
                    debug_print(D_L17_001E6E38);
                } else {
                    if (ship_vars->i11C > 0) {
                        jet_group_state(ship_vars->i11C, 0, 0, 0);
                    }
                    update_moby_class_flags_and_link(0x325, 1, 1, -1);
                    p = D_L17_001600EC + (ship_vars->exit_path << 7);
                    FUN_L00_00216f90(p + 0x30, p + 0x70, 0, 1);
                }
                if (moby->bB0 != 0xFF) {
                    jet_clear_persistent(moby->bB0);
                }
                idx = ship_vars->voice;
                if (idx != -1) {
                    char *e = D_0013E550_ed + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_ed(idx);
                    }
                }
                ship_vars->voice = -1;
                idx = ship_vars->voice2;
                if (idx != -1) {
                    char *e = D_0013E550_ed + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_ed(idx);
                    }
                }
                ship_vars->voice2 = -1;
                jet_reset_camera((&hero.motion.pos), &hero.motion.rot);
                moby->state = 9;
            }
            break;
        case 10:
            break;
        case FIGHTER_JET_STATE_FADE_THEN_REMOVE:
            if (ship_vars->fade == 0.0f) {
                mark_moby_for_removal((struct Obj *)moby);
                return;
            }
            FUN_00213ed8(&ship_vars->fade, 0.0f, frame_time * 4.0f);
            D_L17_0015F3FC = ship_vars->fade;
            break;
        }
        if (moby->pos[0] < 15.0f) {
            moby->pos[0] = 15.0f;
        }
        if (moby->pos[0] > 1008.0f) {
            moby->pos[0] = 1008.0f;
        }
        if (moby->pos[1] < 15.0f) {
            moby->pos[1] = 15.0f;
        }
        if (moby->pos[1] > 1008.0f) {
            moby->pos[1] = 1008.0f;
        }
        if (moby->pos[2] < 15.0f) {
            moby->pos[2] = 15.0f;
        }
        if (moby->pos[2] > 1008.0f) {
            moby->pos[2] = 1008.0f;
        }
        if (moby->state == 4) {
            qcopy_nc((&hero.motion.pos), moby->pos);
            qcopy_nc(&hero.motion.rot, ship_vars->rot);
        }
        if (moby->state == 3 || moby->state == 4) {
            emit_type_74_particles_from_moby_joints(moby, ship_vars);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EFA20), where it is exact; names translated to the US level program. */

extern char *D_L17_001600EC;
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern char (*D_L17_001600EC_t)[128] __asm__("D_L17_001600EC") __attribute__((section(".sdata")));
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

void adjust_fighter_jet_steering_for_flight_bounds(char *moby, float *horizontal_steering_input, float *vertical_steering_input) {
    char *ship_vars = *(char **)(moby + 0x78);
    float flight_bounds_prediction_scratch[4];
    float zz, predicted_horizontal_distance_from_boundary_center, horizontal_boundary_radius, hi, lo, inward_heading, upper_height_limit, k, horizontal_boundary_blend_factor, heading_error;
    scale_vector_xyz(flight_bounds_prediction_scratch, ship_vars + 0x10, *(float *)(ship_vars + 0x64) * (float)scale_game_frames(0x3C));
    add_vector_xyz(flight_bounds_prediction_scratch, flight_bounds_prediction_scratch, moby + 0x10);
    zz = flight_bounds_prediction_scratch[2];
    subtract_vector_xyz(flight_bounds_prediction_scratch, flight_bounds_prediction_scratch, (*(int *)(ship_vars + 0x12C) << 7) + D_L17_001600EC + 0x30);
    predicted_horizontal_distance_from_boundary_center = vector_length_xy(flight_bounds_prediction_scratch);
    horizontal_boundary_radius = vector_length_xy((*(int *)(ship_vars + 0x12C) << 7) + D_L17_001600EC);
    hi = *(float *)(D_L17_001600EC_t[*(int *)(ship_vars + 0x12C)] + 0x28);
    lo = *(float *)(D_L17_001600EC_t[*(int *)(ship_vars + 0x12C)] + 0x38);
    upper_height_limit = hi + lo;
    k = -hi + lo;
    inward_heading = fast_add_rotations(fast_arc_tan(flight_bounds_prediction_scratch[0], flight_bounds_prediction_scratch[1]), 3.141590118408203f);
    if (horizontal_boundary_radius - 10.0f < predicted_horizontal_distance_from_boundary_center) {
        horizontal_boundary_blend_factor = (predicted_horizontal_distance_from_boundary_center - horizontal_boundary_radius + 10.0f) / 10.0f;
        heading_error = fast_subtract_rotations(inward_heading, *(float *)(ship_vars + 0x38));
        if (horizontal_boundary_blend_factor > 1.0f)
            horizontal_boundary_blend_factor = 1.0f;
        else if (horizontal_boundary_blend_factor < 0.0f)
            horizontal_boundary_blend_factor = 0.0f;
        if (heading_error > 0.0f) {
            *horizontal_steering_input = (-1.0f - *horizontal_steering_input) * horizontal_boundary_blend_factor + *horizontal_steering_input;
        } else {
            *horizontal_steering_input = (1.0f - *horizontal_steering_input) * horizontal_boundary_blend_factor + *horizontal_steering_input;
        }
        hero.ship_flags |= 2;
    }
    if (upper_height_limit - 10.0f < zz) {
        float q, w;
        q = (zz - upper_height_limit + 10.0f) / 10.0f;
        w = (0.5235987901687622f - *(float *)(ship_vars + 0x34)) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input = (-1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    } else if (zz < k + 10.0f) {
        float q, w;
        q = (k - zz + 10.0f) / 10.0f;
        w = (*(float *)(ship_vars + 0x34) + 0.5235987901687622f) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input = (1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    }
}
/* Updates the class-1380 lift: rises/lowers between two heights, blends the camera and pulses its colour by state. */
extern char D_L17_001676C0[];
extern char D_L17_001B0DB0[];
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float fast_sin_c2(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 allocate_voice_for_target_entry_u(s32, s32, void *) __asm__("func_0022DA68");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_002ea9d8(void *);
extern int hero_set_state(int, int) __asm__("FUN_L17_0021e530");
extern void FUN_L17_002ee9c0_c(char *, char *, char *) __asm__("FUN_L17_002ee9c0");
extern void FUN_L17_002eea70_c(char *) __asm__("FUN_L17_002eea70");
float absolute_float(float input) __asm__("func_001F99C0");
void mark_moby_for_removal_c2(void *obj) __asm__("FUN_0020c828");
extern char D_0013F350_c2[] __asm__("D_0013F350");

void magnet_elevator_update(struct Moby *moby) __asm__("FUN_L17_002ee2b0");
void magnet_elevator_update(struct Moby *moby) {
    enum {
        MAGNET_ELEVATOR_STATE_INITIALIZE = 0,
        MAGNET_ELEVATOR_STATE_BLEND_CAMERA = 1,
        MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP_WITH_GROUND_CHECK = 2,
        MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP_WITH_GROUND_CHECK = 3,
        MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP = 4,
        MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP = 5,
        MAGNET_ELEVATOR_STATE_WAIT_AT_FIRST_STOP = 6,
        MAGNET_ELEVATOR_STATE_WAIT_AT_SECOND_STOP = 7,
        MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_FIRST_STOP = 8,
        MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_SECOND_STOP = 9
    };
    char *state = P(moby, 0x78);
    float delta[4];
    float rot[4];
    float camera_position_offset[4];
    float camera_position[4];
    float camera_rotation[4];

    scale_vector_xyz(delta, &moby->pos, -1.0f);
    qcopy(rot, &moby->rot);
    if (W(state, 0x7C) == 0) {
        float f =
            1.0f -
            ((*(float *)((char *)(moby) + (0x18))) - (*(float *)((char *)(state) + (0x64)))) /
                ((*(float *)((char *)(state) + (0x60))) - (*(float *)((char *)(state) + (0x64))));
        if (f > 1.0f) {
            f = 1.0f;
        } else if (f < 0.0f) {
            f = 0.0f;
        }
        (*(float *)((char *)(moby) + (0x44))) = f * 3.1415927f;
        (*(float *)((char *)(moby) + (0x44))) = normalize_angle(f * 3.1415927f);
    }
    switch (B(moby, 0x20)) {
    case MAGNET_ELEVATOR_STATE_INITIALIZE:
        (*(float *)((char *)(state) + (0x78))) = random_angle_radians();
        W(state, 0x6C) = -1;
        if (W(state, 0x80) == -1 || **(int **)(D_L17_001B0DB0 + W(state, 0x80) * 4) != 0xB) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        B(moby, 0x20) = 6;
        break;
    case MAGNET_ELEVATOR_STATE_BLEND_CAMERA: {
        float old = (*(float *)((char *)(state) + (0x70)));
        advance_accelerated_scalar((float *)(state + 0x70), (float *)(state + 0x74), 1.0f,
                                   frame_time_sq * 4.0f, frame_time_sq * 4.0f, frame_time * 4.0f);
        if ((*(float *)((char *)(state) + (0x70))) < 1.0f) {
            float t = (1.0f - (*(float *)((char *)(state) + (0x70)))) / (1.0f - old);
            char *base;
            FUN_L17_002ee9c0_c(moby, (char *)camera_position, (char *)camera_rotation);
            subtract_vector_xyz(camera_position_offset, D_L17_001676C0, camera_position);
            base = D_L17_001676C0 - 0x140;
            scale_vector_xyz(camera_position_offset, camera_position_offset, t);
            add_vector_xyz(camera_position, camera_position, camera_position_offset);
            camera_rotation[1] = fast_add_rotations(
                fast_subtract_rotations((*(float *)((char *)(base) + (0x154))), camera_rotation[1]) * t, camera_rotation[1]);
            camera_rotation[2] = fast_add_rotations(
                fast_subtract_rotations((*(float *)((char *)(base) + (0x158))), camera_rotation[2]) * t, camera_rotation[2]);
            FUN_L00_002ea9d8(camera_position);
            FUN_L00_002eaa30(camera_rotation);
        } else {
            unsigned char s = B(moby, 0xBC);
            B(moby, 0xBC) = 1;
            B(moby, 0x20) = s;
        }
        break;
    }
    case MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP_WITH_GROUND_CHECK:
    case MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP_WITH_GROUND_CHECK:
        FUN_L17_002eea70_c(moby);
    case MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP:
    case MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP: {
        float target;
        int s;
        W(moby, 0x90) = fast_tween_color(W(moby, 0x90), 0x80808040, 0.1f);
        if (B(moby, 0x20) == 2 && P(D_0013E533_u, 0x1119) != moby) {
            B(moby, 0x20) = 4;
        }
        if (B(moby, 0x20) == 3 && P(D_0013E533_u, 0x1119) != moby) {
            B(moby, 0x20) = 5;
        }
        if (!voice_is_playing(moby, W(state, 0x6C))) {
            W(state, 0x6C) = allocate_voice_for_target_entry_u(0, 4, (int)moby);
        }
        if (B(moby, 0x20) == 2 || B(moby, 0x20) == 4) {
            target = (*(float *)((char *)(state) + (0x60)));
        } else {
            target = (*(float *)((char *)(state) + (0x64)));
        }
        advance_accelerated_scalar(&moby->pos.z, (float *)(state + 0x68), target,
                                   frame_time_sq * 0.3f, frame_time_sq * 0.3f, frame_time * 3.0f);
        if (absolute_float((*(float *)((char *)(moby) + (0x18))) - target) == 0.0f) {
            s = B(moby, 0x20);
            if (s == 4) {
                B(moby, 0x20) = 6;
            } else if (s == 5) {
                B(moby, 0x20) = 7;
            } else if (s == 2) {
                B(moby, 0x20) = 8;
            } else {
                B(moby, 0x20) = 9;
            }
            if (B(moby, 0xBC) != 0) {
                hero_set_state(HERO_STATE_IDLE, 0);
            }
            if (voice_is_playing(moby, W(state, 0x6C))) {
                release_voice_slot(W(state, 0x6C));
                W(state, 0x6C) = -1;
            }
        }
        break;
    }
    case MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_FIRST_STOP:
    case MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_SECOND_STOP:
        if (voice_is_playing(moby, W(state, 0x6C))) {
            release_voice_slot(W(state, 0x6C));
            W(state, 0x6C) = -1;
        }
        if (P(D_0013E533_u, 0x1119) != moby) {
            if (B(moby, 0x20) == 9) {
                B(moby, 0x20) = 7;
            } else {
                B(moby, 0x20) = 6;
            }
        }
        break;
    case MAGNET_ELEVATOR_STATE_WAIT_AT_FIRST_STOP:
    case MAGNET_ELEVATOR_STATE_WAIT_AT_SECOND_STOP: {
        int i;
        char *hero_state;
        (*(float *)((char *)(state) + (0x78))) =
            fast_add_rotations((*(float *)((char *)(state) + (0x78))), frame_time * 6.2831855f);
        i = func_001FA898_r((fast_sin_c2((*(float *)((char *)(state) + (0x78)))) * 4.0f - 3.0f) *
                            128.0f);
        if (i > 0x80) {
            i = 0x80;
        } else if (i < 0x20) {
            i = 0x20;
        }
        {
            int t = (i << 8) | 0x80000000;
            W(moby, 0x90) = (i << 16) | t | i;
        }
        hero_state = (char *)((char *)&D_0013F350_c2);
        if (H(hero_state, 0x308) != 0 || W(state, 0x7C) == 0) {
            if (P(hero_state, 0x2FC) == moby && H(hero_state, 0x30E) == 0 &&
                vector_distance(hero_state + 0x80, &moby->pos) < 0.5f) {
                allocate_voice_for_target_entry_u(1, 0, (int)moby);
                if (B(moby, 0x20) == 7) {
                    B(moby, 0x20) = 2;
                } else {
                    B(moby, 0x20) = 3;
                }
            } else if (B(moby, 0x20) == 6) {
                char *p = (char *)(&hero.motion.pos);
                float d = vector_distance(&moby->pos, p);
                float e = absolute_float((*(float *)((char *)(p) + (0x258))) -
                                        (*(float *)((char *)(state) + (0x64))));
                if (e < absolute_float((*(float *)((char *)(state) + (0x60))) -
                                      (*(float *)((char *)(state) + (0x64)))) *
                            0.5f &&
                    d < 32.0f && d > 2.0f) {
                    B(moby, 0xBC) = 0;
                    B(moby, 0x20) = 3;
                    W(state, 0x68) = 0;
                }
            } else if (B(moby, 0x20) == 7) {
                char *p = (char *)(&hero.motion.pos);
                float d = vector_distance(&moby->pos, p);
                float e = absolute_float((*(float *)((char *)(p) + (0x258))) -
                                        (*(float *)((char *)(state) + (0x60))));
                if (e < absolute_float((*(float *)((char *)(state) + (0x60))) -
                                      (*(float *)((char *)(state) + (0x64)))) *
                            0.5f &&
                    d < 32.0f && d > 2.0f) {
                    B(moby, 0xBC) = 0;
                    B(moby, 0x20) = 2;
                    W(state, 0x68) = 0;
                }
            }
        }
        break;
    }
    }
    add_vector_xyz(delta, delta, &moby->pos);
    update_entity_matrix(state + 0x20, delta, rot, &moby->rot);
}
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002F04D0), where it is exact; names translated to the US level program. */

extern char D_L17_001676C0[];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");

void build_magnet_elevator_camera_pose(struct Moby *moby, char *camera_position, char *state) __asm__("FUN_L17_002ee9c0");

void build_magnet_elevator_camera_pose(struct Moby *moby, char *camera_position, char *state) {
    subtract_vector_xyz(camera_position, D_L17_001676C0, &moby->pos);
    set_vector_length_xy(camera_position, camera_position, 2.5f);
    *(float *)(camera_position + 8) = 1.5f;
    add_vector_xyz(camera_position, camera_position, &moby->pos);
    clear_u64_value(state);
    *(float *)(state + 8) =
        fast_arc_tan(hero.motion.unkD0.f[0] - *(float *)camera_position, hero.motion.unkD0.f[1] - *(float *)(camera_position + 4));
    *(int *)(state + 4) = 0;
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002F0580), where it is exact; names translated to the US level program. */

extern char D_L17_001B0DB0[];
extern char D_L17_001D9F70[];
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");

void rebuild_magnet_elevator_world_path(struct Moby *moby) __asm__("FUN_L17_002eea70");

void rebuild_magnet_elevator_world_path(struct Moby *moby) {
    char *state = (char *)moby->pvars;
    if (hero.unk308 && hero.ground_moby == moby) {
        char *world_path_point = *(char **)(D_L17_001B0DB0 + *(int *)(state + 0x80) * 4) + 0x10;
        char *point = &moby->rot_row0;
        int i;
        for (i = 10; i >= 0; i--) {
            transform_vector_by_basis(world_path_point, D_L17_001D9F70 + (10 - i) * 0x10, point);
            add_vector_xyz(world_path_point, world_path_point, &moby->pos);
            *(float *)(world_path_point + 0xC) = 1.0f;
            world_path_point += 0x10;
        }
        hero.unk22DA = *(unsigned short *)(state + 0x80);
    }
}
/* UpdateMoby for class 1382: hides or shows the moby by its clip volumes, then runs its state machine. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002F0678), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x20];
    float f20;
    short s24;
    char pad26[3];
    unsigned char b29;
    char pad2A[0x58 - 0x2A];
    unsigned char b58;
    char pad59;
    unsigned char b5A;
    char pad5B[0xC8 - 0x5B];
    short sC8;
    char padCA[0xD0 - 0xCA];
    void *pD0;
    char padD4[0x120 - 0xD4];
    char a120[0x180 - 0x120];
    float v180[4];
    char pad190[0x1C0 - 0x190];
    unsigned char *target;
    char pad1C4[0x1D0 - 0x1C4];
    int path;
    float f1D4;
    float speed;
    float yawvel;
    short t1E0;
    short t1E2;
    unsigned char mode;
    unsigned char b1E5;
    char pad1E6[2];
    int i1E8;
    float f1EC;
    OvlQuad home;
    char pad200[0x230 - 0x200];
    char a230[0x2B0 - 0x230];
    int c2B0;
    int c2B4;
    int c2B8;
    int c2BC;
} Vars1382;

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern float fast_cos_c(float) __asm__("func_001F9DC8");
extern float fast_sin_c(float) __asm__("func_001F9DE0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_002dc9f0(void *, void *);
extern int FUN_L17_002efd48_c(char *moby, float *target, float speed) __asm__("FUN_L17_002efd48");
extern int random_integer_below(int) __asm__("FUN_00213260");
short D_L17_00162200 = 0x909;
extern void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9,
                              float scale, float *v10, float *v11, float *v12);
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void spawn_nearby_effect(int, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void FUN_L00_0025a120(void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_00263b38(void *, float);
extern void spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
void update_robot_dog_hit_reaction_and_target_detection(void *moby_v, int clip_volume_suppressed) __asm__("FUN_L17_002efa48");

void update_robot_dog_hit_reaction_and_target_detection(void *moby_v, int arg) __asm__("FUN_L17_002efa48");
void update_robot_dog_aim_joint(struct Moby *moby) __asm__("FUN_L17_002f00a0");

void update_robot_dog_aim_joint(struct Moby *moby) __asm__("FUN_L17_002f00a0");
void update_robot_dog_glow_effects(struct Moby *moby) __asm__("FUN_L17_002f0210");

void update_robot_dog_glow_effects(struct Moby *moby) __asm__("FUN_L17_002f0210");
extern void clear_vector(void *) __asm__("func_001F99F8");
f32 compute_interpolated_record_value(void *arg0) __asm__("FUN_0020c9e0");
float absolute_float(float input) __asm__("func_001F99C0");
s32 is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");
void blend_moby_animation_c(void *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");
void mark_moby_for_removal_c(void *obj) __asm__("FUN_0020c828");

extern float D_L17_0015F580_macro[] __asm__("D_L17_0015F580") __attribute__((section(".sdata")));
void robot_dog_update(unsigned char *m) __asm__("FUN_L17_002eeb68");
void robot_dog_update(unsigned char *robot_dog_moby) {
    Vars1382 *robot_dog_vars;
    unsigned char *facing_target_moby;
    int clip_volume_suppressed;
    int clip_volume_id_or_animation_value;
    int secondary_clip_volume_id;
    float heading_vector[4];
    float pos[4];

    clip_volume_suppressed = 0;
    robot_dog_vars = *(Vars1382 **)(robot_dog_moby + 0x78);
    if (robot_dog_moby[0x20] != 12) {
        clip_volume_id_or_animation_value = robot_dog_vars->c2B0;
        if (clip_volume_id_or_animation_value != -1) {
            clip_volume_suppressed = is_point_inside_clip_volume(D_L17_001676C0, clip_volume_id_or_animation_value) == 0;
            secondary_clip_volume_id = robot_dog_vars->c2B4;
            if (secondary_clip_volume_id != -1) {
                clip_volume_suppressed &= is_point_inside_clip_volume(D_L17_001676C0, secondary_clip_volume_id) == 0;
            }
        } else if (robot_dog_vars->c2B8 != -1) {
            clip_volume_suppressed = is_point_inside_clip_volume(D_L17_001676C0, clip_volume_id_or_animation_value);
            secondary_clip_volume_id = robot_dog_vars->c2BC;
            if (secondary_clip_volume_id != -1) {
                clip_volume_suppressed |= is_point_inside_clip_volume(D_L17_001676C0, secondary_clip_volume_id);
            }
        }
    }
    update_robot_dog_hit_reaction_and_target_detection(robot_dog_moby, clip_volume_suppressed);
    if (clip_volume_suppressed != 0 && robot_dog_moby[0x20] != 15) {
        robot_dog_moby[0x31] = 0;
        *(unsigned short *)(robot_dog_moby + 0x34) |= 1;
        return;
    }
    robot_dog_moby[0x31] = 1;
    *(unsigned short *)(robot_dog_moby + 0x34) &= 0xFFFE;
    update_robot_dog_aim_joint(robot_dog_moby);
    update_robot_dog_glow_effects(robot_dog_moby);
    facing_target_moby = robot_dog_vars->target;
    FUN_L00_00263b38(robot_dog_vars->a230, 2.1f);
    if (robot_dog_moby[0x31] != 0 && FUN_001f9b48(robot_dog_moby + 0x10, D_L17_001676C0) < 29.0f) {
        FUN_L00_0025a120(robot_dog_moby);
        robot_dog_moby[0x7F] = 0x17;
    }
    switch (robot_dog_moby[0x20]) {
    case ROBOT_DOG_STATE_INITIALIZE:
        *(unsigned short *)(robot_dog_moby + 0x34) |= 0x1000;
        qcopy(&robot_dog_vars->home, robot_dog_moby + 0x10);
        robot_dog_vars->b58 = 8;
        robot_dog_vars->b5A = 7;
        robot_dog_vars->b29 = 0;
        robot_dog_vars->s24 = 1;
        robot_dog_vars->f20 = 1.0f;
        robot_dog_vars->pD0 = &D_L17_00162200;
        robot_dog_vars->f1EC = random_float_between_alt(frame_time * 7.2f, frame_time * 8.7f);
        robot_dog_moby[0x20] = 1;
        blend_moby_animation_c(
            robot_dog_moby, 0,
            random_integer_below(
                *(unsigned char *)(*(char **)(*(char **)(robot_dog_moby + 0x24) + 0x48) + 0x10)),
            0);
        if (robot_dog_vars->i1E8 == -1) {
            robot_dog_vars->i1E8 = robot_dog_vars->path;
        }
        break;
    case ROBOT_DOG_STATE_RECOVER_THEN_REINITIALIZE:
        if (FUN_L00_002dc9f0(robot_dog_moby, robot_dog_vars->a120)) {
            robot_dog_moby[0x20] = 0;
            robot_dog_vars->sC8 = 0;
        }
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_DETECTION_TRIGGER:
        if (robot_dog_vars->mode != 0 && random_integer_below(0x1F) == 0) {
            robot_dog_moby[0x20] = 2;
            if (robot_dog_moby[0x53] != 1) {
                blend_moby_animation_c(robot_dog_moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_STATE_SELECT_RESPONSE_ON_ANIMATION_COMPLETION:
        if (robot_dog_moby[0x70] & 2) {
            int s = robot_dog_vars->mode;
            if (s == 2) {
                if (random_integer_below(3)) {
                    if (robot_dog_moby[0x53] != s) {
                        blend_moby_animation_c(robot_dog_moby, 2, 0, scale_game_frames(10));
                    }
                } else if (D_0013E533_u[0x2EC1] != 3) {
                    robot_dog_moby[0x20] = 7;
                    if (robot_dog_moby[0x53] != 8) {
                        blend_moby_animation_c(robot_dog_moby, 8, 0, scale_game_frames(10));
                    }
                } else {
                    robot_dog_moby[0x20] = 6;
                    if (robot_dog_moby[0x53] != 7) {
                        blend_moby_animation_c(robot_dog_moby, 7, 0, scale_game_frames(10));
                    }
                }
            } else {
                robot_dog_moby[0x20] = 5;
                if (robot_dog_moby[0x53] != 3) {
                    blend_moby_animation_c(robot_dog_moby, 3, 0, scale_game_frames(10));
                }
                robot_dog_vars->t1E0 = scale_game_frames(120);
                robot_dog_vars->t1E2 = scale_game_frames(120);
            }
        }
        break;
    case ROBOT_DOG_STATE_TRACK_TARGET_FACING:
        if (facing_target_moby != 0) {
            float turn;
            float ang;
            float *yaw = (float *)(robot_dog_moby + 0x48);
            int mode;
            ang = fast_arc_tan(*(float *)(facing_target_moby + 0x10) - *(float *)(robot_dog_moby + 0x10),
                               *(float *)(facing_target_moby + 0x14) - *(float *)(robot_dog_moby + 0x14));
            turn = frame_time_sq * 4.712389f;
            approach_angle(yaw, &robot_dog_vars->yawvel, ang, turn, turn, frame_time * 3.4906585f);
            if (absolute_float(robot_dog_vars->yawvel) > frame_time * 0.08726646f) {
                if (robot_dog_vars->yawvel < 0.0f) {
                    if (robot_dog_moby[0x53] != 4) {
                        blend_moby_animation_c(robot_dog_moby, 4, 0, scale_game_frames(10));
                    }
                } else {
                    if (robot_dog_moby[0x53] != 5) {
                        blend_moby_animation_c(robot_dog_moby, 5, 0, scale_game_frames(10));
                    }
                }
            } else {
                if (robot_dog_moby[0x53] != 3) {
                    blend_moby_animation_c(robot_dog_moby, 3, 0, scale_game_frames(10));
                }
            }
            *(float *)(robot_dog_moby + 0x58) = 1.7f;
            mode = robot_dog_vars->mode;
            if (mode == 2 && (fast_dec_timer_s16(&robot_dog_vars->t1E2) || D_0013E533_u[0x2EC1] != 3)) {
                if (random_integer_below(3) == 0) {
                    if (robot_dog_moby[0x53] != mode) {
                        blend_moby_animation_c(robot_dog_moby, 2, 0, scale_game_frames(10));
                    }
                } else if (D_0013E533_u[0x2EC1] != 3) {
                    robot_dog_moby[0x20] = 7;
                    if (robot_dog_moby[0x53] != 8) {
                        blend_moby_animation_c(robot_dog_moby, 8, 0, scale_game_frames(10));
                    }
                } else {
                    robot_dog_moby[0x20] = 6;
                    if (robot_dog_moby[0x53] != 7) {
                        blend_moby_animation_c(robot_dog_moby, 7, 0, scale_game_frames(10));
                    }
                }
            } else if (robot_dog_vars->t1E0 == 0 && robot_dog_vars->mode == 0) {
                robot_dog_moby[0x20] = 3;
                if (robot_dog_moby[0x53] != 0) {
                    clip_volume_id_or_animation_value = random_integer_below(
                        *(unsigned char *)(*(char **)(*(char **)(robot_dog_moby + 0x24) + 0x48) + 0x10));
                    blend_moby_animation_c(robot_dog_moby, 0, clip_volume_id_or_animation_value, scale_game_frames(10));
                }
            }
        } else if (random_integer_below(0x1F) == 0) {
            robot_dog_moby[0x20] = 3;
            if (robot_dog_moby[0x53] != 0) {
                clip_volume_id_or_animation_value = random_integer_below(
                    *(unsigned char *)(*(char **)(*(char **)(robot_dog_moby + 0x24) + 0x48) + 0x10));
                blend_moby_animation_c(robot_dog_moby, 0, clip_volume_id_or_animation_value, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_IDLE_ANIMATION_COMPLETION:
        if (robot_dog_moby[0x70] & 2) {
            robot_dog_moby[0x20] = 1;
            if (robot_dog_moby[0x53] != 0) {
                clip_volume_id_or_animation_value = random_integer_below(
                    *(unsigned char *)(*(char **)(*(char **)(robot_dog_moby + 0x24) + 0x48) + 0x10) - 1);
                blend_moby_animation_c(robot_dog_moby, 0, clip_volume_id_or_animation_value, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER:
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER_FAST_PULSE: {
        int s = robot_dog_vars->mode;
        if (s != 2) {
            if (random_integer_below(20) == 0) {
                robot_dog_moby[0x20] = 5;
                if (robot_dog_moby[0x53] != 3) {
                    blend_moby_animation_c(robot_dog_moby, 3, 0, scale_game_frames(10));
                }
                robot_dog_vars->t1E2 = scale_game_frames(120);
                robot_dog_vars->t1E0 = scale_game_frames(120);
            }
            break;
        }
        if (robot_dog_vars->f1D4 != 0.0f ||
            point_in_path_xy((&hero.motion.pos), ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                             ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) != 0) {
            if (random_integer_below(20) == 0) {
                robot_dog_moby[0x20] = 8;
                if (robot_dog_moby[0x53] != 1) {
                    blend_moby_animation_c(robot_dog_moby, 1, 0, scale_game_frames(10));
                }
            }
        } else if (robot_dog_moby[0x70] & 2) {
            if (random_integer_below(3) == 0) {
                if (robot_dog_moby[0x53] != s) {
                    blend_moby_animation_c(robot_dog_moby, 2, 0, scale_game_frames(10));
                }
            } else if (random_integer_below(7) == 0) {
                if (robot_dog_moby[0x53] != 8) {
                    blend_moby_animation_c(robot_dog_moby, 8, 0, scale_game_frames(10));
                }
            } else {
                if (robot_dog_moby[0x53] != 7) {
                    blend_moby_animation_c(robot_dog_moby, 7, 0, scale_game_frames(10));
                }
            }
        }
        break;
    }
    case ROBOT_DOG_STATE_TURN_TOWARD_TARGET: {
        float ang;
        ang = fast_arc_tan(robot_dog_vars->v180[0] - *(float *)(robot_dog_moby + 0x10), robot_dog_vars->v180[1] - *(float *)(robot_dog_moby + 0x14));
        FUN_L00_00258278((char *)robot_dog_moby, &robot_dog_vars->yawvel, ang, 0.03f, 0.3f, frame_time * 7.8539815f);
        if (robot_dog_moby[0x70] & 2) {
            if (robot_dog_vars->f1D4 == 0.0f) {
                if (point_in_path_xy((&hero.motion.pos), ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                                     ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) == 0) {
                    robot_dog_moby[0x20] = 11;
                    if (robot_dog_moby[0x53] != 9) {
                        blend_moby_animation_c(robot_dog_moby, 9, 0, scale_game_frames(10));
                    }
                    break;
                }
            }
            robot_dog_moby[0x20] = 9;
            if (robot_dog_moby[0x53] != 9) {
                blend_moby_animation_c(robot_dog_moby, 9, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case ROBOT_DOG_STATE_APPROACH_TARGET:
        FUN_L17_002efd48_c((char *)robot_dog_moby, robot_dog_vars->v180, robot_dog_vars->f1EC);
        *(float *)(robot_dog_moby + 0x58) = 1.8f;
        if (point_in_path_xy((&hero.motion.pos), ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                             ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) == 0 &&
            robot_dog_vars->f1D4 == 0.0f) {
            robot_dog_moby[0x20] = 11;
        } else if (vector_distance(robot_dog_moby + 0x10, robot_dog_vars->v180) < 1.8f) {
            robot_dog_moby[0x20] = 10;
            if (robot_dog_moby[0x53] != 10) {
                blend_moby_animation_c(robot_dog_moby, 10, 5, scale_game_frames(10));
            }
        } else if (FUN_001f9b48(robot_dog_vars->v180, &robot_dog_vars->home) > 32.0f ||
                   absolute_float(robot_dog_vars->v180[2] - *(float *)(robot_dog_moby + 0x18)) > 3.0f) {
            robot_dog_moby[0x20] = 11;
        }
        break;
    case ROBOT_DOG_STATE_ATTACK_TARGET:
        if (robot_dog_moby[0x70] & 2) {
            robot_dog_moby[0x20] = 11;
            if (robot_dog_moby[0x53] != 9) {
                blend_moby_animation_c(robot_dog_moby, 9, 0, scale_game_frames(10));
            }
        } else if (absolute_float(compute_interpolated_record_value(robot_dog_moby) - 17.5f) <= 0.25f &&
                   absolute_float(robot_dog_vars->v180[2] - *(float *)(robot_dog_moby + 0x18)) < 1.0f &&
                   fast_difference_between_rotations(
                       fast_arc_tan(robot_dog_vars->v180[0] - *(float *)(robot_dog_moby + 0x10),
                                    robot_dog_vars->v180[1] - *(float *)(robot_dog_moby + 0x14)),
                       *(float *)(robot_dog_moby + 0x48)) < 0.2617994f &&
                   robot_dog_vars->target != 0 && vector_distance(robot_dog_moby + 0x10, robot_dog_vars->v180) < 2.75f) {
            heading_vector[0] = fast_cos_c(*(float *)(robot_dog_moby + 0x48)) * 0.2f;
            heading_vector[1] = fast_sin_c(*(float *)(robot_dog_moby + 0x48)) * 0.2f;
            heading_vector[2] = 0.0f;
            qcopy(pos, robot_dog_vars->v180);
            pos[2] += 0.75f;
            spawn_nearby_effect((int)robot_dog_vars->target, (int)robot_dog_moby, 1, pos, heading_vector, 1.0f);
        }
        break;
    case ROBOT_DOG_STATE_RETURN_HOME: {
        float distance_to_home;
        float far;
        float k1;
        float k2;
        float frame_scaled_speed;
        FUN_L17_002efd48_c((char *)robot_dog_moby, (float *)&robot_dog_vars->home, frame_time * 5.0f);
        *(float *)(robot_dog_moby + 0x58) = 1.35f;
        distance_to_home = vector_distance(robot_dog_moby + 0x10, &robot_dog_vars->home);
        if ((distance_to_home < 1.4f && (robot_dog_moby[0x70] & 2)) || distance_to_home < 0.3f) {
            robot_dog_moby[0x20] = 5;
            if (robot_dog_moby[0x53] != 3) {
                blend_moby_animation_c(robot_dog_moby, 3, 0, scale_game_frames(10));
            }
            robot_dog_vars->t1E2 = scale_game_frames(120);
            robot_dog_vars->t1E0 = scale_game_frames(120);
        } else if (robot_dog_moby[0x70] & 2) {
            far = vector_distance(robot_dog_moby + 0x10, robot_dog_vars->v180);
            k1 = multiply_global_factor_ed64(40.0f);
            k2 = multiply_global_factor_ed64(10.0f);
            frame_scaled_speed = robot_dog_vars->speed * frame_time;
            if (far < frame_scaled_speed * k1 * 0.5f + frame_scaled_speed * k2 + 1.5f) {
                if (point_in_path_xy((&hero.motion.pos), ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                                     ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) != 0) {
                    robot_dog_moby[0x20] = 10;
                    if (robot_dog_moby[0x53] != 10) {
                        blend_moby_animation_c(robot_dog_moby, 10, 0, scale_game_frames(10));
                    }
                }
            }
        }
        break;
    }
    case ROBOT_DOG_STATE_FALL_UNTIL_LANDING:
        if (mine_fall(robot_dog_moby, robot_dog_vars->a120) & 0x60) {
            clear_vector(heading_vector);
            robot_dog_moby[0x20] = 5;
            if (robot_dog_moby[0x53] != 3) {
                blend_moby_animation_c(robot_dog_moby, 3, 0, scale_game_frames(10));
            }
            robot_dog_vars->t1E2 = scale_game_frames(120);
            robot_dog_vars->t1E0 = scale_game_frames(120);
        } else if (*(float *)(robot_dog_moby + 0x18) < 2.0f) {
            FUN_L00_00257470(robot_dog_moby, 0, -1);
            mark_moby_for_removal_c(robot_dog_moby);
        }
        break;
    case ROBOT_DOG_STATE_FALL_THEN_REMOVE:
        *(unsigned short *)(robot_dog_moby + 0x34) &= 0xEFFF;
        if (mine_fall(robot_dog_moby, robot_dog_vars->a120) & 0x60) {
            float *zero;
            clear_vector(heading_vector);
            zero = D_L17_0015F580_macro;
            cart_burst(robot_dog_moby, robot_dog_moby + 0x10, -1, 0.5f, 13.0f);
            FUN_L00_00263fd8((char *)robot_dog_moby, 0x6CE, (float *)(robot_dog_moby + 0x10), robot_dog_moby + 0x40, 0, 0, 0.0f, zero,
                             zero, zero);
            FUN_L00_00263fd8((char *)robot_dog_moby, 0x6CF, (float *)(robot_dog_moby + 0x10), robot_dog_moby + 0x40, 0, 0, 0.0f, zero,
                             zero, zero);
            spawn_moby_break_fragments(robot_dog_moby, 0x77D, 1, 0x77D, 1, 3, 2);
            FUN_L00_00257470(robot_dog_moby, 0, -1);
            mark_moby_for_removal_c(robot_dog_moby);
        } else if (*(float *)(robot_dog_moby + 0x18) < 2.0f) {
            FUN_L00_00257470(robot_dog_moby, 0, -1);
            mark_moby_for_removal_c(robot_dog_moby);
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Fleet turret/enemy state update: refreshes the hit reaction, fires the aim query and sets the lock flags. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1558), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x20];
    float f20;
    char pad24[0x14];
    int i38;
    char pad3C[0xD4];
    short h110[3];
    char b116;
    unsigned char b117;
    char pad118[8];
    float v120[4];
    float f130;
    float f134;
    float f138;
    float f13C;
    int i140;
    int i144;
    char pad148[0x15];
    char b15D;
    char pad15E[0x12];
    float f170;
    float f174;
    char pad178[8];
    float v180[4];
    char pad190[0x20];
    float v1B0[4];
    char pad1C0[0x14];
    float f1D4;
    char pad1D8[0xC];
    char b1E4;
    char pad1E5[0xB];
    float f1F0;
    float f1F4;
} Data_2F1558;

typedef struct {
    char pad0[0x10];
    float pos[4];
    unsigned char state;
    char pad21[0x27];
    float f48;
    char pad4C[0x2C];
    Data_2F1558 *data;
    char pad7C[0x28];
    unsigned char bA4;
    char padA5;
    short hA6;
} Moby_2F1558;

typedef struct {
    char pad0[0x10];
    float pos[4];
    Moby_2F1558 *moby;
} Hit_2F1558;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b48(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern int coll_line_fix(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void update_float_countdown(float *) __asm__("FUN_L08_00211b08");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_robot_dog_hit_reaction_and_target_detection(void *moby_v, int clip_volume_suppressed) {
    Moby_2F1558 *moby = (Moby_2F1558 *)moby_v;
    Data_2F1558 *moby_vars;
    Hit_2F1558 *hit_record;
    float position_buffer[4];
    int x;
    float y;
    float knockback_heading;
    int line_query_clear;
    float yaw;
    float target_heading;

    moby_vars = moby->data;
    if (moby->state == 0) {
        return;
    }
    if (moby_vars->i38 != 0) {
        moby_vars->f1D4 = func_001FA898_r(multiply_global_factor_ed64(random_float_between(180.0f, 240.0f)));
        moby_vars->i38 = 0;
    }
    update_float_countdown(&moby_vars->f1D4);
    y = 0;
    hit_record = (Hit_2F1558 *)find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, hit_record, &moby_vars->f20, 0, &x, &y, 0, 4);
    if (x != 1 && moby->state != 15) {
        moby_vars->f20 -= y;
        moby_vars->f130 = frame_time_sq * 42.0f;
        moby_vars->f134 = frame_time_sq * 24.0f;
        moby_vars->b15D = 0;
        moby_vars->i144 = 9;
        moby_vars->f13C = frame_time * 15.7f;
        moby_vars->f138 = frame_time * 19.0f;
        moby_vars->f170 = -1.0f;
        moby_vars->f174 = -1.0f;
        *(OvlQuad *)position_buffer = *(OvlQuad *)hit_record->pos;
        trooper_knockback_arc(position_buffer, &knockback_heading, &moby_vars->f138, &moby_vars->f13C);
        if (hit_record != 0 && hit_record->moby != 0 && hit_record->moby->hA6 == 0x63) {
            knockback_heading = fast_arc_tan(moby_vars->f1F0 - moby->pos[0], moby_vars->f1F4 - moby->pos[1]);
        }
        trooper_start_knockback(knockback_heading, moby, moby_vars->v120, 0xB, 1, 0);
        moby->state = 15;
        moby_vars->b117 = 0xF0;
        trooper_anim_update(moby, moby_vars->h110);
    }
    moby->bA4 = 0xFF;
    if (clip_volume_suppressed != 0) {
        return;
    }
    if (FUN_L00_0025ff38(128.0f, (char *)moby, moby_vars->v180, 0, 0, 0, 0) == 2) {
        return;
    }
    if (FUN_001f9b48(moby->pos, moby_vars->v1B0) < 64.0f) {
        target_heading = fast_arc_tan(moby_vars->v180[0] - moby->pos[0], moby_vars->v180[1] - moby->pos[1]);
        yaw = fast_difference_between_rotations(moby->f48, target_heading);
        qcopy(position_buffer, moby->pos);
        position_buffer[2] += 1.0f;
        line_query_clear = coll_line_fix(position_buffer, (&hero.motion.unkD0), 2, 0, 0) == 0;
        moby_vars->b1E4 = line_query_clear;
        if (yaw < 1.0471976f) {
            moby_vars->b1E4 = line_query_clear << 1;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1858), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern void FUN_L00_00258830(int, int, float, float, float, int);

int move_robot_dog_toward_position(struct Moby *moby, float *target, float speed) __asm__("FUN_L17_002efd48");

int move_robot_dog_toward_position(struct Moby *moby, float *target, float speed) {
    char *data;
    float *yaw = &moby->rot.z;
    float target_yaw;
    float turn;
    float movement_delta[3];
    data = (char *)moby->pvars;
    target_yaw = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
    turn = frame_time_sq * 12.566371f;
    approach_angle(yaw, (float *)(data + 0x1DC), target_yaw, turn, turn, frame_time * 7.3303828f);
    approach_value((float *)(data + 0x1D8), speed, frame_time_sq * 13.0f);
    movement_delta[0] = fast_cos(moby->rot.z) * *(float *)(data + 0x1D8);
    movement_delta[1] = fast_sin(moby->rot.z) * *(float *)(data + 0x1D8);
    movement_delta[2] = -(frame_time * 0.5f);
    FUN_L00_00258830((int)moby, (int)movement_delta, 0.5f, 0.5f, 0.0f, 0);
    return vector_distance(&moby->pos, target) < 0.2f;
}
extern int scale_ticks(int) __asm__("FUN_001f96f8");

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F19B8), where it is exact; names translated to the US level program. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");
void blend_moby_animation_u(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

int FUN_L17_002efea8(struct Moby *moby) {
    int result = process_moby_suck_cannon_suction(moby);
    if (result) {
        moby->state = 12;
    } else if (moby->state == 12) {
        moby->state = 6;
        if (moby->prev_seq != 7) {
            int value = scale_ticks(10);
            blend_moby_animation_u(moby, 7, 0, value);
        }
    }
    return result;
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Aims the turret at the hero: yaw and pitch to him, clamped, become the eased part's targets. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1BB0), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x64];
    float pitch;
    float yaw;
} AimPart_1BB0;

typedef struct {
    char pad0[0x230];
    AimPart_1BB0 part;
} AimData_1BB0;

extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vector_length_xy(void *);
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void subtract_vector_xyz(void *dst, void *a, void *b);

void update_robot_dog_aim_joint(struct Moby *moby) {
    AimData_1BB0 *moby_vars = (AimData_1BB0 *)moby->pvars;
    float aim_joint_smoothing_factor = 0.02f;
    float speed = 0.3f;
    float aim_origin[4];
    float aim_delta[4];
    float yaw;
    float pitch;
    qcopy(aim_origin, &moby->pos);
    aim_origin[2] += 0.5f;
    subtract_vector_xyz(aim_delta, (&hero.motion.unkD0), aim_origin);
    yaw = fast_subtract_rotations(fast_arc_tan(aim_delta[0], aim_delta[1]), moby->rot.z);
    pitch = -fast_arc_tan(vector_length_xy(aim_delta), aim_delta[2]);
    if (yaw > 1.0471976f) {
        yaw = 1.0471976f;
    } else if (yaw < -1.0471976f) {
        yaw = -1.0471976f;
    }
    if (pitch > 0.5235988f) {
        pitch = 0.5235988f;
    } else if (pitch < -0.5235988f) {
        pitch = -0.5235988f;
    }
    moby_vars->part.pitch = pitch;
    moby_vars->part.yaw = yaw;
    update_joint_animation(moby, (char *)&moby_vars->part, 0, aim_joint_smoothing_factor * frame_scale_sq, speed * frame_scale_sq);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1D20), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void draw_sentrybot_effect_quads(char *) __asm__("FUN_L15_0029aba8");
extern void draw_three_moby_effect_quads(char *) __asm__("FUN_L17_002f0448");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_robot_dog_glow_effects(struct Moby *moby) {
    char *data;
    int glow_pulse_period_frames;
    int alpha;
    int mode;
    int i;
    int x, y, z;
    int r, g, b;
    float glow_pulse_sine;
    int color;

    /* Period and base colour by state range; both ranges have the same values in this build. */
    if (moby->state < 16) {
        glow_pulse_period_frames = 0xAA;
        r = 0x32;
        g = 0xB4;
        b = 0x14;
    } else {
        glow_pulse_period_frames = 0xAA;
        r = 0x32;
        g = 0xB4;
        b = 0x14;
    }
    data = (char *)moby->pvars;
    glow_pulse_period_frames = scale_game_frames(glow_pulse_period_frames);
    alpha = 0x80;
    mode = 0;
    switch (moby->state) {
    case ROBOT_DOG_STATE_FALL_UNTIL_LANDING:
    case ROBOT_DOG_STATE_TRACK_TARGET_FACING:
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER:
        mode = 1;
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER_FAST_PULSE:
    case ROBOT_DOG_STATE_TURN_TOWARD_TARGET:
    case ROBOT_DOG_STATE_APPROACH_TARGET:
    case ROBOT_DOG_STATE_ATTACK_TARGET:
    case ROBOT_DOG_STATE_RETURN_HOME:
    case 13:
    case 14:
    case ROBOT_DOG_STATE_FALL_THEN_REMOVE:
        mode = 2;
        break;
    case ROBOT_DOG_STATE_RECOVER_THEN_REINITIALIZE:
        mode = -1;
        break;
    }
    if (mode >= 0) {
        if (mode == 2) {
            glow_pulse_period_frames = scale_game_frames(0x32);
        } else if (mode == 1) {
            glow_pulse_period_frames = scale_game_frames(0x5A);
        }
        glow_pulse_sine = fast_sin(((float)(D_L17_0015F5CC % glow_pulse_period_frames) / convert_integer_to_float(glow_pulse_period_frames)) * 2.0f * 3.1415927f +
                     -3.1415927f);
        x = func_001FA898_r(glow_pulse_sine * 20.0f);
        y = func_001FA898_r(glow_pulse_sine * 70.0f);
        z = func_001FA898_r(glow_pulse_sine * 10.0f);
        r += x;
        g += y;
        b += z;
        if (r > 255) {
            r = 255;
        }
        if (g > 255) {
            g = 255;
        }
        if (mode == 2) {
            color = (alpha << 24) | (b << 16) | ((r / 2) << 8) | g;
        } else if (mode != 0) {
            color = (alpha << 24) | (b << 16) | ((r / 2) << 8) | g;
        } else {
            color = (alpha << 24) | (b << 16) | (g << 8) | r;
        }
        moby->glow_color = fast_tween_color(moby->glow_color, color, 0.1f);
        for (i = 0; i < 3; i++) {
            moby_point_position(moby, i + 1, data + 0x200 + i * 16);
        }
        enqueue_callback_list_1_alt(draw_three_moby_effect_quads, moby);
    }
}

/* Builds the two rippling vertex strips around the moby (template points moved to its position, z waved by distance from the centre), then draws them twice with scrolled texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1FE0), where it is exact; names translated to the US level program. */

typedef struct {
    float f0;
    float f4;
} RippleData;

typedef struct {
    float v[2];
} P2;

typedef struct {
    P2 p[2];
} T4;

T4 D_L17_00162268 = {{{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
int D_L17_00162248 = 128;
int D_L17_0016224C = 45;
int D_L17_00162250 = 64;
extern float D_L17_00162254 __attribute__((sda));
extern float D_L17_00162258 __attribute__((sda));
extern float D_L17_0016225C __attribute__((sda));
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern float D_L17_00162260 __attribute__((sda));
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void get_moby_color(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

int D_L17_00162210[2] = {0x94, 0xb};
extern char D_L17_001DAF30[];
extern char D_L17_001DBD10[];
int D_L17_00162218[2] = {(int)D_L17_001DAF30, (int)D_L17_001DBD10};
extern char D_L17_001DB620[];
extern char D_L17_001DBD98[];
int D_L17_00162220[2] = {(int)D_L17_001DB620, (int)D_L17_001DBD98};
extern char D_L17_001DBAC0[];
extern char D_L17_001DBDF0[];
int D_L17_00162228[2] = {(int)D_L17_001DBAC0, (int)D_L17_001DBDF0};
extern char D_L17_001DA040[];
extern char D_L17_001DA730[];
int D_L17_00162230[2] = {(int)D_L17_001DA040, (int)D_L17_001DA730};
extern char D_L17_001DA7B8[];
extern char D_L17_001DAC58[];
int D_L17_00162238[2] = {(int)D_L17_001DA7B8, (int)D_L17_001DAC58};

void draw_moby_radial_ripple_mesh_pair(struct Moby *moby) __asm__("FUN_L17_002f04d0");

void draw_moby_radial_ripple_mesh_pair(struct Moby *moby) {
    RippleData *ripple_vars = (RippleData *)moby->pvars;
    float scale;
    int r, g, b;
    int color;
    int i, j, uv_strip_index, uv_vertex_index, draw_strip_index;

    scale = convert_integer_to_float(D_L17_0015F5CC) * (D_L17_00162260 * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(D_L17_0016224C));
    vu1_add_g_sregister(0x42, ((long)D_L17_00162248 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ripple_vars->f0 = ripple_vars->f4;
    ripple_vars->f4 = fast_add_rotations(ripple_vars->f4,
                               360.0f / multiply_global_factor_ed64(D_L17_00162258) * DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L17_00162250 << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < D_L17_00162210[i]; j++) {
            int *template_vertex_array_slot = D_L17_00162230 + i;
            float *p = (float *)(*template_vertex_array_slot + j * 12);
            float t = D_L17_0016225C *
                      fast_sin(fast_add_rotations(
                          wrap_angle(FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                                      D_L17_00162254) *
                                     6.2831855f / D_L17_00162254),
                          ripple_vars->f4));
            *(float *)(j * 12 + D_L17_00162218[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + D_L17_00162218[i] + 4) = p[1] + moby->pos.y;
            *(float *)(j * 12 + D_L17_00162218[i] + 8) = p[2] + moby->pos.z + t;
            *(int *)(j * 4 + D_L17_00162228[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (uv_strip_index = 0; uv_strip_index < 2; uv_strip_index++) {
            for (uv_vertex_index = 0; uv_vertex_index < D_L17_00162210[uv_strip_index]; uv_vertex_index++) {
                T4 uv_scroll_factors = D_L17_00162268;
                *(float *)(uv_vertex_index * 8 + D_L17_00162220[uv_strip_index]) =
                    *(float *)(uv_vertex_index * 8 + D_L17_00162238[uv_strip_index]) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[0], 1.0f);
                *(float *)(uv_vertex_index * 8 + D_L17_00162220[uv_strip_index] + 4) =
                    *(float *)(uv_vertex_index * 8 + D_L17_00162238[uv_strip_index] + 4) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (draw_strip_index = 0; draw_strip_index < 2; draw_strip_index++) {
            FUN_L00_001fde98(D_L17_00162210[draw_strip_index], D_L17_00162218[draw_strip_index], D_L17_00162228[draw_strip_index],
                             (void *)D_L17_00162220[draw_strip_index], 1);
        }
    }
}

/* Builds the rippling 89-point ring around the moby (template points moved to its position, z waved by distance from the centre), then draws it twice with scrolled texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F2458), where it is exact; names translated to the US level program. */

typedef struct {
    float f0;
    float f4;
} RippleData_c;

typedef struct {
    float v[2];
} P2_c;

typedef struct {
    P2_c p[2];
} T4_c;

typedef struct {
    float x;
    float y;
    float z;
} RingPt;

extern RingPt D_L17_001DBE20[];
extern RingPt D_L17_001DC680[];
T4_c D_L17_001622D0 = {{{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L17_001622CC __attribute__((sda));
extern float D_L17_001DC250[][2];
extern float D_L17_001DCAB0[][2];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
int D_L17_00162280[2] = {0x59, 0};
int D_L17_001622A0[2] = {(int)D_L17_001DC680, 0};
int D_L17_001622A8[2] = {(int)D_L17_001DCAB0, 0};
extern int D_L17_001DCD78[];
int D_L17_001622B0[2] = {(int)D_L17_001DCD78, 0x80};
int D_L17_001622B4 = 128;
int D_L17_001622B8 = 45;
int D_L17_001622BC = 64;
extern float D_L17_001622C0 __attribute__((sda));
extern float D_L17_001622C4 __attribute__((sda));
extern float D_L17_001622C8 __attribute__((sda));
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void get_moby_color(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void FUN_L17_002f0948(struct Moby *moby) {
    RippleData_c *d = (RippleData_c *)moby->pvars;
    T4_c t;
    float scale;
    int r, g, b;
    int color;
    int i, j, n, k;

    scale = convert_integer_to_float(D_L17_0015F5CC) * (D_L17_001622CC * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(D_L17_001622B8));
    vu1_add_g_sregister(0x42, ((long)D_L17_001622B4 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    d->f0 = d->f4;
    d->f4 = fast_add_rotations(d->f4,
                               360.0f / multiply_global_factor_ed64(D_L17_001622C4) * DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L17_001622BC << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < D_L17_00162280[0]; i++) {
        RingPt *p = &D_L17_001DBE20[i];
        float w = D_L17_001622C8 *
                  fast_sin(fast_add_rotations(
                      wrap_angle(FUN_L00_00200260(square_root(p->x * p->x + p->y * p->y),
                                                  D_L17_001622C0) *
                                 6.2831855f / D_L17_001622C0),
                      d->f4));
        D_L17_001DC680[i].x = p->x + moby->pos.x;
        D_L17_001DC680[i].y = p->y + moby->pos.y;
        D_L17_001DC680[i].z = p->z + moby->pos.z + w;
        D_L17_001DCD78[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (n = 0; n < D_L17_00162280[0]; n++) {
            t = D_L17_001622D0;
            D_L17_001DCAB0[n][0] =
                D_L17_001DC250[n][0] + FUN_L00_00200260(scale * t.p[j].v[0], 1.0f);
            D_L17_001DCAB0[n][1] =
                D_L17_001DC250[n][1] + FUN_L00_00200260(scale * t.p[j].v[1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L17_00162280[k], D_L17_001622A0[k], D_L17_001622B0[k],
                             (void *)D_L17_001622A8[k], 1);
        }
    }
}

/* Builds and draws the animated ring of lights around a moby: 45 vertices and colours, then two passes of 45 texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F2BD8), where it is exact; names translated to the US level program. */

typedef struct {
    float f[4];
} Quad4;

typedef struct {
    float prev;
    float angle;
} RingVars;

Quad4 D_L17_00162390 = {{1.0f, -0.62831855f, 1.0f, 0.62831855f}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L17_0016238C __attribute__((sda));
float D_L17_001DD760[135] = {-7.788906f, -7.788906f, 0.005259f, -7.788906f, -3.894453f, 0.005259f, -3.894453f, -7.788906f, 0.005259f, -3.894453f, -3.894453f, 0.005259f, 0.0f, -7.788906f, 0.005259f, 0.0f, -3.894453f, 0.005259f, 3.894453f, -7.788906f, 0.005259f, 3.894453f, -3.894453f, 0.005259f, 7.788906f, -7.788906f, 0.005259f, 3.894453f, -3.894453f, 0.005259f, 7.788906f, -3.894453f, 0.005259f, 3.894453f, 0.0f, 0.005259f, 7.788906f, 0.0f, 0.005259f, 3.894453f, 3.894453f, 0.005259f, 7.788906f, 3.894453f, 0.005259f, 3.894453f, 3.894453f, 0.005259f, 7.788906f, 7.788906f, 0.005259f, 3.894453f, 3.894453f, 0.005259f, 3.894453f, 7.788906f, 0.005259f, 0.0f, 3.894453f, 0.005259f, 0.0f, 7.788906f, 0.005259f, -3.894453f, 3.894453f, 0.005259f, -3.894453f, 7.788906f, 0.005259f, -3.894453f, 3.894453f, 0.005259f, -7.788906f, 7.788906f, 0.005259f, -3.894453f, 3.894453f, 0.005259f, -7.788906f, 3.894453f, 0.005259f, -3.894453f, 0.0f, 0.005259f, -7.788906f, 0.0f, 0.005259f, -3.894453f, 0.0f, 0.005259f, -7.788906f, -3.894453f, 0.005259f, -3.894453f, 0.0f, 0.005259f, -3.894453f, -3.894453f, 0.005259f, 0.0f, 0.0f, 0.005259f, 0.0f, -3.894453f, 0.005259f, 0.0f, 0.0f, 0.005259f, 3.894453f, -3.894453f, 0.005259f, 0.0f, 0.0f, 0.005259f, 3.894453f, 0.0f, 0.005259f, 0.0f, 0.0f, 0.005259f, 3.894453f, 3.894453f, 0.005259f, 0.0f, 0.0f, 0.005259f, 0.0f, 3.894453f, 0.005259f, -3.894453f, 0.0f, 0.005259f, -3.894453f, 3.894453f, 0.005259f};
float D_L17_001DD980[45][2] = {
    {0.0f, 0.0f},
    {0.0f, 0.25f},
    {0.25f, 0.0f},
    {0.25f, 0.25f},
    {0.5f, 0.0f},
    {0.5f, 0.25f},
    {0.75f, 0.0f},
    {0.75f, 0.25f},
    {1.0f, 0.0f},
    {0.75f, 0.25f},
    {1.0f, 0.25f},
    {0.75f, 0.5f},
    {1.0f, 0.5f},
    {0.75f, 0.75f},
    {1.0f, 0.75f},
    {0.75f, 0.75f},
    {1.0f, 1.0f},
    {0.75f, 0.75f},
    {0.75f, 1.0f},
    {0.5f, 0.75f},
    {0.5f, 1.0f},
    {0.25f, 0.75f},
    {0.25f, 1.0f},
    {0.25f, 0.75f},
    {0.0f, 1.0f},
    {0.25f, 0.75f},
    {0.0f, 0.75f},
    {0.25f, 0.5f},
    {0.0f, 0.5f},
    {0.25f, 0.5f},
    {0.0f, 0.25f},
    {0.25f, 0.5f},
    {0.25f, 0.25f},
    {0.5f, 0.5f},
    {0.5f, 0.25f},
    {0.5f, 0.5f},
    {0.75f, 0.25f},
    {0.5f, 0.5f},
    {0.75f, 0.5f},
    {0.5f, 0.5f},
    {0.75f, 0.75f},
    {0.5f, 0.5f},
    {0.5f, 0.75f},
    {0.25f, 0.5f},
    {0.25f, 0.75f},
};
float D_L17_001DDBA0[135] = {0};
float D_L17_001DDDC0[45][2] = {0};
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
int D_L17_00162340[2] = {0x2d, 0};
int D_L17_00162360[2] = {(int)D_L17_001DDBA0, 0};
int D_L17_00162368[2] = {(int)D_L17_001DDDC0, 0};
int D_L17_001DDF28[45] = {0};
int D_L17_00162370[1] = {(int)D_L17_001DDF28};
int D_L17_00162374 = 128;
int D_L17_00162378 = 45;
int D_L17_0016237C = 64;
extern float D_L17_00162380 __attribute__((sda));
extern float D_L17_00162384 __attribute__((sda));
extern float D_L17_00162388 __attribute__((sda));
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void get_moby_color(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void draw_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L17_002f10c8");

void draw_moby_radial_ripple_mesh(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int output_vertex_byte_offset = 0;
    RingVars *data = (RingVars *)moby->pvars;
    unsigned int uv_vertex_index;
    int j;
    int k;
    int color;
    Quad4 uv_scroll_factors;
    int r;
    int g;
    int b;
    float scale;
    Quad4 *v_scroll_factors;
    float *output_vertex;
    scale = convert_integer_to_float(D_L17_0015F5CC) * (D_L17_0016238C * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(D_L17_00162378));
    vu1_add_g_sregister(0x42, ((long)D_L17_00162374 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = fast_add_rotations(data->angle, 360.0f / multiply_global_factor_ed64(D_L17_00162384) *
                                                      DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    v_scroll_factors = (Quad4 *)&uv_scroll_factors.f[1];
    color = (D_L17_0016237C << 24) | (b << 16) | (g << 8) | r;
    output_vertex = D_L17_001DDBA0;
    do {
        float *template_vertex = &D_L17_001DD760[i * 3];
        float x = template_vertex[0];
        float y = template_vertex[1];
        float rad = square_root(x * x + y * y);
        float radial_phase_input = FUN_L00_00200260(rad, D_L17_00162380);
        float w = wrap_angle(radial_phase_input * 6.2831855f / D_L17_00162380);
        float z = fast_sin(fast_add_rotations(w, data->angle));
        output_vertex[0] = template_vertex[0] + moby->pos.x;
        *(float *)((char *)D_L17_001DDBA0 + 4 + output_vertex_byte_offset) = template_vertex[1] + moby->pos.y;
        output_vertex[2] = template_vertex[2] + moby->pos.z + D_L17_00162388 * z;
        D_L17_001DDF28[i] = color;
        i++;
        output_vertex_byte_offset += 12;
        output_vertex += 3;
    } while (i < 45);
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 45; uv_vertex_index++) {
            uv_scroll_factors = D_L17_00162390;
            D_L17_001DDDC0[uv_vertex_index][0] =
                D_L17_001DD980[uv_vertex_index][0] + FUN_L00_00200260(scale * uv_scroll_factors.f[j * 2], 1.0f);
            D_L17_001DDDC0[uv_vertex_index][1] =
                D_L17_001DD980[uv_vertex_index][1] + FUN_L00_00200260(scale * v_scroll_factors->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L17_00162340[k], D_L17_00162360[k], D_L17_00162370[k],
                             (void *)D_L17_00162368[k], 1);
        }
    }
}

#include "qcopy.h"

/* UpdateMoby 1448: a moby that alternates between two camera points and starts a cutscene on a button press. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F3450), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} LevelState;

typedef struct {
    char pad0[0x20];
    char area[0x40];
    int a[2];
    int b[2];
    char pad70[8];
    int cam[2];
    int dir;
    int cut;
    int w88;
    char pad8C[8];
    int sel;
    int side;
    float f9C;
    float fA0;
    int wA4;
    int go;
    int sfx;
    int timer;
} Vars1448;

extern LevelState D_L17_001BBB30;
extern char *D_L17_001600EC;
extern char D_0013A4E0[];
extern char D_0013E533[];
extern char D_L17_001B0DB0[];
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float);
extern int D_L17_001BADD0[];
extern int remove_help_message_for_selector(int) __asm__("FUN_L00_002644e0");
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames(int);
extern short D_L17_001623A8 __attribute__((sda));
extern short D_L17_001623AC __attribute__((sda));
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_002ea9d8(void *);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern void set_selected_object_float_triplets(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void clear_vector(void *);
extern void fade_to_black(int) __asm__("FUN_001f4a58");
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
extern void vu_euler_rotation_basis(void *, void *);
int update_shuttle_travel_and_camera(char *moby, int idx) __asm__("FUN_L17_002f2180");
void update_shuttle_help_message(struct Moby *moby) __asm__("FUN_L17_002f20d8");

void update_shuttle_help_message(struct Moby *moby) __asm__("FUN_L17_002f20d8");
extern char D_0014C190[];
extern char D_0013CAE4[];

void shuttle_update(struct Moby *m) __asm__("FUN_L17_002f1940");
void shuttle_update(struct Moby *shuttle_moby) {
    enum {
        SHUTTLE_STATE_INITIALIZE = 0,
        SHUTTLE_STATE_WAIT_FOR_ACTIVATION = 1,
        SHUTTLE_STATE_TRAVEL = 2,
        SHUTTLE_STATE_RESET_AFTER_TRAVEL = 3
    };

    Vars1448 *shuttle_vars = (Vars1448 *)shuttle_moby->pvars;
    float old_pos[4];
    float old_rot[4];
    float delta[4];
    float b[4];
    float rot[4];
    float a[4];
    int alternate_stop_index;
    int route_index;
    char *p;
    struct Hero *pl;

    qcopy(old_pos, &shuttle_moby->pos);
    qcopy(old_rot, &shuttle_moby->rot);
    shuttle_moby->scale = shuttle_moby->pclass->scale * *(float *)&D_L17_001623AC;
    switch (shuttle_moby->state) {
    case SHUTTLE_STATE_INITIALIZE:
        if (D_L17_001BBB30.collected[(short)shuttle_moby->save_id] != 0 ||
            (*(int *)(((char *)&D_0014C190) +
                      (((short)shuttle_moby->save_id >> 5) * 4 + (current_level_index << 8))) >>
             (shuttle_moby->save_id & 0x1F)) &
                1) {
            shuttle_vars->sel = 0;
            shuttle_vars->dir = 0;
            shuttle_vars->side = 0;
        }
        shuttle_moby->update_dist = 0xFF;
        shuttle_moby->draw_dist = 0x80;
        shuttle_moby->spawn_frame = *(long *)(((char *)hero.moby) + 0x38);
        shuttle_moby->state = 1;
        if (shuttle_moby->prev_seq != 2) {
            blend_moby_animation(shuttle_moby, 2, 0, 0);
        }
        {
            int base = (int)D_L17_001600EC;
            shuttle_vars->sel = 1;
            shuttle_vars->side = 1;
            shuttle_vars->dir = 1;
            *(OvlQuad *)&shuttle_moby->pos = *(OvlQuad *)((shuttle_vars->cam[1] << 7) + base + 0x30);
            *(OvlQuad *)&shuttle_moby->rot = *(OvlQuad *)((shuttle_vars->cam[1] << 7) + base + 0x70);
        }
        break;
    case SHUTTLE_STATE_WAIT_FOR_ACTIVATION:
        if (shuttle_moby->prev_seq == 2 && (shuttle_moby->anim_flags & 2) && shuttle_vars->sfx == 0) {
            allocate_voice_for_target_entry(1, 0, (int)shuttle_moby);
            shuttle_vars->sfx = 0;
        }
        pl = &hero;
        if (((unsigned char *)pl->ground_moby) == shuttle_moby && pl->air_frames.s == 0) {
            update_shuttle_help_message(shuttle_moby);
            if ((*(int *)(((char *)&D_0013CAE4)) & 0x10) && D_L17_0015F594 == 10) {
                int s = shuttle_vars->dir;
                if (s >= 0) {
                    shuttle_vars->go = 1;
                    switch (s) {
                    case SHUTTLE_STOP_FIRST: {
                        float x = *(float *)&D_L17_001623AC;
                        float y = *(float *)&D_L17_001623A8;
                        shuttle_vars->sel = 1;
                        shuttle_vars->f9C = x;
                        shuttle_vars->fA0 = y;
                        shuttle_vars->side = 0;
                        break;
                    }
                    case SHUTTLE_STOP_SECOND: {
                        float x = *(float *)&D_L17_001623A8;
                        float y = *(float *)&D_L17_001623AC;
                        shuttle_vars->side = s;
                        shuttle_vars->f9C = x;
                        shuttle_vars->fA0 = y;
                        shuttle_vars->sel = 0;
                        break;
                    }
                    }
                }
            }
        }
        if (shuttle_vars->go != 0) {
            shuttle_vars->go = 0;
            remove_help_message_for_selector(10);
            shuttle_moby->state = 2;
            allocate_voice_for_target_entry(2, 0, (int)shuttle_moby);
            if (shuttle_moby->prev_seq != 0) {
                blend_moby_animation(shuttle_moby, 0, 0, 0);
            }
            fade_to_black(scale_game_frames(10));
            shuttle_vars->w88 = 0;
            route_index = shuttle_vars->side;
            qcopy(a, ((char **)D_L17_001B0DB0)[shuttle_vars->a[route_index]] + 0x10);
            qcopy(b, ((char **)D_L17_001B0DB0)[shuttle_vars->b[route_index]] + 0x10);
            clear_vector(rot);
            rot[2] = fast_arc_tan(b[0] - a[0], b[1] - a[1]);
            rot[1] = -fast_arc_tan(vector_distance(a, b), b[2] - a[2]);
            jet_mount(a, rot, 2, scale_game_frames(300), 0);
            set_selected_object_float_triplets(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            FUN_L00_002ea9d8(a);
            FUN_L00_002eaa30(rot);
            FUN_L00_00216f90(D_L17_001600EC + (shuttle_vars->cut << 7) + 0x30,
                             D_L17_001600EC + (shuttle_vars->cut << 7) + 0x70, 0x72, 0);
        } else if (D_L17_001BBB30.collected[(short)shuttle_moby->save_id] != 0 ||
                   (*(int *)(((char *)&D_0014C190) +
                             (((short)shuttle_moby->save_id >> 5) * 4 +
                              (current_level_index << 8))) >>
                    (shuttle_moby->save_id & 0x1F)) &
                       1) {
            float d =
                vector_distance((&hero.motion.pos), D_L17_001600EC + (shuttle_vars->cam[shuttle_vars->sel] << 7) + 0x30);
            if (vector_distance((&hero.motion.pos),
                             D_L17_001600EC + (shuttle_vars->cam[shuttle_vars->sel ^ 1] << 7) + 0x30) < d) {
                int base;
                int ofs;
                alternate_stop_index = shuttle_vars->sel ^ 1;
                shuttle_vars->dir ^= 1;
                shuttle_vars->side ^= 1;
                shuttle_vars->sel = alternate_stop_index;
                base = (int)D_L17_001600EC;
                ofs = shuttle_vars->cam[alternate_stop_index] << 7;
                qcopy(&shuttle_moby->pos, (char *)(ofs + base + 0x30));
                qcopy(&shuttle_moby->rot, (char *)(ofs + base + 0x70));
                vu_euler_rotation_basis(&shuttle_moby->rot_row0, &shuttle_moby->rot);
                shuttle_moby->scale = shuttle_moby->pclass->scale;
            }
        }
        break;
    case SHUTTLE_STATE_TRAVEL:
        if (update_shuttle_travel_and_camera(shuttle_moby, shuttle_vars->side)) {
            int ofs;
            int base;
            shuttle_moby->state = 3;
            shuttle_vars->wA4 = 0;
            ofs = shuttle_vars->cam[shuttle_vars->sel] << 7;
            base = (int)D_L17_001600EC;
            qcopy(&shuttle_moby->pos, (char *)(ofs + base + 0x30));
            qcopy(&shuttle_moby->rot, (char *)(ofs + base + 0x70));
            vu_euler_rotation_basis(&shuttle_moby->rot_row0, &shuttle_moby->rot);
            shuttle_moby->scale = shuttle_moby->pclass->scale;
            qcopy(b, &shuttle_moby->pos);
            normalize_vector_xyz(a, &shuttle_moby->rot_row0, -2.0f);
            add_vector_xyz(a, a, &shuttle_moby->rot_row2);
            add_vector_xyz(b, b, a);
            clear_vector(rot);
            rot[2] = fast_add_rotations(shuttle_moby->rot.z, 3.1415927f);
            FUN_L00_00216f90(b, rot, 0, 0);
            FUN_L00_002eac18(0);
            *(int *)(((char *)&D_0014C190) +
                     (((short)shuttle_moby->save_id >> 5) * 4 + (current_level_index << 8))) |=
                1 << (shuttle_moby->save_id & 0x1F);
            D_L17_001BADD0[(short)shuttle_moby->save_id >> 5] |=
                1 << (shuttle_moby->save_id & 0x1F);
        }
        break;
    case SHUTTLE_STATE_RESET_AFTER_TRAVEL:
        shuttle_moby->state = 1;
        if (shuttle_moby->prev_seq != 2) {
            blend_moby_animation(shuttle_moby, 2, 0, scale_game_frames(30));
        }
        allocate_voice_for_target_entry(0, 0, (int)shuttle_moby);
        shuttle_vars->sfx = 1;
        break;
    }
    subtract_vector_xyz(delta, &shuttle_moby->pos, old_pos);
    update_entity_matrix(shuttle_vars->area, delta, old_rot, &shuttle_moby->rot);
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F3C18), where it is exact; names translated to the US level program. */

s32 try_set_help_message(s32 arg0, s32 arg1) __asm__("FUN_00215130");

void update_shuttle_help_message(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int mode = *(int *)(data + 0x94);
    switch (mode) {
    case SHUTTLE_STOP_FIRST:
        *(int *)(data + 0x80) = 0;
        break;
    case SHUTTLE_STOP_SECOND:
        *(int *)(data + 0x80) = mode;
        break;
    }
    switch (*(int *)(data + 0x80)) {
    case SHUTTLE_STOP_FIRST:
        try_set_help_message(0xA, 0x426C);
        break;
    case SHUTTLE_STOP_SECOND:
    case 4:
        try_set_help_message(0xA, 0x426B);
        break;
    case 2:
        try_set_help_message(0xA, 0x1786);
        break;
    case 3:
        try_set_help_message(0xA, 0x1784);
        break;
    }
}
#include "sda.h"
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) PathPt_3CC0_f2180;
typedef struct {
    int n;
    int pad[3];
    PathPt_3CC0_f2180 p[1];
} Path_3CC0_f2180;
struct Dat_3CC0_f2180 {
    char pad0[0x60];
    int a[2];
    int b[2];
    int c[2];
    int pad1[3];
    int e84;
    float f88;
    float f8C;
    char pad2[0x0C];
    float f9C;
    float fA0;
    float fA4;
};
extern float func_00214D28_f2180(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int func_001F9850_f2180(int) __asm__("FUN_001f96f8");
extern int func_001FA898_r_f2180(float) __asm__("FUN_001fa6d0");
extern void func_001F9C08_f2180(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void func_L00_002EBE88_f2180(void *) __asm__("FUN_L00_002ea9d8");
extern void func_L00_00217718_f2180(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern float func_L00_001FF860_f2180(float, float) __asm__("FUN_001f9e90");
extern float func_001FA748_f2180(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_f2180(float, float) __asm__("FUN_001fa5c8");
extern float func_001F9D48_f2180(void *, void *) __asm__("FUN_001f9b80");
extern float func_001F9F90_f2180(float) __asm__("FUN_001f9dc8");
extern void func_001F9BC0_f2180(void *) __asm__("FUN_001f99f8");
extern void func_L00_002EBEE0_f2180(void *) __asm__("FUN_L00_002eaa30");
extern float func_L00_0025CE58_f2180(float *p, float *v, float a, float b, float c,
                                     float d) __asm__("FUN_L00_0025be00");
extern char D_L17_001B0DB0[];
extern char *D_L17_001600EC;
int update_shuttle_travel_and_camera(char *moby, int route_index) {
    float camera_rotation[4], camera_position[4];
    struct Dat_3CC0_f2180 *shuttle_vars = *(struct Dat_3CC0_f2180 **)(moby + 0x78);
    Path_3CC0_f2180 *camera_position_path = ((Path_3CC0_f2180 **)D_L17_001B0DB0)[shuttle_vars->a[route_index]];
    Path_3CC0_f2180 *camera_aim_path = ((Path_3CC0_f2180 **)D_L17_001B0DB0)[shuttle_vars->b[route_index]];
    Path_3CC0_f2180 *shuttle_movement_path = ((Path_3CC0_f2180 **)D_L17_001B0DB0)[shuttle_vars->c[route_index]];
    float scaled_path_position, camera_position_index_float, camera_position_blend, camera_aim_blend, moby_path_blend, interpolation_start_angle, turn;
    int i1, i2, i3;

    func_00214D28_f2180(&shuttle_vars->fA4, 1.0f, 1.0f / (float)func_001F9850_f2180(0x384));
    scaled_path_position = shuttle_vars->fA4 * (float)(camera_position_path->n - 1);
    i1 = func_001FA898_r_f2180(scaled_path_position);
    camera_position_index_float = (float)i1;
    camera_position_blend = scaled_path_position - camera_position_index_float;
    camera_aim_blend = shuttle_vars->fA4 * 1.02f;
    if (camera_aim_blend > 1.0f)
        camera_aim_blend = 1.0f;
    scaled_path_position = camera_aim_blend * (float)(camera_aim_path->n - 1);
    i2 = func_001FA898_r_f2180(scaled_path_position);
    camera_aim_blend = scaled_path_position - (float)i2;
    scaled_path_position = shuttle_vars->fA4 * (float)(shuttle_movement_path->n - 1);
    moby_path_blend = scaled_path_position - camera_position_index_float;
    i3 = func_001FA898_r_f2180(scaled_path_position);
    func_001F9C08_f2180(camera_position, &camera_position_path->p[i1], &camera_position_path->p[i1 + 1], camera_position_blend);
    func_001F9C08_f2180(moby + 0x10, &shuttle_movement_path->p[i3], &shuttle_movement_path->p[i3 + 1], moby_path_blend);
    func_L00_002EBE88_f2180(camera_position);
    {
        char *base = D_L17_001600EC + (shuttle_vars->e84 << 7);
        func_L00_00217718_f2180(base + 0x30, base + 0x70, 0x72, 0);
    }
    if (i3 > 0) {
        interpolation_start_angle = func_L00_001FF860_f2180(shuttle_movement_path->p[i3].x - shuttle_movement_path->p[i3 - 1].x, shuttle_movement_path->p[i3].y - shuttle_movement_path->p[i3 - 1].y);
        *(float *)(moby + 0x48) = func_001FA748_f2180(
            func_001FA790_f2180(func_L00_001FF860_f2180(shuttle_movement_path->p[i3 + 1].x - shuttle_movement_path->p[i3].x,
                                                        shuttle_movement_path->p[i3 + 1].y - shuttle_movement_path->p[i3].y),
                                interpolation_start_angle) *
                moby_path_blend,
            interpolation_start_angle);
        interpolation_start_angle = -func_L00_001FF860_f2180(func_001F9D48_f2180(&shuttle_movement_path->p[i3 - 1], &shuttle_movement_path->p[i3]),
                                       shuttle_movement_path->p[i3].z - shuttle_movement_path->p[i3 - 1].z);
        *(float *)(moby + 0x44) = func_001FA748_f2180(
            func_001FA790_f2180(
                -func_L00_001FF860_f2180(func_001F9D48_f2180(&shuttle_movement_path->p[i3], &shuttle_movement_path->p[i3 + 1]),
                                         shuttle_movement_path->p[i3 + 1].z - shuttle_movement_path->p[i3].z),
                interpolation_start_angle) *
                moby_path_blend,
            interpolation_start_angle);
        *(float *)(moby + 0x44) = *(float *)(moby + 0x44) * func_001F9F90_f2180(shuttle_vars->f88);
        *(float *)(moby + 0x48) = func_001FA748_f2180(*(float *)(moby + 0x48), shuttle_vars->f88);
    } else {
        *(float *)(moby + 0x44) = -func_L00_001FF860_f2180(
            func_001F9D48_f2180(&shuttle_movement_path->p[0], &shuttle_movement_path->p[1]), shuttle_movement_path->p[1].z - shuttle_movement_path->p[0].z);
        *(float *)(moby + 0x48) =
            func_L00_001FF860_f2180(shuttle_movement_path->p[1].x - shuttle_movement_path->p[0].x, shuttle_movement_path->p[1].y - shuttle_movement_path->p[0].y);
    }
    if (i1 > 0 && i2 > 0) {
        func_001F9BC0_f2180(camera_rotation);
        interpolation_start_angle = func_L00_001FF860_f2180(camera_aim_path->p[i2].x - camera_position[0], camera_aim_path->p[i2].y - camera_position[1]);
        camera_rotation[2] = func_001FA748_f2180(
            func_001FA790_f2180(
                func_L00_001FF860_f2180(camera_aim_path->p[i2 + 1].x - camera_position[0], camera_aim_path->p[i2 + 1].y - camera_position[1]), interpolation_start_angle) *
                camera_aim_blend,
            interpolation_start_angle);
        interpolation_start_angle = -func_L00_001FF860_f2180(func_001F9D48_f2180(camera_position, &camera_aim_path->p[i2]), camera_aim_path->p[i2].z - camera_position[2]);
        camera_rotation[1] = func_001FA748_f2180(
            func_001FA790_f2180(-func_L00_001FF860_f2180(func_001F9D48_f2180(camera_position, &camera_aim_path->p[i2 + 1]),
                                                         camera_aim_path->p[i2 + 1].z - camera_position[2]),
                                interpolation_start_angle) *
                camera_aim_blend,
            interpolation_start_angle);
        func_L00_002EBEE0_f2180(camera_rotation);
    }
    *(float *)(moby + 0x2C) =
        *(float *)(*(char **)(moby + 0x24) + 0x24) * (shuttle_vars->f9C - (shuttle_vars->f9C - shuttle_vars->fA0) * shuttle_vars->fA4);
    if (shuttle_vars->fA4 > 0.8f) {
        turn = frame_time_sq * 12.566371f;
        func_L00_0025CE58_f2180(&shuttle_vars->f88, &shuttle_vars->f8C, 3.1415927f, turn, turn,
                                frame_time * 6.2831855f);
    }
    return shuttle_vars->fA4 == 1.0f;
}

f32 D_L17_001DD100[1] __attribute__((section(".data"))) = {0};
f32 D_L17_001DD104[1] __attribute__((section(".data"))) = {0};
f32 D_L17_001DD320[1] __attribute__((section(".data"))) = {0};
f32 D_L17_001DD328[1] __attribute__((section(".data"))) = {0};
s32 D_L17_001DD6A8[1] __attribute__((section(".data"))) = {0};

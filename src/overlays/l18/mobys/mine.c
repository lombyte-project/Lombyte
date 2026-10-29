/* Level 18 mine (class 0x238): update, launch, detonation and its method table entries. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "qcopy.h"

extern int scale_game_frames(int);

/* Moby update: hovers, drifts toward the player and plays its animations by state. */
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern int   D_001413D0[];
extern int   select_moby_combat_target(char *, char *, float) __asm__("FUN_L00_0025fcb8");
extern int   FUN_L00_002dc9f0(void *, void *);
extern short D_L18_00161988 __attribute__((sda));
extern float D_L18_00161994 __attribute__((sda));
extern void  FUN_L00_00258830(int, int, float, float, float, int);
void         update_mine_detonation(unsigned char *moby) __asm__("FUN_L18_002d5f20");

void update_mine_detonation(unsigned char *m) __asm__("FUN_L18_002d5f20");

void update_mine(struct Moby *moby) __asm__("FUN_L18_002d5918");

void update_mine(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_BALLISTIC_FLIGHT = 1,
        STATE_POST_FLIGHT_SETTLING = 2,
        STATE_APPROACH_NEARBY_TARGET = 3,
        STATE_WAIT_TO_RESUME_APPROACH = 4
    };
    char *data = (char *)moby->pvars;
    float settling_displacement_or_steering_increment[4];
    update_mine_detonation(moby);
    switch (moby->state) {
    case STATE_INITIALIZE:
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        *(void **)(data + 0xD0) = &D_L18_00161988;
        *(float *)(data + 0x30) = 0.05f;
        moby->state = 5;
        break;
    case STATE_BALLISTIC_FLIGHT:
        fast_vec_add(&moby->pos, &moby->pos, data + 0x180);
        *(float *)(data + 0x188) -= frame_time_sq * 10.0f;
        if (moby->pos.z < *(float *)(data + 0x178)) {
            moby->pos.z = *(float *)(data + 0x178);
            moby->state = 2;
            *(float *)(data + 0x188) = -(D_L18_00161994 * 2.0f * frame_time);
        }
        break;
    case STATE_POST_FLIGHT_SETTLING: {
        float settling_motion_step =
            approach_value((float *)(data + 0x188), 0.0f, D_L18_00161994 * 2.0f * frame_time_sq);
        moby->rot.y += settling_motion_step;
        settling_displacement_or_steering_increment[0] =
            fast_cos(moby->rot.z) * settling_motion_step;
        settling_displacement_or_steering_increment[1] =
            fast_sin(moby->rot.z) * settling_motion_step;
        settling_displacement_or_steering_increment[2] = 0.0f;
        fast_vec_add(&moby->pos, &moby->pos, settling_displacement_or_steering_increment);
        if (*(float *)(data + 0x188) == 0.0f) {
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(12));
            }
            moby->state = 3;
            moby->hit_slot = 0xFF;
        }
        break;
    }
    case STATE_APPROACH_NEARBY_TARGET:
        if (moby->prev_seq == 1 && (moby->anim_flags & 2)) {
            blend_moby_animation(moby, 2, 0, scale_game_frames(12));
        }
        if (moby->visible) {
            char  target_search_output[0x50];
            float previous_position[4];
            int   target_search_result =
                select_moby_combat_target((char *)moby, target_search_output, 6.0f);
            char *hit;

            char *movement_step;
            if (target_search_result == 2)
                return;
            hit = *(char **)(target_search_output + 0x40);
            if (hit == 0)
                return;
            if (vector_distance_xy(&moby->pos, hit + 0x10) < 6.0f) {
                float movement_step_limit = frame_time * 3.0f;
                float horizontal_steering_increment_limit = frame_time_sq * 6.0f;
                qcopy(previous_position, &moby->pos);
                fast_vec_sub(settling_displacement_or_steering_increment,
                             *(char **)(target_search_output + 0x40) + 0x10,
                             &moby->pos);
                settling_displacement_or_steering_increment[2] = -(frame_time_sq * 10.0f);
                if (horizontal_steering_increment_limit <
                    vector_length_xy(settling_displacement_or_steering_increment)) {
                    set_vector_length_xy(settling_displacement_or_steering_increment,
                                         settling_displacement_or_steering_increment,
                                         horizontal_steering_increment_limit);
                }
                movement_step = data + 0x40;
                fast_vec_add(
                    movement_step, movement_step, settling_displacement_or_steering_increment);
                if (movement_step_limit < vector_length_xy(movement_step)) {
                    set_vector_length_xy(movement_step, movement_step, movement_step_limit);
                }
                FUN_L00_00258830((int)moby, (int)movement_step, 0.0f, 0.5f, 0.0f, 0);
                fast_vec_sub(movement_step, &moby->pos, previous_position);
                if (0.0f < *(float *)(data + 0x48)) {
                    *(float *)(data + 0x48) = 0.0f;
                } else if (*(float *)(data + 0x48) < -movement_step_limit) {
                    *(float *)(data + 0x48) = -movement_step_limit;
                }
                if (*(char **)(target_search_output + 0x40) != (char *)D_001413D0[0]) {
                    if (fast_vec_dist(*(char **)(target_search_output + 0x40) + 0x10, &moby->pos) <
                        2.0f) {
                        *(int *)(data + 0x19C) = 1;
                    }
                }
            }
        }
        break;
    case STATE_WAIT_TO_RESUME_APPROACH:
        if (FUN_L00_002dc9f0(moby, data + 0x110)) {
            moby->state = 3;
            *(short *)(data + 0xC8) = 0;
        }
        break;
    }
}

#include "sda.h"

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D70E8), where it is exact; names translated to the US level program. */

typedef struct {
    char          _pad00[0x10];
    unsigned char nframes; /* 0x10 */
} AnimSeq_q;

typedef struct {
    char       _pad00[0x48];
    AnimSeq_q *seqs[1]; /* 0x48 */
} AnimClass_q;

typedef struct {
    char          _pad00[0x24];
    AnimClass_q  *pClass; /* 0x24 */
    char          _pad28[0x50 - 0x28];
    unsigned char frame;     /* 0x50 */
    unsigned char nextFrame; /* 0x51 */
    unsigned char seq;       /* 0x52 */
    unsigned char prevSeq;   /* 0x53 */
    char          _pad54[0x5C - 0x54];
    float         unk5C; /* 0x5C */
    char          _pad60[0x68 - 0x60];
    float        *frameData; /* 0x68 */
    char          _pad6C[4];
    unsigned char unk70; /* 0x70 */
} MobyAnim_q;

extern float FUN_L00_0025abf0_q(float *a, float *b, float *out, float speed, float g) __asm__("FUN_L00_0025abf0");
extern int *D_L18_001AC240_lui[] __asm__("D_L18_001AC240") NOT_SDA;
extern int  D_L18_0015FFD8_m __asm__("D_L18_0015FFD8");
f32         mine_launch_pitch_and_setback_gp __asm__("D_L18_00161994") = 2.5f;

char *launch_class_238_moby_from_list(float  horizontal_speed,
                                      int    launching_moby,
                                      int    moby_list_index,
                                      float *launch_position,
                                      float *target_position,
                                      int    detonation_countdown) __asm__("FUN_L18_002d5cf8");

char *launch_class_238_moby_from_list(float  horizontal_speed,
                                      int    launching_moby,
                                      int    moby_list_index,
                                      float *launch_position,
                                      float *target_position,

                                      int detonation_countdown) {
    float  adjusted_target_position[4];
    float  pitch_and_target_setback_scalar;
    float  ballistic_gravity_magnitude;
    char  *found = 0;
    short *moby_list_entry = (short *)D_L18_001AC240_lui[moby_list_index];
    char  *moby;
    char  *class_238_private_vars;
    char  *d2;
    if (moby_list_entry == 0) {
        return 0;
    }
    {
        char          *base = D_L18_0015FFD8_m;
        unsigned short encoded_moby_entry;
        do {
            encoded_moby_entry = *moby_list_entry;
            {
                char *candidate_moby = base + ((encoded_moby_entry & 0x7FFF) << 8);
                if (MOBY(candidate_moby)->oclass == 0x238) {
                    if (((unsigned char *)candidate_moby)[0x20] == 5) {
                        found = candidate_moby;
                    }
                }
            }
            moby_list_entry++;
        } while ((short)encoded_moby_entry >= 0);
    }
    moby = found;
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        MOBY(moby)->flags = (MOBY(moby)->flags & 0xFFBE) | 0x1000;
        moby[0xBC] = 0;
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        if (((unsigned char *)moby)[0x53] != 0) {
            blend_moby_animation(moby, 0, 0, 1);
        }
        qcopy(moby + 0x10, launch_position);
        pitch_and_target_setback_scalar = *(float *)&mine_launch_pitch_and_setback_gp;
        *(int *)(moby + 0x40) = 0;
        MOBY(moby)->rot.y = -pitch_and_target_setback_scalar;
        target_position[2] = target_position[2] + 0.75f;
        fast_vec_sub(adjusted_target_position, target_position, launch_position);
        set_vector_length_xy(adjusted_target_position,
                             adjusted_target_position,
                             vector_length_xy(adjusted_target_position) -
                                 *(float *)&mine_launch_pitch_and_setback_gp);
        fast_vec_add(adjusted_target_position, adjusted_target_position, launch_position);
        class_238_private_vars = ((char *)MOBY(moby)->pvars);
        qcopy(class_238_private_vars + 0x170, adjusted_target_position);
        fast_vec_sub(class_238_private_vars + 0x180, adjusted_target_position, launch_position);
        *(int *)(class_238_private_vars + 0x188) = 0;
        fast_vec_normalize(class_238_private_vars + 0x180, class_238_private_vars + 0x180, horizontal_speed);
        ballistic_gravity_magnitude = frame_time_sq * 10.0f;
        *(short *)(class_238_private_vars + 0xC8) = 0;
        *(float *)(class_238_private_vars + 0x188) = FUN_L00_0025abf0_q(launch_position,
                                                      adjusted_target_position,
                                                      0,
                                                      horizontal_speed,
                                                      -ballistic_gravity_magnitude);
        MOBY(moby)->rot.z = fast_arc_tan(*(float *)(class_238_private_vars + 0x180), *(float *)(class_238_private_vars + 0x184));
        *(int *)(class_238_private_vars + 0x194) = detonation_countdown;
        *(int *)(class_238_private_vars + 0x198) = launching_moby;
        *(int *)(class_238_private_vars + 0x19C) = 0;
        allocate_voice_for_target_entry(0, 0, (int)moby);
    }
    return moby;
}

/* Blows the moby up when it is hit, shot by the player or close to the target, then hides it. */
extern char D_L18_0015F580_h[8] __asm__("D_L18_0015F580");
extern int  find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void explode_h(void *,
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

void update_mine_detonation(unsigned char *moby) {
    char        *moby_vars = ((char *)MOBY(moby)->pvars);
    struct Hero *hero_ptr;
    int          hit;
    if (moby[0x20] == 5 || moby[0x20] == 0 || moby[0x20] == 4)
        return;
    hero_ptr = &hero;
    if (hero_ptr->state.current == 0x72) {
        moby[0x20] = 5;
        MOBY(moby)->flags = (((short)MOBY(moby)->flags) | 0x41) & ~0x1000;
        MOBY(moby)->collision = 0;
        return;
    }
    hit = find_moby_hit(moby, 0x330000, 0);
    moby[0xA4] = 0xFF;
    if (((unsigned char *)hero_ptr->coll_hit_moby) == moby ||
        ((unsigned char *)hero_ptr->unk240) == moby || *(int *)(moby_vars + 0x19C) != 0) {
        explode_h(moby,
                  D_L18_0015F580_h,
                  0,
                  1.5f,
                  1.0f,
                  0x14,
                  6,
                  0x20,
                  3.0f,
                  1.5f,
                  9.0f,
                  1.5f,
                  1,
                  0.0f,
                  1,
                  0,
                  -1,
                  0);
        moby[0x20] = 5;
        MOBY(moby)->flags = (((short)MOBY(moby)->flags) | 0x41) & ~0x1000;
        MOBY(moby)->collision = 0;
        return;
    }
    if (fast_dec_timer((int *)(moby_vars + 0x194)) == 0 && hit == 0) {
        if (moby[0x20] != 1)
            return;
        if (!(vector_distance_xy(moby + 0x10, &hero_ptr->motion.pos) < 2.0f))
            return;
    }
    explode_h(moby,
              D_L18_0015F580_h,
              0,
              0.0f,
              0.0f,
              5,
              2,
              8,
              1.0f,
              0.5f,
              9.0f,
              0.5f,
              1,
              0.0f,
              0,
              0,
              -1,
              0);
    moby[0x20] = 5;
    MOBY(moby)->flags = (((short)MOBY(moby)->flags) | 0x41) & ~0x1000;
    MOBY(moby)->collision = 0;
}
/* Pauses the moby in state 4 while the check holds, restoring its old state after. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

int update_moby_state_pause(struct Moby *moby) __asm__("FUN_L18_002d6108");

int update_moby_state_pause(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int   state_pause_result = process_moby_suck_cannon_suction(moby);
    if (state_pause_result != 0) {
        unsigned char current_state = moby->state;
        if ((unsigned)(current_state - 3) < 2) {
            state_pause_result = 2;
            if (current_state != 4) {
                moby->command = current_state;
                moby->state = 4;
            }
        } else {
            *(short *)(moby_vars + 0xC8) = 0;
            state_pause_result = 0;
        }
    } else {
        long t = moby->state;
        if (t == 4) {
            moby->state = moby->command;
        }
    }
    return state_pause_result;
}

/* Switch to state four while active, then restore the saved state. */
/* Ported from rac1-decomp (src/overlays/l18/vendor_002A8400.c: func_L18_002D7580), where it is exact; names translated to the US level program. */

extern int store_moby_in_suck_cannon(struct Moby *) __asm__("FUN_L00_002dbb20");

int moby_hold_state_while_active(struct Moby *moby) __asm__("FUN_L18_002d6190");

int moby_hold_state_while_active(struct Moby *moby) {
    int active = store_moby_in_suck_cannon(moby);
    if (active != 0) {
        moby->state = 4;
    } else if (moby->state == 4) {
        moby->state = moby->command;
    }
    return active;
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D7670), where it is exact; names translated to the US level program. */

void hide_moby_and_disable_collision(struct Moby *moby) __asm__("FUN_L18_002d6280");

void hide_moby_and_disable_collision(struct Moby *moby) {
    unsigned short flags;
    moby->state = 5;
    if (moby->prev_seq != 0) {
        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
    }
    flags = moby->flags;
    moby->visible = 0;
    flags |= 1;
    moby->collision = 0;
    flags &= 0xEFFF;
    moby->flags = flags;
}

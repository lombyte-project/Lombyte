/* Weapons: effect slot firing and shot tracking. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/world/collision/collision_line.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0025D750.c: func_L02_00265E58), where it is exact; names translated to the US level program. */

void reset_shot_tracking(char *moby) __asm__("FUN_L02_00264e40");

void reset_shot_tracking(char *moby) {
    int *word_to_clear = (int *)&MOBY(moby)->pos.z;
    int  i;
    for (i = 6; i >= 0; i--, word_to_clear--) {
        *word_to_clear = 0;
    }
    *(short *)(moby + 0x1C) = 0;
}

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0025D750.c: func_L02_00265E88), where it is exact; names translated to the US level program. */

typedef struct {
    char *p[7];
    short timer;
} Slots;

extern char              *spawn_spinning_and_fading_effect_particle(void *,
                                                                    void *,
                                                                    unsigned long,
                                                                    float) __asm__("FUN_L00_0026b230");
extern f32                frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32                frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern float              D_L02_00174460[];
extern float              FUN_L04_001f1978(float);
extern float              fast_vec_dist(void *, void *);
extern int D_L02_0015F5CC MACRO_ADDR;
extern float              D_L02_001600D0 __attribute__((sda));
extern int                D_L02_001600D4 __attribute__((sda));
extern int                D_L02_001600D8 __attribute__((sda));
extern float              D_L02_001600DC __attribute__((sda));
extern float              D_L02_001600E0 __attribute__((sda));
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

void effect_slots_fire(char  *slots,
                       float *position,
                       float *direction,
                       float  distance) __asm__("FUN_L02_00264e70");

void effect_slots_fire(char *slots, float *position, float *direction, float distance) {
    float         origin[4] __attribute__((aligned(16)));
    float         end[4] __attribute__((aligned(16)));
    float         adjusted[4] __attribute__((aligned(16)));
    float         velocity[4] __attribute__((aligned(16)));
    float         offset[4] __attribute__((aligned(16)));
    float         travel_step = 0.0f;
    float         speed = 0.0f;
    float         travel;
    float         acceleration;
    int           n;
    int           first_count;
    int           i;
    int           save_shot;
    char         *shot;
    unsigned long fl;

    {
        int phase = D_L02_0015F5CC & 3;
        save_shot = phase == 0;
    }
    qcopy(origin, position);
    fast_vec_normalize(adjusted, direction, distance + 0.75f);
    fast_vec_add(end, adjusted, origin);
    if (collision_line(origin, end, 2, 0, 0)) {
        travel = fast_vec_dist(origin, D_L02_00174460) - 0.75f;
        if (travel < 0.0f)
            travel = 0.0f;
    } else {
        travel = distance;
    }
    acceleration = frame_time_sq_macro;
    advance_accelerated_scalar(&speed,
                               &travel_step,
                               travel,
                               1.0f,
                               acceleration * 48.0f,
                               (D_L02_001600D0)*frame_time_macro);
    ((float (*)(void *, void *, float))fast_vec_normalize)(adjusted, adjusted, travel_step);
    n = (D_L02_001600D8);
    first_count = (D_L02_001600D4);
    if (travel < 4.0f)
        first_count = 1;
    for (i = 0; i < n + first_count; i++) {
        random_vector(
            offset,
            0.0f,
            (((D_L02_001600DC) - (D_L02_001600E0)) * (travel / distance) + (D_L02_001600E0)) *
                frame_time_macro);
        fast_vec_add(offset, offset, adjusted);
        fast_vec_scale(velocity, adjusted, random_float_between(0.0f, 1.0f));
        fast_vec_add(velocity, velocity, origin);
        if (i < first_count)
            fl = 1;
        else
            fl = 0;
        shot = spawn_spinning_and_fading_effect_particle(velocity, offset, fl, travel);
        if (i == 2 && save_shot) {
            int j;
            for (j = 0; j < 7; j++) {
                if (((Slots *)slots)->p[j] == 0) {
                    ((Slots *)slots)->p[j] = shot;
                    break;
                }
            }
        }
    }
    if (fast_dec_timer_s16(slots + 0x1c)) {
        float sp2 = random_float_between(10.0f, 30.0f);
        sp2 = FUN_L04_001f1978(sp2);
        ((Slots *)slots)->timer = truncate_float_to_s32(sp2);
        random_vector(offset, 0.0f, frame_time_macro + frame_time_macro);
        fast_vec_scale(velocity, adjusted, random_float_between(0.5f, 0.1f));
        fast_vec_add(velocity, velocity, offset);
        velocity[3] = 0.0f;
        spawn_color_decaying_effect_particle(position, velocity, 0);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0025D750.c: func_L02_002661E8), where it is exact; names translated to the US level program. */

extern int collect_mobies_in_sphere(float, void *, int, int, int) __asm__("FUN_L00_001f2868");

void collect_mobies_around_tracked_objects(char **slots,
                                           int    excluded_moby,
                                           int    moby_record_payload) __asm__("FUN_L02_002651d0");

void collect_mobies_around_tracked_objects(char **slots,
                                           int    excluded_moby,

                                           int moby_record_payload) {
    int i;
    for (i = 6; i >= 0; i--, slots++) {
        char *slot_object = *slots;
        if (slot_object == 0)
            continue;
        if ((unsigned char)slot_object[0] != 12) {
            *slots = 0;
            continue;
        }
        if (slot_object[1] < 0) {
            *slots = 0;
        } else {
            collect_mobies_in_sphere(*(float *)(slot_object + 0xC) / 210000.0f * 0.25f,
                                     slot_object + 0x10,
                                     0,
                                     excluded_moby,
                                     moby_record_payload);
            if (*(short *)(slot_object + 0xA) < 5)
                *slots = 0;
        }
    }
}

float D_L16_001600D0 = 16.0f;
int   D_L16_001600D4 = 2;
int   D_L16_001600D8 = 2;
float D_L16_001600DC = 1.0f;
float D_L16_001600E0 = 3.0f;

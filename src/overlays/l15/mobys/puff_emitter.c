/* Level 15 puff emitter moby class: spawns rising puffs at random. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern void FUN_L00_0026f080(float, float, char *, float *);

extern short vertical_effect_motion_scale_gp __asm__("D_L15_00162198") __attribute__((sda));

int l15_puff_emission_random_range __asm__("D_L15_0016219C") = 3;

void puff_emitter_update(struct Moby *moby) __asm__("FUN_L15_002eb928");

/* Moby class 1425 */
void puff_emitter_update(struct Moby *moby) {
    float  vertical_motion_vector[4];
    float *effect_height_setting = (float *)moby->pvars;
    if (random_integer_below(l15_puff_emission_random_range - 1) == 0) {
        char *pos = &moby->pos;
        clear_u64_value(vertical_motion_vector);
        vertical_motion_vector[2] = random_float_between(0.75f, 1.5f) *
                                    *(float *)&vertical_effect_motion_scale_gp * frame_time;
        FUN_L00_0026f080(random_float_between(0.06f, 0.12f) * 210000.0f,
                         *effect_height_setting,
                         pos,
                         vertical_motion_vector);
    }
}

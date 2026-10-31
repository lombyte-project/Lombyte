/* Level 08 moby class 0x669: effect emitter; spawns effects along its width with random rising motion. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/game/item_state.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030A150), where it is exact; names translated to the US level program. */
extern void
     spawn_effect_with_initial_motion(float *, float *, int, float) __asm__("FUN_L08_0027fe00");
void effect_emitter_update(struct Moby *moby) __asm__("FUN_L08_00308c90");

/* Moby class 1641 */
void effect_emitter_update(struct Moby *moby) {
    char *effect_spawn_settings = (char *)moby->pvars;
    float effect_position[4];
    float rotation_basis[12];
    float effect_direction[4];
    int   i;
    for (i = 0; i < *(int *)(effect_spawn_settings + 0x14); i++) {
        effect_position[0] = -1.0f;
        effect_position[1] = random_float_between(-*(float *)(effect_spawn_settings + 4),
                                                  *(float *)(effect_spawn_settings + 4));
        effect_position[2] = 0;
        effect_position[3] = 1.0f;
        euler_rotation_basis(rotation_basis, &moby->rot);
        transform_vector_by_basis(effect_position, effect_position, rotation_basis);
        fast_vec_add(effect_position, effect_position, &moby->pos);
        effect_direction[0] = 0;
        effect_direction[1] =
            random_float_between(*(float *)(effect_spawn_settings + 0x18) * frame_time,
                                 *(float *)(effect_spawn_settings + 0x1C) * frame_time);
        effect_direction[2] =
            random_float_between(*(float *)(effect_spawn_settings + 0x20) * frame_time,
                                 *(float *)(effect_spawn_settings + 0x24) * frame_time);
        effect_direction[3] = 0;
        spawn_effect_with_initial_motion(effect_position,
                                         effect_direction,
                                         *(int *)(effect_spawn_settings + 0x10),
                                         *(float *)(effect_spawn_settings + 0xC));
    }
}

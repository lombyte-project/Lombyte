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
#include "rnc/overlay/l08/mobys.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030A150), where it is exact; names translated to the US level program. */
void effect_emitter_update(struct Moby *moby) __asm__("FUN_L08_00308c90");

typedef struct EffectEmitterVars {
    u8  pad_0[0x4];
    f32 unk4;
    u8  pad_8[0x4];
    f32 unkC;
    s32 unk10;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} EffectEmitterVars;

/* Moby class 1641 */
void effect_emitter_update(struct Moby *moby) {
    EffectEmitterVars *effect_spawn_settings = (EffectEmitterVars *)moby->pvars;
    float              effect_position[4];
    float              rotation_basis[12];
    float              effect_direction[4];
    int                i;
    for (i = 0; i < effect_spawn_settings->unk14; i++) {
        effect_position[0] = -1.0f;
        effect_position[1] =
            random_float_between(-effect_spawn_settings->unk4, effect_spawn_settings->unk4);
        effect_position[2] = 0;
        effect_position[3] = 1.0f;
        euler_rotation_basis(rotation_basis, &moby->rot);
        transform_vector_by_basis(effect_position, effect_position, rotation_basis);
        fast_vec_add(effect_position, effect_position, &moby->pos);
        effect_direction[0] = 0;
        effect_direction[1] = random_float_between(effect_spawn_settings->unk18 * frame_time,
                                                   effect_spawn_settings->unk1C * frame_time);
        effect_direction[2] = random_float_between(effect_spawn_settings->unk20 * frame_time,
                                                   effect_spawn_settings->unk24 * frame_time);
        effect_direction[3] = 0;
        spawn_effect_with_initial_motion(effect_position,
                                         effect_direction,
                                         effect_spawn_settings->unk10,
                                         effect_spawn_settings->unkC);
    }
}

/* Level 02 moby class sandshark_egg. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

extern int step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");

#include "qcopy.h"

#include "rnc/mobys/moby.h"
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E16B8), where it is exact; names translated to the US level program. */

extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");

int  l02_sandshark_egg_effect_initial_color __asm__("D_L02_00161C9C") = 0x6f707070;
int  l02_sandshark_egg_effect_color_endpoint __asm__("D_L02_00161CA0") = 0x707070;
void sandshark_egg_update(struct Moby *moby) __asm__("FUN_L02_002e0280");

typedef struct SandsharkEggVars {
    u8    pad_0[0x60];
    char *linked_moby;
} SandsharkEggVars;

/* Moby class 755 */
void sandshark_egg_update(struct Moby *moby) {
    enum {
        SANDSHARK_EGG_FALL_UNTIL_LANDING = 1,
        SANDSHARK_EGG_HANDOFF_TO_LINKED_MOBY = 2
    };
    float             u[4];
    float             v[4];
    SandsharkEggVars *egg_vars = (SandsharkEggVars *)moby->pvars;
    int               i;
    float            *vp;
    int               effect_frame_count;
    int               random_effect_value;
    char             *linked_moby;
    switch (moby->state) {
    case SANDSHARK_EGG_FALL_UNTIL_LANDING:
        clear_u64_value(u);
        if (step_moby_motion(moby, egg_vars) & 1) {
            moby->state = SANDSHARK_EGG_HANDOFF_TO_LINKED_MOBY;
        }
        vp = v;
        i = 2;
        do {
            qcopy(vp, &moby->pos);
            v[2] += random_float_between(0.0f, 0.5f);
            v[0] += random_float_between(-0.15f, 0.15f);
            v[1] += random_float_between(-0.15f, 0.15f);
            effect_frame_count = scale_game_frames(random_int_between(0x14, 0x28));
            random_effect_value = random_int_between(0x14, 0x28);
            spawn_effect(vp,
                         u,
                         l02_sandshark_egg_effect_initial_color,
                         l02_sandshark_egg_effect_color_endpoint,
                         effect_frame_count,
                         0x28,
                         random_effect_value,
                         1);
            i--;
        } while (i >= 0);
        break;
    case SANDSHARK_EGG_HANDOFF_TO_LINKED_MOBY:
        linked_moby = egg_vars->linked_moby;
        qcopy(linked_moby + 0x10, &moby->pos);
        linked_moby[0x20] = 0x11;
        blend_moby_animation(egg_vars->linked_moby, 5, 0, 1);
        *(int *)(egg_vars->linked_moby + 0x94) =
            *(int *)(*(char **)(egg_vars->linked_moby + 0x24) + 0x10);
        *(float *)(egg_vars->linked_moby + 0x58) = 1.0f;
        update_entity(egg_vars->linked_moby);
        mark_moby_for_removal(moby);
        break;
    }
}

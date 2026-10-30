/* Level 16 moby class breakable_prop (0x320): once hit, drops bolts, breaks into fragments and removes itself. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"

extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7B08), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  FUN_L00_00257470(void *, int, int);

void update_breakable_prop(struct Moby *moby) __asm__("FUN_L16_002d6740");

void update_breakable_prop(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_CHECK_HIT_FOR_REMOVAL = 1,
        MOBY_STATE_REMOVE_WITH_EFFECTS = 2
    };
    int   flag = 0;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case MOBY_STATE_CHECK_HIT_FOR_REMOVAL:
        if (hit_record != 0 && 0.0f < *(float *)(hit_record + 0x2C))
            flag = 1;
        if (flag)
            moby->state = 2;
        break;
    case MOBY_STATE_REMOVE_WITH_EFFECTS:
        allocate_voice_for_target_entry(0, 0, moby);
        FUN_L00_00257470(moby, 0, -1);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x74E, 1, 0x74E, 1, 0xB, 2);
        mark_moby_for_removal(moby);
        break;
    }
}

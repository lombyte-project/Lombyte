/* Level 17 moby class shattering_prop (0x751): when hit, shatters into debris and bolts and leaves its broken placement moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

/* UpdateMoby 1873: waits for a hit, then shatters into debris and bolts, spawns its broken placement moby and deletes itself. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6DA0), where it is exact; names translated to the US level program. */

extern int  FUN_0022da68(int, int, int);
extern void FUN_L00_00257470(void *, int, int);
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

void update_shattering_prop(struct Moby *moby) __asm__("FUN_L17_002f5260");

void update_shattering_prop(struct Moby *moby) {
    enum {
        HIT_EFFECT_INITIALIZE = 0,
        HIT_EFFECT_WAIT_FOR_HIT = 1,
        HIT_EFFECT_EMIT_AND_REMOVE = 2
    };
    int   flag = 0;
    char *hit_result = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case HIT_EFFECT_INITIALIZE:
        moby->state = 1;
        break;
    case HIT_EFFECT_WAIT_FOR_HIT:
        if (hit_result && *(float *)(hit_result + 0x2C) > 0.0f)
            flag = 1;
        if (flag)
            moby->state = 2;
        break;
    case HIT_EFFECT_EMIT_AND_REMOVE:
        FUN_0022da68(0, 0, (int)moby);
        FUN_L00_00257470(moby, 0, -1);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x753, 1, 0x753, 1, 11, 2);
        spawn_moby_break_fragments(moby, 0x756, 1, 0x756, 1, 1, 2);
        spawn_moby_from_placement(moby, 0x752);
        mark_moby_for_removal(moby);
        break;
    }
}

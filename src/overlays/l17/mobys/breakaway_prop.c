/* Level 17 moby class breakaway_prop (0x754): when hit, drops its loose part, sheds debris and bolts and leaves its broken placement moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

/* UpdateMoby 1876: waits for a hit, then drops its loose part, sheds debris and bolts, spawns its broken placement moby and deletes itself. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6ED0), where it is exact; names translated to the US level program. */

extern void  FUN_L00_00257470(void *, int, int);
extern void  spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");
extern void *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float *v10,
                              float *v11,
                              float  scale,
                              float *v12);

extern float D_L17_0015F580_macro[] __asm__("D_L17_0015F580") MACRO_ADDR;
void         update_breakaway_prop(struct Moby *moby) __asm__("FUN_L17_002f5390");

void update_breakaway_prop(struct Moby *moby) {
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
        if (hit_result) {
            if (*(float *)(hit_result + 0x2C) > 0.0f)
                flag = 1;
        }
        if (flag)
            moby->state = 2;
        break;
    case HIT_EFFECT_EMIT_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        FUN_L00_00257470(moby, 0, -1);
        spawn_proximity_gated_bolts(moby);
        FUN_L00_00263fd8((char *)moby,
                         0x753,
                         &moby->pos.x,
                         &moby->rot,
                         0,
                         0,
                         D_L17_0015F580_macro,
                         D_L17_0015F580_macro,
                         0.0f,
                         D_L17_0015F580_macro);
        spawn_moby_break_fragments(moby, 0x756, 1, 0x756, 1, 1, 2);
        spawn_moby_from_placement(moby, 0x755);
        mark_moby_for_removal(moby);
        break;
    }
}

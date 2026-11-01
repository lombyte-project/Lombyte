/* Level 12 moby class effects_draw_trigger (class 293): queues the level effects draw while the hero is in its clip volume. */
#include "types.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/rendering/effects/draw_textured_effect_list.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

extern char         D_L12_00167240[];
typedef struct Moby Moby;

#include "rnc/overlay/shared/world/level_globals.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8588), where it is exact; names translated to the US level program. */

void effects_draw_trigger_update(struct Moby *moby) __asm__("FUN_L12_002e7208");

/* Moby class 293 */
void effects_draw_trigger_update(struct Moby *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER = 1
    };
    char *draw_callback_vars = (char *)moby->pvars;
    switch (moby->state) {
    case DRAW_CALLBACK_INITIALIZE:
        moby->state = DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER;
        moby->update_dist = 0xFF;
        break;
    case DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER: {
        int clip_volume_index = *(int *)(draw_callback_vars + 0xC);
        if (clip_volume_index == -1 ||
            is_point_inside_clip_volume(D_L12_00167240, clip_volume_index)) {
            add_draw_callback((void *)draw_level_12_effects, moby);
        }
    } break;
    }
}

/* Level 12 moby class effects_draw_trigger (class 293): queues the level effects draw while the hero is in its clip volume. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/mobys/moby_spawn.h"

extern char         D_L12_00167240[];
extern int          is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
typedef struct Moby Moby;

#include "rnc/overlay/level_globals.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8588), where it is exact; names translated to the US level program. */

extern void draw_level_12_effects(void) __asm__("FUN_L12_002e71b0");

void       update_effects_draw_trigger(struct Moby *moby) __asm__("FUN_L12_002e7208");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");

void update_effects_draw_trigger(struct Moby *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER = 1
    };
    char *data = (char *)moby->pvars;
    switch (moby->state) {
    case DRAW_CALLBACK_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER: {
        int value = *(int *)(data + 0xC);
        if (value == -1 || is_point_inside_clip_volume(D_L12_00167240, value)) {
            add_draw_callback((void *)draw_level_12_effects, moby);
        }
    } break;
    }
}

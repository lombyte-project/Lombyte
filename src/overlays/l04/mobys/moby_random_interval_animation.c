/* Level 04 moby class 0xA1: plays an animation at random intervals. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002A6C20), where it is exact; names translated to the US level program. */

extern s32 rand();

void        update_moby_random_interval_animation(struct Moby *moby) __asm__("FUN_L04_002a5a08");
extern void refresh_moby_spatial_bounds_from_basis(void *);

void update_moby_random_interval_animation(struct Moby *moby) {
    int *animation_countdown;
    if (moby != 0) {
        moby->collision = 0;
        animation_countdown = (int *)moby->pvars;
        if (*animation_countdown == 0) {
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(3));
            }
            *animation_countdown = scale_game_frames((rand() + 0x1E) & 0xFF);
        }
        if ((moby->anim_flags & 2) && moby->prev_seq != 0) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(3));
        }
        *animation_countdown = *animation_countdown - 1;
        refresh_moby_spatial_bounds_from_basis(moby);
    }
}

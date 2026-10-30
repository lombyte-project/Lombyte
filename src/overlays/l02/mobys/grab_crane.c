/* Level 02 moby class grab_crane. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6B60), where it is exact; names translated to the US level program. */

extern float scale_time(float) __asm__("FUN_001f96b0");

#include "qcopy.h"
/* UpdateMoby_656: steps the moby's animation state from 0/2 (when flagged) and 1/3 (when the timer runs out) */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DDE48), where it is exact; names translated to the US level program. */

extern int FUN_L00_001fefc8(void *);
/* four-state update: waits for a flag, plays a transition, and rolls a random heading on entering state 2 or 7 */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DE7A8), where it is exact; names translated to the US level program. */

void grab_crane_update(struct Moby *moby) __asm__("FUN_L02_002dd370");

void grab_crane_update(struct Moby *moby) {
    enum {
        GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT = 0,
        GRAB_CRANE_ANIM_TIMED_WAIT = 2,
        GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT_BEFORE_RESTART = 3,
        GRAB_CRANE_ANIM_TIMED_WAIT_BEFORE_RESTART = 7
    };
    switch (moby->prev_seq) {
    case GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT:
        if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 2, 0, scale_game_frames(5));
            moby->command = truncate_float_to_s32(scale_time(random_float_between(60.0f, 180.0f)));
        }
        break;
    case GRAB_CRANE_ANIM_TIMED_WAIT:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(5));
        }
        break;
    case GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT_BEFORE_RESTART:
        if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 7, 0, scale_game_frames(5));
            moby->command = truncate_float_to_s32(scale_time(random_float_between(60.0f, 180.0f)));
        }
        break;
    case GRAB_CRANE_ANIM_TIMED_WAIT_BEFORE_RESTART:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(5));
        }
        break;
    }
}

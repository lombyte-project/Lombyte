/* Level 02 moby class crane. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6B60), where it is exact; names translated to the US level program. */

extern float scale_time(float) __asm__("FUN_001f96b0");

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

#include "qcopy.h"
/* UpdateMoby_656: steps the moby's animation state from 0/2 (when flagged) and 1/3 (when the timer runs out) */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DDE48), where it is exact; names translated to the US level program. */

extern int FUN_L00_001fefc8(void *);

/* State machine for a moby that picks a new animation each time the previous one finishes. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E05E0), where it is exact; names translated to the US level program. */

extern int FUN_0022da68(int, int, int);

extern short crane_animation_speed_gp __asm__("D_L02_00161C68") __attribute__((sda));
void         crane_update(struct Moby *moby) __asm__("FUN_L02_002df1a8");

/* Moby class 732 */
void crane_update(struct Moby *moby) {
    enum {
        CRANE_ANIM_RESTART_WAIT = 0,
        CRANE_ANIM_TRANSITION_TO_RANDOM_WAIT = 1,
        CRANE_ANIM_WAIT_FOR_RANDOM_TRANSITION = 2,
        CRANE_ANIM_TRANSITION_TO_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT = 3,
        CRANE_ANIM_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT = 4,
        CRANE_ANIM_TRANSITION_BACK_TO_RANDOM_WAIT = 5,
        CRANE_ANIM_TRANSITION_TO_RESTART_WAIT = 7
    };
    int *crane_vars = (int *)moby->pvars;
    moby->anim_speed = *(float *)&crane_animation_speed_gp;
    switch (moby->prev_seq) {
    case CRANE_ANIM_TRANSITION_TO_RANDOM_WAIT:
    case CRANE_ANIM_TRANSITION_TO_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT:
    case CRANE_ANIM_TRANSITION_BACK_TO_RANDOM_WAIT:
    case CRANE_ANIM_TRANSITION_TO_RESTART_WAIT:
        if (moby->anim_flags & 2) {
            int voice_slot_index;
            moby->anim_time = 0;
            if (moby->prev_seq != moby->command) {
                blend_moby_animation(moby, moby->command, 0, 0);
            }
            voice_slot_index = crane_vars[0];
            if (voice_slot_index != -1) {
                char *voice_slot = ((char *)&voice_pool) + voice_slot_index * 0x70;
                if (*(unsigned char **)(voice_slot + 0x88) == moby &&
                    ((unsigned char *)voice_slot)[0x74] != 0) {
                    release_voice_slot(voice_slot_index);
                }
            }
            crane_vars[0] = -1;
            FUN_0022da68(1, 0, (int)moby);
            moby->command = truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        }
        break;
    case CRANE_ANIM_RESTART_WAIT:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
            moby->command = 2;
            crane_vars[0] = FUN_0022da68(0, 4, (int)moby);
        }
        break;
    case CRANE_ANIM_WAIT_FOR_RANDOM_TRANSITION:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (random_integer_below(0xFF) & 1) {
                if (moby->prev_seq != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
                moby->command = 4;
            } else {
                if (moby->prev_seq != 7) {
                    blend_moby_animation(moby, 7, 0, scale_game_frames(10));
                }
                moby->command = 0;
            }
            crane_vars[0] = FUN_0022da68(0, 4, (int)moby);
        }
        break;
    case CRANE_ANIM_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
            moby->command = 2;
            crane_vars[0] = FUN_0022da68(0, 4, (int)moby);
        }
        break;
    }
}

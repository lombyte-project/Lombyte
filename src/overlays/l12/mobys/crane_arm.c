/* Level 12 moby class crane_arm. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

extern int random_integer_below(int);
extern int scale_game_frames(int);

/* Idle critter: waits a random time, then plays one of its fidget animations (or reacts when the hero comes close). */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002ED550), where it is exact; names translated to the US level program. */

extern int FUN_L00_001fefc8(void *);

void crane_arm_update(unsigned char *m) __asm__("FUN_L12_002ec1d0");

void crane_arm_update(unsigned char *moby) {
    enum {
        CRANE_ARM_INITIALIZE = 0,
        CRANE_ARM_WAIT_DELAY = 1,
        CRANE_ARM_WAIT_ANIMATION_FLAG = 2
    };
    enum {
        CRANE_ARM_ANIM_BASE = 0,
        CRANE_ARM_ANIM_DELAY_A = 1,
        CRANE_ARM_ANIM_DELAY_B = 2
    };
    enum {
        CRANE_ARM_ANIM_BASE_TO_DELAY_A = 3,
        CRANE_ARM_ANIM_RETURN_TO_BASE_A = 4,
        CRANE_ARM_ANIM_BASE_TO_DELAY_B = 5,
        CRANE_ARM_ANIM_RETURN_TO_BASE_B = 6
    };
    float hero_distance = vector_distance_xy(moby + 0x10, &hero.motion.pos);
    switch (moby[0x20]) {
    case CRANE_ARM_INITIALIZE:
        if (moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, 0);
        moby[0x20] = 1;
        moby[0xBC] = random_integer_below(scale_game_frames(0xB4));
        break;
    case CRANE_ARM_WAIT_DELAY:
        if (moby[0x52] == 0 && hero_distance < 9.0f && moby[0xBC] < 2)
            moby[0xBC] = 2;
        if (hero_distance < 12.0f && moby[0x52] == moby[0x53] && moby[0x52] != 0)
            moby[0xBC] = 0;
        if (FUN_L00_001fefc8(moby + 0xBC)) {
            switch (moby[0x52]) {
            case CRANE_ARM_ANIM_BASE:
                if (random_integer_below(0x100) & 1) {
                    if (moby[0x53] != 3)
                        blend_moby_animation(moby, 3, 0, 0);
                } else {
                    if (moby[0x53] != 5)
                        blend_moby_animation(moby, 5, 0, 0);
                }
                break;
            case CRANE_ARM_ANIM_DELAY_A:
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, 0);
                break;
            case CRANE_ARM_ANIM_DELAY_B:
                if (moby[0x53] != 6)
                    blend_moby_animation(moby, 6, 0, 0);
                break;
            }
            moby[0x20] = 2;
        }
        break;
    case CRANE_ARM_WAIT_ANIMATION_FLAG:
        if (moby[0x70] & 2) {
            switch (moby[0x52]) {
            case CRANE_ARM_ANIM_BASE_TO_DELAY_A:
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, 0);
                break;
            case CRANE_ARM_ANIM_RETURN_TO_BASE_A:
            case CRANE_ARM_ANIM_RETURN_TO_BASE_B:
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, 0);
                break;
            case CRANE_ARM_ANIM_BASE_TO_DELAY_B:
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, 0);
                break;
            }
            moby[0x20] = 1;
            moby[0xBC] = random_integer_below(scale_game_frames(0xB4));
        }
        break;
    }
}

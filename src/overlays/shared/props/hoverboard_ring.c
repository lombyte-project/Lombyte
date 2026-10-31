/* Shared moby class hoverboard_ring: the ring that sparkles while the hero rides the hoverboard nearby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"

#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031C0F0), where it is exact; names translated to the US level program. */

extern unsigned char *
spawn_mode_controlled_orbiting_trail_particle(float  f0,
                                              float  f1,
                                              int    a0,
                                              void  *pos,
                                              int    timer,
                                              int    flag,
                                              int    col,
                                              float *vel) __asm__("FUN_L00_002743a8");

void hoverboard_ring_update(char *moby) __asm__("FUN_L05_0031abe0");

/* Moby class 1139 */
void hoverboard_ring_update(char *moby) {
    enum {
        HOVERBOARD_RING_STATE_INIT = 0,
        HOVERBOARD_RING_STATE_ACTIVE = 1
    };
    switch (((unsigned char *)moby)[0x20]) {
    case HOVERBOARD_RING_STATE_INIT:
        ((unsigned char *)moby)[0x30] = 0x80;
        MOBY(moby)->flags |= 0x41;
        moby[0x20] = 1;
        break;
    case HOVERBOARD_RING_STATE_ACTIVE: {
        float bounding_sphere[4];
        int   i;
        qcopy(bounding_sphere, moby + 0x10);
        bounding_sphere[3] = 8.0f;
        if (fast_bsphere_check((char *)bounding_sphere, 128.0f) != -1) {
            if (hero.state.control_mode == 0x16) {
                for (i = 0; i < 3; i++) {
                    float first_random_angle = random_angle_radians();
                    float second_random_angle = random_angle_radians();
                    spawn_mode_controlled_orbiting_trail_particle(first_random_angle,
                                                                  second_random_angle,
                                                                  (int)moby,
                                                                  D_L05_0015F580,
                                                                  scale_game_frames(0x28),
                                                                  1,
                                                                  0x7F,
                                                                  0);
                }
            }
        }
        break;
    }
    }
}

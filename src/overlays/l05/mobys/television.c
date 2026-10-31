/* Level 05 moby class television. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"

extern int   D_L05_0015F5C4;
extern float fast_add_rotations(float, float);

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0031A0A8), where it is exact; names translated to the US level program. */

extern short television_scale_multiplier_gp __asm__("D_L05_00161F08") __attribute__((sda));
void         television_update(struct Moby *moby) __asm__("FUN_L05_00318b98");

/* Moby class 984 */
void television_update(struct Moby *moby) {
    enum {
        TELEVISION_INITIALIZE = 0,
        TELEVISION_UPDATE_BOBBING = 1
    };
    float bobbing_phase;
    char *television_bobbing_state = (char *)moby->pvars;
    int   visibility_control_mode;
    moby->scale = moby->pclass->scale * *(float *)&television_scale_multiplier_gp;
    switch (moby->state) {
    case TELEVISION_INITIALIZE:
        moby->state = 1;
        *(float *)television_bobbing_state = random_angle_radians();
        *(float *)(television_bobbing_state + 4) = moby->pos.z;
        break;
    case TELEVISION_UPDATE_BOBBING:
        visibility_control_mode = D_L05_0015F5C4;
        if (visibility_control_mode == 2) {
            moby->visible = 0;
            moby->flags |= 1;
        } else {
            moby->visible = 1;
            moby->flags &= 0xFFFE;
        }
        bobbing_phase =
            fast_add_rotations(*(float *)television_bobbing_state, frame_time * 0.87266463f);
        *(float *)television_bobbing_state = bobbing_phase;
        moby->pos.z = *(float *)(television_bobbing_state + 4) + fast_sin(bobbing_phase);
        break;
    }
}

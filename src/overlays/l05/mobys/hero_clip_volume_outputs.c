/* Level 05 moby class 0x3D6: watches a clip volume and sets the hero clip-volume values while the hero is inside it. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/rendering/clip_volume.h"

#include "rnc/overlay/l05/mobys/hero_clip_volume_outputs_globals.h"

extern int   D_L05_001612D8_macro[] __asm__("D_L05_001612D8") MACRO_ADDR;
extern float D_L05_001612E0_gp[4] __asm__("D_L05_001612E0") MACRO_ADDR __attribute__((sda));
extern float D_L05_001612E8 __asm__("D_L05_001612E8");
extern int   D_L05_0015F5C4_macro[] __asm__("D_L05_0015F5C4") MACRO_ADDR;

/* Two-state marker: arms the moby, then once its target is found copies the
 * target position and sets the bob height from the frame counter. */
void hero_clip_volume_outputs_update(struct Moby *moby) __asm__("FUN_L05_00318a68");

/* Moby class 982 */
void hero_clip_volume_outputs_update(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_CHECK_HERO_VOLUME = 1
    };
    int   state = moby->state;
    int  *clip_volume_index_ptr = ((int *)moby->pvars);
    char *hero_position;

    switch (state) {
    case MOBY_INITIALIZE:
        moby->state = 1;
        moby->flags |= 1;
        moby->update_dist = 0xFF;
        moby->collision = 0;
        break;
    case MOBY_CHECK_HERO_VOLUME:
        hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *clip_volume_index_ptr) != 0) {
            D_L05_001612D8_macro[1] = state;
            qcopy(D_L05_001612E0_gp, hero_position);
            *(short *)(hero_position + 0x15E) = 5;
            hero_clip_volume_output_w = 48.0f;
            *(volatile float *)&D_L05_001612E8 =
                fast_sin((float)(D_L05_0015F5C4_macro[2] % 360) * 0.017444444f - 3.14f) * 0.25f +
                59.5f;
        } else {
            hero_inside_clip_volume = 0;
        }
        break;
    }
}

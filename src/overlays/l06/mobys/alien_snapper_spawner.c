/* Level 06 moby class 0x42a: alien snapper spawner; once the hero enters its clip volume it releases its listed alien snappers one by one. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/l06/mobys/alien_queen.h"

/* Trigger moby: waits for the hero in its zone, plays its open animation, then launches a listed moby. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_002FEE60), where it is exact; names translated to the US level program. */
extern int    D_L06_0015FFD8;
extern int    is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern short *D_L06_001ABFC0[];
extern void   blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
void          alien_snapper_spawner_update(char *moby) __asm__("FUN_L06_002fda30");

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int is_point_inside_clip_volume(void *, int);

/* Moby class 1066 */
void alien_snapper_spawner_update(char *moby) {
    enum {
        WAIT_FOR_HERO_IN_CLIP_VOLUME = 0,
        WAIT_FOR_ANIMATION_SIGNAL = 1,
        ACTIVATE_LISTED_MOBYS_AT_INTERVAL = 2
    };
    int *moby_vars = ((int *)MOBY(moby)->pvars);
    switch (((unsigned char *)moby)[0x20]) {
    case WAIT_FOR_HERO_IN_CLIP_VOLUME:
        if (moby_vars[1] >= 0 && is_point_inside_clip_volume(&hero.motion.pos, moby_vars[1])) {
            moby[0x20] = 1;
            if (((unsigned char *)moby)[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 5);
            }
        }
        break;
    case WAIT_FOR_ANIMATION_SIGNAL:
        if (moby[0x70] & 2) {
            if (moby_vars[0] >= 0) {
                moby[0x20] = 2;
            } else {
                moby[0x20] = 3;
            }
            if (((unsigned char *)moby)[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, 5);
            }
        }
        break;
    case ACTIVATE_LISTED_MOBYS_AT_INTERVAL:
        if (fast_dec_timer(moby_vars + 2)) {
            float  found = 0.0f;
            short *list = D_L06_001ABFC0[moby_vars[0]];
            if (list != 0) {
                short *moby_list_entry = list;
                char  *pos = moby + 0x10;
                do {
                    if (activate_state_15_moby_at_position(
                            D_L06_0015FFD8 + ((*(unsigned short *)moby_list_entry & 0x7FFF) << 8),
                            pos)) {
                        found = 1.0f;
                        break;
                    }
                } while (*moby_list_entry++ >= 0);
            }
            if (found != 0.0f) {
                moby_vars[2] = scale_game_frames(0x14);
            } else {
                moby[0x20] = 3;
            }
        }
        break;
    }
}

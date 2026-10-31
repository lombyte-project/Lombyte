/* Shared moby class 0x25: removes itself when the hero comes near. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/remove_moby_when_near_hero_globals.h"
#include "qcopy.h"

extern int FUN_00116248_b7(void *, void *) __asm__("FUN_00116248");

extern unsigned char l11_proximity_removal_flag[] __asm__("D_0013D389");
extern char          D_L11_00179B18[];
extern char          raritanium_acquired_text[] __asm__("D_L11_0021A900");

void remove_when_near_hero_update(char *moby) __asm__("FUN_L11_002cb790");

/* Moby class 37 */
void remove_when_near_hero_update(char *moby) {
    if (fast_vec_dist(moby + 0x10, &hero.motion.pos) < 2.0f) {
        l11_proximity_removal_flag[0] = 1;
        FUN_00116248_b7(D_L11_00179B18, raritanium_acquired_text);
        proximity_removal_frame_count = scale_game_frames(0xB4);
        mark_moby_for_removal(moby);
    }
}

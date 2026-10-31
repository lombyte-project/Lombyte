/* Level 11 moby class cleared_group_dialogue_trigger (class 1350): plays a dialogue once its moby group is empty. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031BEF0), where it is exact; names translated to the US level program. */

extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

void cleared_group_dialogue_update(struct Moby *moby) __asm__("FUN_L11_0031aa80");

/* Moby class 1350 */
void cleared_group_dialogue_update(struct Moby *moby) {
    int *dialogue_trigger_vars = (int *)moby->pvars;
    if (dialogue_trigger_vars[0] == -1) {
        mark_moby_for_removal(moby);
    } else if (dialogue_trigger_vars[1] != -1 &&
               count_active_moby_group_members(dialogue_trigger_vars[1], -1) == 0) {
        unsigned char *dialogue_latch_base = item_unlocked;
        if (dialogue_latch_base[0x13] == 0) {
            dialogue_latch_base[0x13] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
    }
}

#include "types.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/tick_space_transition_globals.h"
void tick_space_transition(void) __asm__("FUN_00226e08");
void tick_space_transition(void) {
    s32 new_value;
    s32 old_value;
    s32 fifty;

    old_value = D_0015EE24;
    if (counter_advance_disabled == 0) {
        fifty = 50;
        new_value = old_value + 1;
        D_0015EE24 = new_value;
        if (((new_value % fifty) == 0) && (pal_mode != 0)) {
            D_0015EE24 = old_value + 0xB;
        }
    }
}

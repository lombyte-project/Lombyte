/* Mobys: links and updates every moby. */
#include "types.h"
#include "asm.h"

#include "rnc/mobys/moby.h"
#include "moby_update.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/level_labels.h"

extern struct Moby *suck_moby_list __asm__("D_L00_0015FFE4");
extern void        *active_mobies[] __asm__("D_L00_001ABA00");
extern void         FUN_L00_00250480(struct Moby *moby);

void link_and_update_all_mobys(void) __asm__("FUN_L00_00264740");

void link_and_update_all_mobys(void) {
    struct Moby *moby;
    struct Moby *last;
    struct Moby *current;

    active_mobies[0] = 0;
    suck_moby_list = 0;
    moby = (struct Moby *)moby_table;
    last = 0;
    while (moby->state != 0xFF) {
        if (!(moby->state & 0x80) && !(moby->flags & 2)) {
            if (last != 0) {
                last->next = moby;
            } else {
                suck_moby_list = moby;
            }
            last = moby;
        }
        moby++;
    }
    if (last != 0) {
        last->next = 0;
    }

    current = suck_moby_list;
    while (current != 0) {
        if ((s8)current->state >= 0) {
            if (!(current->flags & 0x40)) {
                FUN_L00_00250480(current);
            }
            MOBY_CALL_UPDATE(current->update, current);
            if (!(((volatile struct Moby *)current)->flags & 4)) {
                update_entity(current);
            }
        }
        current = current->next;
    }
}

#include "types.h"
#include "rnc/mobys/moby.h"
#include "moby_update.h"
#include "rnc/mobys/refresh_moby_spatial_bounds.h"

/* Moby.flags bits this loop tests. */
#define MOBY_SKIP_ANIMATION 0x40
#define MOBY_SKIP_BOUNDS    0x4

extern struct Moby *visible_moby_list __asm__("D_0015FF24") __attribute__((sda));
extern struct Moby *build_resident_visibility_list(void) __asm__("FUN_0020d868");
extern void         advance_moby_animation(struct Moby *) __asm__("FUN_0020d580");
void                update_visible_mobys(void) __asm__("FUN_00212e28");

/* Each live moby on the visible list: its animation step, its class update
 * and its bounds. Animation can replace the update; the update and the
 * bounds refresh can change flags or the list link, so each field is read at
 * its point of use. */
void update_visible_mobys(void) {
    struct Moby *moby;

    visible_moby_list = build_resident_visibility_list();
    moby = visible_moby_list;
    while (moby != 0) {
        if ((s8)moby->state >= 0) {
            if (!(moby->flags & MOBY_SKIP_ANIMATION)) {
                advance_moby_animation(moby);
            }
            MOBY_CALL_UPDATE(moby->update, moby);
            if (!(((volatile struct Moby *)moby)->flags & MOBY_SKIP_BOUNDS)) {
                refresh_moby_spatial_bounds(moby);
            }
        }
        moby = moby->next;
    }
}

extern __typeof__(update_visible_mobys) func_00212E28 __attribute__((alias("FUN_00212e28")));

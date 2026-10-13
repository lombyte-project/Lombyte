#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/gameplay/entities/update_visible_resident_objects/FUN_00212e28.s", FUN_00212e28);
#else
#include "types.h"
#include "rnc/gameplay/entities/moby.h"

/* Moby.flags bits this loop tests. */
#define MOBY_SKIP_ANIMATION 0x40
#define MOBY_SKIP_BOUNDS 0x4

extern struct Moby *visible_moby_list __asm__("D_0015FF24") __attribute__((sda));
extern struct Moby *build_resident_visibility_list(void) __asm__("FUN_0020d868");
extern void advance_resident_object_animation(struct Moby *) __asm__("FUN_0020d580");
extern void refresh_resident_object_spatial_bounds(struct Moby *) __asm__("FUN_0020def8");
void update_visible_resident_objects(void) __asm__("FUN_00212e28");

/* Each live moby on the visible list: its animation step, its class update
 * and its bounds. Animation can replace the update; the update and the
 * bounds refresh can change flags or the list link, so each field is read at
 * its point of use. */
void update_visible_resident_objects(void) {
    struct Moby *moby;
    void (*callback)(struct Moby *);

    visible_moby_list = build_resident_visibility_list();
    moby = visible_moby_list;
    while (moby != 0) {
        if ((s8)moby->state >= 0) {
            if (!(((volatile struct Moby *)moby)->flags & MOBY_SKIP_ANIMATION)) {
                advance_resident_object_animation(moby);
            }
            callback = moby->update;
            if (callback != 0) {
                callback(moby);
            }
            if (!(((volatile struct Moby *)moby)->flags & MOBY_SKIP_BOUNDS)) {
                refresh_resident_object_spatial_bounds(moby);
            }
        }
        moby = moby->next;
    }
}

extern __typeof__(update_visible_resident_objects) func_00212E28
    __attribute__((alias("FUN_00212e28")));

#endif /* NON_MATCHING */

#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00212e28/FUN_00212e28.s", FUN_00212e28);
#else
#include "types.h"
struct Moby {
    u8 pad0[0x20];
    s8 state;
    u8 pad21[7];
    struct Moby *next;
    u8 pad2C[8];
    volatile u16 flags;
    u8 pad36[0x3E];
    void (*update_callback)(struct Moby *);
};
extern struct Moby *visible_moby_list __asm__("D_0015FF24");
extern struct Moby *build_resident_visibility_list(void) __asm__("func_0020D868");
extern void advance_resident_object_animation(struct Moby *) __asm__("func_0020D580");
extern void refresh_resident_object_spatial_bounds(struct Moby *) __asm__("func_0020DEF8");
void update_visible_resident_objects(void) __asm__("FUN_00212e28");

/* Animation can replace the callback; the callback and bounds refresh can
 * mutate flags or the list link. Read each field at its point of use. */
void update_visible_resident_objects(void) {
    struct Moby *moby;
    void (*callback)(struct Moby *);

    moby = visible_moby_list = build_resident_visibility_list();
    while (moby != 0) {
        if (moby->state >= 0) {
            if (!(moby->flags & 0x40)) {
                advance_resident_object_animation(moby);
            }
            callback = moby->update_callback;
            if (callback != 0) {
                callback(moby);
            }
            if (!(moby->flags & 0x4)) {
                refresh_resident_object_spatial_bounds(moby);
            }
        }
        moby = moby->next;
    }
}

extern __typeof__(update_visible_resident_objects) func_00212E28
    __attribute__((alias("FUN_00212e28")));

#endif /* NON_MATCHING */

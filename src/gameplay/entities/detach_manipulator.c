#include "types.h"
struct Manip {
    u8 pad0[8];
    struct Manip *next;
};
#include "rnc/gameplay/entities/moby.h"
extern void fill_transfer_words(void *, s32, s32) __asm__("func_001F97E8");
void detach_manipulator(struct Moby *moby, struct Manip *manip) __asm__("FUN_0020cb88");

void detach_manipulator(struct Moby *moby, struct Manip *manip) {
    struct Manip *p;

    if (manip == 0) {
        return;
    }
    if (moby->manips == manip) {
        moby->manips = manip->next;
    } else {
        p = moby->manips;
        while (p->next != 0 && p->next != manip) {
            p = p->next;
        }
        if (p->next == manip) {
            p->next = manip->next;
        }
    }
    fill_transfer_words(manip, 0, 0x40);
}

extern __typeof__(detach_manipulator) func_0020CB88 __attribute__((alias("FUN_0020cb88")));

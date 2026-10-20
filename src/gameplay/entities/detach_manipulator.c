#include "types.h"
struct Manip {
    u8 pad0[8];
    struct Manip *next;
};
#include "rnc/gameplay/entities/moby.h"
extern void fill_transfer_words(void *, s32, s32) __asm__("func_001F97E8");
void detach_manipulator(struct Moby *moby, struct Manip *manip) __asm__("FUN_0020cb88");

void detach_manipulator(struct Moby *moby, struct Manip *manip) {
    struct Manip *previous_manip;

    if (manip == 0) {
        return;
    }
    if (moby->manips == manip) {
        moby->manips = manip->next;
    } else {
        previous_manip = moby->manips;
        while (previous_manip->next != 0 && previous_manip->next != manip) {
            previous_manip = previous_manip->next;
        }
        if (previous_manip->next == manip) {
            previous_manip->next = manip->next;
        }
    }
    fill_transfer_words(manip, 0, 0x40);
}

extern __typeof__(detach_manipulator) func_0020CB88 __attribute__((alias("FUN_0020cb88")));

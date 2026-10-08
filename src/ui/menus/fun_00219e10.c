#include "types.h"
#include "rnc/ui/menus/menu_system.h"

/* Pause list handler (func_0022DA68's family): one `char *` pad local per
   block. The cursor at menu+0x3C moves on 0x1000/0x4000 within
   menu+0x40, with a notify on change; then the scroll (menu+0x60) is
   clamped around the cursor and the bar position at menu+0x5C derived
   from it, or pinned to 0x60. The re-reads of 0x3C and 0x60 are how
   retail's reloads come out; the page size q - 2 is formed after the
   clamp, and each arm stores the bar position itself. The page size is a
   literal 0x260, so gcc keeps the divide-by-zero trap retail has. The
   switch target has no early return: "else if" instead, which is what
   gives retail's bnel with the store in the delay slot. */
extern u8 D_0013C940[];
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");

int FUN_00219e10(char *menu) {
    {
        char *pad = D_0013C940;
        if (*(int *)(pad + 0x1C4) & 0xD00) {
            if (menu_system.close_locked == 0) {
                return 1;
            }
        }
    }
    {
        char *pad2 = D_0013C940;
        if (*(int *)(pad2 + 0x1C4) & 0x10) {
            struct MenuSystem *g = &menu_system;
            struct MenuPage *t = g->current->back;
            if (t != 0) {
                g->next = t;
            } else if (g->close_locked == 0) {
                return -1;
            }
        }
    }
    {
        char *pad3 = D_0013C940;
        int v = *(int *)(pad3 + 0x1C4);
        int old = *(int *)(menu + 0x3C);

        if ((v & 0x1000) && old != 0) {
            *(int *)(menu + 0x3C) = old - 1;
        }
        if (v & 0x4000) {
            int c = *(int *)(menu + 0x3C) + 1;
            if (c < *(int *)(menu + 0x40)) {
                *(int *)(menu + 0x3C) = c;
            }
        }
        if (*(int *)(menu + 0x3C) != old) {
            allocate_voice_for_target_entry(1, 0x11, *(int *)(menu + 0x14));
        }
    }
    {
        int t = *(int *)(menu + 0x24) * 16;

        if (t / 0x260 >= *(int *)(menu + 0x40)) {
            *(int *)(menu + 0x5C) = 0x60;
        } else {
            int q = (t - 0x28) / 0x260;
            int n;

            int c = *(volatile int *)(menu + 0x3C);
            if (*(int *)(menu + 0x60) >= c) {
                *(int *)(menu + 0x60) = c - 1;
                if (*(int *)(menu + 0x60) < 0) {
                    *(int *)(menu + 0x60) = 0;
                }
            }
            n = q - 2;
            if (*(int *)(menu + 0x60) < *(int *)(menu + 0x3C) - n) {
                *(int *)(menu + 0x60) = *(int *)(menu + 0x3C) - n;
            }
            *(int *)(menu + 0x5C) = 0x190 - *(int *)(menu + 0x60) * 0x260;
        }
    }
    return 0;
}

extern __typeof__(FUN_00219e10) func_00219E10 __attribute__((alias("FUN_00219e10")));

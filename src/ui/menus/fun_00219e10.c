#include "types.h"

/* Pause list handler (func_0022DA68's family): one `char *` pad local per
   block. The cursor at arg0+0x3C moves on 0x1000/0x4000 within
   arg0+0x40, with a notify on change; then the scroll (arg0+0x60) is
   clamped around the cursor and the bar position at arg0+0x5C derived
   from it, or pinned to 0x60. The re-reads of 0x3C and 0x60 are how
   retail's reloads come out; the page size q - 2 is formed after the
   clamp, and each arm stores the bar position itself. The page size is a
   literal 0x260, so gcc keeps the divide-by-zero trap retail has. The
   switch target has no early return: "else if" instead, which is what
   gives retail's bnel with the store in the delay slot. */
extern u8 D_0013C940[];
/* The array spelling matters: a scalar extern lets the assembler relax the
   load to %gp_rel(D_001D5D14)(gp); retail loads it through lui/lw. */
extern s32 D_001D5D14[];
extern u8 D_001D5BF0[];
extern void func_0022DA68(s32, s32, s32);

int FUN_00219e10(char *arg0) {
    {
        char *pad = D_0013C940;
        if (*(int *)(pad + 0x1C4) & 0xD00) {
            if (D_001D5D14[0] == 0) {
                return 1;
            }
        }
    }
    {
        char *pad2 = D_0013C940;
        if (*(int *)(pad2 + 0x1C4) & 0x10) {
            char *g = D_001D5BF0;
            int t = *(int *)(*(char **)(g + 4) + 0x38);
            if (t != 0) {
                *(int *)(g + 8) = t;
            } else if (*(int *)(g + 0x124) == 0) {
                return -1;
            }
        }
    }
    {
        char *pad3 = D_0013C940;
        int v = *(int *)(pad3 + 0x1C4);
        int old = *(int *)(arg0 + 0x3C);

        if ((v & 0x1000) && old != 0) {
            *(int *)(arg0 + 0x3C) = old - 1;
        }
        if (v & 0x4000) {
            int c = *(int *)(arg0 + 0x3C) + 1;
            if (c < *(int *)(arg0 + 0x40)) {
                *(int *)(arg0 + 0x3C) = c;
            }
        }
        if (*(int *)(arg0 + 0x3C) != old) {
            func_0022DA68(1, 0x11, *(int *)(arg0 + 0x14));
        }
    }
    {
        int t = *(int *)(arg0 + 0x24) * 16;

        if (t / 0x260 >= *(int *)(arg0 + 0x40)) {
            *(int *)(arg0 + 0x5C) = 0x60;
        } else {
            int q = (t - 0x28) / 0x260;
            int n;

            int c = *(volatile int *)(arg0 + 0x3C);
            if (*(int *)(arg0 + 0x60) >= c) {
                *(int *)(arg0 + 0x60) = c - 1;
                if (*(int *)(arg0 + 0x60) < 0) {
                    *(int *)(arg0 + 0x60) = 0;
                }
            }
            n = q - 2;
            if (*(int *)(arg0 + 0x60) < *(int *)(arg0 + 0x3C) - n) {
                *(int *)(arg0 + 0x60) = *(int *)(arg0 + 0x3C) - n;
            }
            *(int *)(arg0 + 0x5C) = 0x190 - *(int *)(arg0 + 0x60) * 0x260;
        }
    }
    return 0;
}

extern __typeof__(FUN_00219e10) func_00219E10 __attribute__((alias("FUN_00219e10")));

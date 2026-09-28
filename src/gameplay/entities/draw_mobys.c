#include "types.h"
extern s32 D_0015FF14;
extern u8 D_001E8400[];
extern s32 D_0018A2D8[];
extern s32 DebugPrint();
extern s32 init_moby_class_dists() __asm__("FUN_0020d1f0");
extern void func_0020D278();
extern s32 draw_mobys_clean_up() __asm__("FUN_0020d3b0");
extern s32 func_00211808();
void draw_mobys(void) __asm__("FUN_0020d460");

void draw_mobys(void) {
    register s32 flag;
    s32 *new_var;

    func_0020D278();
    flag = D_0018A2D8[0];
    if (flag != 0) {
        init_moby_class_dists();
        new_var = (s32 *)0x160F00;
        D_0015FF14 = func_00211808(*(s32 *)0x15FF18, *(s32 *)0x15FF14, -1, 1);
        if (*new_var > *(s32 *)0x160F08) {
            DebugPrint(D_001E8400);
        }
    }
    draw_mobys_clean_up();
}

extern void func_0020D460(void) __attribute__((alias("FUN_0020d460")));

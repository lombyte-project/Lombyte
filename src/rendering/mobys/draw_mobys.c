#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/mobys/init_moby_class_dists.h"
#include "rnc/rendering/mobys/draw_mobys_clean_up.h"
#include "rnc/sdk/debug.h"

extern const u8 vu_chain_overflow_message[] __asm__("D_001E8400");
extern s32      D_0018A2D8[];
extern void     draw_mobys_setup() __asm__("func_0020D278");
extern s32      moby_proc() __asm__("func_00211808");
void            draw_mobys(void) __asm__("FUN_0020d460");

void draw_mobys(void) {
    register s32 flag;
    s32         *new_var;

    draw_mobys_setup();
    flag = D_0018A2D8[0];
    if (flag != 0) {
        init_moby_class_dists();
        new_var = (s32 *)0x160F00;
        moby_anim_head = moby_proc(*(s32 *)0x15FF18, *(s32 *)0x15FF14, -1, 1);
        if (*new_var > *(s32 *)0x160F08) {
            DebugPrint(vu_chain_overflow_message);
        }
    }
    draw_mobys_clean_up();
}

extern void func_0020D460(void) __attribute__((alias("FUN_0020d460")));

const u8 vu_chain_overflow_message[] __asm__("D_001E8400") = "vu chain overflow - mobys dropped\n";

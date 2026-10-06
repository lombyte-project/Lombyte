#include "types.h"
extern s32 D_0015FF14;
extern s32 func_00118A80();
extern void stash_moby_class_dists(void) __asm__("func_0020D218");
extern s32 restore_moby_class_dists() __asm__("func_0020D248");
extern s32 func_00211808();
extern s32 vu1_add_g_sregister() __asm__("func_00233980");

void draw_moby_list(s32 arg0, s32 arg1) __asm__("FUN_0020d330");

void draw_moby_list(s32 arg0, s32 arg1) {
    vu1_add_g_sregister(0x47, (long)0x5360B);
    func_00118A80(0);
    restore_moby_class_dists();
    D_0015FF14 = func_00211808(arg0, D_0015FF14, arg1, 0);
    stash_moby_class_dists();
    D_0015FF14 -= 0x10;
}

extern __typeof__(draw_moby_list) func_0020D330 __attribute__((alias("FUN_0020d330")));

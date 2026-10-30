#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

extern s32  flush_cache() __asm__("func_00118A80");
extern void stash_moby_class_dists(void) __asm__("func_0020D218");
extern s32  restore_moby_class_dists() __asm__("func_0020D248");
extern s32  moby_proc() __asm__("func_00211808");
extern s32  vu1_add_g_sregister() __asm__("func_00233980");

void draw_moby_list(s32 source_address, s32 arg1) __asm__("FUN_0020d330");

void draw_moby_list(s32 source_address, s32 arg1) {
    vu1_add_g_sregister(0x47, (long)0x5360B);
    flush_cache(0);
    restore_moby_class_dists();
    moby_anim_head = moby_proc(source_address, moby_anim_head, arg1, 0);
    stash_moby_class_dists();
    moby_anim_head -= 0x10;
}

extern __typeof__(draw_moby_list) func_0020D330 __attribute__((alias("FUN_0020d330")));

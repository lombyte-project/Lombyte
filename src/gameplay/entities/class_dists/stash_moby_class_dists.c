#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
extern s32 fast_mem_copy() __asm__("FUN_001f98d0");
void stash_moby_class_dists(void) __asm__("FUN_0020d218");

void stash_moby_class_dists(void) {
    fast_mem_copy(moby_backup_buffer, 0x70003A00, 0x380);
}

extern void func_0020D218(void) __attribute__((alias("FUN_0020d218")));

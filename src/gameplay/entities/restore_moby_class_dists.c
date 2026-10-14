#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
extern s32 FUN_001f98d0();
void restore_moby_class_dists(void) __asm__("FUN_0020d248");

void restore_moby_class_dists(void) {
    FUN_001f98d0(0x70003A00, moby_backup_buffer, 0x380);
}

extern void func_0020D248(void) __attribute__((alias("FUN_0020d248")));

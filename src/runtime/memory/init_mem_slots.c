#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

void init_mem_slots(void) __asm__("FUN_002015d8");

void init_mem_slots(void) {
    int base = (int)D_24135F & 0xFFFFC000;
    int size = current_vu_chain;
    int level_zero_workspace_base = base + size;
    int memory_slot_three_base = level_zero_workspace_base + size;
    int memory_slot_four_base = memory_slot_three_base + 0x64000;
    int common_archive_base = memory_slot_four_base + 0x30000;
    D_001940C0[0] = base;
    D_001940C0[5] = common_archive_base;
    D_001940C0[6] = common_archive_base;
    D_001940C0[8] = 0x7000000;
    D_001940C0[9] = 0x7100000;
    D_001940C0[1] = base;
    D_001940C0[2] = level_zero_workspace_base;
    D_001940C0[3] = memory_slot_three_base;
    D_001940C0[4] = memory_slot_four_base;
    D_001940C0[10] = 0x7200000;
}

extern __typeof__(init_mem_slots) func_002015D8 __attribute__((alias("FUN_002015d8")));

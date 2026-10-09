#include "types.h"
#include "sda.h"
#include "rnc/storage/disc_table.h"

extern u8 D_001AABC0[];
extern u64 D_0015EEC8 MACRO_ADDR;
extern void load_pif_as_psmt8_h() __asm__("func_001E9168");
extern s32 load() __asm__("func_00216828");

void load_debug_font(void) __asm__("FUN_001e9338");

void load_debug_font(void) {
    u8 sp_slot[32];

    load(D_001AABC0, disc_table.debug_font.sector, disc_table.debug_font.size);
    load_pif_as_psmt8_h(D_001AABC0, sp_slot, *(s32 *)0x15EE88 + 0xC0000, 0x3FFC00);
    D_0015EEC8 = *(u64 *)(void *)sp_slot;
}

/* Defined below its only writer, so retail reaches it with lui. */
u64 D_0015EEC8 MACRO_ADDR = 0;

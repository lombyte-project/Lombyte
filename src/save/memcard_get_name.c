#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/save/memory_card_state.h"

extern char *strncpy(char *, const char *, u32);
void         memcard_get_name(u8 *src) __asm__("FUN_00209030");

void memcard_get_name(u8 *rom_configuration) {
    s32 i;

    if (rom_configuration[0x12] == 'E') {
        D_0013D1D0[2] = rom_configuration[0x12];
    }
    for (i = 3; i < 7; i++) {
        D_0013D1D0[i] = rom_configuration[i + 0xD];
    }
    for (i = 8; i < 11; i++) {
        D_0013D1D0[i] = rom_configuration[i + 0xD];
    }
    for (i = 11; i < 13; i++) {
        D_0013D1D0[i] = rom_configuration[i + 0xE];
    }
    strncpy(D_0013D1E8, D_0013D1D0, 13);
    strncpy(D_0013D200, D_0013D1D0, 13);
    strncpy(D_0013D220, D_0013D1D0, 13);
    strncpy(D_0013D240, D_0013D1D0, 13);
    strncpy(D_0013D240 + 0x14, D_0013D1D0, 13);
    strncpy(D_0013D270, D_0013D1D0, 13);
}

extern __typeof__(memcard_get_name) func_00209030 __attribute__((alias("FUN_00209030")));

/* The save directory and file paths; memcard_get_name writes the disc's
   region and product code over the '*'s of each. */
char D_0013D1D0[] = "/BA****-*****RATCHET";
char D_0013D1E8[] = "/************RATCHET/*";
char D_0013D200[] = "/************RATCHET/icon.sys";
char D_0013D220[] = "/************RATCHET/static.ico";
char D_0013D240[] = "/************RATCHET/************RATCHET";
char D_0013D270[] = "/************RATCHET/save%d.bin";

struct MemoryCardState memory_card_state = {{{0, 0, 0, 0, 0, -3, 0, 0, {0}, 0, 0, 0}}, 0, 0, -1, 0, 0, 1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 1};

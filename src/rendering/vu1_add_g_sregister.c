#include "types.h"
#include "sda.h"

extern s32 *D_00160F00[4] MACRO_ADDR; /* packet cursor, reloaded per store */
extern s32 *D_00160F00_store;

#define PACKET_CURSOR D_00160F00[0]

void vu1_add_g_sregister(s32 register_id, s64 value) __asm__("FUN_00233980");

void vu1_add_g_sregister(s32 register_id, s64 value) {
    PACKET_CURSOR[0] = 0x10000002;
    PACKET_CURSOR[1] = 0;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000002;
    PACKET_CURSOR[4] = 0x8001;
    PACKET_CURSOR[5] = 0x10000000;
    PACKET_CURSOR[6] = 14;
    PACKET_CURSOR[7] = 0;
    *(volatile s64 *)(PACKET_CURSOR + 8) = value;
    PACKET_CURSOR[10] = register_id;
    PACKET_CURSOR[11] = 0;
    D_00160F00_store = PACKET_CURSOR + 12;
}

extern s32 emit_gs_register_write(s32 register_id, s64 value)
    __attribute__((alias("FUN_00233980")));

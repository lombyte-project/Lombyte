#include "types.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/data_labels.h"

extern s32 sceCdGetError();
extern s32 sceCdRead();
extern s32 sceCdSync();
extern s32 sceGsSyncV();

s32 load_disc_sectors_into_global_buffer(void) __asm__("FUN_0012f2b8");

s32 load_disc_sectors_into_global_buffer(void) {
    u8  disc_sector_buffer[0x3000];
    u8  mode[4];
    u32 i;

    mode[0] = 0x20;
    mode[1] = D_0015ED58;
    mode[2] = 0;
    mode[3] = 0;
    do {
        sceCdRead(0x5DC, 6, disc_sector_buffer, mode);
        while (sceCdSync(1) != 0) {
            sceGsSyncV(0);
        }
    } while (sceCdGetError() != 0);
    for (i = 0; i < 0x2960; i++) {
        ((u8 *)&disc_table)[i] = disc_sector_buffer[i];
    }
    return 1;
}

extern __typeof__(load_disc_sectors_into_global_buffer) func_0012F2B8
    __attribute__((alias("FUN_0012f2b8")));

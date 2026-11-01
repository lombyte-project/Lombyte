#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/graph.h"
#include "rnc/sdk/storage/cdvd.h"

s32 wad_get_sectors(s32 lsn, s32 sector_count, s32 dst) __asm__("FUN_0012f208");

s32 wad_get_sectors(s32 lsn, s32 sector_count, s32 sector_read_destination) {
    u8  read_mode[0x8];
    s32 shift = sector_count << 0xB;
    read_mode[0] = 0x20;
    read_mode[1] = D_0015ED58;
    read_mode[2] = 0;
    read_mode[3] = 0;
loop_1:
    sceCdRead(lsn, sector_count, sector_read_destination, read_mode);
    goto loop_3;
block_2:
    sceGsSyncV(0);
loop_3:
    if (sceCdSync(1) != 0) {
        goto block_2;
    }
    if (sceCdGetError() != 0) {
        goto loop_1;
    }
    return shift;
}

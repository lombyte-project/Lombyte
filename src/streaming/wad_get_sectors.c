#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/graph/sce_gs_sync_v.h"
#include "rnc/sdk/storage/cdvd/sceCdGetError.h"
#include "rnc/sdk/storage/cdvd/sceCdRead.h"
#include "rnc/sdk/storage/cdvd/scecdsync.h"

s32 wad_get_sectors(s32 lsn, s32 sector_count, s32 dst) __asm__("FUN_0012f208");

s32 wad_get_sectors(s32 lsn, s32 sector_count, s32 sector_read_destination) {
    u8  sp_slot[0x8];
    s32 shift = sector_count << 0xB;
    sp_slot[0] = 0x20;
    sp_slot[1] = D_0015ED58;
    sp_slot[2] = 0;
    sp_slot[3] = 0;
loop_1:
    sceCdRead(lsn, sector_count, sector_read_destination, sp_slot);
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

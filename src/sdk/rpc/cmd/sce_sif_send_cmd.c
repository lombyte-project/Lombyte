#include "types.h"

typedef struct {
    u32 size  : 8;
    u32 dsize : 24;
    u32 addr;
    u32 id;
} SifCmd;

typedef struct {
    u32 data;
    u32 addr;
    u32 size;
    u32 mode;
} SifDma;

extern u32  sif_command_dma_destination[] __asm__("D_00154E60");
extern void sceSifWriteBackDCache(void *addr, s32 size);
extern s32  sceSifSetDma();
extern s32  isceSifSetDma();
s32         _sceSifSendCmd(u32     packet,
                           s32     mode,
                           SifCmd *cmd,
                           s32     size,
                           u32     extra_data_source,
                           u32     extra_data_destination,

                           s32 dst_size) {
    SifDma       tags[2];
    s32          num;
    register u32 mode44;
    register u32 addr0;

    if ((u32)(size - 0x10) >= 0x61) {
        return 0;
    }
    num = 0;
    if (dst_size > 0) {
        cmd->dsize = dst_size;
        tags[0].data = extra_data_source;
        tags[0].addr = extra_data_destination;
        cmd->addr = extra_data_destination;
        tags[0].size = dst_size;
        tags[0].mode = 0;
        num = 1;
        if (mode & 4) {
            sceSifWriteBackDCache(extra_data_source, dst_size);
        }
    } else {
        cmd->addr = 0;
        cmd->dsize = 0;
    }
    addr0 = sif_command_dma_destination[0];
    tags[num].data = (u32)cmd;
    tags[num].addr = addr0;
    tags[num].size = size;
    cmd->size = size;
    cmd->id = packet;
    tags[num].mode = 0x44;
    sceSifWriteBackDCache(cmd, size);
    num++;
    if (mode & 1) {
        return isceSifSetDma(tags, num);
    }
    return sceSifSetDma(tags, num);
}

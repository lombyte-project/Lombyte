#include "types.h"
#include "rnc/sdk/storage/cdvd.h"

struct CdStream {
    u8  pad_0[0x4];
    s32 sector; /* next sector to read; advanced after a synchronous read */
};

s32 read_cd_stream_sectors(struct CdStream *stream,
                           u32              dst,
                           s32              size,
                           s32              is_async) __asm__("FUN_0023ba60");

s32 read_cd_stream_sectors(struct CdStream *stream,
                           u32              sector_read_destination,
                           s32              requested_byte_count,

                           s32 is_async) {
    u8  readcmd[0x10];
    s32 blocks;
    s32 returned_byte_count;

    blocks = requested_byte_count >> 11;
    returned_byte_count = 0;
    readcmd[0] = 0x64;
    readcmd[1] = 0;
    readcmd[2] = 0;
    sceCdRead(stream->sector, blocks, sector_read_destination, readcmd);
    if (is_async == 0) {
        stream->sector += blocks;
        sceCdSync(0);
        returned_byte_count = requested_byte_count;
    }
    return returned_byte_count;
}

extern __typeof__(read_cd_stream_sectors) func_0023BA60 __attribute__((alias("FUN_0023ba60")));

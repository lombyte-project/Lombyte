#include "types.h"

struct CdStream {
    u8 pad_0[0x4];
    s32 unk4;
};

extern s32 sceCdRead(s32, s32, s32, u8 *);
extern s32 sceCdSync(s32);

s32 read_cd_stream_sectors(struct CdStream *stream, u32 dst, s32 size,
                           s32 is_async) __asm__("FUN_0023ba60");

s32 read_cd_stream_sectors(struct CdStream *stream, u32 dst, s32 size, s32 is_async) {
    u8 readcmd[0x10];
    s32 blocks;
    s32 result;

    blocks = size >> 11;
    result = 0;
    readcmd[0] = 0x64;
    readcmd[1] = 0;
    readcmd[2] = 0;
    sceCdRead(stream->unk4, blocks, dst, readcmd);
    if (is_async == 0) {
        stream->unk4 += blocks;
        sceCdSync(0);
        result = size;
    }
    return result;
}

extern __typeof__(read_cd_stream_sectors) func_0023BA60 __attribute__((alias("FUN_0023ba60")));

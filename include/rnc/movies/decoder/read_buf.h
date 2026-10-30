#ifndef LOMBYTE_RNC_MOVIES_DECODER_READ_BUF_H
#define LOMBYTE_RNC_MOVIES_DECODER_READ_BUF_H

#include "types.h"

#define READ_BUF_SIZE 0x50000

/* Movie read ring (type name from readBufCreate__FP7ReadBuf): CD sectors
   are put at `put`, the demuxer gets the oldest `count` bytes.
   init_all reserves through context+0x50040; the bytes past `size` are
   not proven to be ReadBuf fields. */
struct ReadBuf {
    u8  data[READ_BUF_SIZE];
    s32 put;   /* write offset, modulo size */
    s32 count; /* bytes put and not yet got */
    s32 size;  /* READ_BUF_SIZE */
};

void read_buf_create(struct ReadBuf *buf) __asm__("func_0023B940");
void read_buf_delete(struct ReadBuf *buf) __asm__("func_0023B958");
s32  read_buf_begin_put(struct ReadBuf *buf, u8 **ptr) __asm__("func_0023B960");
void read_buf_end_put(struct ReadBuf *buf, s32 size) __asm__("FUN_0023b990");
s32  read_buf_begin_get(struct ReadBuf *buf, u8 **ptr) __asm__("FUN_0023b9d8");
s32  read_buf_end_get(struct ReadBuf *buf, s32 size) __asm__("GetStateTableWord");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_READ_BUF_H */

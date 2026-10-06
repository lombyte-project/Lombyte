#ifndef LOMBYTE_RNC_SDK_LIBRARY_STDIO_H
#define LOMBYTE_RNC_SDK_LIBRARY_STDIO_H

#include "types.h"

/* SDK/newlib FILE ABI. Evidence: exact __sinit uses a 0x58 stride;
 * __sread/__sseek access flags@0xc, signed fd@0xe, offset@0x50,
 * reentrancy@0x54. __smakebuf and __sfvwrite oracles establish the
 * buffer, callback and unbuffered-byte offsets below.
 * Only the used prefix of the reentrancy object is described. */
typedef struct RncReent RncReent;
typedef struct RncFile RncFile;

typedef struct RncStdioBuffer {
    u8 *_base;
    s32 _size;
} RncStdioBuffer;

struct RncReent {
    u8 reserved[0x38];
    s32 __sdidinit;
    void (*__cleanup)(RncReent *);
};

struct RncFile {
    u8 *_p;
    s32 _r;
    s32 _w;
    u16 _flags;
    s16 _file;
    RncStdioBuffer _bf;
    s32 _lbfsize;
    void *_cookie;
    s32 (*_read)(void *, char *, s32);
    s32 (*_write)(void *, const char *, s32);
    s32 (*_seek)(void *, s32, s32);
    s32 (*_close)(void *);
    RncStdioBuffer _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    RncStdioBuffer _lb;
    s32 _blksize;
    s32 _offset;
    RncReent *_data;
};

typedef struct RncStdioIov {
    const void *iov_base;
    u32 iov_len;
} RncStdioIov;

typedef struct RncStdioUio {
    RncStdioIov *uio_iov;
    s32 uio_iovcnt;
    u32 uio_resid;
} RncStdioUio;

#endif /* LOMBYTE_RNC_SDK_LIBRARY_STDIO_H */

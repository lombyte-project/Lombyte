#ifndef LOMBYTE_RNC_SDK_LIBRARY_STDIO_H
#define LOMBYTE_RNC_SDK_LIBRARY_STDIO_H

#include "types.h"
#include "sda.h"

/* SDK/newlib FILE ABI. Evidence: exact __sinit uses a 0x58 stride;
 * __sread/__sseek access flags@0xc, signed fd@0xe, offset@0x50,
 * reentrancy@0x54. __smakebuf and __sfvwrite oracles establish the
 * buffer, callback and unbuffered-byte offsets below.
 * Only the used fields of the reentrancy object are described: rand
 * keeps its seed at 0x58 and exit walks the atexit list at 0x148. */
typedef struct RncReent RncReent;
typedef struct RncFile  RncFile;
struct _atexit;

typedef struct RncStdioBuffer {
    u8 *_base;
    s32 _size;
} RncStdioBuffer;

struct RncReent {
    s32      _errno;
    RncFile *_stdin;
    RncFile *_stdout;
    RncFile *_stderr;
    u8       reserved0[0x38 - 0x10];
    s32      __sdidinit;
    void (*__cleanup)(RncReent *);
    u8              reserved1[0x58 - 0x40];
    u32             _rand_next;
    u8              reserved2[0x148 - 0x5C];
    struct _atexit *_atexit;
};

struct RncFile {
    u8            *_p;
    s32            _r;
    s32            _w;
    u16            _flags;
    s16            _file;
    RncStdioBuffer _bf;
    s32            _lbfsize;
    void          *_cookie;
    s32 (*_read)(void *, char *, s32);
    s32 (*_write)(void *, const char *, s32);
    s32 (*_seek)(void *, s32, s32);
    s32 (*_close)(void *);
    RncStdioBuffer _ub;
    u8            *_up;
    s32            _ur;
    u8             _ubuf[3];
    u8             _nbuf[1];
    RncStdioBuffer _lb;
    s32            _blksize;
    s32            _offset;
    RncReent      *_data;
};

typedef struct RncStdioIov {
    const void *iov_base;
    u32         iov_len;
} RncStdioIov;

typedef struct RncStdioUio {
    RncStdioIov *uio_iov;
    s32          uio_iovcnt;
    u32          uio_resid;
} RncStdioUio;

/* newlib's reentrancy pointer; reached with %hi/%lo, not through $gp. */
extern RncReent *_impure_ptr __asm__("D_0012F76C") NOT_SDA;

#endif /* LOMBYTE_RNC_SDK_LIBRARY_STDIO_H */

/*
 * __swsetup - guarantee a stream is set up for writing (PS2 SDK newlib).
 *
 * Reconstructed from the upstream newlib libc/stdio/wsetup.c shipped with
 * the EE toolchain.  Two details of the SDK build are load-bearing:
 *
 *  - <sys/reent.h> declares the FILE flags word as `short _flags` (signed).
 *    The shared rnc_stdio.h view spells it u16; with an unsigned spelling
 *    the C front end folds `flags & ~(__SRD | __SEOF)` into a 16-bit
 *    `andi 0xffdb`, while retail clears the flags with a full-word
 *    constant: `li v0,-0x25; and v0,v1,v0`.
 *  - CHECK_INIT is the upstream `do { ... } while (0)` macro.  Its loop
 *    note also makes the compiler 8-byte-align the following label, whose
 *    p2align padding is the retail no-op between the `__sdidinit` reload
 *    and the branch.
 *
 * The SDK reentrant pointer global is named D_0012F76C in the oracle.
 */

#include "types.h"
#include "rnc/sdk/library/stdio.h"

/* FILE with the SDK's signed flags word. */
struct SwFile {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
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

#define __SLBF 0x0001 /* line buffered */
#define __SNBF 0x0002 /* unbuffered */
#define __SRD  0x0004 /* OK to read */
#define __SWR  0x0008 /* OK to write */
#define __SRW  0x0010 /* open for reading & writing */
#define __SEOF 0x0020 /* found EOF */
#define EOF    (-1)

/* Reentrancy anchor; named D_0012F76C in the retail oracle.  The
   unknown-size array spelling keeps absolute %hi/%lo addressing (a plain
   pointer object would be treated as small data and go gp-relative). */
#define _REENT D_0012F76C[0]
extern RncReent *D_0012F76C[];
extern void __sinit(RncReent *);
extern void __smakebuf(struct SwFile *);
extern void _free_r(RncReent *, void *);

/* Called by the main entry point fns to ensure stdio has been initialized. */
#define CHECK_INIT(fp)                                                                             \
    do {                                                                                           \
        if ((fp)->_data == 0)                                                                      \
            (fp)->_data = _REENT;                                                                  \
        if (!(fp)->_data->__sdidinit)                                                              \
            __sinit((fp)->_data);                                                                  \
    } while (0)

#define HASUB(fp) ((fp)->_ub._base != 0)
#define FREEUB(fp)                                                                                 \
    {                                                                                              \
        if ((fp)->_ub._base != (fp)->_ubuf)                                                        \
            _free_r((fp)->_data, (void *)(fp)->_ub._base);                                         \
        (fp)->_ub._base = 0;                                                                       \
    }

/*
 * Various output routines call wsetup to be sure it is safe to write,
 * because either _flags does not include __SWR, or _buf is NULL.
 * _wsetup returns 0 if OK to write, nonzero otherwise.
 */
s32 __swsetup(struct SwFile *fp) {
    /* Make sure stdio is set up. */
    CHECK_INIT(fp);

    /*
     * If we are not writing, we had better be reading and writing.
     */
    if ((fp->_flags & __SWR) == 0) {
        if ((fp->_flags & __SRW) == 0)
            return EOF;
        if (fp->_flags & __SRD) {
            /* clobber any ungetc data */
            if (HASUB(fp))
                FREEUB(fp);
            fp->_flags &= ~(__SRD | __SEOF);
            fp->_r = 0;
            fp->_p = fp->_bf._base;
        }
        fp->_flags |= __SWR;
    }

    /*
     * Make a buffer if necessary, then set _w.
     */
    if (fp->_bf._base == 0)
        __smakebuf(fp);

    if (fp->_flags & __SLBF) {
        /*
         * It is line buffered, so make _lbfsize be -_bufsize
         * for the putc() macro.  We will change _lbfsize back
         * to 0 whenever we turn off __SWR.
         */
        fp->_w = 0;
        fp->_lbfsize = -fp->_bf._size;
    } else
        fp->_w = fp->_flags & __SNBF ? 0 : fp->_bf._size;

    return 0;
}

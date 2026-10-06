/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

/* __sbprintf: newlib vfprintf.c fallback for unbuffered streams. */

#include "types.h"
#include "rnc/sdk/library/stdio.h"

typedef char *va_list;

extern s32 Vfprintf(RncFile *file, const s8 *format, va_list arguments) __asm__("func_00116DA8");
extern s32 fflush(RncFile *file);

#define __SNBF 0x0002
#define __SERR 0x0040

s32 __sbprintf(RncFile *fp, const s8 *fmt, va_list ap) {
    s32 ret;
    RncFile fake;
    u8 buf[1024];

    fake._data = fp->_data;
    fake._flags = (u16)(fp->_flags & ~__SNBF);
    fake._file = fp->_file;
    fake._cookie = fp->_cookie;
    fake._write = fp->_write;
    fake._bf._base = fake._p = buf;
    fake._bf._size = fake._w = sizeof(buf);
    fake._lbfsize = 0;
    ret = Vfprintf(&fake, fmt, ap);
    if (ret >= 0 && fflush(&fake)) {
        ret = -1;
    }
    if (fake._flags & __SERR) {
        fp->_flags |= __SERR;
    }
    return ret;
}

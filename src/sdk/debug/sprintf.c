/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

/* sprintf: newlib sprintf.c with the recovered SDK FILE ABI. */

#include "types.h"
#include "rnc/sdk/library/stdio.h"

typedef char *va_list;

extern RncReent *GlobalReent __asm__("D_0012F76C") __attribute__((section(".data")));
extern s32 Vfprintf(RncFile *file, const s8 *format, va_list arguments) __asm__("func_00116DA8");

#define __SWR   0x0008
#define __SSTR  0x0200
#define INT_MAX 0x7FFFFFFF

s32 sprintf(s8 *str, const s8 *format, ...) {
    s32 ret;
    va_list arguments;
    RncFile f;

    f._flags = (u16)(__SWR | __SSTR);
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = INT_MAX;
    f._data = GlobalReent;
    arguments = __builtin_next_arg(format) - 0x30;
    ret = Vfprintf(&f, format, arguments);
    *f._p = 0;
    return ret;
}

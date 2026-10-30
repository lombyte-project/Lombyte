/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

/* sprintf: newlib sprintf.c with the recovered SDK FILE ABI. */

#include "types.h"
#include "rnc/sdk/library/stdio.h"

typedef char *va_list;

extern s32 vfprintf(RncFile *file, const s8 *format, va_list arguments) __asm__("func_00116DA8");

#define __SWR   0x0008
#define __SSTR  0x0200
#define INT_MAX 0x7FFFFFFF

s32 sprintf(s8 *str, const s8 *format, ...) {
    s32     formatter_result;
    va_list arguments;
    RncFile output_stream;

    output_stream._flags = (u16)(__SWR | __SSTR);
    output_stream._bf._base = output_stream._p = (u8 *)str;
    output_stream._bf._size = output_stream._w = INT_MAX;
    output_stream._data = _impure_ptr;
    arguments = __builtin_next_arg(format) - 0x30;
    formatter_result = vfprintf(&output_stream, format, arguments);
    *output_stream._p = 0;
    return formatter_result;
}

#include "types.h"

typedef struct __sFILE {
    u8    pad_0[0xC];
    s16   _flags;
    s16   _file;
    u8    pad_10[0x40];
    s32   _offset;
    void *_data;
} FILE;
#define __SAPP 0x0100
#define __SOFF 0x1000
extern s64  _lseek_r() __asm__("func_00114518");
extern long _write_r() __asm__("func_001185D0");

int __swrite(void *cookie, char const *write_source, s32 byte_count) {
    register FILE *fp = (FILE *)cookie;
    if (fp->_flags & __SAPP)
        (void)_lseek_r(fp->_data, fp->_file, 0, 2);
    fp->_flags &= ~__SOFF;
    return _write_r(fp->_data, fp->_file, write_source, byte_count);
}

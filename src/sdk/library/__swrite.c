#include "types.h"
typedef struct __sFILE {
    u8 pad_0[0xC];
    s16 _flags;
    s16 _file;
    u8 pad_10[0x40];
    s32 _offset;
    void *_data;
} FILE;
#define __SAPP 0x0100
#define __SOFF 0x1000
extern s64 reentrant_syscall_with_three_arguments() __asm__("func_00114518");
extern long reentrant_write() __asm__("func_001185D0");
int __swrite(void *cookie, char const *buf, s32 n) {
    register FILE *fp = (FILE *)cookie;
    if (fp->_flags & __SAPP)
        (void)reentrant_syscall_with_three_arguments(fp->_data, fp->_file, 0, 2);
    fp->_flags &= ~__SOFF;
    return reentrant_write(fp->_data, fp->_file, buf, n);
}

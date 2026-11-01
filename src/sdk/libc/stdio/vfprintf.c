#include "types.h"
#include "rnc/sdk/library/stdio.h"
#include "rnc/sdk/libc/stdio/_vfprintf_r.h"
#include "rnc/sdk/libc/stdio.h"
#define FILE RncFile
/* newlib local.h */
#define CHECK_INIT(fp)                  \
    do {                                \
        if ((fp)->_data == 0) {         \
            (fp)->_data = _impure_ptr;  \
        }                               \
        if (!(fp)->_data->__sdidinit) { \
            __sinit((fp)->_data);       \
        }                               \
    } while (0)

s32 vfprintf(FILE *fp, const char *fmt0, void *ap) __asm__("FUN_00116da8");

s32 vfprintf(FILE *fp, const char *fmt0, void *ap) {
    CHECK_INIT(fp);
    return _vfprintf_r(fp->_data, fp, fmt0, ap);
}

extern __typeof__(vfprintf) func_00116DA8 __attribute__((alias("FUN_00116da8")));

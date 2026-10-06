#include "types.h"
struct _reent {
    u8 pad0[0x38];
    s32 __sdidinit;
};
typedef struct {
    u8 pad0[0x54];
    struct _reent *_data;
} FILE;
extern struct _reent *D_0012F76C[];
extern void __sinit(struct _reent *);
extern s32 _vfprintf_r(struct _reent *, FILE *, const char *, void *);
/* newlib vfprintf: CHECK_INIT (fp) then _vfprintf_r. */
s32 vfprintf(FILE *fp, const char *fmt0, void *ap) __asm__("FUN_00116da8");

s32 vfprintf(FILE *fp, const char *fmt0, void *ap) {
    do {
        if (fp->_data == 0) {
            fp->_data = D_0012F76C[0];
        }
        if (!fp->_data->__sdidinit) {
            __sinit(fp->_data);
        }
    } while (0);
    return _vfprintf_r(fp->_data, fp, fmt0, ap);
}

extern __typeof__(vfprintf) func_00116DA8 __attribute__((alias("FUN_00116da8")));

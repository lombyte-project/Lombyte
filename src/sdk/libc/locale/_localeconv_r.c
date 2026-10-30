#include "types.h"
/* newlib _localeconv_r: the one static struct lconv. */

int *_localeconv_r(void *state_resource) __asm__("func_001144D8");

extern char D_00152370[0x100];

int *_localeconv_r(void *state_resource) {
    return (int *)D_00152370;
}

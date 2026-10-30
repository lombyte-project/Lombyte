/* newlib __errno: errno is the first word of _impure_ptr's struct _reent. */

#include "rnc/sdk/library/stdio.h"

int *__errno(void) __asm__("func_001138A8");

int *__errno(void) {
    return &_impure_ptr->_errno;
}

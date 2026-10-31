#include "rnc/sdk/library/stdio.h"
#include "rnc/sdk/libc/locale/_localeconv_r.h"

int *localeconv(void) __asm__("GetStateResourceWrapper");

int *localeconv(void) {
    return _localeconv_r(_impure_ptr);
}

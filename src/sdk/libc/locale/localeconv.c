#include "rnc/sdk/library/stdio.h"

int *_localeconv_r(void *state_resource) __asm__("func_001144D8");

int *localeconv(void) __asm__("GetStateResourceWrapper");

int *localeconv(void) {
    return _localeconv_r(_impure_ptr);
}

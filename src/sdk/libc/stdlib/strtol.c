/* newlib strtol: _strtol_r with _impure_ptr. */

#include "rnc/sdk/library/stdio.h"

extern int _strtol_r(RncReent *data, int first, int second, int third) __asm__("func_00116A38");

int strtol(int resource, int first, int second) __asm__("CallGlobalStateResource");

int strtol(int resource, int end_pointer, int radix) {
    return _strtol_r(_impure_ptr, resource, end_pointer, radix);
}

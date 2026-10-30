#include "types.h"
#include "rnc/sdk/library/stdio.h"

s32 RandomNext(void) __asm__("rand");

s32 RandomNext(void) {
    u32 next_seed = _impure_ptr->_rand_next * 0x41C64E6Du + 0x3039u;
    _impure_ptr->_rand_next = next_seed;
    return (s32)(next_seed & 0x7FFFFFFFu);
}

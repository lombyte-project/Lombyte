#include "types.h"
#include "sda.h"

typedef struct RandomState {
    u8 reserved[0x58];
    u32 seed;
} RandomState;

extern RandomState *_impure_ptr __asm__("D_0012F76C") NOT_SDA;

s32 RandomNext(void) __asm__("rand");

s32 RandomNext(void) {
    u32 next_seed = _impure_ptr->seed * 0x41C64E6Du + 0x3039u;
    _impure_ptr->seed = next_seed;
    return (s32)(next_seed & 0x7FFFFFFFu);
}

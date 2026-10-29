#include "types.h"

typedef struct RandomState {
    u8 reserved[0x58];
    u32 seed;
} RandomState;

extern RandomState *GlobalStateResource __asm__("D_0012F76C") __attribute__((section(".data")));

s32 RandomNext(void) __asm__("rand");

s32 RandomNext(void) {
    u32 next_seed = GlobalStateResource->seed * 0x41C64E6Du + 0x3039u;
    GlobalStateResource->seed = next_seed;
    return (s32)(next_seed & 0x7FFFFFFFu);
}

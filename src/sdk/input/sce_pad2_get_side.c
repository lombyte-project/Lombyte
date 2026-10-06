#include "types.h"

typedef struct PadPort {
    u8 pad[0xC];
    u8 *unkC;
    u8 pad2[816 - 0x10];
} PadPort;

extern PadPort D_0015B540[];
extern void synchronize_cache_range(s32, s32) __asm__("func_00118F88");

void *scePad2GetSide(s32 arg0) {
    u8 *sides[2];

    sides[0] = D_0015B540[arg0].unkC;
    sides[1] = sides[0] + 0x80;
    synchronize_cache_range((s32)sides[0], (s32)(sides[0] + 0x100));
    return sides[*(s32 *)(sides[0] + 0x7C) < *(s32 *)(sides[1] + 0x7C)];
}

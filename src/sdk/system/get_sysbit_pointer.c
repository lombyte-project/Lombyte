#include "types.h"

typedef struct SysbitState {
    u8  reserved[8];
    u32 bit_pointer;
    u8  padding[24];
    u32 limit;
    u32 wrap_base;
} SysbitState;

s32 GetSysbitPointer(SysbitState *bitstream, s32 bit_offset) {
    u32 bit_pointer = bitstream->bit_pointer + (bit_offset >> 3);

    if (bit_pointer >= bitstream->limit) {
        bit_pointer -= bitstream->wrap_base;
    }
    return bit_pointer;
}

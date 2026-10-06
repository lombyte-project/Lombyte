#include "types.h"

s32 KernelCopyRdata(u32 *destination, const u32 *source, s32 byte_count) {
    u32 word_count = (u32)byte_count >> 2;
    u32 word_index = 0;

    while (word_index < word_count) {
        *destination = *source;
        ++word_index;
        ++source;
        ++destination;
    }
    return 0;
}

#include "types.h"

void vo_buf_create(void *buffer, s32 a1, s32 a2, s32 count) __asm__("FUN_0023d190");
void vo_buf_create(void *buffer, s32 initial_buffer_word, s32 entry_base_address, s32 count) {
    s32 offset;
    volatile s32 *buffer_words;

    buffer_words = (volatile s32 *)buffer;
    buffer_words[3] = 0;
    buffer_words[0] = initial_buffer_word;
    buffer_words[1] = entry_base_address;
    buffer_words[4] = count;
    buffer_words[2] = 0;
    if (count > 0) {
        offset = 0;
        do {
            *(s32 *)(offset + (u32)buffer_words[1]) = 0;
            count--;
            offset += 0x138C0;
        } while (count != 0);
    }
}

extern __typeof__(vo_buf_create) func_0023D190 __attribute__((alias("FUN_0023d190")));

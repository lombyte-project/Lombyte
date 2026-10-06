#include "eetypes.h"

void PackRenderCommandFields(u64 *command_words, u64 upper_field, u64 middle_field, u64 low_field,
                             u64 tail_field) __asm__("PackRenderCommandFields");

void PackRenderCommandFields(u64 *command_words, u64 upper_field, u64 middle_field, u64 low_field,
                             u64 tail_field) {
    int command_word_index;
    if (middle_field) {
        upper_field <<= 32;
        low_field <<= 8;
        tail_field <<= 16;
    } else {
        upper_field <<= 32;
        low_field <<= 8;
        tail_field <<= 16;
    }
    {
        register u64 packed_command;
        packed_command = upper_field | middle_field;
        packed_command |= low_field;
        packed_command |= tail_field;
        command_word_index = 7;
        command_words[command_word_index] = packed_command;
    }
}

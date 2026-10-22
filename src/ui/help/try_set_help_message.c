#include "types.h"
#include "rnc/data_labels.h"
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 copy_text_to_shared_buffer() __asm__("func_001FF658");
s32 try_set_help_message(s32 owner, s32 message_id) __asm__("FUN_00215130");

s32 try_set_help_message(s32 owner, s32 message_id) {
    s32 help_message_result;

    help_message_result = D_0015F5D4;
    if (help_message_result == owner) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        help_message_result = 2;
        D_00161288 = message_id;
        D_0015F5D0 = help_message_result;
    } else if (help_message_result == 0) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        D_0015F5D4 = owner;
        D_0015F5D0 = 2;
        help_message_result = 1;
        D_00161288 = message_id;
    } else {
        help_message_result = 0;
    }
    return help_message_result;
}

extern __typeof__(try_set_help_message) func_00215130 __attribute__((alias("FUN_00215130")));

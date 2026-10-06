#include "types.h"
extern s32 D_0015F5D0;
extern s32 D_0015F5D4;
extern s32 D_00161288;
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 copy_text_to_shared_buffer() __asm__("func_001FF658");
s32 try_set_help_message(s32 owner, s32 message_id) __asm__("FUN_00215130");

s32 try_set_help_message(s32 owner, s32 message_id) {
    s32 ret;

    ret = D_0015F5D4;
    if (ret == owner) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        ret = 2;
        D_00161288 = message_id;
        D_0015F5D0 = ret;
    } else if (ret == 0) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        D_0015F5D4 = owner;
        D_0015F5D0 = 2;
        ret = 1;
        D_00161288 = message_id;
    } else {
        ret = 0;
    }
    return ret;
}

extern __typeof__(try_set_help_message) func_00215130 __attribute__((alias("FUN_00215130")));

#include "types.h"

extern s32 D_0015F5D0;
extern s32 D_0015F5D4;
extern s32 D_00161288;
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 copy_text_to_shared_buffer() __asm__("FUN_001ff658");
extern s32 try_set_help_message() __asm__("FUN_00215130");

s32 force_help_message(s32 owner, s32 message_id) __asm__("FUN_002151d8");

s32 force_help_message(s32 owner, s32 message_id) {
    s32 result;

    result = try_set_help_message();
    if (result == 0) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        D_0015F5D4 = owner;
        D_0015F5D0 = 2;
        D_00161288 = message_id;
        result = 3;
    }
    return result;
}

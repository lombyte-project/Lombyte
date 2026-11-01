#include "types.h"
#include "rnc/data_labels.h"

extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 copy_text_to_shared_buffer() __asm__("FUN_001ff658");
extern s32 try_set_help_message() __asm__("FUN_00215130");

s32 force_help_message(s32 owner, s32 message_id) __asm__("FUN_002151d8");

s32 force_help_message(s32 help_message_selector, s32 message_id) {
    enum {
        HELP_MESSAGE_REQUEST_REJECTED = 0,
        HELP_MESSAGE_REQUEST_FORCED = 3
    };
    s32 help_message_status;

    help_message_status = try_set_help_message();
    if (help_message_status == HELP_MESSAGE_REQUEST_REJECTED) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        D_0015F5D4 = help_message_selector;
        D_0015F5D0 = 2;
        D_00161288 = message_id;
        help_message_status = HELP_MESSAGE_REQUEST_FORCED;
    }
    return help_message_status;
}

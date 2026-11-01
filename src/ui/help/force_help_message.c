#include "types.h"
#include "rnc/data_labels.h"

extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 copy_text_to_shared_buffer() __asm__("FUN_001ff658");
extern s32 try_set_help_message() __asm__("FUN_00215130");

s32 force_help_message(s32 owner, s32 message_id) __asm__("FUN_002151d8");

s32 force_help_message(s32 help_message_selector, s32 message_id) {
    s32 help_message_status;

    help_message_status = try_set_help_message();
    if (help_message_status == 0) {
        if (message_id != 0) {
            copy_text_to_shared_buffer(get_help_message_text(message_id));
        }
        help_message_owner = help_message_selector;
        help_message_request_state = 2;
        current_help_message_id = message_id;
        help_message_status = 3;
    }
    return help_message_status;
}

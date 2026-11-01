#include "types.h"

#include "rnc/ui/help/message_entry.h"

#include "rnc/ui/help/help_state.h"

extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0");
s32                      find_help_message_index(s32 message_id) __asm__("FUN_001fdca0");

s32 find_help_message_index(s32 message_id) {
    s32 i;
    s32 found = -1;

    i = 0;
    if (help_state.count > 0) {
        if (help_msgs[0].message_id == message_id) {
            found = 0;
        } else {
        next:
            i++;
            if (i < help_state.count) {
                if (help_msgs[i].message_id == message_id) {
                    found = i;
                } else {
                    goto next;
                }
            }
        }
    }
    return found;
}

extern __typeof__(find_help_message_index) func_001fdca0 __attribute__((alias("FUN_001fdca0")));

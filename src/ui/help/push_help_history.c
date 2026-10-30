#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/help/message_state.h"

extern s32 find_help_entry(s16, s32, u16 *) __asm__("func_001FECC8");
void       push_help_history(s32 id) __asm__("FUN_001fed30");

void push_help_history(s32 id) {
    s32 slot;
    s32 i;

    slot = find_help_entry(id, 0, 0);
    if (slot == -1) {
        return;
    }
    for (i = 0; active_message_records[i] != slot && i < active_message_record_count; i++) {
    }
    if (i < active_message_record_count) {
        for (; i < active_message_record_count - 1; i++) {
            active_message_records[i] = active_message_records[i + 1];
        }
        active_message_records[i] = 0;
        active_message_record_count--;
    }
    active_message_records[active_message_record_count] = slot;
    active_message_record_count++;
}

extern __typeof__(push_help_history) func_001FED30 __attribute__((alias("FUN_001fed30")));

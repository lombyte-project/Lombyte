#include "types.h"
#include "sda.h"

extern s32 *help_offset_table __asm__("D_0015EF64");
extern u8 *help_msg_data __asm__("D_0015EF60");
extern s32 help_msg_count __asm__("D_001996FC") NOT_SDA;
extern u8 *help_msgs __asm__("D_0015F6A0") __attribute__((sda));

void QueueDmaTransfer(s32 index) {
    u8 *entry;

    entry = help_msg_data + help_offset_table[index];
    help_msg_count = *(s32 *)entry;
    entry += 8;
    help_msgs = entry;
}

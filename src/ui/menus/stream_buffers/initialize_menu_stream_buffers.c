#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/stream_buffers.h"

void initialize_menu_stream_buffers(s32 mode) __asm__("FUN_00225ac0");

/* Mode zero writes one primary descriptor and clears four entries.
 * Nonzero mode writes two primary descriptors, one secondary descriptor,
 * and two further primary descriptors. Address strides are 0x11800 for
 * the first two groups and 0x4F000 for the final group. The flag word
 * is one only for the final group. */
void initialize_menu_stream_buffers(s32 mode) {
    s32 secondary_address;
    s32 primary_address;
    s32 primary_count;
    s32 secondary_count;
    s32 large_count;
    s32 index;
    s32 end_index;
    s32 clear_remaining;
    struct MenuStreamBuffer *descriptor;

    primary_address = menu_system.help_text_buffer;
    secondary_address = menu_system.unk10C;
    if (mode == 0) {
        primary_count = 1;
        secondary_count = 0;
        large_count = 0;
    } else {
        primary_count = 2;
        secondary_count = 1;
        large_count = 2;
    }
    end_index = primary_count;
    index = 0;
    if (primary_count > 0) {
        for (index = 0; index < primary_count; index++) {
            menu_stream_buffers[index].address = primary_address;
            menu_stream_buffers[index].flags = 0;
            primary_address += 0x11800;
        }
        index = primary_count;
    }
    end_index += secondary_count;
    for (; index < end_index; index++) {
        menu_stream_buffers[index].address = secondary_address;
        menu_stream_buffers[index].flags = 0;
        secondary_address += 0x11800;
    }
    end_index += large_count;
    if (index < end_index) {
        for (; index < end_index; index++) {
            menu_stream_buffers[index].address = primary_address;
            menu_stream_buffers[index].flags = MENU_STREAM_BUFFER_LARGE;
            primary_address += 0x4F000;
        }
        /* Fakematch: the do-while(0) aligns the join label below like retail. */
        do {
            clear_remaining = end_index < MENU_STREAM_BUFFER_COUNT;
        } while (0);
    } else {
        clear_remaining = index < MENU_STREAM_BUFFER_COUNT;
    }
    if (clear_remaining) {
        descriptor = menu_stream_buffers + index;
        do {
            descriptor->flags = 0;
            descriptor->address = 0;
            descriptor++;
            index++;
        } while (index < MENU_STREAM_BUFFER_COUNT);
    }
}

extern __typeof__(initialize_menu_stream_buffers) func_00225AC0 __attribute__((alias("FUN_00225ac0")));

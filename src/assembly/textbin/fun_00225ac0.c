#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00225ac0/FUN_00225ac0.s", FUN_00225ac0);
#else
#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/rendering/graphics_buffer.h"


void initialize_graphics_buffer_descriptors(s32 mode) __asm__("FUN_00225ac0");

/* Mode zero writes one primary descriptor and clears four entries.
 * Nonzero mode writes two primary descriptors, one secondary descriptor,
 * and two further primary descriptors. Address strides are 0x11800 for
 * the first two groups and 0x4F000 for the final group. The second word
 * is zero except for the final group, where it is one. The address words
 * and flag value have no further meaning established by this function. */
void initialize_graphics_buffer_descriptors(s32 mode) {
    s32 secondary_address;
    s32 primary_address;
    s32 primary_count;
    s32 secondary_count;
    s32 streaming_count;
    s32 index;
    s32 end_index;
    s32 clear_remaining;
    struct GraphicsBufferDescriptor *descriptor;

    primary_address = menu_system.help_text_buffer;
    secondary_address = menu_system.unk10C;
    if (mode == 0) {
        primary_count = 1;
        secondary_count = 0;
        streaming_count = 0;
    } else {
        primary_count = 2;
        secondary_count = 1;
        streaming_count = 2;
    }
    end_index = primary_count;
    index = 0;
    if (primary_count > 0) {
        for (index = 0; index < primary_count; index++) {
            graphics_buffer_descriptors[index].address = primary_address;
            graphics_buffer_descriptors[index].flags = 0;
            primary_address += 0x11800;
        }
        index = primary_count;
    }
    end_index += secondary_count;
    for (; index < end_index; index++) {
        graphics_buffer_descriptors[index].address = secondary_address;
        graphics_buffer_descriptors[index].flags = 0;
        secondary_address += 0x11800;
    }
    end_index += streaming_count;
    clear_remaining = index < 5;
    if (index < end_index) {
        for (; index < end_index; index++) {
            graphics_buffer_descriptors[index].address = primary_address;
            graphics_buffer_descriptors[index].flags = 1;
            primary_address += 0x4F000;
        }
        clear_remaining = end_index < 5;
    }
    if (clear_remaining & 1) {
        descriptor = graphics_buffer_descriptors + index;
        do {
            descriptor->flags = 0;
            descriptor->address = 0;
            descriptor++;
            index++;
        } while (index < 5);
    }
}
#endif /* NON_MATCHING */

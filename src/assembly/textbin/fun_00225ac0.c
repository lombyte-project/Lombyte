#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00225ac0/FUN_00225ac0.s", FUN_00225ac0);
#else
#include "types.h"

struct GraphicsBufferDescriptor {
    s32 address;
    s32 flags;
};
struct MenuGraphicsBuffers {
    u8 pad00[0x108];
    s32 primary_buffer;
    s32 secondary_buffer;
};

extern struct MenuGraphicsBuffers menu_graphics_buffers __asm__("D_001D5BF0");
extern struct GraphicsBufferDescriptor graphics_buffer_descriptors[5] __asm__("D_001D60B8");

void initialize_graphics_buffer_descriptors(s32 mode) __asm__("FUN_00225ac0");

/* Mode zero uses one primary buffer; nonzero mode uses the 2/1/2 layout. */
void initialize_graphics_buffer_descriptors(s32 mode) {
    s32 secondary_address;
    s32 primary_address;
    s32 primary_count;
    s32 secondary_count;
    s32 streaming_count;
    s32 index;
    s32 end_index;
    struct GraphicsBufferDescriptor *descriptor;

    primary_address = menu_graphics_buffers.primary_buffer;
    secondary_address = menu_graphics_buffers.secondary_buffer;
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
        descriptor = graphics_buffer_descriptors;
        for (index = primary_count; index != 0; index--) {
            descriptor->address = primary_address;
            descriptor->flags = 0;
            primary_address += 0x11800;
            descriptor++;
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
    for (; index < end_index; index++) {
        graphics_buffer_descriptors[index].address = primary_address;
        graphics_buffer_descriptors[index].flags = 1;
        primary_address += 0x4F000;
    }
    for (; index < 5; index++) {
        graphics_buffer_descriptors[index].flags = 0;
        graphics_buffer_descriptors[index].address = 0;
    }
}
#endif /* NON_MATCHING */

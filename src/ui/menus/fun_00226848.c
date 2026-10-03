#include "rnc/preview_animation.h"

extern u8 moby_class_resources[] __asm__("D_001B3200");
extern u8 class_resource_slots[] __asm__("D_001B3AC0");
extern u8 preview_resource_bindings[] __asm__("D_001D59D8");
extern PreviewAnimationStreamState preview_stream_state __asm__("D_001D5BF0");
extern u8 preview_resource_buffers[] __asm__("D_001D5CA0");
extern u8 preview_resource_ids[] __asm__("D_001D5D38");
extern s32 LookupResourceEntry();
extern void relocate_asset_entry_pointers() __asm__("FUN_002032e0");
extern s32 decompress_wad() __asm__("func_0020B618");
extern s32 get_stream_buffer_size() __asm__("func_00225D88");
extern s32 stash_receive_data() __asm__("func_00232F20");
void load_preview_resource_bindings(s32 first_resource, s32 resource_count) __asm__("FUN_00226848");

void load_preview_resource_bindings(s32 first_resource, s32 resource_count)
{
    s32 count;
    register s32 *class_resource_slot;
    s32 read_address;
    s32 buffer_address;
    s32 compressed_size;
    s32 resource_id;
    s32 animation_index;
    s32 buffer_offset;
    s32 buffer_skip;
    /* Existing counter pin is still required for the retail register allocation. */
    register s32 resource_index asm("s4");
    s32 resource_offset;
    u8 class_slot;
    PreviewResourceBinding *binding;

    count = resource_count;
    resource_index = 0;
    preview_stream_state.resource_count = count;
    preview_stream_state.resource_first = first_resource;
    if (count > 0) {
        resource_offset = first_resource * 4;
        binding = (PreviewResourceBinding *) (preview_resource_bindings + (first_resource * 8));
        do {
            buffer_skip = 0;
            class_slot = class_resource_slots[binding->class_id];
            animation_index = binding->animation_index;
            /* Two bindings use resource buffers 0 and 2. */
            if (count == 2) {
                if (resource_index == 1) {
                    buffer_skip = 1;
                } else {
                    buffer_skip = 0;
                }
            }
            binding += 1;
            resource_id = *((s32 *) (preview_resource_ids + resource_offset));
            resource_offset += 4;
            compressed_size = LookupResourceEntry(resource_id) * 0x10;
            buffer_offset = (resource_index + buffer_skip) * 4;
            resource_index += 1;
            buffer_address = *((s32 *) (preview_resource_buffers + buffer_offset));
            read_address = (buffer_address + get_stream_buffer_size(buffer_address)) - compressed_size;
            stash_receive_data(read_address, resource_id, 0, -1, 0);
            decompress_wad(read_address, buffer_address);
            {
                register s32 animation_offset = animation_index * 4;
                class_resource_slot = (class_slot * 4) + moby_class_resources;
                *((s32 *) (((u8 *) ((*class_resource_slot) + animation_offset)) + 0x48)) = buffer_address;
            }
            relocate_asset_entry_pointers(*class_resource_slot, animation_index);
        } while (resource_index < count);
    }
}

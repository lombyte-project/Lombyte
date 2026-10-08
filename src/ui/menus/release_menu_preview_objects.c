#include "types.h"
#include "rnc/ui/menus/menu_system.h"
extern s32 delete_moby() __asm__("FUN_00225530");
extern s32 complete_stream_buffer_transfer() __asm__("func_00225CD8");
extern void clear_preview_resource_bindings(void) __asm__("FUN_002267b8");

s32 release_menu_preview_objects(s32 preview_address) __asm__("FUN_002242b8");

s32 release_menu_preview_objects(s32 preview_address) {
    s32 *object_slot;
    s32 *resource_slot;
    s32 objects_remaining;
    s32 resources_remaining;
    struct MenuSystem *resources;

    objects_remaining = 0x17;
    object_slot = preview_address + 0x44;
    do {
        objects_remaining -= 1;
        *object_slot = delete_moby(*object_slot);
        object_slot += 1;
    } while (objects_remaining >= 0);
    resources_remaining = 2;
    resources = &menu_system;
    resources->stream_buffer[0] = complete_stream_buffer_transfer(resources->stream_buffer[0]);
    resources->stream_buffer[1] = complete_stream_buffer_transfer(resources->stream_buffer[1]);
    resources->loaded_animation[0] = 0xFF;
    resources->loaded_animation[1] = 0xFF;
    resources->read_buffer_index = 0;
    clear_preview_resource_bindings();
    resource_slot = ((u8 *)resources + 0xB0);
    do {
        resources_remaining -= 1;
        *resource_slot = complete_stream_buffer_transfer(*resource_slot);
        resource_slot += 1;
    } while (resources_remaining >= 0);
    return 0;
}

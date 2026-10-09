#include "rnc/gameplay/entities/moby_class_tables.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/item_preview/preview_animation.h"

extern u8 preview_resource_bindings[] __asm__("D_001D59D8");
void clear_preview_resource_bindings(void) __asm__("FUN_002267b8");
void clear_preview_resource_bindings(void) {
    register s32 class_slot;
    register s32 class_resource_address;
    s32 resource_index;
    register u8 *binding_base;
    PreviewResourceBinding *binding;
    register s32 animation_address;
    register s32 resource_first;
    register s32 resource_count;
    register s32 resource_end;

    resource_index = menu_system.unkA8;
    if (resource_index < (resource_index + menu_system.unkAC)) {
        u8 *class_slots = resident_class_slot_by_id;
        u8 *class_resources = (u8 *)moby_class_resources;
        u8 *bindings = preview_resource_bindings;

        binding_base = bindings;
        binding = (PreviewResourceBinding *)((resource_index * 8) + binding_base);
        do {
            resource_index += 1;
            class_slot = *((u8 *)(binding->class_id + (s32)class_slots));
            class_resource_address = *(s32 *)((class_slot * 4) + class_resources);
            animation_address = class_resource_address + (binding->animation_index * 4);
            *(s32 *)(animation_address + 0x48) = 0;
            binding += 1;
            resource_first = menu_system.unkA8;
            resource_count = menu_system.unkAC;
            resource_end = resource_first + resource_count;
        } while (resource_index < resource_end);
    }
    menu_system.unkAC = 0;
}

#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/world/data/"
            "select_world_object_resource_tables/FUN_00204a40.s",
            FUN_00204a40);
#else
#include "types.h"
/* Verified against the US retail body at 0x00204a40. */
#include "sda.h"

typedef struct {
    char pad00[0x10];
    s32 class_id;
    char pad14[0x38];
} GadgetRec;

/* Render groups at +0; the third group count and patch selector are at +6/+7.
   The runtime table at +0x28 has 0x20-byte entries. The +0x2C meaning is unresolved. */
typedef struct {
    char *render_groups;
    char pad04[2];
    u8 third_render_group_count;
    u8 patch_group_index;
} ClassResourceHeader;

extern s32 runtime_resource_tag __asm__("D_0015FF44") MACRO_ADDR;
extern s32 class_resource_count __asm__("D_0015FF48") MACRO_ADDR;
extern s32 active_class_resource_index __asm__("D_0015FF4C") MACRO_ADDR;
extern s32 active_decode_buffer __asm__("D_0015FF50") MACRO_ADDR;
extern s32 class_resource_ids[] __asm__("D_001CBAC0");
extern s32 compressed_class_resources[] __asm__("D_001CBB20");
extern char class_material_maps[][0x10] __asm__("D_001CBBE0");
extern s16 class_runtime_indices[][0x10] __asm__("D_001CBD60");
extern char resident_indexed_textures[] __asm__("D_001CAAC0");
typedef struct {
    u8 pad0[0x10];
    char *decode_buffers;
} LevelResourceBuffers;
extern LevelResourceBuffers level_resource_buffers __asm__("D_001940C0");
extern u8 resident_class_slot_by_id[] __asm__("D_001B3AC0") NOT_SDA;
extern char *resident_class_resources[] __asm__("D_001B3200") NOT_SDA;
/* Per-slot copy of the resource +0x2C word; its narrower meaning is unresolved. */
extern s32 D_001B6180[];
extern GadgetRec vendor_item_definitions[] __asm__("D_001863D0");
extern u8 gold_weapon_purchased[] __asm__("D_0013E520");
extern u64 gold_weapon_texture_state[] __asm__("D_0019E6F0");
extern void FlushCache(s32);
extern void decompress_wad(s32, void *) __asm__("func_0020B618");
extern void prepare_resident_class_render_data(void *, void *, void *,
                                               s32) __asm__("func_00203338");

void select_world_object_resource_tables(s32 class_id, s32 buffer_index) __asm__("FUN_00204a40");

void select_world_object_resource_tables(s32 class_id, s32 buffer_index) {
    s32 runtime_index;
    s32 resource_table_index;
    char *resource_data;
    u8 class_slot;
    s32 resource_tag;
    s32 vendor_item_index;
    char **resource_table;

    if (active_class_resource_index >= 0 &&
        class_resource_ids[active_class_resource_index] == class_id) {
        return;
    }
    /* Retail leaves the index at class_resource_count if the class is absent. */
    for (active_class_resource_index = 0; active_class_resource_index < class_resource_count;
         active_class_resource_index++) {
        if (class_resource_ids[active_class_resource_index] == class_id) {
            break;
        }
    }
    if (buffer_index == -1) {
        buffer_index = active_decode_buffer == 0;
    }
    active_decode_buffer = buffer_index;
    resource_data = level_resource_buffers.decode_buffers + buffer_index * 0x18000;
    FlushCache(0);
    decompress_wad(compressed_class_resources[active_class_resource_index], resource_data);
    FlushCache(0);
    /* The byte-sized class slot selects both the published pointer and the saved +0x2C word. */
    resource_table = resident_class_resources;
    class_slot = resident_class_slot_by_id[class_id];
    resource_table[class_slot] = resource_data;
    D_001B6180[class_slot] = *(s32 *)(resource_data + 0x2C);
    prepare_resident_class_render_data(resource_data, resident_indexed_textures,
                                       class_material_maps[active_class_resource_index], class_id);
    resource_table_index = active_class_resource_index;
    resource_tag = runtime_resource_tag;
    /* Retail reloads the published resource pointer for each enabled entry. */
    for (runtime_index = 0; runtime_index < 16; runtime_index++) {
        s16 runtime_entry_index = class_runtime_indices[resource_table_index][runtime_index];
        if (runtime_entry_index >= 0) {
            *(s16 *)(*(char **)(resident_class_resources[class_slot] + 0x28) +
                     runtime_index * 0x20 + 0x1A) = runtime_entry_index;
            *(s32 *)(*(char **)(resident_class_resources[class_slot] + 0x28) +
                     runtime_index * 0x20 + 0x1C) = resource_tag;
        }
    }
    for (vendor_item_index = 0; vendor_item_index < 0x25; vendor_item_index++) {
        if (vendor_item_definitions[vendor_item_index].class_id == class_id) {
            ClassResourceHeader *resource_header;
            char *render_group;
            char *patch_packet;

            if (gold_weapon_purchased[vendor_item_index] == 0) {
                return;
            }
            resource_header = (ClassResourceHeader *)resident_class_resources[class_slot];
            if (resource_header->third_render_group_count == 0) {
                return;
            }
            render_group =
                resource_header->render_groups + resource_header->patch_group_index * 0x10;
            patch_packet = *(char **)render_group + (*(s32 *)(render_group + 4) - 4) * 0x10;
            *(u64 *)(patch_packet + 0x20) = gold_weapon_texture_state[0];
            *(u64 *)(patch_packet + 0x30) = gold_weapon_texture_state[2];
            return;
        }
    }
}

extern void func_00204A40(s32 class_id, s32 buffer_index) __attribute__((alias("FUN_00204a40")));

#endif /* NON_MATCHING */

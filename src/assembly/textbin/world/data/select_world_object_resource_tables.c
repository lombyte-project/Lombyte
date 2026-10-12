#include "rnc/gameplay/entities/moby_class_tables.h"
#include "sda.h"
#include "types.h"
#include "asm.h"
#include "rnc/rendering/material_templates.h"
#include "rnc/rendering/resident_class.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/world/data/"
            "select_world_object_resource_tables/FUN_00204a40.s",
            FUN_00204a40);
#else
#include "rnc/gameplay/entities/moby_class_tables.h"
#include "types.h"
/* Verified against the US retail body at 0x00204a40. */
#include "sda.h"

typedef struct {
    char pad00[0x10];
    s32 class_id;
    char pad14[0x38];
} GadgetRec;

typedef struct {
    char pad00[0x1A];
    s16 runtime_index;
    s32 resource_tag;
} ClassRuntimeEntry;

typedef struct {
    char *packet_data;
    s32 packed_packet_extent;
    char pad08[8];
} ClassRenderGroup;

/* Render groups at +0; the third group count and patch selector are at +6/+7.
   The runtime table at +0x28 has 0x20-byte entries. The +0x2C meaning is unresolved. */
typedef struct {
    ClassRenderGroup *render_groups;
    char pad04[2];
    u8 third_render_group_count;
    u8 patch_group_index;
    char pad08[0x20];
    ClassRuntimeEntry *runtime_table;
    s32 original_field_2c;
} ClassResourceHeader;

extern s32 runtime_resource_tag __asm__("D_0015FF44") MACRO_ADDR;
extern s32 class_resource_count __asm__("D_0015FF48") MACRO_ADDR;
extern s32 active_class_resource_index __asm__("D_0015FF4C") MACRO_ADDR;
extern s32 active_decode_buffer __asm__("D_0015FF50") MACRO_ADDR;
#include "rnc/rendering/resident_class.h"
typedef struct {
    u8 pad0[0x10];
    char *decode_buffers;
} LevelResourceBuffers;
extern LevelResourceBuffers level_resource_buffers __asm__("D_001940C0");
/* Per-slot copy of the resource +0x2C word; its narrower meaning is unresolved. */
extern s32 D_001B6180[];
extern GadgetRec vendor_item_definitions[] __asm__("D_001863D0");
extern u8 gold_weapon_purchased[] __asm__("D_0013E520");
#include "rnc/rendering/material_templates.h"
extern void FlushCache(s32);
extern void decompress_wad(s32, void *) __asm__("func_0020B618");
extern void prepare_resident_class_render_data(void *, void *, void *,
                                               s32) __asm__("func_00203338");

void select_world_object_resource_tables(s32 class_id, s32 buffer_index) __asm__("FUN_00204a40");

void select_world_object_resource_tables(s32 class_id, s32 buffer_index) {
    s32 runtime_index;
    s32 resource_table_index;
    ClassResourceHeader *resource_data;
    void **resource_table;
    u8 class_slot;
    s32 resource_tag;
    s32 vendor_item_index;

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
    resource_data = (ClassResourceHeader *)(level_resource_buffers.decode_buffers + buffer_index * 0x18000);
    FlushCache(0);
    decompress_wad(compressed_class_resources[active_class_resource_index], resource_data);
    FlushCache(0);
    /* The byte-sized class slot selects both the published pointer and the saved +0x2C word. */
    resource_table = moby_class_resources;
    class_slot = resident_class_slot_by_id[class_id];
    resource_table[class_slot] = resource_data;
    D_001B6180[class_slot] = resource_data->original_field_2c;
    prepare_resident_class_render_data(resource_data, resident_indexed_textures,
                                       class_material_maps[active_class_resource_index], class_id);
    resource_table_index = active_class_resource_index;
    resource_tag = runtime_resource_tag;
    /* Retail reloads the published resource pointer for each enabled entry. */
    for (runtime_index = 0; runtime_index < 16; runtime_index++) {
        s16 runtime_entry_index = class_runtime_indices[resource_table_index][runtime_index];
        if (runtime_entry_index >= 0) {
            ((ClassResourceHeader *)resource_table[class_slot])->runtime_table[runtime_index].runtime_index =
                runtime_entry_index;
            ((ClassResourceHeader *)resource_table[class_slot])->runtime_table[runtime_index].resource_tag =
                resource_tag;
        }
    }
    for (vendor_item_index = 0; vendor_item_index < 0x25; vendor_item_index++) {
        if (vendor_item_definitions[vendor_item_index].class_id == class_id) {
            ClassResourceHeader *resource_header;
            ClassRenderGroup *render_group;
            char *patch_packet;

            if (gold_weapon_purchased[vendor_item_index] == 0) {
                return;
            }
            resource_header = (ClassResourceHeader *)moby_class_resources[class_slot];
            if (resource_header->third_render_group_count == 0) {
                return;
            }
            render_group = &resource_header->render_groups[resource_header->patch_group_index];
            patch_packet = render_group->packet_data +
                           (render_group->packed_packet_extent - 4) * 0x10;
            *(u64 *)(patch_packet + 0x20) = gold_weapon_texture_state[0];
            *(u64 *)(patch_packet + 0x30) = gold_weapon_texture_state[2];
            return;
        }
    }
}

extern void func_00204A40(s32 class_id, s32 buffer_index) __attribute__((alias("FUN_00204a40")));

#endif /* NON_MATCHING */

char class_material_maps[24][16] = {0};

s16 class_runtime_indices[24][16] = {0};

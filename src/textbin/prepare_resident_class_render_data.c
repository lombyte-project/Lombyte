#include "types.h"
#include "rnc/rendering/resident_class.h"
#include "asm.h"

#include "types.h"
#include "qcopy.h"
#include "eetypes.h"

typedef struct {
    s32 draw_high;
    s32 draw_shift;
    u8 pad8[0x8];
    s32 material_base;
    s32 material_shift;
    u8 pad18[0x8];
    s32 material_index;
    u8 pad24[0x1C];
} ResidentRenderPacket;

typedef struct {
    s32 blocks;
    s32 count;
    s32 auxiliary_data;
    s32 unused_C;
} ResidentRenderGroup;

typedef struct {
    u8 first_selector;
    u8 pad1[0xB];
    s32 target;
} MaterialRun;

typedef struct {
    u8 pad0[0x10];
    u8 count;
    u8 pad11[3];
    s32 optional_data_14;
    u8 pad18[4];
    s32 entry_offsets[1];
} NestedRenderTable;

typedef struct {
    s32 groups;
    u8 group_count_0;
    u8 group_count_1;
    u8 group_count_2;
    u8 pad7[5];
    u8 nested_table_count;
    u8 padD[3];
    s32 optional_table_10;
    s32 optional_data_14;
    s32 optional_table_18;
    s32 *counted_pointers;
    s32 material_runs;
    u8 pad24[4];
    s32 runtime_table;
    u8 pad2C[0x1C];
    s32 nested_tables[1];
} ResidentClassRenderHeader;

extern u8 resident_class_slot_by_id[] __asm__("D_001B3AC0");
extern MaterialMap resident_class_material_maps[] __asm__("D_001B6880");

extern void build_indexed_resident_render_packet(ResidentRenderPacket *, void *, s32, s32, s32, s32,
                                                 s32) __asm__("func_00202D78");
extern void build_template_resident_render_packet(ResidentRenderPacket *packet, s32 draw_high,
                                                  s32 draw_shift, s32 material_base,
                                                  s32 material_shift,
                                                  s32 material_index) __asm__("FUN_00202fd0");

void prepare_resident_class_render_data(ResidentClassRenderHeader *header, u8 *textures,
                                        u8 *material_map, s32 class_id) __asm__("FUN_00203338");

void prepare_resident_class_render_data(ResidentClassRenderHeader *header, u8 *textures,
                                        u8 *material_map, s32 class_id) {
    s32 group_count;
    s32 class_slot;
    s32 group_index;
    s32 packet_quadword;
    s32 groups_remaining;
    s32 packet_extent;
    ResidentRenderGroup *group;
    ResidentRenderGroup *relocation_group;
    MaterialRun *material_run;
    u8 *selector;
    NestedRenderTable *nested_table;
    s32 *entry_offset;
    MaterialMap *slot_materials;
    ResidentRenderPacket *packet;
    s32 packed_extent;
    s32 packet_start;
    s32 material_index;
    s32 pointer_count;
    s32 pointer_index;
    s32 nested_table_index;
    s32 nested_entry_index;

    /* Serialized pointers are relative to the entire class blob. */
    group_count = header->group_count_0 + header->group_count_1 + header->group_count_2;
    if (header->groups != 0) {
        header->groups = (s32)header + header->groups;
        relocation_group = (ResidentRenderGroup *)header->groups;
        if (group_count != 0) {
            groups_remaining = group_count;
            do {
                relocation_group->blocks += (s32)header;
                relocation_group->auxiliary_data += (s32)header;
                groups_remaining--;
                relocation_group++;
            } while (groups_remaining != 0);
        }
    }
    if (header->optional_table_10 != 0) {
        header->optional_table_10 = (s32)header + header->optional_table_10;
    }
    if (header->optional_data_14 != 0) {
        header->optional_data_14 = (s32)header + header->optional_data_14;
    }
    if (header->optional_table_18 != 0) {
        header->optional_table_18 = (s32)header + header->optional_table_18;
    }
    if (header->counted_pointers != 0) {
        header->counted_pointers = (s32 *)((u8 *)header + (s32)header->counted_pointers);
        pointer_count = header->counted_pointers[0];
        for (pointer_index = 0; pointer_index < pointer_count; pointer_index++) {
            header->counted_pointers[pointer_index + 1] += (s32)header;
        }
    }
    if (header->material_runs != 0) {
        header->material_runs = (s32)header + header->material_runs;
        material_run = (MaterialRun *)header->material_runs;
        do {
            material_run->target += (s32)header;
            selector = &material_run->first_selector;
            if (material_run->first_selector != 0xFF) {
                do {
                    *selector = material_map[*selector];
                    selector++;
                } while (*selector != 0xFF);
            }
        } while (material_run->target >= 0 && (material_run++, 1));
    }
    if (header->runtime_table != 0) {
        header->runtime_table = (s32)header + header->runtime_table;
    }
    for (nested_table_index = 0; nested_table_index < header->nested_table_count;
         nested_table_index++) {
        if (header->nested_tables[nested_table_index] != 0) {
            nested_table =
                (NestedRenderTable *)((u8 *)header + header->nested_tables[nested_table_index]);
            header->nested_tables[nested_table_index] = (s32)nested_table;
            if (nested_table->optional_data_14 != 0) {
                nested_table->optional_data_14 = (s32)header + nested_table->optional_data_14;
            }
            for (nested_entry_index = 0; nested_entry_index < nested_table->count;
                 nested_entry_index++) {
                nested_table->entry_offsets[nested_entry_index] =
                    (s32)header + nested_table->entry_offsets[nested_entry_index];
            }
        }
    }

    class_slot = resident_class_slot_by_id[class_id];
    slot_materials = &resident_class_material_maps[class_slot];
    qcopy(slot_materials, material_map);
    group = (ResidentRenderGroup *)header->groups;
    for (group_index = 0; group_index < group_count; group_index++, group++) {
        /* High half counts encoded quadwords; low half locates the packet end. */
        packed_extent = group->count;
        packet_extent = packed_extent >> 16;
        packet_start = packed_extent & 0xFFFF;
        group->count = packet_start;
        packet = (ResidentRenderPacket *)(group->blocks + (packet_start - packet_extent) * 16);
        for (packet_quadword = 0; packet_quadword < packet_extent; packet_quadword += 4) {
            material_index = packet->material_index;
            if (material_index >= 0) {
                material_index = slot_materials->b[material_index];
            }
            if (textures != NULL) {
                build_indexed_resident_render_packet(
                    packet, textures + material_index * 16, packet->draw_high, packet->draw_shift,
                    packet->material_base, packet->material_shift, material_index);
            } else {
                build_template_resident_render_packet(packet, packet->draw_high, packet->draw_shift,
                                                      packet->material_base, packet->material_shift,
                                                      material_index);
            }
            packet++;
        }
    }
}

extern __typeof__(prepare_resident_class_render_data) func_00203338
    __attribute__((alias("FUN_00203338")));

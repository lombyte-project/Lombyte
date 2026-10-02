#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203338/FUN_00203338.s", FUN_00203338);
#else
#include "types.h"
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
    s32 unk8;
    s32 unkC;
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
    s32 unk14;
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
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 *counted_pointers;
    s32 material_runs;
    u8 pad24[4];
    s32 unk28;
    u8 pad2C[0x1C];
    s32 nested_tables[1];
} ResidentClassRenderHeader;

typedef union {
    u128 q;
    u8 b[16];
} MaterialMap;

extern u8 D_001B3AC0[];
extern MaterialMap D_001B6880[];

extern void build_indexed_resident_render_packet(ResidentRenderPacket *, void *, s32, s32, s32, s32, s32) __asm__("func_00202D78");
extern void set_up_vis_gif_viewer(ResidentRenderPacket *q, s32 group_count, s32 prim, s32 a3, s32 t0, s32 material_index) __asm__("FUN_00202fd0");

void prepare_resident_class_render_data(ResidentClassRenderHeader *header, u8 *textures, u8 *material_map, s32 class_id) __asm__("FUN_00203338");

void prepare_resident_class_render_data(ResidentClassRenderHeader *header, u8 *textures, u8 *material_map, s32 class_id) {
    s32 group_count;
    s32 i;
    s32 j;
    s32 k;
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
    s32 m;
    s32 q;
    s32 t;

    group_count = header->group_count_0 + header->group_count_1 + header->group_count_2;
    if (header->groups != 0) {
        header->groups = (s32)header + header->groups;
        relocation_group = (ResidentRenderGroup *)header->groups;
        if (group_count != 0) {
            k = group_count;
            do {
                relocation_group->blocks += (s32)header;
                relocation_group->unk8 += (s32)header;
                k--;
                relocation_group++;
            } while (k != 0);
        }
    }
    if (header->unk10 != 0) {
        header->unk10 = (s32)header + header->unk10;
    }
    if (header->unk14 != 0) {
        header->unk14 = (s32)header + header->unk14;
    }
    if (header->unk18 != 0) {
        header->unk18 = (s32)header + header->unk18;
    }
    if (header->counted_pointers != 0) {
        header->counted_pointers = (s32 *)((u8 *)header + (s32)header->counted_pointers);
        pointer_count = header->counted_pointers[0];
        for (m = 0; m < pointer_count; m++) {
            header->counted_pointers[m + 1] += (s32)header;
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
    if (header->unk28 != 0) {
        header->unk28 = (s32)header + header->unk28;
    }
    for (q = 0; q < header->nested_table_count; q++) {
        if (header->nested_tables[q] != 0) {
            nested_table = (NestedRenderTable *)((u8 *)header + header->nested_tables[q]);
            header->nested_tables[q] = (s32)nested_table;
            if (nested_table->unk14 != 0) {
                nested_table->unk14 = (s32)header + nested_table->unk14;
            }
            if (nested_table->count != 0) {
                t = 0;
                entry_offset = nested_table->entry_offsets;
                do {
                    *entry_offset = (s32)header + *entry_offset;
                    t++;
                    entry_offset++;
                } while (t < nested_table->count);
            }
        }
    }

    i = D_001B3AC0[class_id];
    slot_materials = &D_001B6880[i];
    group = (ResidentRenderGroup *)header->groups;
    slot_materials->q = *(u128 *)material_map;
    for (i = 0; i < group_count; i++, group++) {
        packed_extent = group->count;
        packet_extent = packed_extent >> 16;
        packet_start = packed_extent & 0xFFFF;
        group->count = packet_start;
        packet = (ResidentRenderPacket *)(group->blocks + (packet_start - packet_extent) * 16);
        for (j = 0; j < packet_extent; j += 4) {
            material_index = packet->material_index;
            if (material_index >= 0) {
                material_index = slot_materials->b[material_index];
            }
            if (textures != NULL) {
                build_indexed_resident_render_packet(packet, textures + material_index * 16, packet->draw_high, packet->draw_shift, packet->material_base, packet->material_shift, material_index);
            } else {
                set_up_vis_gif_viewer(packet, packet->draw_high, packet->draw_shift, packet->material_base, packet->material_shift, material_index);
            }
            packet++;
        }
    }
}
#endif /* NON_MATCHING */

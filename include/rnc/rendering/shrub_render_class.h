#ifndef LOMBYTE_RNC_RENDERING_SHRUB_RENDER_CLASS_H
#define LOMBYTE_RNC_RENDERING_SHRUB_RENDER_CLASS_H

#include "types.h"
#include "rnc/rendering/resident_class.h"

typedef struct {
    s16 width;
    s16 height;
    s16 draw_control_count;
    s16 texture_block_offset;
    s16 palette_block_offset;
    s16 mip_block_offset_0;
    s16 mip_block_offset_1;
    s16 mip_block_offset_2;
} ShrubMipTextureDefinition;
typedef struct {
    s32 draw_high;
    s32 draw_shift;
    u8 pad8[8];
    s32 material_base;
    s32 material_shift;
    u8 pad18[8];
    s32 material_index;
    u8 pad24[0x1C];
} ShrubRenderPacket;
typedef struct {
    u8 pad0[0x10];
    union {
        u64 d;
        s32 w[2];
    } tex1;
    u64 payload18;
    union {
        u64 d;
        s32 w[2];
    } tex0;
    u64 payload28;
    u64 miptbp1;
    u64 payload38;
} ShrubMipPacket;
typedef struct {
    s32 count;
    s32 prefix_record_count;
} ShrubRenderGroupHeader;
typedef struct {
    u8 pad0[0x10];
    ShrubRenderGroupHeader group_header;
} ShrubRenderGroup;
typedef struct {
    ShrubRenderGroup *group;
    s32 pad4;
} ShrubRenderGroupReference;
typedef struct {
    u8 pad0[0x10];
    f32 scale;
    u8 pad14[2];
    s16 runtime_count;
    s32 runtime_data;
    ShrubMipPacket *mip_packet;
    u8 pad20[6];
    u16 class_slot;
    s16 group_count;
    u8 pad2A[2];
    s32 packed_geometry;
    u8 pad30[0x10];
    ShrubRenderGroupReference groups[1];
} ShrubRenderClass;

extern ShrubRenderClass *shrub_render_classes[64] __asm__("D_001D7F30");
extern s16 shrub_render_class_ids[64] __asm__("D_001D8030");
/* Class id -> slot in shrub_render_classes. Two more tables of the same size follow
   (D_001D84B0, D_001D88B0), so this one spans exactly the 0x400 bytes up to D_001D84B0. */
extern u8 shrub_render_class_slot_by_id[0x400] __asm__("D_001D80B0");
extern s32 shrub_render_class_fixed_thresholds[64] __asm__("D_001D8CB0");
extern MaterialMap shrub_render_class_material_maps[64] __asm__("D_001D92B0");

#endif /* LOMBYTE_RNC_RENDERING_SHRUB_RENDER_CLASS_H */

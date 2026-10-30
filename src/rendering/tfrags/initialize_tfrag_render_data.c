/* Ported from rac1-decomp (src/game/loaders.c: func_00204918). */
#include "types.h"
#include "rnc/rendering/resident_class.h"
#include "rnc/rendering/render_globals.h"

typedef struct {
    u64 data;
    u64 address;
} GifAD;

typedef struct {
    s32 texture_index;
    s32 pad4[3];
    s32 draw_high;
    s32 draw_shift;
    s32 pad18[2];
    s32 material_base;
    s32 material_shift;
    s32 pad28[10];
} TfragMaterialPacket;

typedef struct {
    u8  pad0[0x10];
    u8 *data;
    u8  pad14[8];
    u16 material_packet_offset;
    u8  pad1E[0xA];
    u8  count;
    u8  pad29[0x17];
} TfragRenderRecord;

typedef struct {
    s32 records_offset;
    s32 count;
    f32 scale;
} TfragRenderHeader;

extern s32                tfrag_render_record_count __asm__("D_00160E90");
extern f32                tfrag_render_ranges[3] __asm__("D_00160EA0");
extern TfragRenderRecord *tfrag_render_records __asm__("D_00160E8C");
extern s32                log2dim(s32) __asm__("func_001F97A0");
extern void               set_tfrag_dists(void) __asm__("func_00233068");

void initialize_tfrag_render_data(
    TfragRenderHeader *header, ResidentRenderTextureDefinition *textures) __asm__("FUN_002040e0");

void initialize_tfrag_render_data(TfragRenderHeader *header,

                                  ResidentRenderTextureDefinition *textures) {
    TfragRenderRecord *records;
    f32                scale;
    s32                record_index;
    s32                packet_index;
    s32                record_count;
    s32                index;

    tfrag_render_record_count = header->count;
    scale = *(f32 *)((u8 *)header + 8);
    tfrag_render_ranges[0] = scale * 6.0f;
    tfrag_render_ranges[1] = scale * 4.0f;
    tfrag_render_ranges[2] = scale + scale;
    set_tfrag_dists();
    records = (TfragRenderRecord *)((u8 *)header + header->records_offset);
    tfrag_render_records = records;
    record_count = tfrag_render_record_count;
    for (index = 0; index < record_count; index++) {
        records[index].data = (u8 *)records + (s32)records[index].data;
    }
    for (record_index = 0; record_index < tfrag_render_record_count; record_index++) {
        for (packet_index = 0; packet_index < tfrag_render_records[record_index].count;
             packet_index++) {
            TfragRenderRecord *record =
                (TfragRenderRecord *)((record_index << 6) + (s32)tfrag_render_records);
            u64 *packet =
                (u64 *)(record->data + record->material_packet_offset + packet_index * 0x50);
            s32 texture_index = ((TfragMaterialPacket *)packet)->texture_index;
            s32 draw_high = ((TfragMaterialPacket *)packet)->draw_high;
            s32 draw_shift = ((TfragMaterialPacket *)packet)->draw_shift;
            s32 material_base = ((TfragMaterialPacket *)packet)->material_base;
            s32 material_shift = ((TfragMaterialPacket *)packet)->material_shift;
            ResidentRenderTextureDefinition *texture = &textures[texture_index];
            s32                              bits = (u16)texture->width << 16;
            s32                              width_units_64 = bits >> 22;
            s32                              width_units_128 = bits >> 23;
            s64                              width_log2;
            s32                              height_log2;
            s32                              gs_block_base;
            u64                              tex0_word;
            u64                              tex1_word;
            u64                              clamp_word;
            u64                              mip_word;

            if (width_units_64 <= 0) {
                width_units_64 = 1;
            }
            if (width_units_128 <= 0) {
                width_units_128 = 1;
            }
            width_log2 = log2dim(bits >> 16);
            height_log2 = log2dim(texture->height);
            gs_block_base = gs_texture_allocation_base >> 8;
            /* Fakematch: the signed/unsigned casts keep game cc1 from regrouping the ORs. */
            tex0_word = (s64)((u64)(((s64)width_units_64 << 14) | ((s64)0x13 << 20) |
                                    ((s64)width_log2 << 26)) |
                              ((u64)height_log2 << 30) | ((u64)1 << 34) |
                              ((u64)(texture->clut_block_offset + gs_block_base) << 37)) |
                        ((s64)4 << 61);
            bits = texture->draw_control_count - 1;
            tex1_word = (s64)(((u64)bits << 2) | ((u64)1 << 5) | ((u64)draw_shift << 6)) |
                        ((s64)(u32)draw_high << 32);
            clamp_word =
                (u64)material_base | ((u64)material_shift << 2) | ((u64)texture_index << 24);
            mip_word =
                (s64)(((u64)width_units_128 << 14) |
                      ((u64)(texture->mip_block_offset_0 + gs_block_base) << 20) | ((u64)1 << 34) |
                      ((u64)(texture->mip_block_offset_1 + gs_block_base) << 40)) |
                ((s64)1 << 54);
            *packet = tex0_word;
            packet += 2;
            *packet = tex1_word;
            packet += 2;
            *packet = clamp_word;
            packet += 2;
            *packet = mip_word;
            ((GifAD *)packet)[1].data = 0;
        }
    }
}

extern __typeof__(initialize_tfrag_render_data) func_002040E0
    __attribute__((alias("FUN_002040e0")));

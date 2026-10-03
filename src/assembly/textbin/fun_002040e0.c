#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002040e0/FUN_002040e0.s", FUN_002040e0);
#else
#include "types.h"

#define SCE_GS_SET_TEX0(tbp, width_units_64, psm, width_log2, height_log2, tcc, tfx, cbp, cpsm, csm, csa, cld) \
    ((u64)(tbp) | ((u64)(width_units_64) << 14) | ((u64)(psm) << 20) | ((u64)(width_log2) << 26) | \
    ((u64)(height_log2) << 30) | ((u64)(tcc) << 34) | ((u64)(tfx) << 35) | ((u64)(cbp) << 37) | \
    ((u64)(cpsm) << 51) | ((u64)(csm) << 55) | ((u64)(csa) << 56) | ((u64)(cld) << 61))
#define SCE_GS_SET_TEX1(lcm, mxl, mmag, mmin, mtba, l, k) \
    ((u64)(lcm) | ((u64)(mxl) << 2) | ((u64)(mmag) << 5) | ((u64)(mmin) << 6) | \
    ((u64)(mtba) << 9) | ((u64)(l) << 19) | ((u64)(k) << 32))
#define SCE_GS_SET_CLAMP(wms, wmt, minu, maxu, minv, maxv) \
    ((u64)(wms) | ((u64)(wmt) << 2) | ((u64)(minu) << 4) | ((u64)(maxu) << 14) | \
    ((u64)(minv) << 24) | ((u64)(maxv) << 34))
#define SCE_GS_SET_MIPTBP1(tbp1, tbw1, tbp2, width_units_128, tbp3, tbw3) \
    ((u64)(tbp1) | ((u64)(tbw1) << 14) | ((u64)(tbp2) << 20) | ((u64)(width_units_128) << 34) | \
    ((u64)(tbp3) << 40) | ((u64)(tbw3) << 54))

typedef struct {
    s32 texture_index;
    s16 width;
    s16 height;
    s16 draw_control_count;
    s16 clut;
    s16 mip_block_offset_0;
    s16 mip_block_offset_1;
} ResidentRenderTextureDefinition;

typedef struct {
    u64 data;
    u64 address;
} GifAD;

typedef struct {
    s32 texture_index;
    s32 pad4[3];
    s32 draw_high;
    s64 draw_shift;
    s32 pad18[2];
    s64 material_base;
    s64 material_shift;
    s32 pad28[10];
} TfragMaterialPacket;

typedef struct {
    u8 pad0[0x10];
    u8 *data;
    u8 pad14[8];
    u16 material_packet_offset;
    u8 pad1E[0xA];
    u8 count;
    u8 pad29[0x17];
} TfragRenderRecord;

typedef struct {
    s32 records_offset;
    s32 count;
    f32 scale;
} TfragRenderHeader;

extern f32 tfrag_render_ranges[3] __asm__("D_00160EA0");
extern TfragRenderRecord *tfrag_render_records __asm__("D_00160E8C");
typedef struct { s32 count; } TfragRecordCount;
extern TfragRecordCount tfrag_render_record_count __asm__("D_00160E90");
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");

extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
extern void set_tfrag_dists(f32 *) __asm__("func_00233068");

void initialize_tfrag_render_data(TfragRenderHeader *header, ResidentRenderTextureDefinition *textures) __asm__("FUN_002040e0");

void initialize_tfrag_render_data(TfragRenderHeader *header, ResidentRenderTextureDefinition *textures)
{
    TfragRenderRecord *records;
    GifAD *packet;
    ResidentRenderTextureDefinition *texture;
    s32 record_count;
    s32 record_index;
    s32 packet_index;
    s32 texture_index;
    s32 draw_high;
    s64 draw_shift;
    s64 material_base;
    s64 material_shift;
    s32 width;
    s64 width_units_64;
    s64 width_units_128;
    s32 width_log2;
    s32 height_log2;
    s32 gs_block_base;
    f32 range_scale;
    u64 tex0_word;
    u64 tex1_word;
    u64 clamp_word;
    u64 mip_word;

    tfrag_render_record_count.count = header->count;
    range_scale = header->scale;
    tfrag_render_ranges[0] = range_scale * 6.0f;
    tfrag_render_ranges[1] = range_scale * 4.0f;
    tfrag_render_ranges[2] = range_scale + range_scale;
    set_tfrag_dists(tfrag_render_ranges);
    records = (TfragRenderRecord *)((u8 *)header + header->records_offset);
    tfrag_render_records = records;
    record_count = tfrag_render_record_count.count;
    /* Serialized packet-data offsets are relative to the record table. */
    for (record_index = 0; record_index < record_count; record_index++) {
        records[record_index].data = (u8 *)records + (s32)records[record_index].data;
    }
    for (record_index = 0; record_index < tfrag_render_record_count.count; record_index++) {
        for (packet_index = 0; packet_index < tfrag_render_records[record_index].count; packet_index++) {
            packet = (GifAD *)(tfrag_render_records[record_index].data + tfrag_render_records[record_index].material_packet_offset + packet_index * 0x50);
            texture_index = ((TfragMaterialPacket *)packet)->texture_index;
            material_base = ((TfragMaterialPacket *)packet)->material_base;
            draw_high = ((TfragMaterialPacket *)packet)->draw_high;
            draw_shift = ((TfragMaterialPacket *)packet)->draw_shift;
            texture = &textures[texture_index];
            material_shift = ((TfragMaterialPacket *)packet)->material_shift;
            width = texture->width;
            width_units_64 = width >> 6;
            width_units_128 = width >> 7;
            if (width_units_128 <= 0) {
                width_units_128 = 1;
            }
            if (width_units_64 <= 0) {
                width_units_64 = 1;
            }
            width_log2 = highest_set_bit_index(width);
            height_log2 = highest_set_bit_index(texture->height);
            gs_block_base = gs_texture_allocation_base >> 8;
            tex0_word = SCE_GS_SET_TEX0(0, width_units_64, 0x13, width_log2, height_log2, 1, 0, texture->clut + gs_block_base, 0, 0, 0, 4);
            tex1_word = SCE_GS_SET_TEX1(0, texture->draw_control_count - 1, 1, draw_shift, 0, 0, draw_high);
            clamp_word = SCE_GS_SET_CLAMP(material_base, material_shift, 0, 0, texture_index, 0);
            mip_word = SCE_GS_SET_MIPTBP1(0, width_units_128, texture->mip_block_offset_0 + gs_block_base, 1, texture->mip_block_offset_1 + gs_block_base, 1);
            /* Preserve the interleaved payload words; clear only the runtime
               address word at packet offset 0x40. */
            packet->data = tex0_word;
            packet++;
            packet->data = tex1_word;
            packet++;
            packet->data = clamp_word;
            packet++;
            packet->data = mip_word;
            packet[1].data = 0;
        }
    }
}

extern __typeof__(initialize_tfrag_render_data) func_002040E0 __attribute__((alias("FUN_002040e0")));

#endif /* NON_MATCHING */

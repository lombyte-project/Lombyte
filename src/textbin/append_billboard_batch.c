#include "types.h"
#include "asm.h"

#include "types.h"
#include "qcopy.h"

/* One glow billboard; append_billboard_batch draws the active ones. */
typedef struct BillboardRecord {
    f32 position[4]; /* x, y, z, w */
    s16 active_count;
    s16 alpha;
    u8 pad14[4];
    f32 angle;
    f32 radius_scale;
} BillboardRecord; /* size 0x20 */

extern BillboardRecord billboard_records[16] __asm__("D_0018ED00");

typedef struct {
    f32 x, y, z, w;
} Vec4;

typedef struct {
    u8 pad0[0x1A8];
    f32 depth_offset;
    u8 pad1AC[0x64];
    f32 projection_scale;
} BillboardViewContext;

#include "rnc/rendering/dma_tag.h"

extern char billboard_quad_header[] __asm__("D_001608E0");
extern u8 camera_position[] __asm__("D_00187080");
extern BillboardViewContext view_context __asm__("D_0018CD00");
extern s32 clip_transform __asm__("D_0018CE80");
extern void FillTransferWords(void *, s32, s32);
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern s32 is_vector_outside_clip(Vec4 *) __asm__("func_001F9958");
extern void fast_vec_sub(Vec4 *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_scale(Vec4 *, Vec4 *, f32) __asm__("func_001F9A68");
extern void multiply_vector_components(Vec4 *, Vec4 *, void *) __asm__("func_001F9A98");
extern f32 fast_vec_length(Vec4 *) __asm__("func_001F9AF0");
extern void transform_vector(Vec4 *, Vec4 *, void *) __asm__("func_001F9D20");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

void append_billboard_batch(void) __asm__("FUN_001f92b0");

void append_billboard_batch(void) {
    Vec4 projected_position;
    Vec4 clip_position;
    f32 distance;
    f32 radius;
    s32 record_index;
    BillboardRecord *record;
    s64 color;
    s64 x;
    s32 y;
    s64 packed_position;
    s32 sine_offset;
    s32 cosine_offset;
    struct DmaTag *tag;
    s64 *packet_words;
    BillboardRecord *records;

    vu1_add_g_sregister(0x42, 0x8000000048);
    for (record_index = 0; record_index < 16; record_index++) {
        records = billboard_records;
        record = records + record_index;
        if (record->active_count <= 0) {
            continue;
        }
        fast_vec_sub(&projected_position, record, camera_position);
        projected_position.w = 1.0f;
        distance = fast_vec_length(&projected_position);
        fast_vec_scale(&projected_position, &projected_position, 1024.0f);
        transform_vector(&projected_position, &projected_position, camera_position - 0x100);
        multiply_vector_components(&clip_position, &projected_position,
                                   (char *)&view_context + 0x180);
        if (is_vector_outside_clip(&clip_position) != 0) {
            FillTransferWords(record, 0, 0x20);
            continue;
        }
        fast_vec_scale(&projected_position, &projected_position,
                       view_context.projection_scale / projected_position.w);
        color = (record->alpha << 24) | 0x808080;
        x = convert_float_to_integer(projected_position.x * 16.0f) + 0x8000;
        y = convert_float_to_integer(projected_position.y * 16.0f) + 0x8000;
        packed_position = ((s64)convert_float_to_integer(projected_position.z * 0.9997f +
                                                         view_context.depth_offset)
                           << 32) |
                          ((s64)y << 16) | x;
        if (distance > 18.0f) {
            distance = 18.0f;
        } else if (distance < 2.0f) {
            distance = 2.0f;
        }
        radius = record->radius_scale * (convert_integer_to_float(record->alpha + 16) * 0.015625f) *
                 ((24.0f - distance) * 16.0f);
        sine_offset = convert_float_to_integer(radius * fast_sin(record->angle));
        cosine_offset = convert_float_to_integer(radius * fast_cos(record->angle));
        render_packet_cursor.tag->tag = 0x10000009;
        render_packet_cursor.tag->addr = 0;
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = 0x50000009;
        tag = render_packet_cursor.tag;
        render_packet_cursor.tag = tag + 1;
        qcopy(tag + 1, billboard_quad_header);
        packet_words = (s64 *)(tag + 2);
        render_packet_cursor.tag = tag + 2;
        packet_words[0] = 5;
        packet_words[1] = get_effect_texture(0x13);
        packet_words[2] = 0x154;
        packet_words[3] = color;
        packet_words[4] = 0;
        packet_words[5] = packed_position + (cosine_offset << 16) + sine_offset;
        packet_words[6] = color;
        packet_words[7] = 0x200;
        packet_words[8] = packed_position + (-sine_offset << 16) + cosine_offset;
        packet_words[9] = color;
        packet_words[10] = 0x2000000;
        packet_words[11] = packed_position + (sine_offset << 16) - cosine_offset;
        packet_words[12] = color;
        packet_words[13] = 0x2000200;
        packet_words[14] = packed_position + (-cosine_offset << 16) - sine_offset;
        packet_words[15] = 0;
        render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x80);
    }
    vu1_add_g_sregister(0x42, 0x8000000044);
}

extern __typeof__(append_billboard_batch) func_001F92B0 __attribute__((alias("FUN_001f92b0")));

BillboardRecord billboard_records[16] = {0};

#ifndef LOMBYTE_RNC_RENDERING_OBJECT_RENDER_CLASS_H
#define LOMBYTE_RNC_RENDERING_OBJECT_RENDER_CLASS_H

#include "types.h"
#include "rnc/rendering/resident_class.h"

typedef struct {
    u64 tex0;
    u64 pad8;
    union {
        u64 d;
        s32 w[2];
    } tex1;
    u64 pad18;
    u64 mip;
    u64 pad28;
    union {
        u64 d;
        s32 w[2];
    } clamp;
    u64 pad38;
    u64 end;
    u64 pad48;
} ObjectRenderRecord;
typedef struct {
    s32 offset;
    u8 pad4[0xC];
} RenderStreamEntry;
typedef struct {
    RenderStreamEntry *streams[3];
    s32 packed_normals;
    u8 pad10[0x10];
    u8 stream_counts[3];
    u8 record_count;
    u8 pad24[2];
    s16 runtime_index;
    s32 runtime_data;
    ObjectRenderRecord *records;
    u8 pad30[0x16];
    u16 class_slot;
    f32 scale;
} ObjectRenderClass;

extern ObjectRenderClass *object_render_classes[128] __asm__("D_001E1700");
extern s16 object_render_class_ids[128] __asm__("D_001E1900");
extern s32 object_render_class_fixed_thresholds[128] __asm__("D_001E2E00");
extern MaterialMap object_render_class_material_maps[128] __asm__("D_001E3600");

#endif /* LOMBYTE_RNC_RENDERING_OBJECT_RENDER_CLASS_H */

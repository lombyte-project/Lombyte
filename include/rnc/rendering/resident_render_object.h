#ifndef LOMBYTE_RNC_RENDERING_RESIDENT_RENDER_OBJECT_H
#define LOMBYTE_RNC_RENDERING_RESIDENT_RENDER_OBJECT_H

#include "types.h"

/* The resident gameplay objects update_resident_gameplay_state drives
   (level_render_state.player and the playback object). */
typedef union {
    u128 quadword;
    f32  components[4];
    u8   bytes[16];
} Vector4;

typedef struct {
    u8  pad0[6];
    u8  metal_count;
    u8  pad7[5];
    u8  count; /* 0x0C */
    u8  padD[0x48 - 0xD];
    s32 frames[1]; /* 0x48 */
} RenderModel;

typedef struct ResidentRenderObject {
    u8           pad0[0x10];
    Vector4      position; /* 0x10 */
    u8           state;    /* 0x20 */
    u8           pad21[3];
    RenderModel *model; /* 0x24 */
    u8           pad28[0xA];
    s16          selected_index; /* 0x32 */
    u16          flags;          /* 0x34 */
    u8           pad36[2];
    s64          lights;   /* 0x38: Moby light state, copied from the hero moby */
    Vector4      rotation; /* 0x40 */
    u8           current_frame;
    u8           next_frame;
    u8           selected_a;
    u8           selected_b;
    f32          blend; /* 0x54 */
    u8           pad58[0x71 - 0x58];
    u8           cached_selector;
    u8           lod_dist;
    u8           metal_alpha;
    u8           pad74[4];
    u8          *animation_positions; /* 0x78 */
    u8           pad7C[3];
    u8           shadow_range;
    u8           pad80[0x10];
    s32          glow_color; /* 0x90 */
    s32          collision;  /* 0x94 */
    u8           pad98[0xE];
    s16          class_id; /* 0xA6 */
    u8           padA8[0xA];
    s16          quad_alpha_jitter; /* 0xB2: build_variant_effect_quads adds a random 0..n-1 */
    u8           padB4[8];
    u8           quad_alpha; /* 0xBC: build_variant_effect_quads base alpha */
    u8           padBD[3];
    Vector4      transform[4]; /* 0xC0 */
} ResidentRenderObject;

#endif /* LOMBYTE_RNC_RENDERING_RESIDENT_RENDER_OBJECT_H */

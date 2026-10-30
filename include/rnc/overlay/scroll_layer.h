#ifndef LOMBYTE_RNC_OVERLAY_SCROLL_LAYER_H
#define LOMBYTE_RNC_OVERLAY_SCROLL_LAYER_H

#include "types.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/mesh_batch.h"

/* One scrolling two-texture overlay layer (0x60 bytes), the element
   update_and_draw_scrolling_overlay_layers (FUN_L01_002b96e0) steps through
   and FUN_L01_00262618 turns into a packet. verts and uvs each
   hold count entries. */
typedef struct {
    Vec4f *verts; /* 0x00: count vertices (z and w lowered and rippled by rippling_scroll_layers) */
    MeshUv
        *uvs; /* 0x04: count uv pairs, one per vertex, read 8 bytes at a time by FUN_L01_00262618 */
    Vec4f *bounds; /* 0x08: sphere tested against the view (computed by bounded_scroll_layers) */
    s32    count;  /* 0x0C: vertex count, also passed to FUN_L01_0021fa98 */
    f32    rate0;  /* 0x10 */
    f32    rate1;  /* 0x14 */
    f32    unk18;  /* 0x18: loaded with rate0/rate1 as one vector */
    s32    frame_step; /* 0x1C */
    f32    du0;        /* 0x20 */
    f32    dv0;        /* 0x24 */
    f32    du1;        /* 0x28 */
    f32    dv1;        /* 0x2C */
    u32    color;      /* 0x30: copied into the packet by FUN_L01_00262618 */
    s32    tex0;       /* 0x34 */
    s32    tex1;       /* 0x38 */
    u8     alpha0;     /* 0x3C */
    u8     alpha1;     /* 0x3D */
    u8     pad3E[2];
    f32    u0;    /* 0x40 */
    f32    v0;    /* 0x44 */
    f32    u1;    /* 0x48 */
    f32    v1;    /* 0x4C */
    s32    frame; /* 0x50 */
    u8     pad54[8];
    f32    glow; /* 0x5C: pulsed by rippling/pulsing_scroll_layers */
} ScrollLayer;

/* Level 01 layer sets. Each layer's bounding sphere is one Vec4f in .lit
   (x, y, z, radius: fast_bsphere_check reads it, bounded_scroll_layers writes it). */
extern Vec4f l01_scrolling_overlay_layer_0_bounds __asm__("D_L01_00161B20");
extern Vec4f l01_scrolling_overlay_layer_1_bounds __asm__("D_L01_00161B30");
extern Vec4f l01_scrolling_overlay_layer_2_bounds __asm__("D_L01_00161B40");
extern Vec4f l01_scrolling_overlay_layer_3_bounds __asm__("D_L01_00161B50");
extern Vec4f l01_scrolling_overlay_layer_4_bounds __asm__("D_L01_00161B60");
extern Vec4f l01_scrolling_overlay_layer_5_bounds __asm__("D_L01_00161B70");
extern Vec4f l01_scrolling_overlay_layer_6_bounds __asm__("D_L01_00161B80");
extern Vec4f l01_scrolling_overlay_layer_7_bounds __asm__("D_L01_00161B90");
extern Vec4f bounded_scroll_layer_0_bounds __asm__("D_L01_00161BB0");
extern Vec4f bounded_scroll_layer_1_bounds __asm__("D_L01_00161BC0");
extern Vec4f rippling_scroll_layer_0_bounds __asm__("D_L01_00161CF0");
extern Vec4f rippling_scroll_layer_1_bounds __asm__("D_L01_00161D00");
extern Vec4f rippling_scroll_layer_2_bounds __asm__("D_L01_00161D10");
extern Vec4f rippling_scroll_layer_3_bounds __asm__("D_L01_00161D20");
extern Vec4f l01_sine_modulated_scroll_layer_0_bounds __asm__("D_L01_00162010");
extern Vec4f l01_sine_modulated_scroll_layer_1_bounds __asm__("D_L01_00162020");
extern Vec4f l01_sine_modulated_scroll_layer_2_bounds __asm__("D_L01_00162030");

/* Level 01 layer sets. Each set's vertex arrays sit just before it, sized by
   each layer's count. */
extern Vec4f l01_scrolling_overlay_layer_0_verts[0x26] __asm__("D_L01_001E1640");
extern f32   l01_scrolling_overlay_layer_0_uvs[0x26][2] __asm__("D_L01_001E18A0");
extern Vec4f l01_scrolling_overlay_layer_1_verts[0x1E] __asm__("D_L01_001E19D0");
extern f32   l01_scrolling_overlay_layer_1_uvs[0x1E][2] __asm__("D_L01_001E1BB0");
extern Vec4f l01_scrolling_overlay_layer_2_verts[0x26] __asm__("D_L01_001E1CA0");
extern f32   l01_scrolling_overlay_layer_2_uvs[0x26][2] __asm__("D_L01_001E1F00");
extern Vec4f l01_scrolling_overlay_layer_3_verts[0x1E] __asm__("D_L01_001E2030");
extern f32   l01_scrolling_overlay_layer_3_uvs[0x1E][2] __asm__("D_L01_001E2210");
extern Vec4f l01_scrolling_overlay_layer_4_verts[0x26] __asm__("D_L01_001E2300");
extern f32   l01_scrolling_overlay_layer_4_uvs[0x26][2] __asm__("D_L01_001E2560");
extern Vec4f l01_scrolling_overlay_layer_5_verts[0x1E] __asm__("D_L01_001E2690");
extern f32   l01_scrolling_overlay_layer_5_uvs[0x1E][2] __asm__("D_L01_001E2870");
extern Vec4f l01_scrolling_overlay_layer_6_verts[0x26] __asm__("D_L01_001E2960");
extern f32   l01_scrolling_overlay_layer_6_uvs[0x26][2] __asm__("D_L01_001E2BC0");
extern Vec4f l01_scrolling_overlay_layer_7_verts[0x1E] __asm__("D_L01_001E2CF0");
extern f32   l01_scrolling_overlay_layer_7_uvs[0x1E][2] __asm__("D_L01_001E2ED0");
/* Drawn 8 at a time by update_and_draw_level01_scrolling_layers. */
extern ScrollLayer l01_scrolling_overlay_layers[8] __asm__("D_L01_001E2FC0");

extern Vec4f bounded_scroll_layer_0_verts[4] __asm__("D_L01_001E32C0");
extern f32   bounded_scroll_layer_0_uvs[4][2] __asm__("D_L01_001E3300");
extern Vec4f bounded_scroll_layer_1_verts[4] __asm__("D_L01_001E3320");
extern f32   bounded_scroll_layer_1_uvs[4][2] __asm__("D_L01_001E3360");
/* bounded_scroll_layers: 2 layers (loops i < 2, draw count 2). */
extern ScrollLayer bounded_scroll_layers[2] __asm__("D_L01_001E3380");

extern Vec4f rippling_scroll_layer_0_verts[0x30] __asm__("D_L01_001FAB40");
extern f32   rippling_scroll_layer_0_uvs[0x30][2] __asm__("D_L01_001FAE40");
extern Vec4f rippling_scroll_layer_1_verts[0x30] __asm__("D_L01_001FAFC0");
extern f32   rippling_scroll_layer_1_uvs[0x30][2] __asm__("D_L01_001FB2C0");
extern Vec4f rippling_scroll_layer_2_verts[0x30] __asm__("D_L01_001FB440");
extern f32   rippling_scroll_layer_2_uvs[0x30][2] __asm__("D_L01_001FB740");
extern Vec4f rippling_scroll_layer_3_verts[0x28] __asm__("D_L01_001FB8C0");
extern f32   rippling_scroll_layer_3_uvs[0x28][2] __asm__("D_L01_001FBB40");
/* rippling_scroll_layers: 4 layers (loops i < 4, draw count 4). */
extern ScrollLayer rippling_scroll_layers[4] __asm__("D_L01_001FBC80");

extern Vec4f l01_sine_modulated_scroll_layer_0_verts[0x5E] __asm__("D_L01_00201660");
extern f32   l01_sine_modulated_scroll_layer_0_uvs[0x5E][2] __asm__("D_L01_00201C40");
extern Vec4f l01_sine_modulated_scroll_layer_1_verts[0x46] __asm__("D_L01_00201F30");
extern f32   l01_sine_modulated_scroll_layer_1_uvs[0x46][2] __asm__("D_L01_00202390");
extern Vec4f l01_sine_modulated_scroll_layer_2_verts[0x52] __asm__("D_L01_002025C0");
extern f32   l01_sine_modulated_scroll_layer_2_uvs[0x52][2] __asm__("D_L01_00202AE0");
/* pulsing_scroll_layers: 3 layers (loop i = 2..0, draw count 3). */
extern ScrollLayer l01_sine_modulated_scroll_layers[3] __asm__("D_L01_00202D80");

#endif /* LOMBYTE_RNC_OVERLAY_SCROLL_LAYER_H */

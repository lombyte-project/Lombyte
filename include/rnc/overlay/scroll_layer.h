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
extern Vec4f D_L01_00161B20;
extern Vec4f D_L01_00161B30;
extern Vec4f D_L01_00161B40;
extern Vec4f D_L01_00161B50;
extern Vec4f D_L01_00161B60;
extern Vec4f D_L01_00161B70;
extern Vec4f D_L01_00161B80;
extern Vec4f D_L01_00161B90;
extern Vec4f D_L01_00161BB0;
extern Vec4f D_L01_00161BC0;
extern Vec4f D_L01_00161CF0;
extern Vec4f D_L01_00161D00;
extern Vec4f D_L01_00161D10;
extern Vec4f D_L01_00161D20;
extern Vec4f D_L01_00162010;
extern Vec4f D_L01_00162020;
extern Vec4f D_L01_00162030;

/* Level 01 layer sets. Each set's vertex arrays sit just before it, sized by
   each layer's count. */
extern Vec4f D_L01_001E1640[0x26];
extern f32   D_L01_001E18A0[0x26][2];
extern Vec4f D_L01_001E19D0[0x1E];
extern f32   D_L01_001E1BB0[0x1E][2];
extern Vec4f D_L01_001E1CA0[0x26];
extern f32   D_L01_001E1F00[0x26][2];
extern Vec4f D_L01_001E2030[0x1E];
extern f32   D_L01_001E2210[0x1E][2];
extern Vec4f D_L01_001E2300[0x26];
extern f32   D_L01_001E2560[0x26][2];
extern Vec4f D_L01_001E2690[0x1E];
extern f32   D_L01_001E2870[0x1E][2];
extern Vec4f D_L01_001E2960[0x26];
extern f32   D_L01_001E2BC0[0x26][2];
extern Vec4f D_L01_001E2CF0[0x1E];
extern f32   D_L01_001E2ED0[0x1E][2];
/* Drawn 8 at a time by update_and_draw_level01_scrolling_layers. */
extern ScrollLayer l01_scrolling_overlay_layers[8] __asm__("D_L01_001E2FC0");

extern Vec4f D_L01_001E32C0[4];
extern f32   D_L01_001E3300[4][2];
extern Vec4f D_L01_001E3320[4];
extern f32   D_L01_001E3360[4][2];
/* bounded_scroll_layers: 2 layers (loops i < 2, draw count 2). */
extern ScrollLayer bounded_scroll_layers[2] __asm__("D_L01_001E3380");

extern Vec4f D_L01_001FAB40[0x30];
extern f32   D_L01_001FAE40[0x30][2];
extern Vec4f D_L01_001FAFC0[0x30];
extern f32   D_L01_001FB2C0[0x30][2];
extern Vec4f D_L01_001FB440[0x30];
extern f32   D_L01_001FB740[0x30][2];
extern Vec4f D_L01_001FB8C0[0x28];
extern f32   D_L01_001FBB40[0x28][2];
/* rippling_scroll_layers: 4 layers (loops i < 4, draw count 4). */
extern ScrollLayer rippling_scroll_layers[4] __asm__("D_L01_001FBC80");

extern Vec4f D_L01_00201660[0x5E];
extern f32   D_L01_00201C40[0x5E][2];
extern Vec4f D_L01_00201F30[0x46];
extern f32   D_L01_00202390[0x46][2];
extern Vec4f D_L01_002025C0[0x52];
extern f32   D_L01_00202AE0[0x52][2];
/* pulsing_scroll_layers: 3 layers (loop i = 2..0, draw count 3). */
extern ScrollLayer l01_sine_modulated_scroll_layers[3] __asm__("D_L01_00202D80");

#endif /* LOMBYTE_RNC_OVERLAY_SCROLL_LAYER_H */

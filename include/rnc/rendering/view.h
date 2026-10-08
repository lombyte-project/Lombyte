#ifndef LOMBYTE_RNC_RENDERING_VIEW_H
#define LOMBYTE_RNC_RENDERING_VIEW_H

#include "types.h"
#include "rnc/math/vector.h"

/* View and projection state at D_0018CD00. init_view_context and
   configure_graphics_projection (FUN_001f33b8) set the clip distances,
   field of view, screen size and fog endpoints; update_view_context
   derives the projection matrices, scales and fog packet words. */
struct View {
    s32 fog_color;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    Vec4 regs[5];
    Vec4 gif[4];
    f32 near_clip;
    f32 far_clip;
    f32 aspect_x;
    f32 aspect_y;
    Vec4 fov;
    f32 proj[16];
    f32 proj2[16];
    f32 proj3[16];
    Vec4 inverse_screen_scale;
    Vec4 screen_scale;
    Vec4 screen_bias;
    Vec4 inverse_projection_scale;
    Vec4 viewport_aspect;
    Vec4 aspect_scale;
    Vec4 clip_scale;
    Vec4 clip_distances;
    f32 half_width;   /* 0x200: display width * 0.5 (init_view_context) */
    f32 half_height;  /* 0x204: display height * 0.5 */
    f32 scr_x;
    f32 scr_y;
    f32 fog_mul;
    f32 fog_add;
    f32 fog_near_dist;
    f32 fog_far_dist;
    f32 fog_slope;
    f32 fog_base;
    f32 fog_near_int;
    f32 fog_far_int;
    s32 fog_r;        /* 0x230: GS FOGCOL (register 0x3D) red, reset_gs_registers */
    s32 fog_g;        /* 0x234 */
    s32 fog_b;        /* 0x238 */
};

extern struct View view_context __asm__("D_0018CD00");

#endif /* LOMBYTE_RNC_RENDERING_VIEW_H */

/* Level 18 billboard quad pair draw for mobys (no caller found in C). */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/rendering/effect_texture.h"

#include "sda.h"

typedef struct {
    float f[4];
} __attribute__((aligned(16))) Vec130;

typedef struct {
    float u, v;
} UVec130;

typedef struct {
    Vec130       corner[4];
    unsigned int color[4];
    UVec130      uv[4];
    long         unk70, tex, unk80, unk88;
} Qec130;

extern unsigned int l18_billboard_quad_pair_first_color __asm__("D_L18_001620A0")
    __attribute__((sda));
extern int   D_L18_00162054 __asm__("D_L18_00162054") __attribute__((sda));
extern float l18_billboard_quad_pair_first_corner_scale_base __asm__("D_L18_001620A8")
    __attribute__((sda));
extern float l18_billboard_quad_pair_second_corner_scale_base __asm__("D_L18_001620AC")
    __attribute__((sda));
extern Vec130 D_L18_001677C0 __asm__("D_L18_001677C0") NOT_SDA;

/* One corner and one (u, v) per quad corner (both draw loops run i < 4). */
Vec130 billboard_quad_pair_corners[4] __asm__("D_L18_001D9CA0") = {
    {{0.0f, -1.0f, 1.0f, 1.0f}},
    {{0.0f, -1.0f, -1.0f, 1.0f}},
    {{0.0f, 1.0f, 1.0f, 1.0f}},
    {{0.0f, 1.0f, -1.0f, 1.0f}},
};

UVec130 billboard_quad_pair_uvs[4] __asm__("D_L18_001D9CE0") = {
    {0},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
};

extern void  vscl_ec130(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  vmul_ec130(float, void *, void *) __asm__("FUN_001f9a68");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void  draw_ec130(void *, void *, int) __asm__("FUN_001f7d30");

void draw_moby_billboard_quad_pair(void *mv) __asm__("FUN_L18_002ec130");

void draw_moby_billboard_quad_pair(void *mv) {
    Qec130       quad;
    Vec130       quad_transform[4];
    Vec130      *moby_position;
    unsigned int uniform_quad_color;
    float        corner_scale;
    int          i;
    moby_position = (Vec130 *)mv;
    quad.tex = get_effect_texture(0xB);
    moby_position = (Vec130 *)((char *)moby_position + 0x10);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    fast_vec_sub(&quad_transform[3], &D_L18_001677C0, moby_position);
    vscl_ec130(&quad_transform[3], &quad_transform[3], 0.3f);
    fast_vec_add(&quad_transform[3], &quad_transform[3], moby_position);
    quad_transform[3].f[3] = 1.0f;
    fast_vec_sub(&quad_transform[0], &D_L18_001677C0, &quad_transform[3]);
    vscl_ec130(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], (Vec130 *)&hero.gravity_dir);
    vscl_ec130(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    uniform_quad_color = l18_billboard_quad_pair_first_color;
    quad.color[3] = uniform_quad_color;
    quad.color[2] = uniform_quad_color;
    quad.color[1] = uniform_quad_color;
    quad.color[0] = uniform_quad_color;
    corner_scale = l18_billboard_quad_pair_first_corner_scale_base + randf_sym(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        vmul_ec130(corner_scale, &quad.corner[i], &billboard_quad_pair_corners[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &quad_transform[0]);
        quad.uv[i].u = billboard_quad_pair_uvs[i].u;
        quad.uv[i].v = billboard_quad_pair_uvs[i].v;
    }
    draw_ec130(&quad, 0, 0);
    uniform_quad_color = D_L18_00162054;
    quad.color[3] = uniform_quad_color;
    quad.color[2] = uniform_quad_color;
    quad.color[1] = uniform_quad_color;
    quad.color[0] = uniform_quad_color;
    corner_scale = l18_billboard_quad_pair_second_corner_scale_base + randf_sym(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        vmul_ec130(corner_scale, &quad.corner[i], &billboard_quad_pair_corners[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &quad_transform[0]);
        quad.uv[i].u = billboard_quad_pair_uvs[i].u;
        quad.uv[i].v = billboard_quad_pair_uvs[i].v;
    }
    draw_ec130(&quad, 0, 0);
}

#include "types.h"

/* Lens flare: 16 sprites along the line from the flare source's screen
   position through the screen centre, faded by distance and by how far
   the source is off centre. */

typedef struct {
    u8 pad_0[0x10];
    f32 pos[4];
    u8 flags;
} FlareMoby;

typedef struct {
    FlareMoby *moby;
    s32 state;
    f32 intensity;
    s32 pad_c;
    s16 alpha[16];
    s16 tex[16];
    s32 color[16];
    f32 dist[16];
    f32 scale[16];
} LensFlare;


extern u8 D_00187080[];
#include "rnc/rendering/screen.h"
extern LensFlare D_00187300;

extern f32 FUN_001f9b48(void *, void *);
extern f32 ConvertIntegerToFloat(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern float AbsoluteFloat(float input) __asm__("func_001F99C0");
extern void project_to_screen(f32 *, void *) __asm__("FUN_001f2070");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("FUN_001f5450");

void FUN_001edc50(void) {
    f32 scr[4];
    f32 ctr[4];
    f32 d[4];
    f32 off[4];
    f32 at[4];
    f32 dist;
    f32 fade;
    f32 fx;
    f32 fy;
    f32 half;
    s32 i;
    s32 size;
    s64 tex;
    s32 rgba;
    s32 w;
    s32 x;
    s32 y;

    if (D_00187300.state != 1 || (D_00187300.moby->flags & 0x80)) {
        D_00187300.state = 0;
        return;
    }
    if (D_00187300.intensity <= 0.0f) {
        return;
    }
    dist = FUN_001f9b48(D_00187080, D_00187300.moby->pos);
    ctr[0] = ConvertIntegerToFloat(screen_extent.half_width);
    ctr[1] = ConvertIntegerToFloat(screen_extent.half_height);
    project_to_screen(scr, D_00187300.moby->pos);
    scr[0] = (scr[0] - ConvertIntegerToFloat(screen_extent.left)) * 0.0625f;
    scr[1] = (scr[1] - ConvertIntegerToFloat(screen_extent.top)) * 0.0625f;
    d[0] = ctr[0] - scr[0];
    d[1] = ctr[1] - scr[1];
    for (i = 0; i < 16; i++) {
        tex = get_effect_texture(D_00187300.tex[i] + 6);
        size = (i != 0) ? 0x20 : 0x40;
        off[0] = d[0] * D_00187300.dist[i];
        off[1] = d[1] * D_00187300.dist[i];
        at[0] = ctr[0] + off[0];
        at[1] = ctr[1] + off[1];
        fade = dist - 5.0f;
        if (fade > 1.0f) {
            fade = 1.0f;
        } else if (fade < 0.0f) {
            fade = 0.0f;
        }
        fx = AbsoluteFloat(d[0]) / ctr[0] * -5.0f + 5.5f;
        if (fx > 1.0f) {
            fx = 1.0f;
        } else if (fx < 0.0f) {
            fx = 0.0f;
        }
        fy = AbsoluteFloat(d[1]) / ctr[1] * -5.0f + 5.5f;
        if (fy > 1.0f) {
            fy = 1.0f;
        } else if (fy < 0.0f) {
            fy = 0.0f;
        }
        fade = fade * (fx * fy);
        rgba = (truncate_float_to_s32(fade * D_00187300.alpha[i]) << 24) |
               (D_00187300.color[i] & 0xFFFFFF);
        w = truncate_float_to_s32(size * D_00187300.scale[i]);
        half = w >> 1;
        x = truncate_float_to_s32(at[0] - half);
        y = truncate_float_to_s32(at[1] - half);
        draw_textured_quad(x, y, w, w, 0, 0, size, size, rgba, tex);
    }
    D_00187300.intensity = 0.0f;
}

extern __typeof__(FUN_001edc50) func_001EDC50 __attribute__((alias("FUN_001edc50")));

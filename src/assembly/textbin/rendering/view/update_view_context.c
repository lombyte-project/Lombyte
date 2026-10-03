#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/view/update_view_context/FUN_001f2d98.s", FUN_001f2d98);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "qzero.h"

typedef union { u128 q; f32 f[4]; s32 i[4]; } Vec4;

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
    f32 width;
    f32 height;
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
};

struct FogRegs2 { u8 pad0[0x28]; s32 mul; s32 max; s32 far_int; s32 near_int; };
struct Clip { Vec4 constants[3]; };

extern s32 D_0015ED80;
extern struct View view_context __asm__("D_0018CD00");
extern Vec4 D_0018CFC0[4];
extern f32 D_00160720[4];
extern f32 D_00160B30;
extern f32 D_001DFF50[];
extern f32 D_001E6B70[];
extern struct FogRegs2 D_001DE9B0;
extern struct FogRegs2 D_001DEA00;
extern Vec4 D_001DEA40[2];
extern Vec4 D_001DE710[3];
extern struct { u8 pad0[0x10]; s32 mul; s32 far_int; s32 near_int; u8 pad1c[0x44]; Vec4 constants[3]; u8 pad90[0x10]; s32 x; s32 y; } D_001DE740;

extern f32 func_001F9E90(f32, f32);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern void func_001F9838(void *, void *, s32);
extern void set_tfrag_dists(void) __asm__("func_00233068");

void update_view_context(void) __asm__("FUN_001f2d98");

void update_view_context(void) {
    struct View *view;
    struct View *packet_view;
    f32 depth_scale;
    f32 horizontal_clip_scale;
    f32 vertical_clip_scale;
    f32 fog_distance_range;
    f32 fog_intensity_range;
    f32 fog_distance_slope;
    f32 near_clip;
    f32 far_clip;
    f32 fog_multiplier;
    f32 horizontal_fov;
    f32 vertical_fov;
    f32 viewport_width;
    f32 viewport_height;
    s32 far_bits;
    s32 near_bits;
    s32 mul_bits;

    depth_scale = -8388080.0f;
    if (D_0015ED80 != 0) {
        view_context.fov.f[1] = view_context.fov.f[0] * 0.756f;
    } else {
        view_context.fov.f[1] = view_context.fov.f[0] * 0.775f;
    }
    view = &view_context;
    view->aspect_x = view->scr_x / view->width;
    view->aspect_y = view->scr_y / view->height;
    view->fov.f[2] = 1.0f / fast_cos(func_001F9E90(1.0f, view->fov.f[0]));
    view->fov.f[3] = 1.0f / fast_cos(func_001F9E90(1.0f, view->fov.f[1]));
    horizontal_clip_scale = 1.0f / fast_cos(func_001F9E90(1.0f, view->fov.f[0] * view->aspect_x));
    vertical_clip_scale = 1.0f / fast_cos(func_001F9E90(1.0f, view->fov.f[1] * view->aspect_y));
    horizontal_clip_scale /= view->fov.f[2];
    vertical_clip_scale /= view->fov.f[3];
    view->aspect_scale.f[0] = view->aspect_x;
    view->aspect_scale.f[1] = view->aspect_y;
    view->aspect_scale.f[2] = 1.0f / view->aspect_x;
    view->aspect_scale.f[3] = 1.0f / view->aspect_y;
    view->clip_scale.f[0] = view->fov.f[2];
    view->clip_scale.f[1] = view->fov.f[3];
    view->clip_scale.f[2] = (horizontal_clip_scale + vertical_clip_scale) * 0.5f;
    view->clip_distances.f[0] = view->far_clip;
    near_clip = view->near_clip;
    view->clip_distances.f[1] = near_clip;
    qcopy(&D_0018CFC0[0], &view->fov);
    qcopy(&D_0018CFC0[1], &view->aspect_scale);
    qcopy(&D_0018CFC0[2], &view->clip_scale);
    qcopy(&D_0018CFC0[3], &view->clip_distances);

    far_bits = *(s32 *)&view->fog_far_int;
    near_bits = *(s32 *)&view->fog_near_int;
    fog_distance_range = view->fog_far_dist - view->fog_near_dist;
    fog_intensity_range = view->fog_far_int - view->fog_near_int;
    fog_multiplier = fog_intensity_range * near_clip / fog_distance_range;
    fog_distance_slope = fog_intensity_range / (fog_distance_range / 1024.0f);
    view->fog_add = (view->fog_near_int * view->fog_far_dist - view->fog_far_int * view->fog_near_dist) / fog_distance_range;
    view->fog_slope = fog_distance_slope;
    view->fog_base = view->fog_near_int - view->fog_near_dist / 1024.0f * fog_distance_slope;
    D_001E6B70[0] = fog_multiplier;
    D_001DFF50[0] = fog_multiplier;
    D_00160B30 = fog_multiplier;
    view->fog_mul = fog_multiplier;
    mul_bits = *(s32 *)&view->fog_mul;
    D_001DE740.mul = mul_bits;
    D_001DE740.far_int = far_bits;
    D_001DE740.near_int = near_bits;
    D_001DEA00.max = 0x437EFFFF;
    D_001DEA00.mul = mul_bits;
    D_001DEA00.near_int = near_bits;
    D_001DE9B0.mul = mul_bits;
    D_001DE9B0.near_int = near_bits;
    D_001DEA00.far_int = far_bits;
    D_001DE9B0.far_int = far_bits;
    D_001DE9B0.max = 0x437EFFFF;
    set_tfrag_dists();

    near_clip = view->near_clip;
    far_clip = view->far_clip;
    horizontal_fov = view->fov.f[0];
    vertical_fov = view->fov.f[1];
    viewport_width = view->width;
    viewport_height = view->height;
    fog_multiplier = view->fog_mul;
    D_00160720[0] = 0.5f / 210000.0f;
    D_00160720[1] = -0.5f / 210000.0f;
    D_00160720[2] = 0.5f;
    view->proj[0] = viewport_width / (horizontal_fov * near_clip);
    view->proj[1] = 0;
    view->proj[2] = 0;
    view->proj[3] = 0;
    view->proj[4] = 0;
    view->proj[5] = viewport_height / (vertical_fov * near_clip);
    view->proj[6] = 0;
    view->proj[7] = 0;
    view->proj[8] = 0;
    view->proj[9] = 0;
    view->proj[10] = (far_clip + near_clip) / (near_clip * (far_clip - near_clip)) * depth_scale;
    view->proj[11] = 1.0f / near_clip * fog_multiplier;
    view->proj[12] = 0;
    view->proj[13] = 0;
    view->proj[14] = -2.0f * near_clip * far_clip / (near_clip * (far_clip - near_clip)) * depth_scale;
    view->proj[15] = 0;
    D_00160720[3] = viewport_width / (horizontal_fov * 210000.0f);
    func_001F9838(view->proj2, view->proj, 0x40);
    view->proj2[11] = 1.0f / view->near_clip;
    func_001F9838(view->proj3, view->proj2, 0x40);
    view->proj3[0] /= view->scr_x;
    view->proj3[5] /= view->scr_y;
    view->proj3[10] /= depth_scale;
    view->proj3[14] /= depth_scale;
    view->inverse_screen_scale.f[0] = 1.0f / view->scr_x;
    view->inverse_screen_scale.f[1] = 1.0f / view->scr_y;
    view->inverse_screen_scale.f[2] = 1.0f / depth_scale;
    view->inverse_screen_scale.f[3] = 1.0f / view->fog_mul;
    view->screen_scale.f[0] = view->scr_x;
    view->screen_scale.f[1] = view->scr_y;
    view->screen_scale.f[2] = depth_scale;
    view->screen_scale.f[3] = view->fog_mul;
    view->screen_bias.f[0] = 2048.0f;
    view->screen_bias.f[1] = 2048.0f;
    view->screen_bias.f[2] = 8388112.0f;
    view->screen_bias.f[3] = view->fog_add;
    view->inverse_projection_scale.f[0] = view->scr_x / view->proj[0];
    view->inverse_projection_scale.f[1] = view->scr_y / view->proj[5];
    view->inverse_projection_scale.f[2] = depth_scale / view->proj[10];
    view->inverse_projection_scale.f[3] = view->fog_mul;
    view->viewport_aspect.f[0] = view->scr_x / view->width;
    view->viewport_aspect.f[1] = view->scr_y / view->height;
    view->viewport_aspect.f[2] = 1.0f;
    view->viewport_aspect.f[3] = 1.0f;
    view->fog_color = *(s32 *)&view->fog_far_int;
    view->unk4 = 0x103E4000;
    view->unk8 = 0xE;
    view->unkC = 0;
    qzero(&view->regs[0]);

    packet_view = &view_context;
    packet_view->gif[0].i[0] = *(s32 *)&packet_view->fog_mul;
    packet_view->gif[0].i[1] = 0x303E4000;
    packet_view->gif[0].i[2] = 0x412;
    packet_view->gif[0].i[3] = 0x303EC000;
    packet_view->gif[1].i[0] = *(s32 *)&packet_view->fog_mul;
    packet_view->gif[1].i[1] = 0x302E4000;
    packet_view->gif[1].i[2] = 0x412;
    packet_view->gif[1].i[3] = 0x302EC000;
    packet_view->gif[2].i[0] = *(s32 *)&packet_view->fog_mul;
    packet_view->gif[2].i[1] = 0x20364000;
    packet_view->gif[2].i[2] = 0x41;
    packet_view->gif[2].i[3] = 0;
    packet_view->gif[3].i[0] = *(s32 *)&packet_view->fog_mul;
    packet_view->gif[3].i[1] = 0x20264000;
    packet_view->gif[3].i[2] = 0x41;
    packet_view->gif[3].i[3] = 0;
    qcopy(&packet_view->regs[0], &packet_view->gif[0]);
    qcopy(&packet_view->regs[1], &packet_view->viewport_aspect);
    qcopy(&packet_view->regs[2], &packet_view->inverse_screen_scale);
    qcopy(&packet_view->regs[3], &packet_view->screen_scale);
    qcopy(&packet_view->regs[4], &packet_view->screen_bias);
    qcopy(&D_001DE710[0], &packet_view->screen_bias);
    qcopy(&D_001DE710[1], &packet_view->screen_scale);
    qcopy(&D_001DE710[2], &packet_view->viewport_aspect);
    qcopy(&D_001DE740.constants[0], &packet_view->screen_bias);
    qcopy(&D_001DE740.constants[1], &packet_view->inverse_screen_scale);
    qcopy(&D_001DE740.constants[2], &packet_view->screen_scale);
    D_001DE740.x = packet_view->viewport_aspect.i[0];
    D_001DE740.y = packet_view->viewport_aspect.i[1];
    qcopy(&D_001DEA40[0], &packet_view->screen_scale);
    qcopy(&D_001DEA40[1], &packet_view->screen_bias);
}
extern __typeof__(update_view_context) func_001F2D98 __attribute__((alias("FUN_001f2d98")));

#endif /* NON_MATCHING */

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
    Vec4 unk180;
    Vec4 unk190;
    Vec4 unk1A0;
    Vec4 unk1B0;
    Vec4 unk1C0;
    Vec4 unk1D0;
    Vec4 unk1E0;
    Vec4 unk1F0;
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

struct FogRegs { u8 pad0[0x10]; f32 mul; f32 far_int; f32 near_int; };
struct FogRegs2 { u8 pad0[0x28]; s32 mul; s32 max; s32 far_int; s32 near_int; };
struct Clip { Vec4 v[3]; };

extern s32 D_0015ED80;
extern struct View D_0018CD00;
extern Vec4 D_0018CFC0[4];
extern f32 D_00160720[4];
extern f32 D_00160B30;
extern f32 D_001DFF50[];
extern f32 D_001E6B70[];
extern struct FogRegs2 D_001DE9B0;
extern struct FogRegs2 D_001DEA00;
extern Vec4 D_001DEA40[2];
extern Vec4 D_001DE710[3];
extern struct { u8 pad0[0x10]; s32 mul; s32 far_int; s32 near_int; u8 pad1c[0x44]; Vec4 v[3]; u8 pad90[0x10]; s32 x; s32 y; } D_001DE740;

extern f32 func_001F9E90(f32, f32);
extern f32 func_001F9DC8(f32);
extern void func_001F9838(void *, void *, s32);
extern void func_00233068(void);

void update_view_context(void) __asm__("FUN_001f2d98");

void update_view_context(void) {
    struct View *v;
    struct View *p;
    f32 zfar;
    f32 k;
    f32 a;
    f32 b;
    f32 d;
    f32 di;
    f32 m;
    f32 n;
    f32 f;
    f32 fm;
    f32 fx;
    f32 fy;
    f32 w;
    f32 h;
    s32 far_bits;
    s32 near_bits;
    s32 mul_bits;

    zfar = -8388080.0f;
    if (D_0015ED80 != 0) {
        D_0018CD00.fov.f[1] = D_0018CD00.fov.f[0] * 0.756f;
    } else {
        D_0018CD00.fov.f[1] = D_0018CD00.fov.f[0] * 0.775f;
    }
    v = &D_0018CD00;
    v->aspect_x = v->scr_x / v->width;
    v->aspect_y = v->scr_y / v->height;
    v->fov.f[2] = 1.0f / func_001F9DC8(func_001F9E90(1.0f, v->fov.f[0]));
    v->fov.f[3] = 1.0f / func_001F9DC8(func_001F9E90(1.0f, v->fov.f[1]));
    a = 1.0f / func_001F9DC8(func_001F9E90(1.0f, v->fov.f[0] * v->aspect_x));
    b = 1.0f / func_001F9DC8(func_001F9E90(1.0f, v->fov.f[1] * v->aspect_y));
    a /= v->fov.f[2];
    b /= v->fov.f[3];
    v->unk1D0.f[0] = v->aspect_x;
    v->unk1D0.f[1] = v->aspect_y;
    v->unk1D0.f[2] = 1.0f / v->aspect_x;
    v->unk1D0.f[3] = 1.0f / v->aspect_y;
    v->unk1E0.f[0] = v->fov.f[2];
    v->unk1E0.f[1] = v->fov.f[3];
    v->unk1E0.f[2] = (a + b) * 0.5f;
    v->unk1F0.f[0] = v->far_clip;
    n = v->near_clip;
    v->unk1F0.f[1] = n;
    qcopy(&D_0018CFC0[0], &v->fov);
    qcopy(&D_0018CFC0[1], &v->unk1D0);
    qcopy(&D_0018CFC0[2], &v->unk1E0);
    qcopy(&D_0018CFC0[3], &v->unk1F0);

    far_bits = *(s32 *)&v->fog_far_int; /*H*/
    near_bits = *(s32 *)&v->fog_near_int; /*H*/
    d = v->fog_far_dist - v->fog_near_dist; /*H*/
    di = v->fog_far_int - v->fog_near_int; /*H*/
    fm = di * n / d; /*H*/
    m = di / (d / 1024.0f); /*H*/
    v->fog_add = (v->fog_near_int * v->fog_far_dist - v->fog_far_int * v->fog_near_dist) / d; /*H*/
    v->fog_slope = m; /*H*/
    v->fog_base = v->fog_near_int - v->fog_near_dist / 1024.0f * m; /*H*/
    D_001E6B70[0] = fm; /*H*/
    D_001DFF50[0] = fm; /*H*/
    D_00160B30 = fm; /*H*/
    v->fog_mul = fm; /*H*/
    mul_bits = *(s32 *)&v->fog_mul; /*H*/
    D_001DE740.mul = mul_bits; /*H*/
    D_001DE740.far_int = far_bits; /*H*/
    D_001DE740.near_int = near_bits; /*H*/
    D_001DEA00.max = 0x437EFFFF; /*H*/
    D_001DEA00.mul = mul_bits; /*H*/
    D_001DEA00.near_int = near_bits; /*H*/
    D_001DE9B0.mul = mul_bits; /*H*/
    D_001DE9B0.near_int = near_bits; /*H*/
    D_001DEA00.far_int = far_bits; /*H*/
    D_001DE9B0.far_int = far_bits; /*H*/
    D_001DE9B0.max = 0x437EFFFF; /*H*/
    func_00233068();

    n = v->near_clip;
    f = v->far_clip;
    fx = v->fov.f[0];
    fy = v->fov.f[1];
    w = v->width;
    h = v->height;
    fm = v->fog_mul;
    D_00160720[0] = 0.5f / 210000.0f;
    D_00160720[1] = -0.5f / 210000.0f;
    D_00160720[2] = 0.5f;
    v->proj[0] = w / (fx * n);
    v->proj[1] = 0;
    v->proj[2] = 0;
    v->proj[3] = 0;
    v->proj[4] = 0;
    v->proj[5] = h / (fy * n);
    v->proj[6] = 0;
    v->proj[7] = 0;
    v->proj[8] = 0;
    v->proj[9] = 0;
    v->proj[10] = (f + n) / (n * (f - n)) * zfar;
    v->proj[11] = 1.0f / n * fm;
    v->proj[12] = 0;
    v->proj[13] = 0;
    v->proj[14] = -2.0f * n * f / (n * (f - n)) * zfar;
    v->proj[15] = 0;
    D_00160720[3] = w / (fx * 210000.0f);
    func_001F9838(v->proj2, v->proj, 0x40);
    v->proj2[11] = 1.0f / v->near_clip;
    func_001F9838(v->proj3, v->proj2, 0x40);
    v->proj3[0] /= v->scr_x;
    v->proj3[5] /= v->scr_y;
    v->proj3[10] /= zfar;
    v->proj3[14] /= zfar;
    v->unk180.f[0] = 1.0f / v->scr_x;
    v->unk180.f[1] = 1.0f / v->scr_y;
    v->unk180.f[2] = 1.0f / zfar;
    v->unk180.f[3] = 1.0f / v->fog_mul;
    v->unk190.f[0] = v->scr_x;
    v->unk190.f[1] = v->scr_y;
    v->unk190.f[2] = zfar;
    v->unk190.f[3] = v->fog_mul;
    v->unk1A0.f[0] = 2048.0f;
    v->unk1A0.f[1] = 2048.0f;
    v->unk1A0.f[2] = 8388112.0f;
    v->unk1A0.f[3] = v->fog_add;
    v->unk1B0.f[0] = v->scr_x / v->proj[0];
    v->unk1B0.f[1] = v->scr_y / v->proj[5];
    v->unk1B0.f[2] = zfar / v->proj[10];
    v->unk1B0.f[3] = v->fog_mul;
    v->unk1C0.f[0] = v->scr_x / v->width;
    v->unk1C0.f[1] = v->scr_y / v->height;
    v->unk1C0.f[2] = 1.0f;
    v->unk1C0.f[3] = 1.0f;
    v->fog_color = *(s32 *)&v->fog_far_int;
    v->unk4 = 0x103E4000;
    v->unk8 = 0xE;
    v->unkC = 0;
    qzero(&v->regs[0]);

    p = &D_0018CD00;
    p->gif[0].i[0] = *(s32 *)&p->fog_mul;
    p->gif[0].i[1] = 0x303E4000;
    p->gif[0].i[2] = 0x412;
    p->gif[0].i[3] = 0x303EC000;
    p->gif[1].i[0] = *(s32 *)&p->fog_mul;
    p->gif[1].i[1] = 0x302E4000;
    p->gif[1].i[2] = 0x412;
    p->gif[1].i[3] = 0x302EC000;
    p->gif[2].i[0] = *(s32 *)&p->fog_mul;
    p->gif[2].i[1] = 0x20364000;
    p->gif[2].i[2] = 0x41;
    p->gif[2].i[3] = 0;
    p->gif[3].i[0] = *(s32 *)&p->fog_mul;
    p->gif[3].i[1] = 0x20264000;
    p->gif[3].i[2] = 0x41;
    p->gif[3].i[3] = 0;
    qcopy(&p->regs[0], &p->gif[0]);
    qcopy(&p->regs[1], &p->unk1C0);
    qcopy(&p->regs[2], &p->unk180);
    qcopy(&p->regs[3], &p->unk190);
    qcopy(&p->regs[4], &p->unk1A0);
    qcopy(&D_001DE710[0], &p->unk1A0);
    qcopy(&D_001DE710[1], &p->unk190);
    qcopy(&D_001DE710[2], &p->unk1C0);
    qcopy(&D_001DE740.v[0], &p->unk1A0);
    qcopy(&D_001DE740.v[1], &p->unk180);
    qcopy(&D_001DE740.v[2], &p->unk190);
    D_001DE740.x = p->unk1C0.i[0];
    D_001DE740.y = p->unk1C0.i[1];
    qcopy(&D_001DEA40[0], &p->unk190);
    qcopy(&D_001DEA40[1], &p->unk1A0);
}
#endif /* NON_MATCHING */

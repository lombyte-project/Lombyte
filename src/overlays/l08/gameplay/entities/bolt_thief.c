/* Level 08 moby classes: bolt_thief, shrub. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

/* finds the first of eight linked entries whose direction lines up with the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E15A0), where it is exact; names translated to the US level program. */

extern char D_L08_001675C0[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float FUN_001f9dc8(float);
extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_001f9bf8(void *, void *, float);
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

int FUN_L08_002e0228(struct Moby *m) {
    float a[4];
    float b[4];
    int *p = (int *)((char *)m->pvars + 0x60);
    int i;
    for (i = 0; i < 8; i++) {
        if (p[i] != 0) {
            float d;
            build_spherical_offset(a, 1.0f, m->rot.z, -m->rot.y);
            FUN_001f9a28(b, (char *)p[i] + 0x10, D_L08_001675C0);
            FUN_001f9bf8(b, b, 1.0f);
            d = fast_vec_dot(b, a);
            if (FUN_001f9dc8(0.06981317f) < d)
                return p[i];
        }
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E2A10), where it is exact; names translated to the US level program. */

extern int hero_set_state(int, int) __asm__("FUN_L08_00230b38");

/* Sets the moby's byte 0xBC and calls hero_set_state(0x32, 1). */
void FUN_L08_002e1698(struct Moby *arg) {
    arg->command = 1;
    hero_set_state(0x32, 1);
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E3010), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern char D_0013E533[];
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);

void *FUN_L08_002e1c98(struct Moby *parent, int oClass, int joint, int pjoint) {
    unsigned char *m = func_0020D348_m(oClass);
    float v0[4], v10[4];
    if (m != 0) {
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        *(unsigned short *)(m + 0x34) |= 0x4000;
        *(long *)(m + 0x38) = *(long *)(*(char **)(D_0013E533 + 0x2E9D) + 0x38);
        qcopy(m + 0x40, &parent->rot);
        moby_point_position(m, joint, v0);
        moby_point_position(parent, pjoint, v10);
        FUN_001f9a28(m + 0x10, v10, v0);
        FUN_L00_00250df8(m);
    }
    return m;
}
/* builds a basis from a direction vector and writes the orientation into the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E6130), where it is exact; names translated to the US level program. */

extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");

void FUN_L08_002e4db8(struct Moby *a, float *v) {
    float m[16];
    float n[16];
    float r[4];
    float s[4];
    char *d = (char *)a->pvars;
    r[1] = v[2];
    r[2] = v[0];
    r[0] = -v[1];
    fast_vec_cross(r, v, r);
    fast_vec_cross(s, r, v);
    qcopy(m, r);
    qcopy(m + 4, s);
    qcopy(m + 8, v);
    clear_u64_value(m + 12);
    d += 0x10;
    m[15] = 1.0f;
    matrix_to_euler(m, d);
    euler_rotation_matrix(n, (float *)d);
    n[15] = 42.0f;
}
/* spawns the child mobys this object owns and fills its part table */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9B60), where it is exact; names translated to the US level program. */

extern int FUN_L08_002e1c98_c() __asm__("FUN_L08_002e1c98");

void FUN_L08_002e8788(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *e = data + 0xBC;
    int i = 0;
    char *c;
    char *c2;
    char *c3;
    *(char **)(data + 0x60) = moby;
    *(int *)(data + 0x64) = 0;
    *(int *)(data + 0x68) = 0;
    *(int *)(data + 0x6C) = 0;
    *(char **)(data + 0x90) = FUN_L08_002e1c98_c(moby, 0x1C0, 1, 0);
    *(char **)(data + 0x94) = moby;
    *(int *)(data + 0x98) = 1;
    *(int *)(data + 0x9C) = 0;
    c = FUN_L08_002e1c98_c(moby, 0x1BD, 0, 2);
    *(char **)(data + 0x70) = c;
    *(int *)(data + 0x7C) = 2;
    *(char **)(data + 0x74) = moby;
    *(int *)(data + 0x78) = 0;
    *(float *)(c + 0x2C) = *(float *)(*(char **)(c + 0x24) + 0x24) * 0.75f;
    c2 = FUN_L08_002e1c98_c(moby, 0x1BA, 1, 1);
    *(char **)(data + 0x80) = c2;
    *(char **)(data + 0x84) = moby;
    *(int *)(data + 0x88) = 1;
    *(int *)(data + 0x8C) = 1;
    c3 = FUN_L08_002e1c98_c(c2, 0x1B9, 0, 0);
    *(char **)(data + 0xA0) = c3;
    *(char **)(data + 0xA4) = *(char **)(data + 0x80);
    *(int *)(data + 0xA8) = 0;
    *(int *)(data + 0xAC) = 0;
    do {
        int j = i + 2;
        i++;
        *(char **)(e - 0xC) = FUN_L08_002e1c98_c(*(char **)(data + 0x90), 0x1BB, 0, j);
        *(char **)(e - 8) = *(char **)(data + 0x90);
        *(int *)(e - 4) = 0;
        *(int *)e = j;
        e += 0x10;
    } while (i < 8);
}
/* Re-aims the twelve child mobys of a parent, adjusts the parent's spin, then runs the children's follow-up. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9F78), where it is exact; names translated to the US level program. */

extern float D_0015ED6C_d __asm__("D_0015ED6C");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9a28(void *, void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);

extern short D_L08_00161D28 __attribute__((sda));
void FUN_L08_002e8ba0(struct Moby *a) {
    char *data = (char *)a->pvars;
    float v0[4], v10[4], v20[4];
    char *base = data + 0x60;
    int i;
    for (i = 0; i < 12; i++) {
        int *e = (int *)(base + 0x1C + i * 16);
        char *m = (char *)e[-3];
        if (m != 0) {
            *(float *)(m + 0x48) = a->rot.z;
            moby_point_position(m, e[-1], v0);
            moby_point_position((void *)e[-2], e[0], v10);
            FUN_001f9a28(v20, v10, v0);
            FUN_001f9a10(m + 0x10, m + 0x10, v20);
        }
    }
    if (*(int *)(data + 0x90) != 0) {
        char *r = *(char **)(data + 0x90);
        *(float *)(*(char **)(data + 0x90) + 0x40) = fast_add_rotations(
            *(float *)(r + 0x40),
            *(float *)&D_L08_00161D28 * DEG_TO_RAD * D_0015ED6C_d);
    }
    for (i = 0; i < 12; i++) {
        char *m = *(char **)(base + 0x10 + i * 16);
        if (m != 0)
            FUN_L00_00250df8(m);
    }
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002EADF0), where it is exact; names translated to the US level program. */

extern void FUN_001f9a68(void *, void *, float);

extern short D_L08_00161D38 __attribute__((sda));
void FUN_L08_002e9a18(struct Moby *arg) {
    char *data = (char *)arg->pvars;
    float v0[4];
    float v1[4];
    float v2[4];
    int i;
    char *p;
    char *parts = data + 0x60;
    for (i = 0; i < 16; i++) {
        char *e = parts + 0x1C + i * 16;
        char *part = *(char **)(e - 0xC);
        if (part != 0) {
            *(float *)(part + 0x48) = arg->rot.z;
            moby_point_position(part, *(int *)(e - 4), v0);
            moby_point_position(*(void **)(e - 8), *(int *)e, v1);
            FUN_001f9a28(v2, v1, v0);
            FUN_001f9a10(part + 0x10, part + 0x10, v2);
        }
    }
    if (*(char **)(data + 0x90) != 0) {
        *(float *)(*(char **)(data + 0x90) + 0x40) =
            fast_add_rotations(*(float *)(*(char **)(data + 0x90) + 0x40),
                               *(float *)&D_L08_00161D38 * DEG_TO_RAD * D_0015ED6C_d);
    }
    p = parts + 0x10;
    for (i = 0; i < 16; i++) {
        char *o = *(char **)p;
        if (o != 0) {
            FUN_L00_00250df8(o);
            o = *(char **)p;
            if (*(unsigned short *)(o + 0x34) & 0x8000) {
                FUN_001f9a68(o + 0xD0, o + 0xD0, -1.0f);
            }
        }
        p += 16;
    }
}

#include "rnc/gameplay/hero.h"

/* Draws the Batalia cannon's HUD: a four-part frame, markers for the eight linked and four indexed targets
   (relative to the cannon's heading), the inner frame, a 32-segment health dial and two rotating needles. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E0258), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct RData51_2E0258 RData51_2E0258;

struct RData51_2E0258 { char pad0[0x58]; int health, pad5C; Moby *linked[8]; int indices[4]; char pad90[0x10]; int timer; };

extern char *D_L08_0015FFD8_lui __asm__("D_L08_0015FFD8") __attribute__((section(".data")));
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_normalize_angle(f32) __asm__("func_001FA610");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations_c(f32, f32) __asm__("func_001FA5C8");
extern float FUN_001f9e90(float, float);
extern int D_0015EDB4_d __asm__("D_0015EDB4") __attribute__((section(".data"))); /* no foreign declaration */
extern int D_L08_0015F5CC_lui __asm__("D_L08_0015F5CC") __attribute__((section(".data"))); /* no foreign declaration */
extern f32 D_L08_00161AC0_gp __asm__("D_L08_00161AC0") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161AC8_gp __asm__("D_L08_00161AC8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161AD4_gp __asm__("D_L08_00161AD4") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161AD8_gp __asm__("D_L08_00161AD8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161ADC_gp __asm__("D_L08_00161ADC") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B60_gp __asm__("D_L08_00161B60") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B64_gp __asm__("D_L08_00161B64") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B68_gp __asm__("D_L08_00161B68") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B6C_gp __asm__("D_L08_00161B6C") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B70_gp __asm__("D_L08_00161B70") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B74_gp __asm__("D_L08_00161B74") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B78_gp __asm__("D_L08_00161B78") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B7C_gp __asm__("D_L08_00161B7C") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B80_gp __asm__("D_L08_00161B80") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B84_gp __asm__("D_L08_00161B84") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B88_gp __asm__("D_L08_00161B88") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B8C_gp __asm__("D_L08_00161B8C") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B90_gp __asm__("D_L08_00161B90") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B94_gp __asm__("D_L08_00161B94") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B98_gp __asm__("D_L08_00161B98") __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B9C_gp __asm__("D_L08_00161B9C") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BA0_gp __asm__("D_L08_00161BA0") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BA4_gp __asm__("D_L08_00161BA4") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BA8_gp __asm__("D_L08_00161BA8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BAC_gp __asm__("D_L08_00161BAC") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BB0_gp __asm__("D_L08_00161BB0") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BB4_gp __asm__("D_L08_00161BB4") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BB8_gp __asm__("D_L08_00161BB8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BBC_gp __asm__("D_L08_00161BBC") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BC0_gp __asm__("D_L08_00161BC0") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BC4_gp __asm__("D_L08_00161BC4") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BC8_gp __asm__("D_L08_00161BC8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BCC_gp __asm__("D_L08_00161BCC") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BD0_gp __asm__("D_L08_00161BD0") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BD4_gp __asm__("D_L08_00161BD4") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BD8_gp __asm__("D_L08_00161BD8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BDC_gp __asm__("D_L08_00161BDC") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BE0_gp __asm__("D_L08_00161BE0") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BE4_gp __asm__("D_L08_00161BE4") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BE8_gp __asm__("D_L08_00161BE8") __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BEC_gp __asm__("D_L08_00161BEC") __attribute__((sda)); /* no foreign declaration */
extern int fast_tween_color() __asm__("FUN_001fa6e0");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 truncate_float_to_s32_c3(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void FUN_L02_0020bc88(void *, void *, void *, s32, s32);
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void func_00234C98_2E0258(int, long) __asm__("FUN_00233980");
extern long texture51(int) __asm__("FUN_001f44b8");
extern void draw_quad51(void *, void *, void *, long, int) __asm__("FUN_L02_0020bc88");
extern int func_001FA898_2E0258(float) __asm__("FUN_001fa6d0");
extern float func_001FA888_2E0258(int) __asm__("FUN_001fa6c0");
extern float func_001FA748_2E0258(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_2E0258(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA7D8_2E0258(float) __asm__("FUN_001fa610");
extern float func_001F9D10_2E0258(void *, void *) __asm__("FUN_001f9b48");
extern float func_001F9F90_2E0258(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2E0258(float) __asm__("FUN_001f9de0");
extern float func_L00_001FF860_2E0258(float, float) __asm__("FUN_001f9e90");
extern int func_001FA8A8_2E0258(int, int, float) __asm__("FUN_001fa6e0");
extern int func_001F9850_2E0258(int) __asm__("FUN_001f96f8");
extern char *D_L08_0015FFD8;
extern int D_L08_0015F5CC __asm__("D_L08_0015F5CC");
extern unsigned char draw_map51[4] __asm__("D_0015EDB4");
extern float D_L08_00161AC0 __asm__("D_L08_00161AC0");
extern float D_L08_00161B60 __asm__("D_L08_00161B60");
extern float D_L08_00161B64 __asm__("D_L08_00161B64");
extern float D_L08_00161B68 __asm__("D_L08_00161B68");
extern float D_L08_00161B6C __asm__("D_L08_00161B6C");
extern float D_L08_00161B70 __asm__("D_L08_00161B70");
extern float D_L08_00161B74 __asm__("D_L08_00161B74");
extern float D_L08_00161B78 __asm__("D_L08_00161B78");
extern float D_L08_00161B7C __asm__("D_L08_00161B7C");
extern float D_L08_00161B80 __asm__("D_L08_00161B80");
extern float D_L08_00161B84 __asm__("D_L08_00161B84");
extern float D_L08_00161B88 __asm__("D_L08_00161B88");
extern float D_L08_00161B8C __asm__("D_L08_00161B8C");
extern float D_L08_00161B90 __asm__("D_L08_00161B90");
extern float D_L08_00161B94 __asm__("D_L08_00161B94");
extern float D_L08_00161B98 __asm__("D_L08_00161B98");
extern float D_L08_00161B9C __asm__("D_L08_00161B9C");
extern int D_L08_00161AC8 __asm__("D_L08_00161AC8");
extern int D_L08_00161AD4 __asm__("D_L08_00161AD4");
extern int D_L08_00161AD8 __asm__("D_L08_00161AD8");
extern int D_L08_00161ADC __asm__("D_L08_00161ADC");
extern int D_L08_00161BA0 __asm__("D_L08_00161BA0");
extern int D_L08_00161BA4 __asm__("D_L08_00161BA4");
extern int D_L08_00161BA8 __asm__("D_L08_00161BA8");
extern int D_L08_00161BAC __asm__("D_L08_00161BAC");
extern int D_L08_00161BB0 __asm__("D_L08_00161BB0");
extern int D_L08_00161BB4 __asm__("D_L08_00161BB4");
extern int D_L08_00161BB8 __asm__("D_L08_00161BB8");
extern int D_L08_00161BBC __asm__("D_L08_00161BBC");
extern int D_L08_00161BC0 __asm__("D_L08_00161BC0");
extern int D_L08_00161BC4 __asm__("D_L08_00161BC4");
extern int D_L08_00161BC8 __asm__("D_L08_00161BC8");
extern int D_L08_00161BCC __asm__("D_L08_00161BCC");
extern int D_L08_00161BD0 __asm__("D_L08_00161BD0");
extern int D_L08_00161BD4 __asm__("D_L08_00161BD4");
extern int D_L08_00161BD8 __asm__("D_L08_00161BD8");
extern int D_L08_00161BDC __asm__("D_L08_00161BDC");
extern int D_L08_00161BE0 __asm__("D_L08_00161BE0");
extern int D_L08_00161BE4 __asm__("D_L08_00161BE4");
extern int D_L08_00161BE8 __asm__("D_L08_00161BE8");
extern int D_L08_00161BEC __asm__("D_L08_00161BEC");

void FUN_L08_002deee0(Moby *moby) {
    long xy[4];
    int color_words[4], uv_words[4];
    float relative[4];
    RData51_2E0258 *data = (*(RData51_2E0258 **)&moby->pvars);
    long texture;
    int segments;
    int i, j, k, n, sign, color;
    int *indices;
    float sign_x, sign_y, angle, radius, fraction, next_angle;
    int outer_x, outer_y, inner_x, inner_y;
    int color_a, color_b;
    long base;
    func_00234C98_2E0258(0x42, 0x8000000044L);
    texture = texture51(D_L08_00161BA0 + 0x28);
    uv_words[0] = D_L08_00161BB0;
    uv_words[1] = D_L08_00161BB4;
    uv_words[2] = D_L08_00161BB8;
    uv_words[3] = D_L08_00161BBC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AC8;
    for (i = 0; i < 4; i++) {
        sign_x = (i & 1) ? -1.0f : 1.0f;
        sign_y = (i >= 2) ? -1.0f : 1.0f;
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 + D_L08_00161B8C * sign_y) * 16.0f) << 16) - (int)0x80000000);
        xy[0] += (func_001FA898_2E0258(D_L08_00161B80 + D_L08_00161B88 * sign_x) << 4) + 0x8000;
        xy[1] += (unsigned int)((func_001FA898_2E0258(D_L08_00161B84 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (func_001FA898_2E0258(D_L08_00161B80 + D_L08_00161B88 * sign_x) << 4) + 0x8000;
        xy[2] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 + D_L08_00161B8C * sign_y) * 16.0f) << 16) - (int)0x80000000);
        xy[2] += (func_001FA898_2E0258(D_L08_00161B80) << 4) + 0x8000;
        xy[3] += (unsigned int)((func_001FA898_2E0258(D_L08_00161B84 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (func_001FA898_2E0258(D_L08_00161B80) << 4) + 0x8000;
        draw_quad51(xy, uv_words, color_words, texture, 1);
    }
    texture = texture51(D_L08_00161BA4);
    uv_words[0] = D_L08_00161BC0;
    uv_words[1] = D_L08_00161BC4;
    uv_words[2] = D_L08_00161BC8;
    uv_words[3] = D_L08_00161BCC;
    for (k = 0; k < 8; k++) {
        float angle, radius;
        if (data->linked[k]) {
            if ((signed char)data->linked[k]->state >= 0) {
                if (data->linked[k]->state == 2) {
                    if (D_L08_0015F5CC & 0x10) {
                        color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AD4;
                    } else {
                        color_words[0] = color_words[1] = color_words[2] = color_words[3] = 0x60000000;
                    }
                } else {
                    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161ADC;
                }
                angle = func_001FA790_2E0258(func_L00_001FF860_2E0258((*(float (*)[4])&data->linked[k]->pos)[0] - (*(float (*)[4])&moby->pos)[0], (*(float (*)[4])&data->linked[k]->pos)[1] - (*(float (*)[4])&moby->pos)[1]), (*(float (*)[4])&moby->rot)[2]);
                radius = func_001F9D10_2E0258((*(float (*)[4])&data->linked[k]->pos), (*(float (*)[4])&moby->pos)) * D_L08_00161AC0;
                sign = draw_map51[0] ? -1 : 1;
                relative[0] = (float)-sign * func_001F9FA8_2E0258(angle) * radius;
                relative[1] = -func_001F9F90_2E0258(angle) * radius;
                xy[3] = 0xFFFFF000000000L;
                xy[2] = 0xFFFFF000000000L;
                xy[1] = 0xFFFFF000000000L;
                xy[0] = 0xFFFFF000000000L;
                xy[0] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 + D_L08_00161B94) * 16.0f + relative[1]) << 16) - (int)0x80000000);
                xy[0] += func_001FA898_2E0258((D_L08_00161B80 + D_L08_00161B90) * 16.0f + relative[0]) + 0x8000;
                xy[1] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 - D_L08_00161B94) * 16.0f + relative[1]) << 16) - (int)0x80000000);
                xy[1] += func_001FA898_2E0258((D_L08_00161B80 + D_L08_00161B90) * 16.0f + relative[0]) + 0x8000;
                xy[2] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 + D_L08_00161B94) * 16.0f + relative[1]) << 16) - (int)0x80000000);
                xy[2] += func_001FA898_2E0258((D_L08_00161B80 - D_L08_00161B90) * 16.0f + relative[0]) + 0x8000;
                xy[3] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 - D_L08_00161B94) * 16.0f + relative[1]) << 16) - (int)0x80000000);
                xy[3] += func_001FA898_2E0258((D_L08_00161B80 - D_L08_00161B90) * 16.0f + relative[0]) + 0x8000;
                draw_quad51(xy, uv_words, color_words, texture, 1);
            } else data->linked[k] = 0;
        }
    }
    indices = data->indices;
    for (n = 0; n < 4; n++) {
        float angle, radius;
        if (((Moby *)D_L08_0015FFD8)[*indices].state < 0x7F) {
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AD8;
            angle = func_001FA790_2E0258(func_L00_001FF860_2E0258((*(float (*)[4])&((Moby *)D_L08_0015FFD8)[*indices].pos)[0] - (*(float (*)[4])&moby->pos)[0], (*(float (*)[4])&((Moby *)D_L08_0015FFD8)[*indices].pos)[1] - (*(float (*)[4])&moby->pos)[1]), (*(float (*)[4])&moby->rot)[2]);
            radius = func_001F9D10_2E0258((*(float (*)[4])&((Moby *)D_L08_0015FFD8)[*indices].pos), (*(float (*)[4])&moby->pos)) * D_L08_00161AC0;
            relative[0] = -func_001F9FA8_2E0258(angle) * radius;
            relative[1] = -func_001F9F90_2E0258(angle) * radius;
            xy[3] = 0xFFFFF000000000L;
            xy[2] = 0xFFFFF000000000L;
            xy[1] = 0xFFFFF000000000L;
            xy[0] = 0xFFFFF000000000L;
            xy[0] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 + D_L08_00161B9C) * 16.0f + relative[1]) << 16) - (int)0x80000000);
            xy[0] += func_001FA898_2E0258((D_L08_00161B80 + D_L08_00161B98) * 16.0f + relative[0]) + 0x8000;
            xy[1] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 - D_L08_00161B9C) * 16.0f + relative[1]) << 16) - (int)0x80000000);
            xy[1] += func_001FA898_2E0258((D_L08_00161B80 + D_L08_00161B98) * 16.0f + relative[0]) + 0x8000;
            xy[2] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 + D_L08_00161B9C) * 16.0f + relative[1]) << 16) - (int)0x80000000);
            xy[2] += func_001FA898_2E0258((D_L08_00161B80 - D_L08_00161B98) * 16.0f + relative[0]) + 0x8000;
            xy[3] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B84 - D_L08_00161B9C) * 16.0f + relative[1]) << 16) - (int)0x80000000);
            xy[3] += func_001FA898_2E0258((D_L08_00161B80 - D_L08_00161B98) * 16.0f + relative[0]) + 0x8000;
            draw_quad51(xy, uv_words, color_words, texture, 1);
        }
        indices++;
    }
    uv_words[0] = D_L08_00161BB0;
    uv_words[1] = D_L08_00161BB4;
    uv_words[2] = D_L08_00161BB8;
    uv_words[3] = D_L08_00161BBC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AC8;
    texture = texture51(D_L08_00161BA0 + 0x28);
    for (i = 0; i < 4; i++) {
        sign_x = (i & 1) ? -1.0f : 1.0f;
        sign_y = (i >= 2) ? -1.0f : 1.0f;
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B64 + D_L08_00161B6C * sign_y) * 16.0f) << 16) - (int)0x80000000);
        xy[0] += (func_001FA898_2E0258(D_L08_00161B60 + D_L08_00161B68 * sign_x) << 4) + 0x8000;
        xy[1] += (unsigned int)((func_001FA898_2E0258(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (func_001FA898_2E0258(D_L08_00161B60 + D_L08_00161B68 * sign_x) << 4) + 0x8000;
        xy[2] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B64 + D_L08_00161B6C * sign_y) * 16.0f) << 16) - (int)0x80000000);
        xy[2] += (func_001FA898_2E0258(D_L08_00161B60) << 4) + 0x8000;
        xy[3] += (unsigned int)((func_001FA898_2E0258(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (func_001FA898_2E0258(D_L08_00161B60) << 4) + 0x8000;
        draw_quad51(xy, uv_words, color_words, texture, 1);
    }
    texture = texture51(D_L08_00161BA4);
    uv_words[0] = D_L08_00161BD0;
    uv_words[1] = D_L08_00161BD4;
    uv_words[2] = D_L08_00161BD8;
    uv_words[3] = D_L08_00161BDC;
    if ((float)data->health > 100.0f) color = func_001FA8A8_2E0258(0x6000FFFF, 0x6000FF00, ((float)data->health - 100.0f) / 100.0f);
    else color = func_001FA8A8_2E0258(0x600000FF, 0x6000FFFF, (float)data->health / 100.0f);
    segments = func_001FA898_2E0258((1.0f - (float)data->health / 200.0f) * 32.0f);
    for (j = 0; j < 32; j++) {
        float angle, next_angle;
        if (j >= segments) {
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = color;
        } else {
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = (color & 0xFFFFFF) + 0x20000000;
        }
        angle = func_001FA7D8_2E0258((float)j * 0.19634955f);
        next_angle = func_001FA748_2E0258(angle, 0.19634955f);
        outer_x = func_001FA898_2E0258(D_L08_00161B70 * func_001F9FA8_2E0258(angle));
        outer_y = func_001FA898_2E0258(D_L08_00161B74 * -func_001F9F90_2E0258(angle));
        inner_x = func_001FA898_2E0258(D_L08_00161B70 * func_001F9FA8_2E0258(next_angle));
        inner_y = func_001FA898_2E0258(D_L08_00161B74 * -func_001F9F90_2E0258(next_angle));
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B64 + (float)inner_y) * 16.0f) << 16) - (int)0x80000000);
        xy[0] += (func_001FA898_2E0258(D_L08_00161B60 + (float)inner_x) << 4) + 0x8000;
        xy[1] += (unsigned int)((func_001FA898_2E0258(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (func_001FA898_2E0258(D_L08_00161B60) << 4) + 0x8000;
        xy[2] += (unsigned int)((func_001FA898_2E0258((D_L08_00161B64 + (float)outer_y) * 16.0f) << 16) - (int)0x80000000);
        xy[2] += (func_001FA898_2E0258(D_L08_00161B60 + (float)outer_x) << 4) + 0x8000;
        xy[3] += (unsigned int)((func_001FA898_2E0258(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (func_001FA898_2E0258(D_L08_00161B60) << 4) + 0x8000;
        draw_quad51(xy, uv_words, color_words, texture, 1);
    }
    if (func_001F9850_2E0258(15) < data->timer) {
        color = func_001FA8A8_2E0258(0x5000FFFF, 0x500000FF, func_001FA888_2E0258(data->timer - func_001F9850_2E0258(15)) / (float)func_001F9850_2E0258(15));
    } else {
        color = func_001FA8A8_2E0258(0x5000FF00, 0x5000FFFF, func_001FA888_2E0258(data->timer) / (float)func_001F9850_2E0258(15));
    }
    uv_words[0] = D_L08_00161BE0;
    uv_words[1] = D_L08_00161BE4;
    uv_words[2] = D_L08_00161BE8;
    uv_words[3] = D_L08_00161BEC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = color;
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[2] * 4.0f, 0.0f));
    xy[0] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[0] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[2] * 4.0f, 1.5707964f));
    xy[1] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[1] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[2] * 4.0f, -1.5707964f));
    xy[2] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[2] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[2] * 4.0f, 3.1415927f));
    xy[3] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[3] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    draw_quad51(xy, uv_words, color_words, texture51(D_L08_00161BA8), 1);
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[1] * 8.0f, 0.0f));
    xy[0] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[0] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[1] * 8.0f, 1.5707964f));
    xy[1] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[1] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[1] * 8.0f, -1.5707964f));
    xy[2] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[2] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = func_001FA7D8_2E0258(func_001FA748_2E0258((*(float (*)[4])&moby->rot)[1] * 8.0f, 3.1415927f));
    xy[3] += (unsigned int)((func_001FA898_2E0258(func_001F9FA8_2E0258(angle) * D_L08_00161B7C * 16.0f) << 16) - (int)0x80000000);
    xy[3] += func_001FA898_2E0258(func_001F9F90_2E0258(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    draw_quad51(xy, uv_words, color_words, texture51(D_L08_00161BAC), 1);
}
/* Updates the mounted Batalia cannon: entry/exit, aiming, alternating barrels, waves, and attached visuals. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E16A0), where it is exact; names translated to the US level program. */

typedef struct CannonData52_2E16A0 CannonData52_2E16A0;

struct CannonData52_2E16A0 {
    char pad0[0x40]; short fire_timer, exit_timer; int pad44; float pitch_step, yaw_step;
    short pad50, camera_timer, barrel, pad56; int health; short wave_timer;
    unsigned char target_index, target_timer; Moby *linked[8]; int indices[4];
    Moby *guns[2]; float range; int platform_index, lock_timer;
    Moby *visual; float visual_angle;
};

typedef struct Hero52_2E16A0 {
    char pad0[0x80]; float pos[4], rot[4]; char padA0[0x156F]; unsigned char interact;
    char pad1610[0x710]; float input[2]; char pad1D28[0x358]; Moby *moby;
} Hero52_2E16A0;

typedef struct CannonCamera52_2E16A0 { char pad0[0x160]; float mag0; int pad164, timer0, pad16C; float mag1; int pad174, timer1; } CannonCamera52_2E16A0;

char *FUN_L00_0026dd20(void *pos, void *dir, int a, int b, float f);
char *FUN_L08_002e7eb0(int owner, float *position, float *rotation, int value);
extern char *D_L08_0015FFD8;
extern char D_0013CAE0[];
extern f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 random_float_between_c3(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern float D_0015ED70_c __asm__("D_0015ED70");
extern float FUN_001f9b80(void *, void *);
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_0013CAE4 __attribute__((section(".data"))); /* no foreign declaration */
extern int D_0013F350;
extern float D_0015ED60;
extern int D_0015EDDC;
extern int D_L08_00161A90 __attribute__((sda));
extern float D_L08_00161AE8_gp __asm__("D_L08_00161AE8") __attribute__((sda));
extern float D_L08_00161AEC_gp __asm__("D_L08_00161AEC") __attribute__((sda));
extern float D_L08_00161AF0_gp __asm__("D_L08_00161AF0") __attribute__((sda));
extern float D_L08_00161AF4_gp __asm__("D_L08_00161AF4") __attribute__((sda));
extern float D_L08_00161AF8_gp __asm__("D_L08_00161AF8") __attribute__((sda));
extern float D_L08_00161AFC_gp __asm__("D_L08_00161AFC") __attribute__((sda));
extern float D_L08_00161B00_gp __asm__("D_L08_00161B00") __attribute__((sda));
extern int D_L08_00161B10 __attribute__((sda));
extern int D_L08_00161B20 __attribute__((sda));
extern int D_L08_00161B30 __attribute__((sda));
extern int D_L08_00161B40 __attribute__((sda));
extern int D_L08_00161B50 __attribute__((sda));
extern int D_L08_00161BF0 __attribute__((sda));
extern int D_L08_00167480;
extern int FUN_001f9770_c() __asm__("FUN_001f9770");
extern int FUN_L00_001fefc8();
extern int FUN_L00_001ff378();
extern int FUN_L00_00211250();
extern int FUN_L00_00257b90(int, int);
extern s32 D_0015ED84;
extern s32 allocate_voice_for_target_entry_alt_c(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern u8 D_0014C050[];
extern void FUN_L08_00230b38(int, int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void clear_blocks_16(void *, s32) __asm__("func_001F9810");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
int FUN_L08_002e0228_c(struct Moby *m) __asm__("FUN_L08_002e0228");
void FUN_L00_00216f90(void *pos, void *rot, int mode, int flag);
void FUN_L00_002ea9d8(char *src);
void FUN_L00_002eaa30(char *src);
void FUN_L00_002eaaa0(void *pa, void *pb, s32 mode, s32 w, s32 x);
void FUN_L00_002eac18(int mode);
void FUN_L08_002deee0_c(struct Moby *moby) __asm__("FUN_L08_002deee0");
void FUN_L08_00306b30(u8 *object, s32 mode);
void FUN_L08_00313ba8(void);
extern Hero52_2E16A0 hero52 __asm__("D_0013F350");
extern char D_0013E633_2E16A0[] __asm__("D_0013CAE0");
extern char D_0013A5E0_2E16A0[] __asm__("D_0013CAE4");
extern unsigned char visited52[][16] __asm__("D_0014C050");
extern int invert52 __asm__("D_0015EDDC");
extern char *D_L08_0015FFD8;
extern int D_0015EE84_2E16A0 __asm__("D_0015ED84");
extern float D_0015EE60_2E16A0 __asm__("D_0015ED60");
extern float D_0015EE6C_2E16A0 __asm__("D_0015ED6C");
extern float D_0015EE70_2E16A0 __asm__("D_0015ED70");
extern CannonCamera52_2E16A0 cannon_camera52 __asm__("D_L08_00167480");
extern short D_L08_00161B20_2E16A0 __asm__("D_L08_00161A90") __attribute__((sda));
extern short D_L08_00161BA0_2E16A0 __asm__("D_L08_00161B10") __attribute__((sda));
extern short D_L08_00161BB0_2E16A0 __asm__("D_L08_00161B20") __attribute__((sda));
extern short D_L08_00161BC0_2E16A0 __asm__("D_L08_00161B30") __attribute__((sda));
extern short D_L08_00161BD0_2E16A0 __asm__("D_L08_00161B40") __attribute__((sda));
extern short D_L08_00161BE0_2E16A0 __asm__("D_L08_00161B50") __attribute__((sda));
extern short D_L08_00161C80_2E16A0 __asm__("D_L08_00161BF0") __attribute__((sda));
extern float D_L08_00161AE8_gp __asm__("D_L08_00161AE8") __attribute__((sda));
extern float D_L08_00161AEC_gp __asm__("D_L08_00161AEC") __attribute__((sda));
extern float D_L08_00161AF0_gp __asm__("D_L08_00161AF0") __attribute__((sda));
extern float D_L08_00161AF4_gp __asm__("D_L08_00161AF4") __attribute__((sda));
extern float D_L08_00161AF8_gp __asm__("D_L08_00161AF8") __attribute__((sda));
extern float D_L08_00161AFC_gp __asm__("D_L08_00161AFC") __attribute__((sda));
extern float D_L08_00161B00_gp __asm__("D_L08_00161B00") __attribute__((sda));
extern int tick52(short *) __asm__("FUN_001f9770");
extern float func_001F9F90_2E16A0(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2E16A0(float) __asm__("FUN_001f9de0");
extern float func_001FA748_2E16A0(float, float) __asm__("FUN_001fa580");
extern float func_001FA850_2E16A0(float, float) __asm__("FUN_001fa688");
extern float func_L00_001FF860_2E16A0(float,float) __asm__("FUN_001f9e90");
extern void fast_vec_add(void *,void *,void *) __asm__("FUN_001f9a10");
extern void func_001F99D8_2E16A0(void *,int) __asm__("FUN_001f9810");
extern void func_001FA1F8_2E16A0(void *,void *) __asm__("FUN_001fa030");
extern void func_001F9BC0_2E16A0(void *) __asm__("FUN_001f99f8");
extern void func_001F9EC0_2E16A0(void *,void *,void *) __asm__("FUN_001f9cf8");
extern int hero_state52(int,int) __asm__("FUN_L08_00230b38");
extern void func_L00_002EC0C8_2E16A0(int) __asm__("FUN_L00_002eac18");
extern void fast_vec_normalize(void *,void *,float) __asm__("FUN_001f9bf8");
extern void func_L00_001FF328_2E16A0(void *,void *,float) __asm__("FUN_L00_001ff378");
extern void func_L00_00217718_2E16A0(void *,void *,int,int) __asm__("FUN_L00_00216f90");
extern void func_L08_00307FF0_2E16A0(char *,int) __asm__("FUN_L08_00306b30");
extern void func_L00_00211908_2E16A0(void) __asm__("FUN_L00_00211250");
extern void func_L00_002EBF50_2E16A0(void *,void *,int,int,int) __asm__("FUN_L00_002eaaa0");
extern void func_L00_002EBE88_2E16A0(void *) __asm__("FUN_L00_002ea9d8");
extern void func_L00_002EBEE0_2E16A0(void *) __asm__("FUN_L00_002eaa30");
extern float func_001F9D48_2E16A0(void *,void *) __asm__("FUN_001f9b80");
extern float func_001F9CE8_2E16A0(void *) __asm__("FUN_001f9b20");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float func_002140F8_2E16A0(float,float) __asm__("FUN_002132a8");
extern float func_00214D28_2E16A0(float *,float,float) __asm__("FUN_00213ed8");
extern int func_001F9850_2E16A0(int) __asm__("FUN_001f96f8");
extern int func_L08_002E15A0_2E16A0(char *) __asm__("FUN_L08_002e0228");
extern int func_L00_00258BC8_2E16A0(int,int) __asm__("FUN_L00_00257b90");
extern int func_L00_001FEF78_2E16A0(void *) __asm__("FUN_L00_001fefc8");
extern void *func_0020D348_2E16A0(int) __asm__("FUN_0020c4f8");
extern void func_00215C00_2E16A0(void *,float,float,float) __asm__("FUN_00214db0");
extern void func_00213DE0_2E16A0(void *,int,int,int) __asm__("FUN_00212f90");
extern void func_L08_002E9288_2E16A0(void *,void *,void *,int) __asm__("FUN_L08_002e7eb0");
extern int sound52(int,int,void *) __asm__("FUN_0022da68");
extern char *func_L00_0026EBC0_2E16A0(char *,char *,int,int,float) __asm__("FUN_L00_0026dd20");
extern void func_L08_00315068_2E16A0(void) __asm__("FUN_L08_00313ba8");
extern void func_0020D678_2E16A0(void *) __asm__("FUN_0020c828");
extern void func_L00_00251E30_2E16A0(void *) __asm__("FUN_L00_00250df8");
extern void func_001F49B0_2E16A0(void *,void *) __asm__("FUN_001f4600");
extern void func_L08_002E0258_2E16A0(void *) __asm__("FUN_L08_002deee0");

extern unsigned char D_0014095F[];
void FUN_L08_002e0328(Moby *moby) {
    float position[4], offset[4], work[4], direction[4], muzzle[4], scatter[4], random_vec[4];
    CannonData52_2E16A0 *data = (*(CannonData52_2E16A0 **)&moby->pvars);
    int i, target, all_done, dead, best, min_life;
    Moby **links;
    float yaw, pitch, magnitude, best_angle;
    qcopy(position, (*(float (*)[4])&moby->pos));
    position[2] += 0.4f;
    if (tick52(&data->exit_timer) && moby->command == 1) {
        qcopy(hero52.pos, position);
        qcopy(hero52.rot, (*(float (*)[4])&moby->rot));
        offset[0] = func_001F9F90_2E16A0(hero52.rot[2]) * -0.7f;
        offset[1] = func_001F9FA8_2E16A0(hero52.rot[2]) * -0.7f;
        offset[2] = 0.0f;
        fast_vec_add(hero52.pos, hero52.pos, offset);
        func_001F99D8_2E16A0((char *)hero52.pos + 0x60, 0x90);
        if (data->health == 0) {
            Moby *node;
            (*(float (*)[4])&moby->rot)[1] = 0.0f; moby->command = 2;
            func_L00_002EC0C8_2E16A0(0); moby->state = 4;
            node = (Moby *)(D_L08_0015FFD8 + (data->platform_index << 8));
            qcopy(direction, (*(float (*)[4])&node->rot));
            direction[2] = func_001FA748_2E16A0(direction[2], 3.1415927f);
            fast_vec_normalize(work, (*(float (*)[16])&node->rot_row0), 2.5f);
            fast_vec_add(work, work, (*(float (*)[4])&node->pos));
            func_L00_00217718_2E16A0(work, direction, 0, 1);
            func_L08_00307FF0_2E16A0((char *)node, 0);
            return;
        }
        if (visited52[D_0015EE84_2E16A0][moby->mission] != 0xFF && (hero52.interact & 1)) func_L00_00211908_2E16A0();
        if (((*(int *)D_0013A5E0_2E16A0 & 0x10) || (*(unsigned char *)(D_0014095F) & 1)) && visited52[D_0015EE84_2E16A0][moby->mission] == 0xFF) {
            hero52.interact = 0;
            hero_state52(0, 1);
            moby->command = 2; (*(float (*)[4])&moby->rot)[1] = 0.0f;
            func_001FA1F8_2E16A0((*(float (*)[16])&moby->rot_row0), (*(float (*)[4])&moby->rot));
            func_001F9BC0_2E16A0(direction); direction[2] = (*(float (*)[4])&moby->rot)[2];
            func_001F9EC0_2E16A0(work, &D_L08_00161B20_2E16A0, (*(float (*)[16])&moby->rot_row0));
            fast_vec_add(work, work, (*(float (*)[4])&moby->pos));
            func_L00_00217718_2E16A0(work, direction, 0, 1);
            func_L00_002EC0C8_2E16A0(0);
            hero52.rot[2] = func_001FA748_2E16A0(hero52.rot[2], 3.1415927f);
            moby->state = 4;
        } else {
            int *indices; int count;
            if (moby->state != 5) {
                data->camera_timer = 2;
                func_L00_002EBF50_2E16A0((*(float (*)[4])&moby->pos), (*(float (*)[4])&moby->rot), 1, 0, 0);
            }
            moby->state = 5;
            {
            int waiting = 2, activated = 3;
            indices = data->indices;
            for (count = 3; count >= 0; count--) {
                if (((Moby *)D_L08_0015FFD8)[*indices].state == waiting)
                    ((Moby *)D_L08_0015FFD8)[*indices].state = activated;
                indices++;
            }
            }
        }
    } else if (moby->command == 2) {
        if (func_001F9D48_2E16A0(position, (*(float (*)[4])&hero52.moby->pos)) > 1.0f) {
            moby->command = 0;
            data->exit_timer = func_001F9850_2E16A0(60);
        }
    } else if (visited52[D_0015EE84_2E16A0][moby->mission] == 0xFF) {
        if (func_001F9D48_2E16A0(position, (*(float (*)[4])&hero52.moby->pos)) < 1.0f && hero52.pos[2] - (*(float (*)[4])&moby->pos)[2] < 7.0f && data->exit_timer == 0 && data->health > 0) {
            moby->command = 1;
            hero_state52(50, 1);
        }
    }
    switch (moby->state) {
    case 0: {
        Moby *node; int *indices; int count;
        moby->state = 4; data->health = 200; data->range = 10.0f;
        node = func_0020D348_2E16A0(0x710); data->visual = node;
        if (node) {
            qcopy((*(float (*)[4])&node->pos), (*(float (*)[4])&moby->pos)); qcopy((*(float (*)[4])&node->rot), (*(float (*)[4])&moby->rot));
            node->draw_dist = 0x30; data->visual->visible = 1; (*(long *)&data->visual->spawn_frame) = (*(long *)&moby->spawn_frame);
        }
        (*(float (*)[4])&moby->rot)[1] = 0.0f;
        data->fire_timer = func_001F9850_2E16A0(30); data->exit_timer = 0;
        data->pitch_step = 0.0f; data->yaw_step = 0.0f;
        indices = (int *)&data->linked[7]; count = 7;
        do { *indices-- = 0; } while (--count >= 0);
        data->wave_timer = func_001F9850_2E16A0(180); data->target_timer = 0; data->target_index = 0;
        if (visited52[D_0015EE84_2E16A0][moby->mission] != 0xFF) data->visual_angle = 0.0f;
        else data->visual_angle = 1.5707964f;
        break;
    }
    case 5:
        if (data->health < 0) data->health = 0;
        work[0] = hero52.input[0];
        work[1] = invert52 ? hero52.input[1] : -hero52.input[1];
        magnitude = func_001F9CE8_2E16A0(work);
        if (magnitude > 1.0f) func_L00_001FF328_2E16A0(work, work, magnitude);
        yaw = D_L08_00161AE8_gp * 0.017453292f * D_0015EE6C_2E16A0 * -work[0];
        pitch = -(D_L08_00161AEC_gp * 0.017453292f * D_0015EE6C_2E16A0) * work[1];
        if ((data->yaw_step >= 0.0f && data->yaw_step < yaw) || (data->yaw_step <= 0.0f && yaw < data->yaw_step))
            func_00214D28_2E16A0(&data->yaw_step, yaw, D_L08_00161AF0_gp * 0.017453292f * D_0015EE70_2E16A0);
        else func_00214D28_2E16A0(&data->yaw_step, yaw, D_L08_00161AF8_gp * 0.017453292f * D_0015EE70_2E16A0);
        if ((data->pitch_step >= 0.0f && data->pitch_step < pitch) || (data->pitch_step <= 0.0f && pitch < data->pitch_step))
            func_00214D28_2E16A0(&data->pitch_step, pitch, D_L08_00161AF4_gp * 0.017453292f * D_0015EE70_2E16A0);
        else func_00214D28_2E16A0(&data->pitch_step, pitch, D_L08_00161AFC_gp * 0.017453292f * D_0015EE70_2E16A0);
        (*(float (*)[4])&moby->rot)[2] = func_001FA748_2E16A0((*(float (*)[4])&moby->rot)[2], data->yaw_step);
        (*(float (*)[4])&moby->rot)[1] = func_001FA748_2E16A0((*(float (*)[4])&moby->rot)[1], data->pitch_step);
        if ((*(float (*)[4])&moby->rot)[1] < -0.7853982f) (*(float (*)[4])&moby->rot)[1] = -0.7853982f;
        else if ((*(float (*)[4])&moby->rot)[1] > 0.34906584f) (*(float (*)[4])&moby->rot)[1] = 0.34906584f;
        target = func_L08_002E15A0_2E16A0((char *)moby);
        if (target) {
            if (data->lock_timer < func_001F9850_2E16A0(30)) data->lock_timer++;
        } else if (data->lock_timer) data->lock_timer--;
        tick52(&data->fire_timer);
        if ((*(int *)D_0013CAE0 & 0x20) && data->fire_timer == 0) {
            if (data->barrel == 1) {
                int count;
                func_00215C00_2E16A0(direction, D_L08_00161B00_gp * D_0015EE6C_2E16A0, (*(float (*)[4])&moby->rot)[2], -(*(float (*)[4])&moby->rot)[1]);
                func_00213DE0_2E16A0(data->guns[0], 1, 0, 0);
                func_001F9EC0_2E16A0(muzzle, &D_L08_00161BC0_2E16A0, (*(float (*)[16])&moby->rot_row0));
                fast_vec_add(muzzle, (*(float (*)[4])&moby->pos), muzzle);
                func_L08_002E9288_2E16A0(moby, muzzle, direction, target);
                sound52(0,0,moby);
                cannon_camera52.mag0 = 0.03f;
                cannon_camera52.timer0 = func_001F9850_2E16A0(10);
                cannon_camera52.mag1 = 0.03f;
                cannon_camera52.timer1 = func_001F9850_2E16A0(7);
                data->barrel = 0; data->fire_timer = func_001F9850_2E16A0(20);
                count = 4;
                do {
                    *(u128 *)random_vec = 0;
                    random_vec[0] = func_002140F8_2E16A0(-1.0f,1.0f);
                    random_vec[1] = func_002140F8_2E16A0(-1.0f,1.0f);
                    random_vec[2] = func_002140F8_2E16A0(-1.0f,1.0f);
                    *(u128 *)scatter = *(u128 *)random_vec;
                    fast_vec_normalize(scatter, scatter, fast_vec_length(direction) * (D_0015EE60_2E16A0 * -0.35000002f + 1.0f));
                    fast_vec_add(scatter, direction, scatter);
                    fast_vec_normalize(scatter, scatter, func_002140F8_2E16A0(D_0015EE6C_2E16A0 * 3.0f, D_0015EE6C_2E16A0 * 6.0f));
                    min_life = func_001F9850_2E16A0(5);
                    func_L00_0026EBC0_2E16A0((char *)muzzle, (char *)scatter, 0x5F2F4F6F, func_L00_00258BC8_2E16A0(min_life, func_001F9850_2E16A0(10)), 50000.0f);
                } while (--count >= 0);
                fast_vec_normalize(direction, direction, D_0015EE6C_2E16A0 * 0.5f);
                min_life = func_001F9850_2E16A0(4);
                func_L00_0026EBC0_2E16A0((char *)muzzle, (char *)direction, 0x2F4F7F7F, func_L00_00258BC8_2E16A0(min_life,func_001F9850_2E16A0(7)),500000.0f);
            } else {
                int count;
                func_00215C00_2E16A0(direction, D_L08_00161B00_gp * D_0015EE6C_2E16A0, (*(float (*)[4])&moby->rot)[2], -(*(float (*)[4])&moby->rot)[1]);
                func_00213DE0_2E16A0(data->guns[1], 1, 0, 0);
                func_001F9EC0_2E16A0(muzzle, &D_L08_00161BD0_2E16A0, (*(float (*)[16])&moby->rot_row0));
                fast_vec_add(muzzle, (*(float (*)[4])&moby->pos), muzzle);
                func_L08_002E9288_2E16A0(moby, muzzle, direction, func_L08_002E15A0_2E16A0((char *)moby));
                sound52(0,0,moby);
                cannon_camera52.mag0 = 0.03f;
                cannon_camera52.timer0 = func_001F9850_2E16A0(10);
                cannon_camera52.mag1 = 0.03f;
                cannon_camera52.timer1 = func_001F9850_2E16A0(7);
                data->barrel = 1; data->fire_timer = func_001F9850_2E16A0(20);
                count = 4;
                do {
                    *(u128 *)random_vec = 0;
                    random_vec[0] = func_002140F8_2E16A0(-1.0f,1.0f);
                    random_vec[1] = func_002140F8_2E16A0(-1.0f,1.0f);
                    random_vec[2] = func_002140F8_2E16A0(-1.0f,1.0f);
                    *(u128 *)scatter = *(u128 *)random_vec;
                    fast_vec_normalize(scatter, scatter, fast_vec_length(direction) * (D_0015EE60_2E16A0 * -0.35000002f + 1.0f));
                    fast_vec_add(scatter, direction, scatter);
                    fast_vec_normalize(scatter, scatter, func_002140F8_2E16A0(D_0015EE6C_2E16A0 * 3.0f, D_0015EE6C_2E16A0 * 6.0f));
                    min_life = func_001F9850_2E16A0(5);
                    func_L00_0026EBC0_2E16A0((char *)muzzle, (char *)scatter, 0x5F2F4F6F, func_L00_00258BC8_2E16A0(min_life, func_001F9850_2E16A0(10)), 50000.0f);
                } while (--count >= 0);
                fast_vec_normalize(direction, direction, D_0015EE6C_2E16A0 * 0.5f);
                min_life = func_001F9850_2E16A0(4);
                func_L00_0026EBC0_2E16A0((char *)muzzle, (char *)direction, 0x2F4F7F7F, func_L00_00258BC8_2E16A0(min_life,func_001F9850_2E16A0(7)),500000.0f);
            }
        }
        if (tick52(&data->camera_timer)) {
            func_L08_00315068_2E16A0();
            func_001F9EC0_2E16A0(direction, &D_L08_00161BE0_2E16A0, (*(float (*)[16])&moby->rot_row0));
            fast_vec_add(direction, direction, (*(float (*)[4])&moby->pos));
            func_L00_002EBE88_2E16A0(direction); func_L00_002EBEE0_2E16A0((*(float (*)[4])&moby->rot));
        }
        func_001F49B0_2E16A0(func_L08_002E0258_2E16A0, moby);
        if (tick52(&data->wave_timer)) {
            int count;
            dead = 0;
            all_done = 1; links = data->linked; count = 7;
            do {
                if (*links && (signed char)(*links)->state >= 0)
                    all_done = (*links)->state == 2 ? all_done : 0;
                links++;
            } while (--count >= 0);
            if (all_done) {
                int cutoff = -1;
                int *indices = data->indices;
                int *walk = indices;
                count = 3;
                do {
                    Moby *node;
                    node = (Moby *)((*walk << 8) + (unsigned int)D_L08_0015FFD8);
                    if ((signed char)node->state <= cutoff) dead++;
                    walk++;
                } while (--count >= 0);
                switch (dead) {
                case 0: data->range = 10.0f; data->wave_timer = func_001F9850_2E16A0(285); break;
                case 1: data->range = 12.5f; data->wave_timer = func_001F9850_2E16A0(240); break;
                case 2: data->range = 15.0f; data->wave_timer = func_001F9850_2E16A0(195); break;
                case 3: data->range = 17.5f; data->wave_timer = func_001F9850_2E16A0(150); break;
                case 4:
                    if (visited52[D_0015EE84_2E16A0][moby->mission] != 0xFF) {
                        Moby *node;
                        (*(float (*)[4])&moby->rot)[1] = 0.0f; moby->command = 2;
                        func_L00_002EC0C8_2E16A0(0); moby->state = 4;
                        node = (Moby *)(D_L08_0015FFD8 + (data->platform_index << 8));
                        qcopy(muzzle, (*(float (*)[4])&node->rot)); muzzle[2] = func_001FA748_2E16A0(muzzle[2], 3.1415927f);
                        fast_vec_normalize(direction, (*(float (*)[16])&node->rot_row0), 2.5f);
                        fast_vec_add(direction, direction, (*(float (*)[4])&node->pos));
                        func_L00_00217718_2E16A0(direction, muzzle, 0, 1);
                        func_L08_00307FF0_2E16A0((char *)node, 1);
                        data->visual_angle = 1.5707964f; return;
                    }
                    break;
                }
                if (func_L00_001FEF78_2E16A0(&data->target_timer) || (signed char)((Moby *)D_L08_0015FFD8)[*(int *)((unsigned int)indices + (data->target_index << 2))].state < 0) {
                    data->target_timer = 1; best_angle = 3.1415927f; best = -1;
                    for (i=0; i<4; i++) {
                        Moby *node;
                        node = (Moby *)(D_L08_0015FFD8 + (indices[i] << 8));
                        if (func_001FA850_2E16A0(func_L00_001FF860_2E16A0((*(float (*)[4])&node->pos)[0]-hero52.pos[0], (*(float (*)[4])&node->pos)[1]-hero52.pos[1]), hero52.rot[2]) < best_angle && node->state < 0x7F) {
                            best = i;
                            best_angle = func_001FA850_2E16A0(func_L00_001FF860_2E16A0((*(float (*)[4])&node->pos)[0]-hero52.pos[0], (*(float (*)[4])&node->pos)[1]-hero52.pos[1]), hero52.rot[2]);
                        }
                    }
                    data->target_index = best;
                }
                ((Moby *)D_L08_0015FFD8)[*(int *)((unsigned int)indices + (data->target_index << 2))].command = 1;
            }
        }
        break;
    case 4:
        break;
    case 6:
        func_0020D678_2E16A0(data->guns[0]); func_0020D678_2E16A0(data->guns[1]); func_0020D678_2E16A0(moby); return;
    }
    if (data->guns[0] == 0) {
        Moby *node;
        node = func_0020D348_2E16A0(0x236); data->guns[0] = node;
        if (node) {
            node->update_dist = 0xFF; data->guns[0]->draw_dist = 0x7E; data->guns[0]->visible = 1;
            (*(long *)&data->guns[0]->spawn_frame) = (*(long *)&moby->spawn_frame);
            func_001F9EC0_2E16A0(work, &D_L08_00161BA0_2E16A0, (*(float (*)[16])&moby->rot_row0));
            fast_vec_add(work, (*(float (*)[4])&moby->pos), work);
            {
            Moby *gun = data->guns[0];
            qcopy((*(float (*)[4])&gun->pos), work); qcopy((*(float (*)[4])&gun->rot), (*(float (*)[4])&moby->rot));
            func_L00_00251E30_2E16A0(gun);
        }
        }
    } else {
        func_001F9EC0_2E16A0(work, &D_L08_00161BA0_2E16A0, (*(float (*)[16])&moby->rot_row0));
        fast_vec_add(work, (*(float (*)[4])&moby->pos), work);
        {
            Moby *gun = data->guns[0];
            qcopy((*(float (*)[4])&gun->pos), work); qcopy((*(float (*)[4])&gun->rot), (*(float (*)[4])&moby->rot));
            if ((gun->anim_flags & 2) && gun->prev_seq == 1) func_00213DE0_2E16A0(gun,0,0,0);
        }
    }

    if (data->guns[1] == 0) {
        Moby *node;
        node = func_0020D348_2E16A0(0x236); data->guns[1] = node;
        if (node) {
            node->update_dist = 0xFF; data->guns[1]->draw_dist = 0x7E; data->guns[1]->visible = 1;
            (*(long *)&data->guns[1]->spawn_frame) = (*(long *)&moby->spawn_frame);
            func_001F9EC0_2E16A0(work, &D_L08_00161BB0_2E16A0, (*(float (*)[16])&moby->rot_row0));
            fast_vec_add(work, (*(float (*)[4])&moby->pos), work);
            {
            Moby *gun = data->guns[1];
            qcopy((*(float (*)[4])&gun->pos), work); qcopy((*(float (*)[4])&gun->rot), (*(float (*)[4])&moby->rot));
            func_L00_00251E30_2E16A0(gun);
        }
        }
    } else {
        func_001F9EC0_2E16A0(work, &D_L08_00161BB0_2E16A0, (*(float (*)[16])&moby->rot_row0));
        fast_vec_add(work, (*(float (*)[4])&moby->pos), work);
        {
            Moby *gun = data->guns[1];
            qcopy((*(float (*)[4])&gun->pos), work); qcopy((*(float (*)[4])&gun->rot), (*(float (*)[4])&moby->rot));
            if ((gun->anim_flags & 2) && gun->prev_seq == 1) func_00213DE0_2E16A0(gun,0,0,0);
        }
    }

    if (data->visual) {
        func_001F9EC0_2E16A0(work, &D_L08_00161C80_2E16A0, (*(float (*)[16])&moby->rot_row0));
        fast_vec_add((*(float (*)[4])&data->visual->pos), work, (*(float (*)[4])&moby->pos));
        {
            Moby *visual = data->visual;
            qcopy((*(float (*)[4])&visual->rot), (*(float (*)[4])&moby->rot));
            (*(float (*)[4])&data->visual->rot)[1] = func_001FA748_2E16A0((*(float (*)[4])&visual->rot)[1], data->visual_angle);
        }
        func_L00_00251E30_2E16A0(data->visual);
    }
}
#include "sda.h"

/* Update function for moby classes 441-443 on level 08 (falling debris): picks its spin, falls with a smoke trail, bursts into particles near the ground, then falls on until out of bounds. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E2A38), where it is exact; names translated to the US level program. */

typedef struct {
    float vel[4];              /* 0x00 */
    float spin_x;              /* 0x10 */
    float spin_y;              /* 0x14 */
    int life;                  /* 0x18 */
} Data_2E2A38;

typedef struct {
    char pad0[0x10];
    float pos[4];              /* 0x10 */
    unsigned char state;       /* 0x20 */
    char pad21[0x1F];
    float rot[4];              /* 0x40 */
    char pad50[0x28];
    Data_2E2A38 *data;         /* 0x78 */
} Moby_2E2A38;

typedef struct {
    int v[2];
} Pair_2E2A38;

extern Pair_2E2A38 D_L08_00161C30 __attribute__((section(".data")));
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C_c __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_0015ED70 __attribute__((section(".sdata")));
extern float FUN_001f96b0_c(float) __asm__("FUN_001f96b0");
extern int D_L08_00161C00 __attribute__((sda));
extern int D_L08_00161C04 __attribute__((sda));
extern int D_L08_00161C08 __attribute__((sda));
extern float D_L08_00161C0C __attribute__((sda));
extern float D_L08_00161C10 __attribute__((sda));
extern float D_L08_00161C14 __attribute__((sda));
extern float D_L08_00161C18 __attribute__((sda));
extern int D_L08_00161C1C __attribute__((sda));
extern int D_L08_00161C20 __attribute__((sda));
extern int D_L08_00161C24 __attribute__((sda));
extern float D_L08_00161C28 __attribute__((sda));
extern float D_L08_00161C2C __attribute__((sda));
extern int spawn_spark() __asm__("FUN_00218888");
extern int FUN_L00_00257b90(int, int);
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
void FUN_L00_0026a9f0(void *pos, void *dir, int col, int d, int n, int b1A, int b18, int b19, float x, float y);
extern void func_L00_0026B890_2E2A38(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern char *func_00219780_2E2A38(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void FUN_L08_002e16c0(Moby_2E2A38 *m) {
    Data_2E2A38 *d = m->data;
    float v10[4];
    Pair_2E2A38 tab;
    float v30[4];
    float a40[4];
    int i;

    switch (m->state) {
    case 1:
        d->spin_x = random_float_between_c2(-(D_0015ED6C_c * 1.5707964f), D_0015ED6C_c * 1.5707964f);
        d->spin_y = random_float_between_c2(-(D_0015ED6C_c * 1.5707964f), D_0015ED6C_c * 1.5707964f);
        d->life = (int)random_float_between_c2((float)scale_game_frames_alt(0x5A), (float)scale_game_frames_alt(0x96));
        m->state = 2;
    case 2:
        d->vel[2] -= D_0015ED70 * 10.8f * 0.5f;
        add_vector_xyz(m->pos, m->pos, d);
        m->rot[0] = fast_add_rotations_c(m->rot[0], d->spin_x);
        m->rot[1] = fast_add_rotations_c(m->rot[1], d->spin_y);
        if (m->pos[0] < 8.0f || m->pos[1] < 8.0f || m->pos[0] > 500.0f || m->pos[1] > 500.0f ||
            m->pos[2] > 500.0f) {
            mark_moby_for_removal_c(m);
            return;
        }
        if (random_integer_below_c(3) == 0) {
            int *q;
            int a;
            int b;
            clear_u64_value(v10);
            tab = D_L08_00161C30;
            q = &tab.v[random_integer_below_c(2)];
            {
                int lo = scale_game_frames_alt(15), hi = scale_game_frames_alt(20);
                a = FUN_L00_00257b90(lo, hi);
            }
            {
                int lo = scale_game_frames_alt(25);
                b = FUN_L00_00257b90(lo, scale_game_frames_alt(30));
            }
            func_L00_0026B890_2E2A38(m->pos, v10, 0x2F3F3F7F, *q, a, b, 0, 0, 400000.0f,
                                     random_float_between_c2(8.0f, 16.0f) * D_0015ED6C_c);
        }
        if (m->pos[2] < 15.2f) {
            for (i = 0; i < D_L08_00161C00; i++) {
                float ang = random_angle_radians_c();
                int c1;
                int c2;
                int c3;
                qcopy(a40, m->pos);
                a40[0] += random_float_between_c2(-3.0f, 3.0f);
                a40[1] += random_float_between_c2(-3.0f, 3.0f);
                v10[2] = D_L08_00161C0C * D_0015ED6C_c;
                v10[0] += fast_cos(ang) * D_L08_00161C14;
                v10[1] += fast_sin(ang) * D_L08_00161C14;
                v30[2] = D_L08_00161C10 * D_0015ED6C_c;
                v30[0] += fast_cos(ang) * D_L08_00161C18;
                v30[1] += fast_sin(ang) * D_L08_00161C18;
                v10[3] = D_L08_00161C28;
                v30[3] = D_L08_00161C2C;
                c1 = truncate_float_to_s32_c2(FUN_001f96b0_c(random_float_between_c2((float)D_L08_00161C1C, (float)(D_L08_00161C1C * 2))));
                c2 = truncate_float_to_s32_c2(FUN_001f96b0_c(random_float_between_c2((float)D_L08_00161C20, (float)(D_L08_00161C20 * 2))));
                c3 = truncate_float_to_s32_c2(FUN_001f96b0_c(random_float_between_c2((float)D_L08_00161C24, (float)(D_L08_00161C24 * 2))));
                func_00219780_2E2A38(a40, v10, v30, D_L08_00161C04, D_L08_00161C08, c1, c2, c3, -1);
            }
            m->state = 3;
        }
        break;
    case 3:
        d->vel[2] -= D_0015ED70 * 10.8f * 0.5f;
        add_vector_xyz(m->pos, m->pos, d);
        m->rot[0] = fast_add_rotations_c(m->rot[0], d->spin_x);
        m->rot[1] = fast_add_rotations_c(m->rot[1], d->spin_y);
        if (m->pos[0] < 8.0f || m->pos[1] < 8.0f || m->pos[2] < 8.0f || m->pos[0] > 500.0f ||
            m->pos[1] > 500.0f) {
            mark_moby_for_removal_c(m);
        }
        break;
    }
}
/* Builds the target board: spawns the frame, the three rows of eight targets
 * and their mounts, and records each piece with the piece it hangs from. */

typedef struct {
    struct Moby *moby;
    struct Moby *anchor;
    s32 joint;   /* joint on this piece */
    s32 pjoint;  /* joint on the anchor it hangs from */
} TargetSlot;

typedef struct {
    u8 pad0[0x60];
    TargetSlot slots[34];
    u8 pad280[0xC];
    s16 points;
} TargetBoardVars;

void FUN_L08_002e1d70(struct Moby *moby) {
    TargetBoardVars *vars = (TargetBoardVars *)moby->pvars;
    struct Moby *m;
    s32 i;
    s32 pjoint;

    vars->slots[0].moby = moby;
    vars->slots[0].anchor = 0;
    vars->slots[0].joint = 0;
    vars->slots[0].pjoint = 0;
    vars->slots[3].moby = FUN_L08_002e1c98((char *)moby, 0x1C1, 0, 2);
    vars->slots[3].anchor = moby;
    vars->slots[3].joint = 0;
    vars->slots[3].pjoint = 2;
    pjoint = 3;
    vars->slots[1].moby = FUN_L08_002e1c98((char *)moby, 0x1C2, 0, pjoint);
    vars->slots[1].anchor = moby;
    vars->slots[1].joint = 0;
    vars->slots[1].pjoint = pjoint;
    m = FUN_L08_002e1c98((char *)moby, 0x1C2, 0, 4);
    vars->slots[2].moby = m;
    m->flags |= 0x8000;
    vars->slots[2].anchor = moby;
    vars->slots[2].joint = 0;
    vars->slots[2].pjoint = 4;
    vars->slots[7].moby = FUN_L08_002e1c98((char *)moby, 0x1C0, 0, 0);
    vars->slots[7].anchor = moby;
    vars->slots[7].joint = 0;
    vars->slots[7].pjoint = 0;
    vars->slots[8].moby = FUN_L08_002e1c98((char *)moby, 0x1C0, 1, 1);
    vars->slots[8].anchor = moby;
    vars->slots[8].joint = 1;
    vars->slots[8].pjoint = 1;
    vars->slots[9].moby = FUN_L08_002e1c98((char *)vars->slots[8].moby, 0x1C0, 1, 0);
    vars->slots[9].anchor = vars->slots[8].moby;
    vars->slots[9].joint = 1;
    vars->slots[9].pjoint = 0;
    vars->slots[5].moby = FUN_L08_002e1c98((char *)vars->slots[9].moby, 0x1BD, 0, 0);
    vars->slots[5].anchor = vars->slots[9].moby;
    vars->slots[5].joint = 0;
    vars->slots[5].pjoint = 0;
    vars->slots[6].moby = FUN_L08_002e1c98((char *)vars->slots[7].moby, 0x1BA, 1, 1);
    vars->slots[6].anchor = vars->slots[7].moby;
    vars->slots[6].joint = 1;
    vars->slots[6].pjoint = 1;
    vars->slots[4].moby = FUN_L08_002e1c98((char *)moby, 0x1B9, 0, 0);
    vars->slots[4].anchor = vars->slots[6].moby;
    vars->slots[4].joint = 0;
    vars->slots[4].pjoint = 0;
    for (i = 0; i < 8; i++) {
        vars->slots[10 + i].moby = FUN_L08_002e1c98((char *)vars->slots[7].moby, 0x1BB, 0, i + 2);
        vars->slots[10 + i].anchor = vars->slots[7].moby;
        vars->slots[10 + i].joint = 0;
        vars->slots[10 + i].pjoint = i + 2;
    }
    for (i = 0; i < 8; i++) {
        vars->slots[18 + i].moby = FUN_L08_002e1c98((char *)vars->slots[8].moby, 0x1BB, 0, i + 2);
        vars->slots[18 + i].anchor = vars->slots[8].moby;
        vars->slots[18 + i].joint = 0;
        vars->slots[18 + i].pjoint = i + 2;
    }
    for (i = 0; i < 8; i++) {
        vars->slots[26 + i].moby = FUN_L08_002e1c98((char *)vars->slots[9].moby, 0x1BB, 0, i + 2);
        vars->slots[26 + i].anchor = vars->slots[9].moby;
        vars->slots[26 + i].joint = 0;
        vars->slots[26 + i].pjoint = i + 2;
    }
}
#include "rnc/gameplay/hero.h"
extern float D_L08_00161C3C __attribute__((sda));
extern f32 FUN_001f9e90(f32, f32);

typedef struct {
    struct Moby *part;      /* moved along with the anchor's joint */
    struct Moby *anchor;
    int part_joint;
    int anchor_joint;
} PartLink;

typedef struct {
    u8 pad0[0x60];
    PartLink links[34];     /* 0x60: entry 0 is unused */
} LinkedPartsVars;

/* Keeps the 33 linked parts glued to their anchors' joints and facing the moby, spins three of them (the middle
   one backwards), turns link 3 towards the hero, then updates every part. */
void FUN_L08_002e2078(struct Moby *m) {
    LinkedPartsVars *v = (LinkedPartsVars *)m->pvars;
    PartLink *links = v->links;
    float a[4];
    float b[4];
    float d[4];
    int i;

    for (i = 1; i < 34; i++) {
        struct Moby *part = v->links[i].part;
        if (part != 0) {
            part->rot.z = m->rot.z;
            moby_point_position(part, v->links[i].part_joint, a);
            moby_point_position(v->links[i].anchor, v->links[i].anchor_joint, b);
            FUN_001f9a28(d, b, a);
            FUN_001f9a10(&part->pos, &part->pos, d);
        }
    }
    if (v->links[7].part != 0) {
        v->links[7].part->rot.x = fast_add_rotations(v->links[7].part->rot.x, D_L08_00161C3C * DEG_TO_RAD * D_0015ED6C_d);
    }
    if (v->links[8].part != 0) {
        v->links[8].part->rot.x = fast_add_rotations(v->links[8].part->rot.x, -(D_L08_00161C3C * DEG_TO_RAD * D_0015ED6C_d));
    }
    if (v->links[9].part != 0) {
        v->links[9].part->rot.x = fast_add_rotations(v->links[9].part->rot.x, D_L08_00161C3C * DEG_TO_RAD * D_0015ED6C_d);
    }
    if (v->links[3].part != 0) {
        v->links[3].part->rot.z = FUN_001f9e90(hero.motion.pos.f[0] - v->links[3].part->pos.x, hero.motion.pos.f[1] - v->links[3].part->pos.y);
    }
    for (i = 1; i < 34; i++) {
        if (links[i].part != 0) {
            FUN_L00_00250df8(links[i].part);
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L08_002e2250.s", FUN_L08_002e2250);
#else
extern int target_board_check_hit(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern void FUN_L00_0025f090(void *, void *, s32, f32, f32);
extern void FUN_001f9a28(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern float D_0015ED6C;

void FUN_L08_002e2250(struct Moby *moby) {
    u8 mark = 0xff;
    int selected = 0;
    TargetBoardVars *vars = (TargetBoardVars *)moby->pvars;
    TargetSlot *slots = vars->slots;
    TargetSlot *slot;
    int i = 0;
    int hits = 0;
    int sound = -1;
    int total;

    slot = slots;
    do {
        if (slot->moby != 0) {
            if (target_board_check_hit(slot->moby, 0x10000, 0) != 0) {
                if (i < 7) {
                    if (hits < 1) {
                        hits = 1;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (i < 10) {
                    if (hits < 2) {
                        hits = 2;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (hits < 4) {
                    hits += 4;
                    selected = i;
                    sound = 2;
                    goto mark_hit;
                }
mark_hit:
                slot->moby->hit_slot = mark;
            }
        }
        i++;
        slot++;
    } while (i < 34);
    vars->points = (u16)vars->points + hits;
    total = vars->points;
    if (total >= 8) {
        vars->points = total - (total / 8) * 8;
        sound = 0;
        if (selected != 0) {
            u8 *vector;
            slots += selected;
            slot = slots;
            vector = slot->moby->pvars;
            FUN_L00_0025f090(moby, &slot->moby->pos, -1, 2.0f, 13.0f);
            FUN_001f9a28(vector, &slot->moby->pos, &vars->slots[selected].anchor->pos);
            normalize_vector_xyz(vector, vector, D_0015ED6C * 10.0f);
            slot->moby->state = 1;
            slot->moby = 0;
            return;
        }
        {
            int start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            TargetSlot *base = slots;
            i = 0;
            do {
                int index = (int)((unsigned int)i + (unsigned int)start) % 24 + 10;
                slots = base + index;
                slot = slots;
                i++;
                if (slot->moby != 0) {
                    u8 *vector = slot->moby->pvars;
                    FUN_L00_0025f090(moby, &slot->moby->pos, -1, 2.0f, 13.0f);
                    FUN_001f9a28(vector, &slot->moby->pos, &vars->slots[index].anchor->pos);
                    normalize_vector_xyz(vector, vector, D_0015ED6C * 10.0f);
                    slot->moby->state = 1;
                    slot->moby = 0;
                    return;
                }
            } while (i < 24);
            moby->state = 99;
        }
    }
play_sound:
    if (sound >= 0)
        allocate_voice_for_target_entry(sound, 0, moby);
}
#endif /* NON_MATCHING */
#include "rnc/overlay/swarm_path.h"

/* Gun turret on a gunship: cruises one path firing at its target, or circles a second path firing at random. */
typedef struct {
    SwarmPathFollower follow[2]; /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    PartLink links[34];          /* 0x60: entry 0 is unused */
    s32 target_index;            /* 0x280: moby slot of the target */
    f32 turn_speed;              /* 0x284 */
    s32 fire_timer;              /* 0x288 */
    s16 pad28C;
    s16 sound;                   /* 0x28E */
} GunshipVars;

extern s32 D_L08_00161C4C __attribute__((sda));
extern f32 D_L08_00161C38 __attribute__((sda));
extern f32 D_L08_00161C44 __attribute__((sda));
extern f32 D_L08_00161C48 __attribute__((sda));
extern u8 D_0014C050[];
extern SwarmPath *D_L08_001B0CB0[];
extern struct Moby *D_L08_0015FFD8_m __asm__("D_L08_0015FFD8");

extern s32 FUN_L00_0028d8c0(struct Moby *, s32);
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern f32 multiply_global_factor_ed64(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern s32 tick_countdown_32_alt(s32 *) __asm__("FUN_001f9740");
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern void FUN_L00_0025f090(void *, void *, s32, f32, f32);
extern struct Moby *FUN_L08_002de3e0(void *, struct Moby *, void *, f32, f32);
extern void FUN_L08_002e2250(struct Moby *);

void FUN_L08_002e24e8(struct Moby *moby) {
    GunshipVars *vars = (GunshipVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby *target;
    struct Moby *shot;
    struct Moby *bolt;
    SwarmTargetVars *target_vars;
    Vec4 point;
    Vec4 step;
    Vec4 muzzle;
    Vec4 velocity;
    s32 waypoint;
    s32 next;
    s32 after;
    s32 i;
    s32 slot;
    f32 distance;
    f32 heading;
    f32 next_heading;
    f32 turn;
    f32 pitch;
    f32 yaw;

    if (D_L08_00161C4C != 0) {
        return;
    }
    if (!FUN_L00_0028d8c0(moby, vars->sound)) {
        vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case 0:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->follow[0].path = D_L08_001B0CB0[vars->follow[0].path_id];
        vars->follow[1].path = D_L08_001B0CB0[vars->follow[1].path_id];
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        qcopy(&moby->pos, &vars->follow[1].path->points[0]);
        FUN_L08_002e1d70(moby);
        moby->state = 2;
        moby->rot.z = FUN_001f9e90(vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   vars->follow[1].path->points[1].f[1] - moby->pos.y);
        moby->flags |= 0x4000;
        break;
    case 1:
        waypoint = (vars->follow[0].point + vars->follow[0].path->count + vars->follow[0].direction) % vars->follow[0].path->count;
        target = &D_L08_0015FFD8_m[vars->target_index];
        FUN_L00_00258278(moby,
                         FUN_001f9e90(vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                                      vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
                         &vars->turn_speed, 0.004f, 0.3f, 0.02f);
        FUN_L08_002e2250(moby);
        FUN_L08_002e2078(moby);
        qcopy(&point, &vars->follow[0].path->points[waypoint]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            vars->follow[0].point = waypoint;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > D_L08_00161C38 * frame_time) {
            normalize_vector_xyz(&step, &step, D_L08_00161C38 * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        if (random_integer_below(0xFF) & 1) {
            moby_point_position(vars->links[3].part, 1, &muzzle);
        } else {
            moby_point_position(vars->links[4].part, 1, &muzzle);
        }
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(FUN_001f9e90(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                                 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        normalize_vector_xyz(&velocity, &step, (D_L08_00161C38 + D_L08_00161C38) * frame_time);
        shot = FUN_L08_002de3e0(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case 2:
        follower = &vars->follow[1];
        next = (follower->point + 1) % follower->path->count;
        after = (follower->point + 2) % follower->path->count;
        heading = FUN_001f9e90(follower->path->points[next].f[0] - follower->path->points[follower->point].f[0],
                               follower->path->points[next].f[1] - follower->path->points[follower->point].f[1]);
        next_heading = FUN_001f9e90(follower->path->points[after].f[0] - follower->path->points[next].f[0],
                                    follower->path->points[after].f[1] - follower->path->points[next].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = FUN_001f9b48(&moby->pos, &follower->path->points[next]);
        distance /= FUN_001f9b48(&follower->path->points[follower->point],
                                 &follower->path->points[next]);
        /* the blended heading is computed but unused: the turn below steers towards the next point */
        fast_add_rotations(turn * distance, next_heading);
        heading = FUN_001f9e90(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_0025be00(&moby->rot.z, &vars->turn_speed, heading,
                         D_L08_00161C44 * DEG_TO_RAD * frame_time_sq, D_L08_00161C44 * DEG_TO_RAD * frame_time_sq,
                         D_L08_00161C48 * DEG_TO_RAD * frame_time);
        qcopy(&point, &follower->path->points[next]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > D_L08_00161C38 * frame_time) {
            normalize_vector_xyz(&step, &step, D_L08_00161C38 * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        FUN_L08_002e2078(moby);
        if (!tick_countdown_32_alt(&vars->fire_timer)) {
            break;
        }
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby_point_position(vars->links[4].part, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        normalize_vector_xyz(&velocity, &step, (D_L08_00161C38 + D_L08_00161C38) * frame_time);
        bolt = FUN_L08_002de3e0(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = vars->links[4].part->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case 3: {
        SwarmPath *path;

        moby->state = 1;
        path = vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z = FUN_001f9e90(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        moby->rot.x = 0.0f;
        FUN_L08_002e2078(moby);
        break;
    }
    case 0x63:
        for (i = 1; i < 34; i++) {
            struct Moby *part = vars->links[i].part;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                FUN_L00_0025f090(moby, &part->pos, -1, 3.0f, 13.0f);
                subtract_vector_xyz(drift, &part->pos, &moby->pos);
                normalize_vector_xyz(drift, drift, frame_time * 5.0f);
                part->state = 1;
                vars->links[i].part = NULL;
            }
        }
        FUN_L00_0025f090(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
#include "sda.h"

/* updates matching objects in the selected object range */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E4118), where it is exact; names translated to the US level program. */

extern char *D_L08_0015FFD8;

void FUN_L08_002e2da0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int oclass = 0x279;
    char *base = D_L08_0015FFD8;
    int *ids = (int *)(data + 0x130);
    char **output = (char **)(data + 0x158);
    int i = 9;
    for (;;) {
        char *other = (char *)((*ids << 8) + (unsigned int)base);
        if (*(short *)(other + 0xA6) == oclass) {
            *output++ = other;
        }
        --i;
        if (i < 0)
            return;
        ++ids;
    }
}
extern float D_0013F3D4;
extern float D_0015ED6C;
extern float D_L08_00161C50 __attribute__((sda));
extern char D_001413D0[];
extern void FUN_L00_00263ac8(float, struct Moby *, int, void *);
extern void FUN_L00_0025a120(struct Moby *);
extern void *FUN_L00_0025a420(void *, int, int);
extern void FUN_L00_0025c558(struct Moby *, void *, float, int, int, int);
extern int FUN_L00_0025c698(struct Moby *, void *);
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void FUN_L00_0025d538(struct Moby *, void *);
extern void FUN_L00_0025f730(void *, float);
extern int FUN_L00_0025f878(struct Moby *);
extern void FUN_L00_002a7210(struct Moby *, struct Moby *, float, float);
extern void FUN_00212f90(struct Moby *, int, int, int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9e90(float, float);

typedef struct {
    char pad0[0x29];
    unsigned char b29;
    char pad2A[0x16];
    Vec4 v40;
    char pad50[0x10];
    char x60[7];
    unsigned char b67;
    char pad68[8];
    char x70[0x10];
    float f80, f84, f88, f8C;
    int w90, w94;
    char pad98[0x15];
    unsigned char bAD;
    char padAE[0x22];
    SwarmPathFollower follow[2];
    char pad130[0x28];
    struct Moby *list[0x32];
    int w220;
    float f224;
    char pad228[4];
    int w22C;
    char pad230[0x30];
    unsigned char x260[1];
} BoltThiefVars;

void FUN_L08_002e2df0(struct Moby *moby) {
    Vec4 point;
    Vec4 step;
    BoltThiefVars *data;

    data = moby->pvars;
    FUN_L00_00263ac8(2.5f, moby, 1, data->x260);
    if (moby->visible != 0) {
        if (FUN_001f9b48(&moby->pos, D_L08_001675C0) < 29.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x17;
        }
    }
    if (FUN_L00_0025a420(moby, 0x210000, 0) != 0 && moby->state != 8) {
        int i;

        for (i = 0; i < 0x32; i++) {
            struct Moby *other = data->list[i];

            if (other != 0 && other->state == 3) {
                other->state = 1;
            }
        }
        {
            float speed8 = D_0015ED6C * 8.0f;
            float speed10 = D_0015ED6C * 10.0f;
            data->f80 = 0.008f;
            data->f84 = 0.0005f;
            data->w94 = 1;
            data->f88 = speed8;
            data->f8C = speed10;
            data->bAD = 0;
        }
        FUN_L00_0025c558(moby, data->x70, FUN_001f9e90(moby->pos.x - hero.motion.pos.f[0], moby->pos.y - hero.motion.pos.f[1]), 4, 1, 0);
        data->b67 = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 8;
    }
    moby->hit_slot = 0xFF;
    switch (moby->state) {
    case 0:
        data->follow[0].path = D_L08_001B0CB0[data->follow[0].path_id];
        qcopy(&moby->pos, &data->follow[0].path->points[0]);
        data->follow[0].point = 1;
        if (data->follow[1].path_id >= 0) {
            data->follow[1].path = D_L08_001B0CB0[data->follow[1].path_id];
            data->follow[1].point = 1;
        }
        moby->flags |= 0x1000;
        data->b29 = 0;
        moby->spawn_frame = *(u64 *)(*(char **)D_001413D0 + 0x38);
        clear_u64_value(&data->v40);
        moby->state = 1;
        if (moby->prev_seq != 1) {
            FUN_00212f90(moby, 1, 0, 0);
        }
        break;
    case 1:
        qcopy(&point, &D_L08_001B0CB0[data->w220]->points[0]);
        if (FUN_001f9b48(hero.motion.pos.f, &point) < 24.0f) {
            FUN_L08_002e2da0((char *)moby);
            moby->state = 5;
            if (moby->prev_seq != 3) {
                FUN_00212f90(moby, 3, 0, 6);
            }
        }
        break;
    case 2: {
        SwarmPathFollower *follower;
        SwarmPath *path;
        int next;
        float heading;

        follower = &data->follow[0];
        next = (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = FUN_001f9e90(follower->path->points[next].f[0] - moby->pos.x, follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        path = follower->path;
        qcopy(&point, &path->points[next]);
        if (path->points[next].f[3] == 0.0f) {
            moby->state = 3;
            if (moby->prev_seq != 2) {
                FUN_00212f90(moby, 2, 0, 6);
            }
        } else {
            subtract_vector_xyz(&step, &point, &moby->pos);
            normalize_vector_xyz(&step, &step, D_L08_00161C50 * D_0015ED6C);
            add_vector_xyz(&moby->pos, &moby->pos, &step);
            if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
                follower->point = next;
            }
        }
        break;
    }
    case 3:
        moby->anim_speed = 3.0f;
        if (moby->anim_flags & 2) {
            int i;

            FUN_L08_002e2da0((char *)moby);
            for (i = 0; i < 0x32; i++) {
                struct Moby *other = data->list[i];

                if (other != 0 && (signed char)other->state >= 0) {
                    other->state = 3;
                }
            }
            moby->state = 4;
            if (moby->prev_seq != 6) {
                FUN_00212f90(moby, 6, 0, 6);
            }
            moby->anim_speed = 1.0f;
        }
        break;
    case 4: {
        int active_count;
        int i;

        active_count = 0;
        for (i = 0; i < 0x32; i++) {
            struct Moby *other = data->list[i];

            if (other != 0 && (signed char)other->state >= 0) {
                FUN_L00_0024f7c8(moby, 0, &step);
                active_count++;
                subtract_vector_xyz(&point, &step, &other->pos);
                FUN_L00_0025f730(&point, D_0015ED6C * 4.0f);
                add_vector_xyz(&other->pos, &other->pos, &point);
                if (vector_length_xyz(&point) < 2.0f * D_0015ED6C) {
                    other->state = 4;
                    data->list[i] = 0;
                    data->w22C++;
                }
            }
        }
        if (active_count == 0) {
            moby->state = 5;
            if (moby->prev_seq != 3) {
                FUN_00212f90(moby, 3, 0, 6);
            }
        }
        break;
    }
    case 5: {
        SwarmPathFollower *follower;
        int next;
        int found;
        int i;
        float heading;

        follower = &data->follow[0];
        found = 0;
        next = (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = FUN_001f9e90(follower->path->points[next].f[0] - moby->pos.x, follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[next]);
        for (i = 0; i < 0x32; i++) {
            struct Moby *other = data->list[i];

            if (other != 0 && (signed char)other->state >= 0) {
                if (FUN_001f9b80(&moby->pos, &other->pos) < 6.0f) {
                    if (moby->prev_seq != 6) {
                        FUN_00212f90(moby, 6, 0, scale_game_frames(0x14));
                    }
                    FUN_L00_002a7210(data->list[i], moby, 0.0f, 0.0f);
                    data->list[i] = 0;
                }
                found = 1;
            }
        }
        if (found == 0 && moby->prev_seq != 3) {
            FUN_00212f90(moby, 3, 0, scale_game_frames(0x14));
        }
        if (follower->path->count - 1 == next) {
            if (data->follow[1].path_id >= 0) {
                moby->state = 6;
                break;
            }
            mark_moby_for_removal(moby);
            return;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        normalize_vector_xyz(&step, &step, D_L08_00161C50 * D_0015ED6C);
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        break;
    }
    case 6: {
        SwarmPathFollower *follower;
        int next;
        float heading;

        follower = &data->follow[1];
        next = (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = FUN_001f9e90(follower->path->points[next].f[0] - moby->pos.x, follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[next]);
        subtract_vector_xyz(&step, &point, &moby->pos);
        normalize_vector_xyz(&step, &step, D_L08_00161C50 * D_0015ED6C);
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        if (FUN_001f9b48(hero.motion.pos.f, &moby->pos) > 19.0f) {
            moby->state = 7;
        }
        break;
    }
    case 7: {
        float *hero_position;
        float distance;
        float heading;

        heading = FUN_001f9e90(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
        hero_position = hero.motion.pos.f;
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        distance = FUN_001f9b48(hero_position, &moby->pos);
        if (distance < 17.0f || FUN_L00_0025f878(moby) != 0) {
            SwarmPathFollower *follower;
            int next;

            follower = &data->follow[1];
            next = (follower->point + follower->path->count + follower->direction) % follower->path->count;
            moby->state = 6;
            if (FUN_001f9b48(hero_position, &follower->path->points[next]) < distance) {
                follower->direction *= -1;
            }
        }
        break;
    }
    case 8:
        if (FUN_L00_0025c698(moby, data->x70) & 3) {
            FUN_L00_0025f090(moby, &moby->pos, -1, 1.0f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    FUN_L00_0025d538(moby, data->x60);
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L08_002e3870.s", FUN_L08_002e3870);
#else
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E4BE8), where it is exact; names translated to the US level program. */
/* NEAR: 12 words; the four callback and bolt-table address luis get their callee-saved registers in another order. */
/* Batalia turret bolt: rides its turret, flies to the hero when shot and is collected, falls and bounces when the turret dies. */

typedef struct Hero Hero;

typedef int u128_4BE8 __attribute__((mode(TI)));

typedef struct {
    float v0[4];               /* 0x00 */
    float v10[4];              /* 0x10 */
    float v20[4];              /* 0x20 */
    float v30[4];              /* 0x30 */
    float velocity[4];         /* 0x40 */
    float spin_x, spin_y, spin_z;  /* 0x50 */
    int pad5C;
    float f60, f64;            /* 0x60 */
    int timer;                 /* 0x68 */
    short s6C, s6E;            /* 0x6C */
    union { int i; float f; } u70;
    float f74;                 /* 0x74 */
    union { int i; char *p; } u78;
    union { int i; float f; } u7C;
} Vars_4BE8;

typedef struct {
    char pad0[0x18];
    int moby;                  /* 0x18 */
    char pad1C[4];
    float pos[4];              /* 0x20 */
    char pad30[0x10];
    float n[4];                /* 0x40 */
} Coll_4BE8;

typedef struct {
    u8 pad_00[0xB1];
    u8 id;
} BoltSource_4BE8;

typedef struct {
    u16 unk0;
    u16 bolts;
} BoltSourceStats_4BE8;

typedef struct {
    unsigned char pad[0x454];
    unsigned char killed[1];
} KillFlags_4BE8;

extern BoltSourceStats_4BE8 D_0014D590[][64];
extern Coll_4BE8 D_L08_00174640;
extern KillFlags_4BE8 D_L08_001BADD0;
extern KillFlags_4BE8 D_L08_001BBA30;
extern Moby *D_L08_0015FFDC __attribute__((section(".sdata")));
extern char D_0013E533[];
extern f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float D_L08_00161C88 __attribute__((sda));
extern float D_L08_00161C8C __attribute__((sda));
extern float D_L08_00161C94 __attribute__((sda));
extern float D_L08_00161C98 __attribute__((sda));
extern float D_L08_00161C9C __attribute__((sda));
extern float D_L08_00161CA0 __attribute__((sda));
extern float D_L08_00161CA4 __attribute__((sda));
extern float D_L08_00161CA8 __attribute__((sda));
extern float D_L08_00161CAC __attribute__((sda));
extern float D_L08_00161CB0 __attribute__((sda));
extern short D_L08_00161CB4 __attribute__((sda));
extern short D_L08_00161CB8 __attribute__((sda));
extern short D_L08_00161CBC __attribute__((sda));
extern float D_L08_00161CD0 __attribute__((sda));
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9e90(float, float);
extern float FUN_L00_0025bc00(float *, float, float, int);
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_L08_0015F5CC; /* no foreign declaration */
extern int D_L08_00161CC0 __attribute__((sda));
extern int D_L08_00161CC4 __attribute__((sda));
extern int D_L08_00161CC8 __attribute__((sda));
extern int D_L08_00161CCC __attribute__((sda));
extern int D_L08_00161CD4 __attribute__((sda));
extern int D_L08_00174658; /* no foreign declaration */
extern int FUN_001efa68(void *, void *, int, int, int);
extern int FUN_L00_001f0d60(float, void *, int, void *);
extern int FUN_L00_002604f0(void *, int, void *, void *, void *, void *);
extern int queue_animation_update_alt(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 D_0013DF38[];
extern s32 D_0015ED84 __attribute__((section(".sdata")));
extern s32 D_0015ED98 __attribute__((section(".sdata")));
extern s32 allocate_voice_for_target_entry_alt(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 tick_countdown_32_alt(s32 *) __asm__("FUN_001f9740");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern u8 D_0014C050[];
extern unsigned char *FUN_L00_002603d0(int);
extern unsigned char *FUN_L00_00272060(float *pos, int a, float f0, float f1, int col, int mode, int b, float *vel, float f2);
extern unsigned char D_0014BEC0[][4];
extern unsigned char D_L08_0015FC88[] __attribute__((section(".sdata")));
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_00257d78(void *, float, float);
extern void FUN_L00_0025f730(void *a, float x);
extern void FUN_L00_00260668(void *, int, void *, void *, void *, void *);
extern void FUN_L08_0026a0f8(void *, void *, float);
extern void FUN_L08_002e4db8_3870(char *a, float *v) __asm__("FUN_L08_002e4db8");
extern void FUN_L08_002e4e90_3870(char *m, float *out, float *rot) __asm__("FUN_L08_002e4e90");
extern void FUN_L08_002e5188_3870(char *m) __asm__("FUN_L08_002e5188");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void multiply_matrix_basis_columns(void *, void *, void *) __asm__("func_001FA328");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013F350_3870[] __asm__("D_0013F350");
extern char D_0013F3D0[];
extern char D_0013F490[];
extern float func_001FA850_2E16A0(float, float) __asm__("FUN_001fa688");
extern float func_001F9CE8_2E16A0(void *) __asm__("FUN_001f9b20");
extern float func_00214D28_2E16A0(float *,float,float) __asm__("FUN_00213ed8");
extern void func_001F49B0_2E16A0(void *,void *) __asm__("FUN_001f4600");
extern void func_L00_0023A658_4BE8(void) __asm__("FUN_L08_002444a8");
extern void func_L00_0023A690_4BE8(void) __asm__("FUN_L08_002444e0");
extern void func_L00_0023A788_4BE8(void) __asm__("FUN_L08_002445d8");
extern unsigned char D_0014C050_4BE8[][16] __asm__("D_0014C050");
extern int D_L08_00174658_lui __asm__("D_L08_00174658") __attribute__((section(".data")));

void FUN_L08_002e3870(Moby *moby) {
    float dir[4];
    float point[4];
    float local[4];
    float scratch[16];

    switch (moby->state) {
    case 0: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
        int linked;

        switch (moby->oclass) {
        case 0x279:
            moby->scale = moby->pclass->scale * *(float *)&D_L08_00161CB4;
            break;
        case 0x3DC:
            moby->scale = moby->pclass->scale * *(float *)&D_L08_00161CB8;
            break;
        case 0x3DD:
            moby->scale = moby->pclass->scale * *(float *)&D_L08_00161CBC;
            break;
        }
        linked = 0;
        if (D_L08_00161CD4 > D_L08_0015F5CC) {
            D_L08_00161CD4 = D_L08_0015F5CC;
        }
        moby->flags &= ~0x100;
        qcopy(vars->v0, &moby->pos);
        if (vars->s6E == 0) {
            normalize_vector_xyz(point, &moby->rot_row2, 0.1f);
            add_vector_xyz(point, point, &moby->pos);
            scale_vector_xyz(local, &moby->rot_row2, -2.0f);
            add_vector_xyz(local, local, &moby->pos);
            if (FUN_001efa68(point, local, 2, 0, 0) != 0) {
                qcopy(vars->v0, D_L08_00174640.pos);
                linked = D_L08_00174640.moby;
                normalize_vector_xyz(dir, D_L08_00174640.n, 1.0f);
            } else {
                qcopy(vars->v0, &moby->pos);
                clear_u64_value(dir);
                dir[2] = 1.0f;
            }
        }
        FUN_L08_002e4db8_3870((char *)moby, dir);
        vars->f60 = random_angle_radians();
        vars->f64 = moby->rot.x;
        FUN_L08_002e4e90_3870((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        if (linked != 0 && FUN_L00_002603d0(linked) != 0) {
            vars->u7C.i = linked;
            FUN_L00_00260668(moby, linked, vars->v0, vars->v10, vars->v20, vars->v30);
        }
        moby->state = 1;
        vars->s6C = 0;
        vars->u70.i = truncate_float_to_s32(random_float_between(0.0f, (float)scale_game_frames_alt(600)));
        vars->u78.i = D_L08_00161CCC;
        break;
    }
    case 1: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        if (vars->u7C.i != 0) {
            FUN_L00_002604f0(moby, vars->u7C.i, vars->v20, vars->v30, vars->v0, vars->v10);
            if ((signed char)((Moby *)vars->u7C.i)->state < 0) {
                vars = (Vars_4BE8 *)moby->pvars;
                clear_u64_value(vars->velocity);
                FUN_L08_0026a0f8(vars, &moby->rot_row1, D_L08_00161C98 * 0.017453292f * D_0015ED6C);
                moby->state = 4;
                break;
            }
        }
        if (absolute_float(moby->pos.z - ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f[2]) < ((Hero *)(((char *)&D_0013F350_3870)))->reach_distance
            && FUN_001f9b80(&moby->pos, ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f) < ((Hero *)(((char *)&D_0013F350_3870)))->reach_height) {
            vars->timer = scale_game_frames_alt(1);
            moby->state = 2;
        }
        if (moby->visible == 0) {
            if (moby > D_L08_0015FFDC && FUN_001f9b80(&moby->pos, ((char *)&D_0013F3D0)) > 24.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            break;
        }
        switch (moby->oclass) {
        case 0x3DD:
            vars->f60 = fast_add_rotations(vars->f60, D_L08_00161CAC * 0.017453292f * D_0015ED6C);
            vars->f64 = fast_add_rotations(vars->f64, D_L08_00161CB0 * 0.017453292f * D_0015ED6C);
            break;
        case 0x3DC:
            vars->f60 = fast_add_rotations(vars->f60, D_L08_00161CA4 * 0.017453292f * D_0015ED6C);
            vars->f64 = fast_add_rotations(vars->f64, D_L08_00161CA8 * 0.017453292f * D_0015ED6C);
            break;
        default:
            vars->f60 = fast_add_rotations(vars->f60, D_L08_00161C9C * 0.017453292f * D_0015ED6C);
            vars->f64 = fast_add_rotations(vars->f64, D_L08_00161CA0 * 0.017453292f * D_0015ED6C);
            break;
        }
        FUN_L08_002e4e90_3870((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        if (vars->s6C != 0) {
            vars->f74 = fast_add_rotations(vars->f74, D_0015ED6C * 5.2359877f);
            if (vars->u70.i > 12) {
                vars->u78.i += 0x5000000;
            } else {
                vars->u78.i -= 0x5000000;
            }
            if (tick_countdown_32_alt(&vars->u70.i) != 0) {
                vars->s6C = 0;
                vars->u70.i = truncate_float_to_s32(random_float_between((float)scale_game_frames_alt(300), (float)scale_game_frames_alt(600)));
            } else if (moby->visible != 0 && D_L08_00161CC8 != 0) {
                func_001F49B0_2E16A0(FUN_L08_002e5188_3870, moby);
                return;
            }
        } else if (tick_countdown_32_alt(&vars->u70.i) != 0) {
            vars->u70.i = 24;
            vars->f74 = random_angle_radians();
            vars->s6C = 1;
        }
        break;
    }
    case 2: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        euler_rotation_matrix(scratch, vars->v10);
        qcopy(dir, scratch + 8);
        if (tick_countdown_32_alt(&vars->timer) != 0) {
            Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
            float deg_to_rad = 0.017453292f;

            subtract_vector_xyz(point, &moby->pos, ((char *)&D_0013F3D0));
            normalize_vector_xyz(point, point, 1.0f);
            cross_vectors_xyz(point, dir, point);
            FUN_L08_0026a0f8(vars, point, D_L08_00161C94 * deg_to_rad * D_0015ED6C);
            moby->flags |= 0x100;
            normalize_vector_xyz(vars->velocity, dir, D_0015ED6C * 8.0f);
            vars->f74 = 0.0f;
            vars->u70.f = D_0015ED6C * 8.0f;
            vars->u78.p = *(char **)((char *)&D_0013F3D0 + 0x2000);
            vars->u7C.f = 0.5f;
            if (D_L08_00161CD4 + scale_game_frames_alt(3) < D_L08_0015F5CC) {
                D_L08_00161CD4 = D_L08_0015F5CC;
                allocate_voice_for_target_entry_alt(0, 0x20, moby);
                vars->f64 = 0.0f;
            } else {
                vars->f64 = random_float_between(-30.0f, 30.0f) * deg_to_rad;
            }
            moby->state = 3;
        } else {
            if (scale_game_frames_alt(10) < vars->timer) {
                float goal_yaw = fast_add_rotations(vars->f60, fast_sin((float)(vars->timer % 8 - 4) * 0.7854f) * 0.17453292f);

                FUN_L00_0025bc00(&vars->f60, goal_yaw, 0.5235988f, 0);
            } else {
                float goal_yaw = fast_add_rotations(FUN_001f9e90(((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f[0] - moby->pos.x,
                                                          ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f[1] - moby->pos.y),
                                        1.5707964f);

                FUN_L00_0025bc00(&vars->f60, goal_yaw, 0.08726646f, 0);
                if (func_001FA850_2E16A0(vars->f60, goal_yaw) < 0.08726646f) {
                    vars->timer = 0;
                }
            }
            FUN_L08_002e4e90_3870((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        }
        break;
    }
    case 3: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
        float dist;
        float fade_range;

        if (vars->u78.p == 0) {
            break;
        }
        normalize_vector_xyz(point, vars->u78.p + 0xE0, vars->u7C.f);
        add_vector_xyz(point, point, vars->u78.p + 0x10);
        subtract_vector_xyz(dir, point, &moby->pos);
        if (vars->f64 != 0.0f) {
            float sine = fast_sin(vars->f64);
            float cosine = fast_cos(vars->f64);

            dir[0] = cosine * dir[0] + sine * dir[1];
            dir[1] = cosine * dir[1] - sine * dir[0];
        }
        dist = distance_xyz(point, &moby->pos);
        normalize_vector_xyz(vars->velocity, vars->velocity, vars->u70.f);
        add_vector_xyz(&moby->pos, &moby->pos, vars->velocity);
        normalize_vector_xyz(dir, dir, vars->f74);
        add_vector_xyz(&moby->pos, &moby->pos, dir);
        add_vector_xyz(&moby->pos, &moby->pos, ((char *)&D_0013F490));
        subtract_vector_xyz(local, &moby->pos, ((char *)&D_0013F490 + 0x160));
        transform_vector(local, local, ((char *)&D_0013F490 - 0x100));
        if (((Moby *)vars->u78.p)->oclass != 0 || local[2] > 0.0f) {
            func_00214D28_2E16A0(&vars->u70.f, 0.0f, D_L08_00161C8C * D_0015ED70);
            func_00214D28_2E16A0(&vars->f74, D_L08_00161C88 * D_0015ED6C, D_L08_00161C8C * D_0015ED70);
        } else {
            normalize_vector_xyz(scratch, ((char *)&D_0013F490 - 0x120), vars->u70.f);
            subtract_vector_xyz(scratch + 4, scratch, vars->velocity);
            FUN_L00_0025f730(scratch + 4, D_L08_00161C8C * D_0015ED70);
            add_vector_xyz(vars->velocity, vars->velocity, scratch + 4);
        }
        if (dist < vars->f74) {
            vars->f74 = dist;
        }
        fade_range = vars->f74 * (float)scale_game_frames_alt(20);
        if (dist < fade_range) {
            if (distance_xyz(point, &moby->pos) > 0.52f) {
                if (D_L08_00161CC0 != 0) {
                    moby->alpha = (distance_xyz(point, &moby->pos) + fade_range) * 128.0f / (fade_range + fade_range);
                }
                if (D_L08_00161CC4 != 0) {
                    moby->scale = moby->pclass->scale * *(float *)&D_L08_00161CB4 * (distance_xyz(point, &moby->pos) + fade_range) / (fade_range + fade_range);
                }
            } else {
                int bolts;
                int spin;
                float shrink;
                float size;
                BoltSource_4BE8 *source;

                switch (moby->oclass) {
                case 0x305: {
                    int i;
                    int j;
                    int collected;

                    D_0014BEC0[D_0015ED84][moby->command] = 1;
                    collected = 0;
                    for (i = 0; i < 20; i++) {
                        for (j = 0; j < 4; j++) {
                            if (D_0014BEC0[i][j] != 0) {
                                collected++;
                            }
                        }
                    }
                    bolts = collected * 100;
                    break;
                }
                case 0x3DD:
                    bolts = 10;
                    break;
                case 0x3DC:
                    bolts = 5;
                    break;
                default:
                    bolts = 1;
                    break;
                }
                shrink = 0.7f;
                FUN_L00_00257d78(scratch, D_0015ED6C * shrink, D_0015ED6C);
                scale_vector_xyz(scratch + 4, scratch, 10.0f);
                add_vector_xyz(scratch + 4, scratch + 4, &moby->pos);
                spin = random_integer_below(2);
                if (spin == 0) {
                    spin--;
                }
                size = random_float_between(0.4f, 0.5f);
                FUN_L00_00272060(scratch + 4, scale_game_frames_alt(0x19), size * 0.2f, size, 0x7F207F7F, 0, spin, scratch, 0.0f);
                spin = -spin;
                FUN_L00_00272060(scratch + 4, scale_game_frames_alt(0x19), size * 0.14f, size * shrink, 0x7F7F7F7F, 1, spin, scratch, 0.0f);
                source = moby->parent;
                if (source && !(source->id & 0x80)) {
                    D_0014D590[D_0015ED84][source->id].bolts += bolts;
                }
                {
                    void *f0 = func_L00_0023A658_4BE8, *f1 = func_L00_0023A690_4BE8, *f2 = func_L00_0023A788_4BE8;
                    D_0015ED98 += bolts;
                    D_0013DF38[D_0015ED84] += bolts;
                    queue_animation_update_alt(2, 0x754E, f0, f1, f2, &D_0015ED98, 9999999);
                }
                if (moby < D_L08_0015FFDC) {
                    D_L08_001BADD0.killed[(short)moby->save_id] = moby->mission + 2;
                    if (moby->mission == 0xFF || (D_L08_0015FC88[moby->mission] != 0xFF && D_0014C050_4BE8[D_0015ED84][moby->mission] == 0xFF)) {
                        D_L08_001BBA30.killed[(short)moby->save_id] = moby->mission + 2;
                    }
                }
                mark_moby_for_removal(moby);
                return;
            }
        }
        multiply_matrix_basis_columns(&moby->rot_row0, vars, &moby->rot_row0);
        break;
    }
    case 4: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        add_vector_xyz(&moby->pos, &moby->pos, vars->velocity);
        vars->velocity[2] -= D_0015ED70 * 10.8f;
        multiply_matrix_basis_columns(&moby->rot_row0, vars, &moby->rot_row0);
        if (moby->pos.z < 2.0f || moby->pos.x > 510.0f || moby->pos.x < 2.0f || moby->pos.y > 510.0f || moby->pos.y < 2.0f) {
            int bolts;
            BoltSource_4BE8 *source;

            switch (moby->oclass) {
            case 0x3DD:
                bolts = 10;
                break;
            case 0x3DC:
                bolts = 5;
                break;
            default:
                bolts = 1;
                break;
            }
            source = moby->parent;
            if (source && !(source->id & 0x80)) {
                D_0014D590[D_0015ED84][source->id].bolts += bolts;
            }
            D_0015ED98 += bolts;
            D_0013DF38[D_0015ED84] += bolts;
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < vars->f60 - 4.0f && moby->pos.z < ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f[2] - 4.0f) {
            Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

            *(u128_4BE8 *)vars->v10 = *(u128_4BE8 *)((char *)&D_0013F350_3870 + 0x90);
            vars->timer = scale_game_frames_alt(1);
            moby->state = 2;
            break;
        }
        if (FUN_L00_001f0d60(0.25f, &moby->pos, 0x22, moby) != 0) {
            subtract_vector_xyz(dir, &moby->pos, D_L08_00174640.pos);
            FUN_L00_001ff660(vars->velocity, vars->velocity, dir);
            if (FUN_001f9e90(dir[2], func_001F9CE8_2E16A0(dir)) < 0.7853982f
                && FUN_001f9e90(D_L08_00174640.n[2], func_001F9CE8_2E16A0(D_L08_00174640.n)) < 0.7853982f) {
                Coll_4BE8 *hit = &D_L08_00174640;
                int frames;
                float per_frame;

                moby->flags &= ~0x100;
                vars = (Vars_4BE8 *)moby->pvars;
                frames = truncate_float_to_s32(multiply_global_factor_ed64(D_L08_00161CD0 + random_float_between(-10.0f, 10.0f)));
                per_frame = 1.0f / convert_integer_to_float(frames);
                qcopy(vars->v0, hit->pos);
                matrix_to_euler(&moby->rot_row0, &moby->rot);
                normalize_vector_xyz(dir, hit->n, 1.0f);
                FUN_L08_002e4db8_3870((char *)moby, dir);
                vars->f60 = random_angle_radians();
                vars->f64 = random_angle_radians();
                FUN_L08_002e4e90_3870((char *)moby, vars->v20, vars->v30);
                subtract_vector_xyz(vars->velocity, vars->v20, &moby->pos);
                scale_vector_xyz(vars->velocity, vars->velocity, per_frame);
                if (hit->moby != 0) {
                    unsigned char *carrier = FUN_L00_002603d0(hit->moby);

                    if (carrier != 0) {
                        add_vector_xyz(vars->velocity, vars->velocity, carrier + 0x10);
                        scale_vector_xyz(point, carrier + 0x10, convert_integer_to_float(frames));
                        add_vector_xyz(vars->v20, vars->v20, point);
                        add_vector_xyz(vars->v0, vars->v0, point);
                    }
                }
                vars->velocity[2] += D_0015ED70 * 10.8f * (float)(frames >> 1);
                vars->spin_x = fast_subtract_rotations(vars->v30[0], moby->rot.x) * per_frame;
                vars->spin_y = fast_subtract_rotations(vars->v30[1], moby->rot.y) * per_frame;
                vars->spin_z = fast_subtract_rotations(vars->v30[2], moby->rot.z) * per_frame;
                moby->state = 5;
                vars->timer = frames;
                vars->u7C.i = D_L08_00174658_lui;
                if (absolute_float(moby->pos.z - ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f[2]) < ((Hero *)(((char *)&D_0013F350_3870)))->reach_distance
                    && FUN_001f9b80(&moby->pos, ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f) < ((Hero *)(((char *)&D_0013F350_3870)))->reach_height) {
                    Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

                    vars->timer = scale_game_frames_alt(1);
                    moby->state = 2;
                }
            }
        }
        break;
    }
    case 5: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin_x);
        moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin_y);
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin_z);
        add_vector_xyz(&moby->pos, &moby->pos, vars->velocity);
        vars->velocity[2] -= D_0015ED70 * 10.8f;
        if (tick_countdown_32_alt(&vars->timer) != 0) {
            int frames;
            float per_frame;

            if (absolute_float(moby->pos.z - ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f[2]) < ((Hero *)(((char *)&D_0013F350_3870)))->reach_distance
                && FUN_001f9b80(&moby->pos, ((Hero *)(((char *)&D_0013F350_3870)))->motion.pos.f) < ((Hero *)(((char *)&D_0013F350_3870)))->reach_height) {
                vars->timer = scale_game_frames_alt(1);
                moby->state = 2;
                break;
            }
            if (moby->command != 0) {
                moby->state = 1;
                if (vars->u7C.i != 0) {
                    Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

                    FUN_L00_00260668(moby, vars->u7C.i, vars->v0, vars->v10, vars->v20, vars->v30);
                } else {
                    qcopy(&moby->pos, vars->v20);
                    qcopy(&moby->rot, vars->v30);
                }
                break;
            }
            frames = truncate_float_to_s32((float)scale_game_frames_alt(20));
            per_frame = 1.0f / convert_integer_to_float(frames);
            moby->command++;
            vars->spin_x = fast_subtract_rotations(vars->v30[0], moby->rot.x) * per_frame;
            vars->spin_y = fast_subtract_rotations(vars->v30[1], moby->rot.y) * per_frame;
            vars->spin_z = fast_subtract_rotations(vars->v30[2], moby->rot.z) * per_frame;
            subtract_vector_xyz(vars->velocity, vars->v20, &moby->pos);
            scale_vector_xyz(vars->velocity, vars->velocity, per_frame);
            vars->timer = frames;
            if (vars->u7C.i != 0) {
                unsigned char *carrier = FUN_L00_002603d0(vars->u7C.i);

                if (carrier != 0) {
                    add_vector_xyz(vars->velocity, vars->velocity, carrier + 0x10);
                    scale_vector_xyz(dir, carrier + 0x10, convert_integer_to_float(frames));
                    add_vector_xyz(vars->v20, vars->v20, dir);
                    add_vector_xyz(vars->v0, vars->v0, dir);
                }
            }
            vars->velocity[2] += D_0015ED70 * 10.8f * (float)(frames >> 1);
        }
        break;
    }
    }
}
#endif

#include "qcopy.h"

/* aims a turret: builds its barrel frame from the base rotation, yaw and pitch, writes the barrel rotation
 * and the muzzle point (offset per turret class) */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E6208), where it is exact; names translated to the US level program. */

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern short D_L08_00161C64 __attribute__((sda));
extern short D_L08_00161C7C __attribute__((sda));
extern short D_L08_00161C80 __attribute__((sda));
extern short D_L08_00161C78 __attribute__((sda));
extern short D_L08_00161CB8 __attribute__((sda));
extern short D_L08_00161CBC __attribute__((sda));
extern short D_L08_00161C84 __attribute__((sda));
extern short D_L08_00161C68 __attribute__((sda));
extern short D_L08_00161C6C __attribute__((sda));
extern short D_L08_00161CB4 __attribute__((sda));
extern short D_L08_00161C70 __attribute__((sda));
extern short D_L08_00161C74 __attribute__((sda));
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");

void FUN_L08_002e4e90(struct Moby *moby, float *out, float *rot) {
    float r[16];
    float b[16];
    float up[4];
    float side[4];
    float fwd[4];
    float dir[4];
    float u[4];
    float v[4];
    char *data = (char *)moby->pvars;
    float sx, sy, sz;
    float t;
    switch (moby->oclass) {
    case 0x3DC:
        sx = *(float *)&D_L08_00161C68 * *(float *)&D_L08_00161CB8;
        sy = *(float *)&D_L08_00161C74 * *(float *)&D_L08_00161CB8;
        sz = *(float *)&D_L08_00161C80 * *(float *)&D_L08_00161CB8;
        break;
    case 0x3DD:
        sx = *(float *)&D_L08_00161C6C * *(float *)&D_L08_00161CBC;
        sy = *(float *)&D_L08_00161C78 * *(float *)&D_L08_00161CBC;
        sz = *(float *)&D_L08_00161C84 * *(float *)&D_L08_00161CBC;
        break;
    default:
        sx = *(float *)&D_L08_00161C64 * *(float *)&D_L08_00161CB4;
        sy = *(float *)&D_L08_00161C70 * *(float *)&D_L08_00161CB4;
        sz = *(float *)&D_L08_00161C7C * *(float *)&D_L08_00161CB4;
        break;
    }
    euler_rotation_matrix(b, (float *)(data + 0x10));
    normalize_vector_xyz(u, b, -fast_sin(*(float *)(data + 0x60)));
    normalize_vector_xyz(v, b + 4, fast_cos(*(float *)(data + 0x60)));
    add_vector_xyz(dir, u, v);
    t = sx * DEG_TO_RAD;
    normalize_vector_xyz(u, dir, fast_cos(t));
    normalize_vector_xyz(v, b + 8, fast_sin(t));
    add_vector_xyz(up, u, v);
    normalize_vector_xyz(u, b, -fast_cos(*(float *)(data + 0x60)));
    normalize_vector_xyz(v, b + 4, -fast_sin(*(float *)(data + 0x60)));
    add_vector_xyz(fwd, u, v);
    cross_vectors_xyz(side, up, fwd);
    normalize_vector_xyz(u, side, fast_cos(*(float *)(data + 0x64)));
    normalize_vector_xyz(v, fwd, fast_sin(*(float *)(data + 0x64)));
    add_vector_xyz(r, u, v);
    normalize_vector_xyz(u, side, -fast_sin(*(float *)(data + 0x64)));
    normalize_vector_xyz(v, fwd, fast_cos(*(float *)(data + 0x64)));
    add_vector_xyz(r + 4, u, v);
    qcopy(r + 8, up);
    r[11] = 0.0f;
    clear_u64_value(r + 12);
    r[15] = 1.0f;
    matrix_to_euler(r, rot);
    rot[3] = 0.0f;
    normalize_vector_xyz(u, b + 8, sy);
    add_vector_xyz(out, data, u);
    normalize_vector_xyz(v, dir, sz);
    add_vector_xyz(out, out, v);
}
typedef struct {
    s64 vertices[4];
    u32 colors[4];
    u32 texture_coordinates[4];
} Quad_5188;

extern void add_vector_xyz_5188(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void normalize_vector_xyz_5188(void *, void *, float) __asm__("FUN_001f9bf8");
extern float vector_length_5188(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9d20_5188(void *, void *, void *) __asm__("FUN_001f9d20");
extern float fast_cos_5188(float) __asm__("FUN_001f9dc8");
extern float fast_sin_5188(float) __asm__("FUN_001f9de0");
extern float float_from_int_5188(int) __asm__("FUN_001fa6c0");
extern int truncate_float_to_s32_5188(float) __asm__("FUN_001fa6d0");
extern float add_angle_5188(float, float) __asm__("FUN_001fa580");
extern void vu1_add_g_sregister_5188(int, u64) __asm__("FUN_00233980");
extern u64 get_effect_texture_5188(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad_5188(void *, void *, void *, u64, int) __asm__("FUN_L02_0020bc88");
extern float D_L08_00161C5C __attribute__((sda));
extern float D_L08_00161C60 __attribute__((sda));
extern char D_L08_0016D240[] __attribute__((section(".data")));

void FUN_L08_002e5188(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    Quad_5188 quad;
    float pos[4];
    float direction[4];
    float length;
    float radius;
    float x0, x1, x2, x3, y0, y1, y2, y3;
    int base_x, base_y;
    s64 base_z;
    u32 color;
    char *projection_constants;

    if (*(short *)(data + 0x6c) == 0) return;

    qcopy(pos, &moby->pos);
    normalize_vector_xyz_5188(direction, (char *)moby + 0xe0, D_L08_00161C5C);
    add_vector_xyz_5188(pos, pos, direction);
    normalize_vector_xyz_5188(direction, (char *)moby + 0xc0, D_L08_00161C60);
    add_vector_xyz_5188(pos, pos, direction);
    fast_vec_sub(pos, pos, D_L08_001675C0);
    length = vector_length_5188(pos);
    pos[3] = 1.0f;
    fast_vec_scale(pos, pos, 1024.0f);
    FUN_001f9d20_5188(pos, pos, D_L08_001675C0 - 0x100);
    projection_constants = D_L08_0016D240;
    fast_vec_scale(pos, pos, *(float *)(projection_constants + 0x210) / pos[3]);

    base_x = truncate_float_to_s32_5188(pos[0] * 16.0f) + 0x8000;
    base_y = (truncate_float_to_s32_5188(pos[1] * 16.0f) + 0x8000) << 16;
    base_z = (s64)truncate_float_to_s32_5188(pos[2] * 0.9997f + *(float *)(projection_constants + 0x1a8)) << 32;
    if (length > 16.0f) length = 16.0f;
    else if (length < 4.0f) length = 4.0f;
    radius = float_from_int_5188(*(signed char *)(data + 0x7b) + 16) * 0.015625f * (50.0f - length * 2.5f);
    x0 = radius * fast_sin_5188(*(float *)(data + 0x74));
    y0 = radius * fast_cos_5188(*(float *)(data + 0x74));
    x1 = radius * fast_sin_5188(add_angle_5188(*(float *)(data + 0x74), 1.57079637f));
    y1 = radius * fast_cos_5188(add_angle_5188(*(float *)(data + 0x74), 1.57079637f));
    x2 = radius * fast_sin_5188(add_angle_5188(*(float *)(data + 0x74), 4.71238899f));
    y2 = radius * fast_cos_5188(add_angle_5188(*(float *)(data + 0x74), 4.71238899f));
    x3 = radius * fast_sin_5188(add_angle_5188(*(float *)(data + 0x74), 3.14159274f));
    y3 = radius * fast_cos_5188(add_angle_5188(*(float *)(data + 0x74), 3.14159274f));

    quad.texture_coordinates[0] = 0;
    quad.texture_coordinates[1] = 0x200;
    quad.texture_coordinates[2] = 0x2000000;
    quad.texture_coordinates[3] = 0x2000200;
    color = *(u32 *)(data + 0x78);
    quad.colors[3] = color;
    quad.colors[2] = color;
    quad.colors[1] = color;
    quad.colors[0] = color;
    quad.vertices[3] = base_z;
    quad.vertices[2] = base_z;
    quad.vertices[1] = base_z;
    quad.vertices[0] = base_z;
    quad.vertices[0] += (truncate_float_to_s32_5188(y0) << 20) + base_y;
    quad.vertices[0] += (truncate_float_to_s32_5188(x0) << 4) + base_x;
    quad.vertices[1] += (truncate_float_to_s32_5188(y1) << 20) + base_y;
    quad.vertices[1] += (truncate_float_to_s32_5188(x1) << 4) + base_x;
    quad.vertices[2] += (truncate_float_to_s32_5188(y2) << 20) + base_y;
    quad.vertices[2] += (truncate_float_to_s32_5188(x2) << 4) + base_x;
    quad.vertices[3] += (truncate_float_to_s32_5188(y3) << 20) + base_y;
    quad.vertices[3] += (truncate_float_to_s32_5188(x3) << 4) + base_x;
    vu1_add_g_sregister_5188(0x42, 0x8000000048LL);
    draw_geometry_quad_5188(quad.vertices, quad.texture_coordinates, quad.colors, get_effect_texture_5188(0x13), 1);
    vu1_add_g_sregister_5188(0x42, 0x8000000044LL);
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9CB0), where it is exact; names translated to the US level program. */


extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C;
extern s32 allocate_voice_for_target_entry_alt(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void func_0022ED80_2E9CB0(int, int, int) __asm__("FUN_0022da68");

void FUN_L08_002e88d8(char *m) {
    char *data = *(char **)(m + 0x78);
    char *base = data + 0x60;
    char **p = (char **)base;
    char **sq;
    int n = 0;
    int idx = 0;
    int r = -1;
    int i;
    for (i = 0; i < 13; i++, p += 4) {
        if (*p != 0) {
            if (find_moby_hit(*p, 0x10000, 0) != 0) {
                if (i < 3) {
                    if (n <= 0) { n = 1; r = 1; }
                } else if (i < 5) {
                    if (n < 2) { n = 2; r = 1; }
                } else {
                    if (n < 4) { n += 4; idx = i; r = 2; }
                }
                ((unsigned char *)*p)[0xA4] = 0xFF;
            }
        }
    }
    *(unsigned short *)(data + 0x13C) += n;
    if (*(short *)(data + 0x13C) >= 8) {
        *(short *)(data + 0x13C) = *(short *)(data + 0x13C) % 8;
        if (idx != 0) {
            int o1 = idx * 16;
            char **q = (char **)(base + o1);
            char *t = *q;
            char *d2;
            d2 = *(char **)(t + 0x78);
            FUN_L00_0025f090(m, t + 0x10, -1, 2.0f, 13.0f);
            subtract_vector_xyz(d2, *q + 0x10, *(char **)(o1 + data + 0x64) + 0x10);
            normalize_vector_xyz(d2, d2, D_0015ED6C * 10.0f);
            (*q)[0x20] = 1;
            *q = 0;
            return;
        } else {
            int s = truncate_float_to_s32_c(random_float_between_c(0.0f, 23.0f));
                        char *b2 = base;
            for (i = 0; i < 8; i++) {
                int k = (i + s) % 8 + 5;
                int off = k * 16;
                base = b2 + off; sq = (char **)base;
                if (*sq != 0) {
                    char *d2 = *(char **)(*sq + 0x78);
                    FUN_L00_0025f090(m, *sq + 0x10, -1, 2.0f, 13.0f);
                    subtract_vector_xyz(d2, *sq + 0x10, *(char **)(data + off + 0x64) + 0x10);
                    normalize_vector_xyz(d2, d2, D_0015ED6C * 10.0f);
                    (*sq)[0x20] = 1;
                    *sq = 0;
                    return;
                }
            }
            m[0x20] = 0x63;
        }
    }
    if (r >= 0) func_0022ED80_2E9CB0(r, 0, (int)m);
}
/* Flying gunship: cruises one path and fires at its target, or circles a second path and fires at random. */

typedef struct {
    SwarmPathFollower follow[2]; /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    SwarmPart root;              /* 0x60 */
    SwarmPart parts[12];         /* 0x70 */
    s32 target_index;            /* 0x130: moby slot of the target */
    f32 turn_speed;              /* 0x134 */
    s16 can_skip;                /* 0x138: non-zero lets the circling jump ahead (see state 2) */
    s16 fire_timer;              /* 0x13A */
    s16 pad13C;
    s16 sound;                   /* 0x13E */
} SwarmVars;

extern s32 D_L08_00161D30 __attribute__((sda));
extern f32 D_L08_00161D24 __attribute__((sda));
extern s32 D_L08_0015F5C4;
extern s64 D_L08_0016D090 __attribute__((section(".data")));
extern u8 D_0014C050[];
extern SwarmPath *D_L08_001B0CB0[];
extern struct Moby *D_L08_0015FFD8_m __asm__("D_L08_0015FFD8");

extern s32 FUN_L00_0028d8c0(struct Moby *, s32);
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 multiply_global_factor_ed64(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern s32 FUN_001f9770(s16 *);
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern void FUN_L00_0025f090(void *, void *, s32, f32, f32);
extern struct Moby *FUN_L08_002de3e0(void *, struct Moby *, void *, f32, f32);
extern void FUN_L08_002e8788(char *);
extern void FUN_L08_002e88d8_u(struct Moby *) __asm__("FUN_L08_002e88d8");
extern void FUN_L08_002e8ba0(struct Moby *);

void FUN_L08_002e8cd0(struct Moby *moby) {
    SwarmVars *vars = (SwarmVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby *target;
    struct Moby *shot;
    struct Moby *bolt;
    SwarmTargetVars *target_vars;
    Vec4 point;
    Vec4 step;
    Vec4 muzzle;
    Vec4 velocity;
    s32 waypoint;
    s32 next;
    s32 after;
    s32 i;
    s32 slot;
    f32 distance;
    f32 heading;
    f32 next_heading;
    f32 turn;
    f32 pitch;
    f32 yaw;

    if (D_L08_00161D30 != 0) {
        return;
    }
    if (!FUN_L00_0028d8c0(moby, vars->sound)) {
        vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case 0:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->follow[0].path = D_L08_001B0CB0[vars->follow[0].path_id];
        vars->follow[1].path = D_L08_001B0CB0[vars->follow[1].path_id];
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby->flags |= 0x4000;
        qcopy(&moby->pos, &vars->follow[1].path->points[0]);
        FUN_L08_002e8788((char *)moby);
        moby->state = 2;
        moby->rot.z = FUN_001f9e90(vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   vars->follow[1].path->points[1].f[1] - moby->pos.y);
        break;
    case 1:
        waypoint = (vars->follow[0].point + vars->follow[0].path->count + vars->follow[0].direction) % vars->follow[0].path->count;
        target = &D_L08_0015FFD8_m[vars->target_index];
        FUN_L00_00258278(moby,
                         FUN_001f9e90(vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                                      vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
                         &vars->turn_speed, 0.004f, 0.3f, 0.02f);
        FUN_L08_002e88d8_u(moby);
        FUN_L08_002e8ba0((char *)moby);
        qcopy(&point, &vars->follow[0].path->points[waypoint]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            vars->follow[0].point = waypoint;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > D_L08_00161D24 * frame_time) {
            normalize_vector_xyz(&step, &step, D_L08_00161D24 * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(FUN_001f9e90(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                                 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        normalize_vector_xyz(&velocity, &step, (D_L08_00161D24 + D_L08_00161D24) * frame_time);
        shot = FUN_L08_002de3e0(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case 2:
        follower = &vars->follow[1];
        if (D_L08_0015F5C4 == 2 && vars->can_skip != 0 && D_L08_0016D090 == 0x200000000) {
            qcopy(&moby->pos, &follower->path->points[15]);
            follower->point = 15;
            moby->rot.z = FUN_001f9e90(follower->path->points[11].f[0] - moby->pos.x,
                                       follower->path->points[11].f[1] - moby->pos.y);
        }
        next = (follower->point + 1) % follower->path->count;
        after = (follower->point + 2) % follower->path->count;
        heading = FUN_001f9e90(follower->path->points[next].f[0] - follower->path->points[follower->point].f[0],
                               follower->path->points[next].f[1] - follower->path->points[follower->point].f[1]);
        next_heading = FUN_001f9e90(follower->path->points[after].f[0] - follower->path->points[next].f[0],
                                    follower->path->points[after].f[1] - follower->path->points[next].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = FUN_001f9b48(&moby->pos, &follower->path->points[next]);
        distance /= FUN_001f9b48(&follower->path->points[follower->point],
                                 &follower->path->points[next]);
        moby->rot.z = fast_add_rotations(turn * distance, next_heading);
        qcopy(&point, &follower->path->points[next]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > D_L08_00161D24 * frame_time) {
            normalize_vector_xyz(&step, &step, D_L08_00161D24 * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        FUN_L08_002e8ba0((char *)moby);
        if (!FUN_001f9770(&vars->fire_timer)) {
            break;
        }
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        normalize_vector_xyz(&velocity, &step, (D_L08_00161D24 + D_L08_00161D24) * frame_time);
        bolt = FUN_L08_002de3e0(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = vars->parts[3].moby->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case 3: {
        SwarmPath *path;

        moby->state = 1;
        path = vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z = FUN_001f9e90(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        FUN_L08_002e8ba0((char *)moby);
        break;
    }
    case 0x63:
        for (i = 0; i < 12; i++) {
            struct Moby *part = vars->parts[i].moby;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                FUN_L00_0025f090(moby, &part->pos, -1, 3.0f, 13.0f);
                subtract_vector_xyz(drift, &part->pos, &moby->pos);
                normalize_vector_xyz(drift, drift, frame_time * 5.0f);
                part->state = 1;
                vars->parts[i].moby = NULL;
            }
        }
        FUN_L00_0025f090(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
/* Builds the moby's part hierarchy: body, head, arms and eight trailing segments. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002EA930), where it is exact; names translated to the US level program. */

void *FUN_L08_002e1c98_c2(char *parent, int oClass, int joint,
                          int pjoint) __asm__("FUN_L08_002e1c98");

void FUN_L08_002e9558(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *e = data + 0xFC;
    int i = 0;
    char *c;
    char *c2;
    char *c6;
    char *c8;
    *(char **)(data + 0x60) = moby;
    *(int *)(data + 0x64) = 0;
    *(int *)(data + 0x68) = 0;
    *(int *)(data + 0x6C) = 0;
    *(char **)(data + 0x90) = FUN_L08_002e1c98_c2(moby, 0x1C0, 0, 0);
    *(char **)(data + 0x94) = moby;
    *(int *)(data + 0x98) = 0;
    *(int *)(data + 0x9C) = 0;
    c = FUN_L08_002e1c98_c2(moby, 0x1BD, 0, 2);
    *(char **)(data + 0x70) = c;
    *(int *)(data + 0x7C) = 2;
    *(char **)(data + 0x74) = moby;
    *(int *)(data + 0x78) = 0;
    *(float *)(c + 0x2C) = *(float *)(*(char **)(c + 0x24) + 0x24) * 0.75f;
    c2 = FUN_L08_002e1c98_c2(*(char **)(data + 0x90), 0x1BA, 1, 1);
    *(char **)(data + 0x80) = c2;
    *(char **)(data + 0x84) = *(char **)(data + 0x90);
    *(int *)(data + 0x88) = 1;
    *(int *)(data + 0x8C) = 1;
    *(char **)(data + 0xA0) = FUN_L08_002e1c98_c2(c2, 0x1B9, 0, 0);
    *(char **)(data + 0xA4) = *(char **)(data + 0x80);
    *(int *)(data + 0xA8) = 0;
    *(int *)(data + 0xAC) = 0;
    *(char **)(data + 0xB0) = FUN_L08_002e1c98_c2(moby, 0x1D0, 0, 1);
    *(char **)(data + 0xB4) = moby;
    *(int *)(data + 0xB8) = 0;
    *(int *)(data + 0xBC) = 1;
    c6 = FUN_L08_002e1c98_c2(moby, 0x1D0, 0, 1);
    *(char **)(data + 0xC0) = c6;
    *(char **)(data + 0xC4) = moby;
    *(int *)(data + 0xC8) = 0;
    *(int *)(data + 0xCC) = 1;
    *(unsigned short *)(c6 + 0x34) |= 0x8000;
    *(char **)(data + 0xD0) = FUN_L08_002e1c98_c2(*(char **)(data + 0x80), 0x1D1, 0, 0);
    *(char **)(data + 0xD4) = *(char **)(data + 0x80);
    *(int *)(data + 0xD8) = 0;
    *(int *)(data + 0xDC) = 0;
    c8 = FUN_L08_002e1c98_c2(*(char **)(data + 0x80), 0x1D1, 0, 0);
    *(char **)(data + 0xE0) = c8;
    *(char **)(data + 0xE4) = *(char **)(data + 0x80);
    *(int *)(data + 0xE8) = 0;
    *(int *)(data + 0xEC) = 0;
    *(unsigned short *)(c8 + 0x34) |= 0x8000;
    do {
        int j = i + 2;
        i++;
        *(char **)(e - 0xC) = FUN_L08_002e1c98_c2(*(char **)(data + 0x90), 0x1BB, 0, j);
        *(char **)(e - 8) = *(char **)(data + 0x90);
        *(int *)(e - 4) = 0;
        *(int *)e = j;
        e += 0x10;
    } while (i < 8);
}
extern void FUN_L00_0025f090(void *, void *, s32, f32, f32);
extern void FUN_001f9a28(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern float D_0015ED6C;

void FUN_L08_002e9758(struct Moby *moby) {
    int selected = 0;
    char *data = (char *)moby->pvars;
    SwarmPart *slots = (SwarmPart *)(data + 0x60);
    SwarmPart *slot = slots;
    int i = 0;
    int hits = 0;
    int sound = -1;
    int total;
    u8 mark = 0xff;

    do {
        if (slot->moby != 0) {
            if (find_moby_hit(slot->moby, 0x10000, 0) != 0) {
                if (i < 3) {
                    if (hits < 1) {
                        hits = 1;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (i < 9) {
                    if (hits < 2) {
                        hits = 2;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (hits < 4) {
                    hits += 4;
                    selected = i;
                    sound = 2;
                    goto mark_hit;
                }
mark_hit:
                slot->moby->hit_slot = mark;
            }
        }
        i++;
        slot++;
    } while (i < 17);
    total = *(u16 *)(data + 0x17c) + hits;
    *(s16 *)(data + 0x17c) = total;
    if ((s16)total >= 12) {
        *(s16 *)(data + 0x17c) = (s16)total % 12;
        if (selected != 0) {
            SwarmPart *chosen = &slots[selected];
            u8 *vector;
            char *position = data;
            vector = chosen->moby->pvars;
            FUN_L00_0025f090(moby, &chosen->moby->pos, -1, 2.0f, 13.0f);
            position += selected * 16;
            FUN_001f9a28(vector, &chosen->moby->pos,
                         &(*(struct Moby **)(position + 0x64))->pos);
            normalize_vector_xyz(vector, vector, D_0015ED6C * 10.0f);
            chosen->moby->state = 1;
            chosen->moby = 0;
            return;
        }
        {
            int start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            SwarmPart *base = slots;
            i = 0;
            do {
                int index = (i + start) % 8 + 9;
                SwarmPart *candidate;
                slots = &base[index];
                candidate = slots;
                if (candidate->moby != 0) {
                    u8 *vector = candidate->moby->pvars;
                    char *position = data;
                    FUN_L00_0025f090(moby, &candidate->moby->pos, -1, 2.0f, 13.0f);
                    position += index * 16;
                    FUN_001f9a28(vector, &candidate->moby->pos,
                                 &(*(struct Moby **)(position + 0x64))->pos);
                    normalize_vector_xyz(vector, vector, D_0015ED6C * 10.0f);
                    candidate->moby->state = 1;
                    candidate->moby = 0;
                    return;
                }
                i++;
            } while (i < 8);
            moby->state = 99;
        }
    }
    if (sound >= 0)
        allocate_voice_for_target_entry(sound, 0, moby);
}

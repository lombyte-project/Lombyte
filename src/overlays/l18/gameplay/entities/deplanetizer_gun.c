/* Level 18 moby classes: deplanetizer_gun, gadgebot. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"

#include "rnc/math/vector.h"
#include "qcopy.h"
typedef struct { char pad[0x30]; int state; int n; char pad38[0x140]; struct Moby *objects[8]; } L18SequenceState;
extern L18SequenceState D_L18_0016D260_update __asm__("D_L18_0016D260");
extern int D_L18_0015F5C4;
extern int frames_9780(int) __asm__("FUN_001f96f8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void rot_9780(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_9780(void *, void *, void *) __asm__("FUN_001f9a10");
extern void norm_9780(void *, void *, float) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00263b70(void *, void *, void *, float, float);
extern void enqueue_callback_list_1_overlay(void *, void *) __asm__("FUN_L18_002020c8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_L18_002fa250(void *, void *, void *, float, float);
extern void FUN_L18_002f9780(void);
extern void FUN_L18_002f9a98(void *);
extern void FUN_L18_002f9c20(struct Moby *);
extern void FUN_L18_002f9eb8(struct Moby *);
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int,
                               float, float, float, float, int, float, int, int, int, int)
    __asm__("FUN_L00_0025e450");
extern char *func_L00_0026DEA0_c(void *, int, void *, float, float, float, float, int)
    __asm__("FUN_L00_0026d000");
extern struct Moby *D_L18_0015FFE4;
extern struct Moby *D_L18_00162528;
extern float D_L18_00162490 __attribute__((sda));
extern float D_L18_00162494 __attribute__((sda));
extern int D_L18_0016249C __attribute__((sda));
extern int D_L18_001624A0 __attribute__((sda));
extern int D_L18_001624A4 __attribute__((sda));
extern int D_L18_001624A8 __attribute__((sda));
extern int D_L18_001624AC __attribute__((sda));
extern int D_L18_001624B0 __attribute__((sda));
extern int D_L18_001624B4 __attribute__((sda));
extern int D_L18_001624B8 __attribute__((sda));
extern int D_L18_001624C0 __attribute__((sda));
extern int D_L18_001624C4 __attribute__((sda));
extern int D_L18_001624C8 __attribute__((sda));
extern float D_L18_001624D0 __attribute__((sda));
/* First component of the vector passed at gp - 0x4720. */
extern float D_L18_001624E0 __attribute__((sda));
extern float D_L18_001624F0 __attribute__((sda));
extern float D_L18_001624F4 __attribute__((sda));
extern float D_L18_0016174C;
extern int D_L18_001617F4, D_L18_001617F8;
extern float D_L18_0015ED60;

extern Vec4 D_L18_00162530 __attribute__((section(".sdata")));
extern Vec4 D_L18_00162540 __attribute__((section(".sdata")));
typedef struct { float radius[5]; } EffectRadii;
extern EffectRadii D_L18_001F2C20;
extern char D_L18_0015F580[] __attribute__((section(".sdata")));
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void rotate_vector_about_axis(void *, void *, void *, float) __asm__("FUN_00214890");
extern void FUN_L00_002bd0a0(void *);

extern int D_L18_00162498 __attribute__((sda));
extern int D_L18_001624BC __attribute__((sda));
extern int D_L18_00162524;
extern float D_L18_00162520;
extern float D_L18_0016252C;
extern char D_L18_001679D0[];
/* Preserve the state-4 frame conversion and the state-7 raw-frame arithmetic. */
void FUN_L18_002f88e8(struct Moby *moby) {
    union { EffectRadii radii; struct { Vec4 pos, velocity; } vectors; } scratch;
    Vec4 direction, a, b;
    struct Moby *target;
    float radius, numerator, denominator, fraction, saved;
    float *data;
    unsigned char *effect;
    int i, n;

    switch (moby->state) {
    case 0:
        D_L18_00162498 = 355;
        D_L18_0016249C = 365;
        D_L18_001624BC = 60;
        D_L18_001624C0 = 200;
        D_L18_001624C4 = 190;
        D_L18_001624C8 = 365;
        moby->update_dist = 255;
        moby->state = 1;
        return;
    case 1:
        break;
    default:
        return;
    }
    if (D_L18_0015F5C4 != 2)
    return;
    if (D_L18_0016D260_update.state == 4) {
        if (D_L18_0016D260_update.n == 1) {
            {
                struct Moby *target;
                for (target = D_L18_0015FFE4; target; target = target->next) {
                    if (target->oclass == 0x59B) {
                        D_L18_00162528 = target;
                        break;
                    }
                }
            }
        }
        if (frames_9780(500) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <= frames_9780(860)) {
            int i;
            scratch.radii = D_L18_001F2C20;
            for (i = 3; i < 8; i++) {
                if (D_L18_0016D260_update.objects[i]) {
                    moby_point_position(D_L18_0016D260_update.objects[i], 0, &direction);
                    radius = scratch.radii.radius[i - 3];
                    if (i != 3 || D_L18_0016D260_update.n < frames_9780(645) ||
                        frames_9780(736) < D_L18_0016D260_update.n) {
                        if (i == 5 && frames_9780(645) <= D_L18_0016D260_update.n &&
                            D_L18_0016D260_update.n <= frames_9780(730))
                        radius = 300000.0f;
                        FUN_L00_00263b70(&direction, &direction, D_L18_0015F580, radius, radius * 0.4f);
                    }
                }
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(706)) {
            struct Moby *target = D_L18_0016D260_update.objects[7];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(710)) {
            struct Moby *target = D_L18_0016D260_update.objects[4];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(720)) {
            struct Moby *target = D_L18_0016D260_update.objects[6];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(730)) {
            struct Moby *target = D_L18_0016D260_update.objects[5];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(748)) {
            scratch.vectors.pos.q = 0;
            scratch.vectors.pos.f[0] = 634.556030273f;
            scratch.vectors.pos.f[1] = 454.162994385f;
            scratch.vectors.pos.f[2] = 102.999000549f;
            scratch.vectors.pos.f[3] = 1.0f;
            clear_u64_value(&scratch.vectors.velocity);
            scratch.vectors.velocity.f[2] = 1.0f;
            func_L00_0025F4A8_alt(moby, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 300, 200, 200,
                12.0f, 6.0f, 9.0f, 1.0f, -1, 60.0f, 0, 60, -1, 0);
        }
        if (frames_9780(D_L18_001624BC) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(D_L18_001624C0)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - frames_9780(D_L18_001624BC));
            denominator = convert_integer_to_float(frames_9780(D_L18_001624C0 - D_L18_001624BC));
            numerator = numerator / denominator;
            D_L18_00162524 = truncate_float_to_s32(numerator * 255.0f);
            enqueue_callback_list_1_overlay(FUN_L18_002f9c20, D_L18_00162528);
        }
        if (frames_9780(D_L18_001624C4) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(D_L18_001624C8)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - frames_9780(D_L18_001624C4));
            denominator = convert_integer_to_float(frames_9780(D_L18_001624C8 - D_L18_001624C4));
            numerator = numerator / denominator;
            numerator *= 200.0f;
            numerator += 0.25f;
            D_L18_0016252C = numerator;
            enqueue_callback_list_1_overlay(FUN_L18_002f9eb8, D_L18_00162528);
        }
        if (frames_9780(D_L18_00162498) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(D_L18_001624A0)) {
            enqueue_callback_list_1_overlay(FUN_L18_002f9780, D_L18_0016D260_update.objects[0]);
            if (D_L18_0016D260_update.n >= frames_9780(D_L18_0016249C)) {
                D_L18_00162520 = 10.0f;
            } else {
                numerator = convert_integer_to_float(D_L18_0016D260_update.n - frames_9780(D_L18_00162498));
                denominator = convert_integer_to_float(frames_9780(D_L18_0016249C - D_L18_00162498));
                numerator = numerator / denominator;
                D_L18_00162520 = D_L18_00162490 + (D_L18_00162494 - D_L18_00162490) * numerator;
            }
        }
    }
    if (D_L18_0016D260_update.state == 7) {
        if (D_L18_0016D260_update.n == 1) {
            D_L18_001624BC = D_L18_001624A4;
            D_L18_001624C0 = D_L18_001624A8;
            D_L18_001624C4 = D_L18_001624AC;
            D_L18_001624C8 = D_L18_001624B0;
            D_L18_00162498 = D_L18_001624B4;
            D_L18_0016249C = D_L18_001624B8;
            {
                struct Moby *target;
                for (target = D_L18_0015FFE4; target; target = target->next) {
                    if (target->oclass == 0x59B) {
                        D_L18_00162528 = target;
                        break;
                    }
                }
            }
        }
        if (frames_9780(D_L18_001624BC) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(D_L18_001624C0)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - D_L18_001624BC);
            denominator = convert_integer_to_float(D_L18_001624C0 - D_L18_001624BC);
            numerator = numerator / denominator;
            D_L18_00162524 = truncate_float_to_s32(numerator * 255.0f);
            enqueue_callback_list_1_overlay(FUN_L18_002f9c20, D_L18_00162528);
        }
        if (frames_9780(D_L18_001624C4) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(D_L18_001624C8)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - D_L18_001624C4);
            denominator = convert_integer_to_float(D_L18_001624C8 - D_L18_001624C4);
            numerator = numerator / denominator;
            numerator *= 200.0f;
            numerator += 0.25f;
            D_L18_0016252C = numerator;
            enqueue_callback_list_1_overlay(FUN_L18_002f9eb8, D_L18_00162528);
        }
        if (frames_9780(D_L18_00162498) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(D_L18_001624A0)) {
            enqueue_callback_list_1_overlay(FUN_L18_002f9780, D_L18_0016D260_update.objects[0]);
            if (D_L18_0016D260_update.n >= D_L18_0016249C) {
                D_L18_00162520 = 10.0f;
            } else {
                numerator = convert_integer_to_float(D_L18_0016D260_update.n - D_L18_00162498);
                denominator = convert_integer_to_float(D_L18_0016249C - D_L18_00162498);
                numerator = numerator / denominator;
                D_L18_00162520 = D_L18_00162490 + (D_L18_00162494 - D_L18_00162490) * numerator;
            }
        }
    }
    if (D_L18_0016D260_update.state == 1 && D_L18_0016D260_update.objects[3])
    enqueue_callback_list_1_overlay(FUN_L18_002f9a98, D_L18_0016D260_update.objects[3]);
    if (D_L18_0016D260_update.state == 5 && frames_9780(138) <= D_L18_0016D260_update.n &&
        D_L18_0016D260_update.n <= frames_9780(264)) {
        struct Moby *target = D_L18_0016D260_update.objects[2];
        if (target) {
            if (frames_9780(140) < D_L18_0016D260_update.n) {
                int i;
                moby_point_position(target, 0, &scratch.vectors.pos);
                moby_point_position(target, 2, &scratch.vectors.velocity);
                for (i = 0; i < 12; i++) {
                    fraction = convert_integer_to_float(i) / 12.0f;
                    interpolate_vector(&a, &scratch.vectors.pos, &D_L18_00162530, fraction);
                    interpolate_vector(&b, &scratch.vectors.velocity, &D_L18_00162540, fraction);
                    FUN_L18_002fa250(&a, &a, D_L18_0015F580, D_L18_001624D0, D_L18_001624D0 * 0.4f);
                    FUN_L18_002fa250(&b, &b, D_L18_0015F580, D_L18_001624D0, D_L18_001624D0 * 0.4f);
                }
                qcopy(&D_L18_00162530, &scratch.vectors.pos);
                qcopy(&D_L18_00162540, &scratch.vectors.velocity);
            } else {
                moby_point_position(target, 0, &D_L18_00162530);
                moby_point_position(target, 2, &D_L18_00162540);
            }
        }
    }
    if (D_L18_0016D260_update.state == 1 && frames_9780(446) <= D_L18_0016D260_update.n &&
        D_L18_0016D260_update.n < frames_9780(502)) {
        int color, color2;
        struct Moby *target = D_L18_0016D260_update.objects[4];
        if (target) {
            data = (float *)moby->pvars;
            data[4] = 0.0f;
            moby_point_position(target, 3, &scratch.vectors.pos);
            moby_point_position(target, 10, &scratch.vectors.velocity);
            subtract_vector_xyz(&direction, &scratch.vectors.velocity, &scratch.vectors.pos);
            norm_9780(&moby->rot_row1, &direction, 1.0f);
            qcopy(data, &scratch.vectors.velocity);
            color = 0x207F7F;
            color2 = 0x7F7F20;
            data[18] = fast_arc_tan(moby->rot_row1.x, moby->rot_row1.y);
            data[19] = fast_arc_tan(vector_length_xy(&moby->rot_row1), moby->rot_row1.z);
            saved = D_L18_0016174C;
            D_L18_0016174C = 24.0f;
            FUN_L00_002bd0a0(moby);
            D_L18_0016174C = saved;
            D_L18_001617F4 = color;
            D_L18_001617F8 = color2;
        }
    }
    if (D_L18_0016D260_update.state != 2 && D_L18_0016D260_update.state != 0) {
        if (D_L18_0016D260_update.state != 3)
        return;
        if (frames_9780(586) <= D_L18_0016D260_update.n)
        goto burst;
    }
    if (D_L18_0016D260_update.objects[2])
    enqueue_callback_list_1_overlay(FUN_L18_002f9a98, D_L18_0016D260_update.objects[2]);
burst:
    if (D_L18_0016D260_update.state == 3 && D_L18_0016D260_update.n == frames_9780(586)) {
        char *matrix = D_L18_001679D0;
        rot_9780(&scratch.vectors.pos, &D_L18_001624E0, matrix);
        add_9780(&scratch.vectors.pos, &scratch.vectors.pos, matrix - 0x210);
        qcopy(&scratch.vectors.velocity, matrix + 0x10);
        for (i = 0; i < 32; i++) {
            rotate_vector_about_axis(&direction, &scratch.vectors.velocity, D_L18_001679D0,
                convert_integer_to_float(i) * 0.19634955f);
            norm_9780(&direction, &direction, D_L18_001624F0 * D_L18_0015ED60);
            n = random_integer_below(16);
            if (random_integer_below(2))
            n = -n;
            effect = (unsigned char *)func_L00_0026DEA0_c(&scratch.vectors.pos, n, &direction,
                0.0f, 1.0f, 1.01f, D_L18_001624F4, 0x7F7F7F7F);
            if (effect) {
                unsigned char *data = effect + 0x20;
                *(short *)(effect + 10) = frames_9780(60);
                effect[9] = truncate_float_to_s32(4.0f) + 64;
                *(int *)(data + 4) = 2;
                data[10] = 127;
                data[11] = effect[10];
            }
        }
    }
}

typedef unsigned int u128_9780 __attribute__((mode(TI)));
typedef struct {
    float m[4][4];
    int col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_9780;
typedef struct { char pad[0x30]; int state; int n; } ML_9780;
extern ML_9780 D_L18_0016D260_9780 __asm__("D_L18_0016D260");
extern int frames_9780(int) __asm__("FUN_001f96f8");
extern unsigned long tex_9780(int) __asm__("FUN_001f44b8");
extern void gs_9780(int, long) __asm__("FUN_00233980");
extern void rot_9780(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_9780(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_9780(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_9780(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_9780(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_9780(void *, void *, float) __asm__("FUN_001f9a68");
extern void xform_9780(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_9780(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L18_00162550;
extern volatile float D_L18_00162554;
extern volatile float D_L18_00162558;
extern float D_L18_0016255C;
extern int D_L18_001624F8 __attribute__((sda));
extern int D_L18_001624FC __attribute__((sda));
float D_L18_001DFB90[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};
void FUN_L18_002f9780(void) {
    Pk_9780 pk;
    float mat[4][4];
    float up[4];
    int i;
    int color;
    float tilt;
    char *target;

    if (D_L18_0016D260_9780.n == frames_9780(D_L18_00162498 + 1)) {
        D_L18_00162550 = 1.0f;
        D_L18_00162554 = 0.0f;
        if (D_L18_0016D260_9780.state == 7) {
            tilt = -0.15f;
        } else {
            tilt = 0.15f;
        }
        D_L18_00162558 = tilt;
        D_L18_0016255C = 1.0f;
    }
    pk.b = tex_9780(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    gs_9780(0x4A, 0);
    gs_9780(0x47, 0x51001);
    rot_9780(mat[3], &D_L18_00162550, D_L18_001679D0);
    target = D_L18_001679D0;
    target -= 0x210;
    add_9780(mat[3], mat[3], target);
    sub_9780(mat[0], mat[3], target);
    norm_9780(mat[0], mat[0], 1.0f);
    *(u128_9780 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    cross_9780(mat[1], mat[0], up);
    norm_9780(mat[1], mat[1], 1.0f);
    cross_9780(mat[2], mat[1], mat[0]);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    color = D_L18_001624F8;
    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    for (i = 0; i < 4; i++) {
        scale_9780(pk.m[i], D_L18_001DFB90[i], D_L18_00162520);
        xform_9780(pk.m[i], pk.m[i], mat);
    }
    draw_9780(&pk, 0, 0);
    color = D_L18_001624FC;
    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    for (i = 0; i < 4; i++) {
        scale_9780(pk.m[i], D_L18_001DFB90[i], D_L18_00162520 * 0.75f);
        xform_9780(pk.m[i], pk.m[i], mat);
    }
    draw_9780(&pk, 0, 0);
    for (i = 0; i < 4; i++) {
        scale_9780(pk.m[i], D_L18_001DFB90[i], D_L18_00162520 * 0.5f);
        xform_9780(pk.m[i], pk.m[i], mat);
    }
    draw_9780(&pk, 0, 0);
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FAEF8), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L18_00162454;
extern float D_L18_00162450;
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int game_frame __asm__("D_L18_0015F5CC");
extern int D_L18_00162458;
extern int D_L18_0016245C;
extern int D_L18_00162460;
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_L00_00263618(void *, int, float, float);
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern float D_L18_00162454 ;
extern float D_L18_00162450 ;

extern short D_L18_00162500 __attribute__((sda));
void FUN_L18_002f9a98(void *moby) {
    char a[16];
    char b[32];
    char c[16];
    char d[16];
    char e[16];
    int n = scale_game_frames(D_L18_00162460);
    float f = convert_integer_to_float(game_frame % n);
    int h;
    f = f / convert_integer_to_float(n);
    f = f * 6.18318f;
    f = fast_sin(f - 3.14159f);
    h = fast_tween_color(D_L18_00162458, D_L18_0016245C, f * 0.5f + 0.5f);
    build_moby_bone_transform(moby, 1, b);
    scale_vector_xyz(e, c, *(float *)&D_L18_00162500);
    subtract_vector_xyz(a, d, e);
    FUN_L00_00263618(a, h, D_L18_00162450, D_L18_00162454);
    build_moby_bone_transform(moby, 2, b);
    scale_vector_xyz(e, c, *(float *)&D_L18_00162500);
    subtract_vector_xyz(a, d, e);
    FUN_L00_00263618(a, h, D_L18_00162450, D_L18_00162454);
}
/* Draws the moby's two glow quads facing the camera, in the inner and outer tints. */
typedef unsigned int u128_9c20 __attribute__((mode(TI)));
typedef struct {
    float m[4][4];
    int col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_9c20;
typedef struct {
    char pad[0x34];
    int n;
} ML_9c20;
extern ML_9c20 D_L18_0016D260_9c20 __asm__("D_L18_0016D260");
extern char D_L18_001677C0_9c20[] __asm__("D_L18_001677C0");
extern int frames_9c20(int) __asm__("FUN_001f96f8");
extern unsigned long tex_9c20(int) __asm__("FUN_001f44b8");
extern void gs_9c20(int, long) __asm__("FUN_00233980");
extern void rot_9c20(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_9c20(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_9c20(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_9c20(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_9c20(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_9c20(void *, void *, float) __asm__("FUN_001f9a68");
extern void xform_9c20(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_9c20(void *, void *, int) __asm__("FUN_001f7d30");

extern int D_L18_00162560;
extern int D_L18_00162564;
extern float D_L18_00162568;
extern float D_L18_0016256C;
extern int D_L18_00162504 __attribute__((sda));
extern float D_L18_00162508 __attribute__((sda));
extern float D_L18_0016250C __attribute__((sda));
extern int D_L18_00162510 __attribute__((sda));
void FUN_L18_002f9c20(struct Moby *m) {
    Pk_9c20 pk;
    float mat[4][4];
    float up[4];
    int c;
    int c2;
    int i;
    if (D_L18_0016D260_9c20.n == frames_9c20(D_L18_001624BC + 1)) {
        D_L18_00162568 = -45.0f;
        D_L18_0016256C = 1.0f;
        D_L18_00162560 = 0;
        D_L18_00162564 = 0;
    }
    pk.b = tex_9c20(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    gs_9c20(0x4A, 0);
    gs_9c20(0x47, 0x51001);
    rot_9c20(mat[3], &D_L18_00162560, &m->rot_row0);
    add_9c20(mat[3], mat[3], &m->pos);
    sub_9c20(mat[0], mat[3], D_L18_001677C0_9c20);
    norm_9c20(mat[0], mat[0], 1.0f);
    *(u128_9c20 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    cross_9c20(mat[1], mat[0], up);
    norm_9c20(mat[1], mat[1], 1.0f);
    cross_9c20(mat[2], mat[1], mat[0]);
    c = D_L18_00162504 | (D_L18_00162524 << 24);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    pk.col[3] = c;
    pk.col[2] = c;
    pk.col[1] = c;
    pk.col[0] = c;
    for (i = 0; i < 4; i++) {
        scale_9c20(pk.m[i], D_L18_001DFB90[i], D_L18_00162508);
        xform_9c20(pk.m[i], pk.m[i], mat);
    }
    draw_9c20(&pk, 0, 0);
    c2 = D_L18_00162510 | (D_L18_00162524 << 24);
    pk.col[3] = c2;
    pk.col[2] = c2;
    pk.col[1] = c2;
    pk.col[0] = c2;
    for (i = 0; i < 4; i++) {
        scale_9c20(pk.m[i], D_L18_001DFB90[i], D_L18_0016250C);
        xform_9c20(pk.m[i], pk.m[i], mat);
    }
    draw_9c20(&pk, 0, 0);
}

extern float D_L18_00162514 __attribute__((sda));
extern float D_L18_00162518 __attribute__((sda));
extern float D_L18_0016251C __attribute__((sda));
extern float D_L18_001DFBD0[4][4];
void FUN_L18_002f9eb8(struct Moby *m) {
    Pk_9c20 pk;
    float mat[4][4];
    float up[4];
    Vec4 vertices[4];
    int i = 0;
    int color;

    pk.b = tex_9c20(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    gs_9c20(0x4A, 0);
    gs_9c20(0x47, 0x51001);
    rot_9c20(mat[3], &D_L18_00162560, &m->rot_row0);
    add_9c20(mat[3], mat[3], &m->pos);
    *(u128_9c20 *)up = 0;
    up[2] = -1.0f;
    up[3] = 1.0f;
    rot_9c20(mat[0], up, &m->rot_row0);
    sub_9c20(mat[1], mat[3], D_L18_001677C0_9c20);
    cross_9c20(mat[2], mat[1], mat[0]);
    norm_9c20(mat[2], mat[2], 1.0f);
    cross_9c20(mat[1], mat[0], mat[2]);

    color = D_L18_00162504 | 0xFF000000;
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;

    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    i = 0;
    do {
        qcopy_nc(&vertices[i], D_L18_001DFBD0[i]);
        {
            vertices[i].f[2] *= D_L18_00162514;
            vertices[i].f[0] *= D_L18_0016252C;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        xform_9c20(pk.m[i], &vertices[i], mat);
    }
    draw_9c20(&pk, 0, 0);

    pk.col[3] = D_L18_00162510 | 0x7F000000;
    pk.col[2] = pk.col[3];
    pk.col[1] = pk.col[3];
    pk.col[0] = pk.col[3];
    i = 0;
    do {
        qcopy_nc(&vertices[i], D_L18_001DFBD0[i]);
        {
            vertices[i].f[2] *= D_L18_00162518;
            vertices[i].f[0] *= D_L18_0016252C;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        xform_9c20(pk.m[i], &vertices[i], mat);
    }
    draw_9c20(&pk, 0, 0);

    pk.col[3] = D_L18_00162510 | 0x7F000000;
    pk.col[2] = pk.col[3];
    pk.col[1] = pk.col[3];
    pk.col[0] = pk.col[3];
    i = 0;
    do {
        qcopy_nc(&vertices[i], D_L18_001DFBD0[i]);
        {
            vertices[i].f[2] *= D_L18_0016251C;
            vertices[i].f[0] *= D_L18_0016252C;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        xform_9c20(pk.m[i], &vertices[i], mat);
    }
    draw_9c20(&pk, 0, 0);
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FB6B0), where it is exact; names translated to the US level program. */

extern int FUN_001f96f8(int);
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
unsigned char *FUN_L00_0026d000(void *pos, int n, void *vel, int color, float r, float lo, float hi,
                                float size);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern char *func_L00_0026DEA0_c(void *, int, void *, float, float, float, float,
                                 int) __asm__("FUN_L00_0026d000");

void FUN_L18_002fa250(void *a, void *b, void *c, float f0, float f1) {
    char v0[16];
    char v1[16];
    int i;
    char *m;
    char *p;
    int x;
    int k2, k7f;
    qcopy(v0, a);
    qcopy(v1, b);
    for (i = 1; i >= 0; i--) {
        m = (char *)random_integer_below(16);
        m = func_L00_0026DEA0_c(v0, random_integer_below(2) ? -(int)m : (int)m, c, 0.0f, 1.0f, 0.9f,
                                f0, 0x7F204080);
        p = m + 0x20;
        if (m) {
            *(short *)(m + 0xA) = FUN_001f96f8(15);
            m[9] = func_001FA898_r(4.0f) + 0x40;
            *(int *)(p + 4) = 2;
            p[0xA] = 0x7F;
            p[0xB] = m[0xA];
        }
    }
    x = 16;
    k2 = 2;
    k7f = 0x7F;
    i = 2;
    do {
        int y = x;
        x = -x;
        m = func_L00_0026DEA0_c(v1, y, c, 0.0f, 1.0f, 0.97f, f1, 0x7FFFFFFF);
        p = m + 0x20;
        if (m) {
            m[9] = func_001FA898_r(4.0f) + 0x40;
            *(short *)(m + 0xA) = FUN_001f96f8(4);
            m[8] = random_integer_below(0xFF);
            *(int *)(p + 4) = k2;
            p[0xA] = k7f;
            p[0xB] = m[0xA];
        }
    } while (--i >= 0);
}
#include "sda.h"

/* State machine: waits for level flags, then turns the moby around and back. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBAA0), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern int D_L18_0015F5C4;
extern int D_L18_0016D290_lui __asm__("D_L18_0016D290") __attribute__((section(".data")));

void FUN_L18_002fa640(struct Moby *moby) {
    switch (moby->state) {
    case 0:
        moby->state = 1;
        break;
    case 1:
        if (D_L18_0015F5C4 == 2 && D_L18_0016D290_lui == 3) {
            moby->flags |= 1;
            moby->visible = 0;
            moby->rot.x = fast_add_rotations(moby->rot.x, 3.1415927f);
            moby->state = 2;
        }
        break;
    case 2:
        if (D_L18_0015F5C4 != 2) {
            unsigned short fl = moby->flags;
            moby->state = 3;
            fl &= 0xFFFE;
            moby->visible = 1;
            moby->flags = fl;
        }
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* updates a moby that drags a child moby along with it */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBB88), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_00260738(char *a, void *b, void *c, void *d);
extern char *func_0020D348_m(int) __asm__("FUN_0020c4f8");

void FUN_L18_002fa728(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char tmp[16];
    char *child;
    switch (*(unsigned char *)(moby + 0x20)) {
    case 0:
        moby[0x20] = 1;
        break;
    case 1:
        FUN_001f9a28(tmp, moby + 0x10, data + 0x60);
        FUN_L00_00260738(data + 0x20, tmp, data + 0x70, moby + 0x40);
        break;
    }
    if (*(unsigned char *)(moby + 0xBC) != 0 && *(int *)(data + 0x80) != 0 &&
        *(int *)(data + 0x84) == 0) {
        char *c = func_0020D348_m(0x764);
        *(char **)(data + 0x84) = c;
        if (c != 0) {
            *(unsigned short *)(c + 0x32) = *(unsigned short *)(moby + 0x32);
            (*(char **)(data + 0x84))[0x31] = 1;
            *(long *)(*(char **)(data + 0x84) + 0x38) = *(long *)(moby + 0x38);
            *(float *)(*(char **)(data + 0x84) + 0x2C) *=
                *(float *)(moby + 0x2C) / *(float *)(*(char **)(moby + 0x24) + 0x24);
        }
    }
    qcopy(data + 0x60, moby + 0x10);
    qcopy(data + 0x70, moby + 0x40);
    child = *(char **)(data + 0x84);
    if (child != 0) {
        qcopy(child + 0x10, moby + 0x10);
        qcopy(child + 0x40, moby + 0x40);
        FUN_L00_00250df8(child);
    }
}
extern int D_L18_001AC240[];
extern int D_L18_0015FFD8;
/* 0x002fa888, 88 bytes.
 * Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c:
 * func_L18_002FBCE8), where it is exact; names translated to the US level
 * program. */
void FUN_L18_002fa888(int idx, int value) {
    unsigned short *table = (unsigned short *)D_L18_001AC240[idx];

    if (table != 0) {
        unsigned short *p = table;
        do {
            unsigned char *moby = (unsigned char *)(((p[0] & 0x7FFF) << 8) + D_L18_0015FFD8);

            if (*(short *)(moby + 0xA6) == 0x630) {
                moby[0xBC] = value;
            }
        } while ((short)*p++ >= 0);
    }
}

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

#define NOT_SDA

#define MACRO_ADDR

/* Remove a vendor moby and any child it owns. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBD40), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x84];
    void *child;
} L18VD_a8e0;

typedef struct {
    char pad0[0x78];
    L18VD_a8e0 *data;
    char pad7C[0x2A];
    short class_id;
} L18VM_a8e0;

void FUN_L18_002fa8e0(L18VM_a8e0 *moby) {
    if (moby->class_id == 0x630) {
        void *child = moby->data->child;
        if (child != 0) {
            mark_moby_for_removal(child);
        }
        mark_moby_for_removal(moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FC188), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x30];
    unsigned int n;
    char pad2[0x178 - 0x34];
    char *arr[5];
} ML;

typedef struct {
    char pad[0x20];
    unsigned char state;
    char pad2[0xF];
    unsigned char v;
} M2;

extern ML D_L18_0016D260;
extern int D_L18_0015F5C4;
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00263b70(void *, void *, void *, float, float);

extern short D_L18_00162578 __attribute__((sda));
extern short D_L18_0016257C __attribute__((sda));
extern short D_L18_00162580 __attribute__((sda));
void FUN_L18_002fad28(M2 *m) {
    char buf[16];
    char buf2[16];
    char buf3[16];
    int idx;
    char *p;
    switch (m->state) {
    case 0:
        m->v = 0xFF;
        m->state = 1;
        break;
    case 1:
        if (D_L18_0015F5C4 == 2) {
            if (D_L18_0016D260.n < 4) {
                idx = 0;
                if (D_L18_0016D260.n == 0 || D_L18_0016D260.n == D_L18_0015F5C4 ||
                    D_L18_0016D260.n == 3)
                    idx = 3;
                if (D_L18_0016D260.n == 1)
                    idx = 4;
                p = D_L18_0016D260.arr[idx];
                moby_point_position(p, 8, buf);
                moby_point_position(p, 9, buf2);
                FUN_001f9a28(buf3, buf, buf2);
                fast_vec_normalize(buf3, buf3, *(float *)&D_L18_00162578);
                FUN_L00_00263b70(buf, buf, buf3, *(float *)&D_L18_0016257C,
                                 *(float *)&D_L18_00162580);
            }
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FD058), where it is exact; names translated to the US level program. */

typedef unsigned int QVd058 __attribute__((mode(TI), aligned(16)));

extern char D_L18_001625F0[4] __attribute__((sda));
extern char D_L18_00162608 __attribute__((sda));
extern float vector_distance_xy(float *, float *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int FUN_L00_0025c698(void *, void *);
extern int FUN_L00_0025e450();
extern int FUN_L00_002dc9f0(void *, void *);
extern int is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int random_integer_below(int);
extern int scale_game_frames(int);
extern float D_L18_00162600 __attribute__((sda));
extern void FUN_L00_002599e8(void *, float, float, float, int, int, int, int, int);
extern void FUN_L18_002fc100(struct Moby *);
void FUN_L18_002fc860(struct Moby *m);
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void scale_vector_xyz(void *, void *, float);
extern void vu_euler_rotation_basis(void *, void *);
float FUN_L18_002fc4e0(char *self, float *src);
extern float fd9a0_c(void *, void *) __asm__("FUN_L18_002fc4e0");
extern float f9d48_i(void *, void *) __asm__("FUN_001f9b80");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");

void FUN_L18_002fbb98(unsigned char *m) {
    char tmp[16];
    unsigned char *e = *(unsigned char **)(m + 0x78);
    unsigned char *s0;
    FUN_L18_002fc100((struct Moby *)m);
    switch (m[0x20]) {
    case 0: {
        unsigned short h;
        m[0x20] = 8;
        if (random_integer_below(2))
            *(unsigned short *)(m + 0x34) |= 0x8000;
        else
            *(unsigned short *)(m + 0x34) &= 0x7FFF;
        *(unsigned short *)(m + 0x34) |= 0x100;
        m[0x73] = D_L18_00162608;
        *(void **)(e + 0x140) = &D_L18_001625F0;
        *(float *)(e + 0x20) = 1.0f;
        e[0x58] = 8;
        *(short *)(e + 0x24) = 1;
        e[0x5C] = 1;
        *(float *)(e + 0x30) = 0.5f;
        *(float *)(e + 0x1F8) = random_angle_radians();
        m[0x30] = 0xFF;
        h = *(unsigned short *)(m + 0x34);
        *(short *)(m + 0x32) = 0xFF;
        h |= 1;
        m[0x31] = 0;
        h &= 0xEFFF;
        *(int *)(m + 0x94) = 0;
        *(unsigned short *)(m + 0x34) = h;
        break;
    }
    case 1: {
        float a = 10.0f;
        float r;
        s0 = m + 0x10;
        *(float *)(e + 0x1D8) = *(float *)(e + 0x1D8) - frame_time_sq * a;
        add_vector_xyz(s0, s0, e + 0x1D0);
        qcopy(tmp, s0);
        ((float *)tmp)[2] += 1.0f;
        r = probe_ground_height(tmp, 0, 0.5f);
        if (*(float *)(m + 0x18) < r) {
            *(float *)(m + 0x18) = r;
            m[0x20] = 2;
            if (m[0x53])
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
        } else if (*(float *)(m + 0x18) < a) {
            m[0x20] = 6;
        }
        break;
    }
    case 7:
        if (FUN_L00_002dc9f0(m, e + 0x70)) {
            m[0x20] = 2;
            *(short *)(e + 0x138) = 0;
        }
        break;
    case 2:
        if (*(int *)(e + 0x1C4) == 2) {
            if (!(10.0f < f9d48_i(m + 0x10, e + 0x180)))
                break;
        }
        m[0x20] = 3;
        if (m[0x53] != 1)
            blend_moby_animation(m, 1, 0, scale_game_frames(10));
        break;
    case 3: {
        float f;
        *(QVd058 *)tmp = *(QVd058 *)(e + 0x180);
        f = fd9a0_c(m, tmp);
        if (*(int *)(e + 0x1C4) == 2 && f < 5.0f) {
            m[0x20] = 2;
            if (m[0x53])
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            break;
        }
        if (f < 1.5f) {
            m[0x20] = 4;
            if (m[0x53] != 2) {
                int v = (int)D_L18_00162600;
                blend_moby_animation(m, 2, v, scale_game_frames(5));
            }
        }
        break;
    }
    case 4: {
        float r, k, b, d;
        s0 = m + 0x48;
        r = fast_arc_tan(*(float *)(e + 0x180) - *(float *)(m + 0x10),
                         *(float *)(e + 0x184) - *(float *)(m + 0x14));
        k = 6.2831855f;
        b = frame_time_sq * k;
        d = frame_time * k;
        FUN_L00_0025be00((float *)s0, (float *)(e + 0x1E8), r, b, b, d);
        if (is_value_within_interpolated_window((char *)m, 14.0f))
            FUN_L00_002599e8(m, 0.5f, 1.0f, 1.0f, 1, 1, 0, 1, 0);
        if (m[0x70] & 2) {
            if (*(int *)(e + 0x1C4) == 2) {
                m[0x20] = 2;
                if (m[0x53])
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                break;
            }
            if (!(2.5f < f9d48_i(m + 0x10, e + 0x180)))
                break;
            m[0x20] = 3;
            if (m[0x53] != 1)
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            break;
        }
        break;
    }
    case 5:
        if (!(FUN_L00_0025c698(m, e + 0x70) & 0x140))
            break;
    case 6:
        qcopy(tmp, m + 0x10);
        ((float *)tmp)[2] += 0.5f;
        func_L00_0025F4A8_alt(m, e + 0x40, tmp, 0.0f, 0.0f, 5, 2, 4, 1.0f, 0.5f, 9.0f, 0.5f, 4,
                              0.0f, 0, 1, -1, 0);
        m[0x20] = 8;
        if (m[0x53])
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
        *(int *)(m + 0x94) = 0;
        *(unsigned short *)(m + 0x34) = (*(unsigned short *)(m + 0x34) | 0x41) & 0xEFFF;
        break;
    }
    s0 = m + 0xC0;
    vu_euler_rotation_basis(s0, m + 0x40);
    if (*(float *)(e + 0x1F4) != 0.0f) {
        float g;
        approach_value((float *)(e + 0x1F4), 0.0f, frame_time * 4.0f);
        g = 1.0f;
        scale_vector_xyz(m + 0xE0, m + 0xE0, *(float *)(e + 0x1F4) + g);
        g = g - *(float *)(e + 0x1F4);
        scale_vector_xyz(s0, s0, g);
        scale_vector_xyz(m + 0xD0, m + 0xD0, g);
        enqueue_callback_list_1(FUN_L18_002fc860, m);
    }
}
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
extern int game_frame __asm__("D_L18_0015F5CC");
extern int D_L18_0015F5C4;
extern int *D_L18_001B0EB0[] __asm__("D_L18_001B0EB0");
extern void FUN_L18_002fca98(struct Moby *moby);
extern float fast_add_rotations_c100(float, float) __asm__("FUN_001fa580");
extern float fast_sin_c100(float) __asm__("FUN_001f9de0");
extern int tween_color_c100(int, int, float) __asm__("FUN_001fa6e0");
extern void blend_moby_animation_c100(void *, int, int, int) __asm__("FUN_00212f90");
extern int scale_game_frames_c100(int) __asm__("FUN_001f96f8");
extern char *find_moby_hit_c100(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction_c100(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_00213928");
extern int random_integer_below_c100(int) __asm__("FUN_00213260");
extern void knockback_c100(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void start_fall_c100(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void anim_reset_c100(void *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_c100(void *, void *) __asm__("FUN_L00_0025d538");
extern int FUN_L00_0025ff38_c100(float, void *, void *, int, int, int *, int) __asm__("FUN_L00_0025ff38");

typedef struct {
    char pad0[0x20];
    float health;              /* 0x20 */
    char pad24[0x3C];
    char anim[0x7];            /* 0x60 */
    unsigned char anim_flags;  /* 0x67 */
    char pad68[8];
    char fall[0x10];           /* 0x70 */
    float unk80;
    float unk84;
    float unk88;
    float unk8C;
    int unk90;
    int unk94;
    float unk98;
    char pad9C[0x11];
    unsigned char unkAD;       /* 0xAD */
    char padAE[0xE];
    float unkBC;
    float unkC0;
    float unkC4;
    char padC8[0xB8];
    u128 target;               /* 0x180 */
    char pad190[0x30];
    struct Moby *target_moby;  /* 0x1C0 */
    int target_mode;           /* 0x1C4 */
    char pad1C8[0x18];
    int path;                  /* 0x1E0 */
    char pad1E4[0x14];
    float spin;                /* 0x1F8 */
} SentryVars_c100;

extern int D_L18_00162604 __attribute__((sda));
extern int D_L18_0016260C __attribute__((sda));
extern int D_L18_00162610 __attribute__((sda));
extern float D_L18_001625FC __attribute__((sda));
extern float D_L18_00162614 __attribute__((sda));
extern float D_L18_00162618 __attribute__((sda));
extern float D_L18_0016261C __attribute__((sda));
extern float D_L18_00162620 __attribute__((sda));
extern float D_L18_00162624 __attribute__((sda));
extern float D_L18_00162628 __attribute__((sda));
/* Sentry update: spins and tints the moby, takes hits (knockback, then a fall), and follows its path toward the hero. */
void FUN_L18_002fc100(struct Moby *m) {
    SentryVars_c100 *d = (SentryVars_c100 *)m->pvars;
    union { u128 q; float f[4]; } v;
    int hit;
    float dmg;
    float angle;
    float speed;
    char *coll;
    int a;                      /* the reaction kind, later the path */
    int b;                      /* the reaction flags, later the path's slot in the table */

    if (m->state == 0 || m->state == 8) {
        return;
    }
    if (m->state != 7 && game_frame != D_L18_00162604) {
        D_L18_00162604 = game_frame;
        enqueue_callback_list_1(FUN_L18_002fca98, m);
    }
    speed = D_L18_00162614;
    if (m->state == 4 || m->state == 5) {
        speed = D_L18_00162618;
    }
    d->spin = fast_add_rotations_c100(d->spin, speed * DEG_TO_RAD * frame_time);
    m->glow_color = tween_color_c100(D_L18_0016260C, D_L18_00162610,
                                fast_sin_c100(d->spin) * 0.5f + 0.5f);
    if (m->pos.z < 100.0f && m->state != 7) {
        m->state = 6;
    }
    m->scale = m->pclass->scale * D_L18_001625FC;
    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        if (m->state != 7) {
            m->state = 8;
            if (m->prev_seq != 0) {
                blend_moby_animation_c100(m, 0, 0, scale_game_frames_c100(10));
            }
            m->collision = 0;
            m->flags = (m->flags | 0x41) & ~0x1000;
            return;
        }
    }
    dmg = 0.0f;
    coll = find_moby_hit_c100(m, 0x330000, 0);
    hit_reaction_c100(m, coll, &d->health, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && m->state != 6 && dmg != 0.0f) {
        d->health -= dmg;
        if (d->health <= 0.0f) {
            if (random_integer_below_c100(2) != 0) {
                m->state = 6;
            } else {
                b = 0x200;
                d->unk90 = b;
                d->unk98 = 0.5f;
                d->unk80 = D_L18_0016261C * frame_time_sq;
                d->unk84 = D_L18_00162620 * frame_time_sq;
                d->unk88 = D_L18_00162624 * frame_time;
                d->unk8C = D_L18_00162628 * frame_time;
                a = 0x29;
                d->unk94 = a;
                d->unkAD = 0;
                d->unkBC = frame_time + frame_time;
                m->flags &= ~0x1000;
                v.q = *(u128 *)(coll + 0x10);
                knockback_c100(&v, &angle, &d->unk88, &d->unk8C);
                start_fall_c100(m, d->fall, 3, 1, 0, angle);
                d->unkC0 = 7.5f;
                d->unkC4 = 15.0f;
                m->state = 5;
                m->collision = 0;
                d->anim_flags = 0x78;
                anim_reset_c100(m, d->anim);
            }
        }
    }
    m->hit_slot = 0xFF;
    anim_update_c100(m, d->anim);
    b = (int)&D_L18_001B0EB0[d->path];
    a = *(int *)b;
    FUN_L00_0025ff38_c100(512.0f, m, &d->target, 0, 0, (int *)(a + 0x10), *(int *)a);
    if (d->target_mode == 2) {
        if (hero.state.control_mode == 0xF) {
            int *list = (int *)hero.unk560;
            qcopy(&d->target, (char *)list + *list * 16);
        } else {
            qcopy(&d->target, &hero.motion.pos);
        }
    }
    if (d->target_moby == 0) {
        d->target_moby = hero.moby;
    }
}
#include "eetypes.h"

/* Turns and accelerates the moby toward a target point and returns the remaining distance. */
typedef struct {
    float v[4];
} __attribute__((aligned(16))) QVec;

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float vector_distance_xy(float *, float *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern float fast_cos(float) __asm__("func_001F9DC8");
extern void FUN_L00_00258830(int, int, float, float, float, int);
extern void FUN_L00_00261d78(float, void *, void *, void *);

float FUN_L18_002fc4e0(char *self, float *src) {
    QVec a;
    float v[4];
    float b[4];
    float z = 0.0f;
    float h = 0.5f;
    char *o = *(char **)(self + 0x78);
    float dist;
    *(u128 *)&a = *(u128 *)src;
    {
        float d = fast_add_rotations(
            fast_arc_tan(a.v[0] - *(float *)(self + 0x10), a.v[1] - *(float *)(self + 0x14)),
            *(float *)(o + 0x1EC));
        FUN_L00_0025be00((float *)(self + 0x48), (float *)(o + 0x1E8), d, frame_time_sq * 6.2831855f,
                         frame_time_sq * 6.2831855f, frame_time * 6.2831855f);
    }
    b[0] = z;
    dist = vector_distance_xy((float *)(self + 0x10), a.v);
    advance_accelerated_scalar(b, (float *)(o + 0x1F0), dist, frame_time_sq * 6.0f, frame_time_sq * 6.0f,
                               frame_time * 5.0f);
    v[0] = fast_cos(*(float *)(self + 0x48)) * *(float *)(o + 0x1F0);
    v[1] = fast_sin(*(float *)(self + 0x48)) * *(float *)(o + 0x1F0);
    v[2] = -(frame_time + frame_time);
    FUN_L00_00258830((int)self, (int)v, h, h, z, 0);
    FUN_L00_00261d78(h, *(void **)(o + 0x1E0), self + 0x10, self + 0x10);
    return dist;
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"
extern int D_L18_001AC240[];
extern struct Moby *moby_table __asm__("D_L18_0015FFD8");

/* Pvars fields set when a group moby is launched. */
typedef struct {
    u8 pad0[0x1D0];
    f32 velocity[4];   /* 0x1D0: launch direction scaled to speed; [2] replaced by the arc speed */
    u8 pad1E0[0xC];
    f32 spin;          /* 0x1EC: random 15..45 degrees, in radians */
    u8 pad1F0[4];
    f32 unk1F4;        /* 0x1F4: set to 1.0 on launch */
} LaunchedMobyVars;
extern void FUN_001f9a28(void *, void *, void *);
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_L00_0025abf0(void *, void *, float, float, int);
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");

/* Launches the first idle (state 8) class-0x772 moby of group `group` from `from` towards `to` at `speed`.
   Returns 1 when one was launched. */
int FUN_L18_002fc668(int group, void *from, void *to, float speed) {
    unsigned short *p = (unsigned short *)D_L18_001AC240[group];
    struct Moby *table;
    struct Moby *moby;

    if (p == 0)
        return 0;
    table = moby_table;
    for (;;) {
        moby = &table[*p & 0x7FFF];
        if (moby->oclass == 0x772) {
            if (moby->state == 8) {
                LaunchedMobyVars *v = (LaunchedMobyVars *)moby->pvars;
                moby->flags &= ~0x41;
                moby->collision = moby->pclass->collision;
                moby->flags |= 0x1000;
                qcopy(&moby->pos, from);
                moby->state = 1;
                v->unk1F4 = 1.0f;
                FUN_001f9a28(v->velocity, to, from);
                v->velocity[2] = 0.0f;
                fast_vec_normalize(v->velocity, v->velocity, speed);
                v->velocity[2] = FUN_L00_0025abf0(from, to, speed, -(frame_time_sq * 10.0f), 0);
                v->spin = randf_sym(15.0f, 45.0f) * 0.017453292f;
                return 1;
            }
        }
        if ((short)*p++ < 0)
            return 0;
    }
}
/* Counts the listed mobys of class 0x772 that are not in state 8. */

extern int D_L18_001AC240[];
extern int D_L18_0015FFD8;

short FUN_L18_002fc7e0(int idx) {
    unsigned short *p = (unsigned short *)D_L18_001AC240[idx];
    short n = 0;
    int base;
    if (p == 0)
        return 0;
    base = D_L18_0015FFD8;
    while (1) {
        unsigned char *moby = (unsigned char *)(base + ((p[0] & 0x7FFF) << 8));
        if (*(short *)(moby + 0xA6) == 0x772 && moby[0x20] != 8)
            n++;
        if ((short)*p++ < 0)
            return n;
    }
    return n;
}
/* Draws the moby's glow as a camera-facing quad sized by its charge, coloured between two tints. */
#include "qcopy.h"
typedef struct {
    float m[4][4];
    int col[4];
    float uv[4][2];
    unsigned long a, b, c, d;
} Pk_c860;
extern char D_L18_001677C0_g[] __asm__("D_L18_001677C0");
extern void clear_g(void *) __asm__("FUN_001f99f8");
extern void sub_g(void *, void *, void *) __asm__("FUN_001f9a28");
extern void cross_g(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void norm_g(void *, void *, float) __asm__("FUN_001f9bf8");
extern unsigned long tex_g(int) __asm__("FUN_001f44b8");
extern void draw_g(void *, void *, int) __asm__("FUN_001f7d30");

extern int D_L18_0016262C __attribute__((sda));
extern int D_L18_00162630 __attribute__((sda));
extern int D_L18_00162634 __attribute__((sda));
extern int D_L18_00162638 __attribute__((sda));
extern int D_L18_0016263C __attribute__((sda));
extern int D_L18_00162640 __attribute__((sda));
extern int D_L18_00162644 __attribute__((sda));
extern float D_L18_00162648 __attribute__((sda));
extern float D_L18_0016264C __attribute__((sda));
extern float D_L18_00162650 __attribute__((sda));
extern float D_L18_001EFCD0[4][2];
void FUN_L18_002fc860(struct Moby *m) {
    char *d = (char *)m->pvars;
    Pk_c860 pk;
    float mat[4][4];
    float w;
    float h;
    int c;
    int i;
    float *p;
    h = *(float *)(d + 0x1F4) + 1.0f + D_L18_0016264C;
    w = 1.0f - *(float *)(d + 0x1F4) + D_L18_00162650;
    qcopy(mat[3], &m->pos);
    mat[3][2] -= D_L18_00162648;
    clear_g(mat[2]);
    mat[2][2] = h;
    sub_g(mat[0], D_L18_001677C0_g, &m->pos);
    cross_g(mat[1], mat[0], mat[2]);
    cross_g(mat[0], mat[1], mat[2]);
    norm_g(mat[0], mat[0], w);
    norm_g(mat[1], mat[1], w);
    pk.b = tex_g(D_L18_00162644);
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 0;
    pk.d = (unsigned long)D_L18_0016262C | ((unsigned long)D_L18_00162630 << 2) | ((unsigned long)D_L18_00162634 << 4) |
           ((unsigned long)D_L18_00162638 << 6) | 0x8000000000ULL;
    c = fast_tween_color(D_L18_0016263C, D_L18_00162640, *(float *)(d + 0x1F4));
    for (i = 0; i < 4; i++) {
        pk.uv[i][0] = D_L18_001EFCD0[i][0];
        pk.uv[i][1] = D_L18_001EFCD0[i][1];
        p = pk.m[i];
        clear_g(p);
        if (i & 1)
            p[2] = 1.0f;
        else
            p[2] = 0.0f;
        if (i < 2)
            p[1] = 1.0f;
        else
            p[1] = -1.0f;
        p[3] = 1.0f;
        pk.col[i] = c;
    }
    draw_g(&pk, mat, 0);
}
#include "sda.h"

/* For each listed moby of matching type, spawns effects at five points. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FDF58), where it is exact; names translated to the US level program. */

extern int D_L18_0015FFD8;
extern short D_L18_00162654;
extern unsigned short *D_L18_001AC240_c[] __asm__("D_L18_001AC240");
extern void FUN_L00_00263618(void *, int, float, float);

void FUN_L18_002fca98(struct Moby *moby) {
    unsigned short *p = D_L18_001AC240_c[moby->group];
    if (p != 0) {
        while (1) {
            unsigned char *obj = D_L18_0015FFD8 + ((p[0] & 0x7FFF) << 8);
            unsigned short *next = p + 1;
            if (*(short *)(obj + 0xA6) == 0x772 && obj[0x20] != 0 && obj[0x20] < 6) {
                int i;
                int col = (*(int *)(obj + 0x90) & 0xFFFFFF) + (*(int *)&D_L18_00162654 << 24);
                float v[4];
                for (i = 2; i < 5; i++) {
                    moby_point_position(obj, i, v);
                    FUN_L00_00263618(v, col, 0.175f, 0.08f);
                }
            }
            if (*(short *)p < 0)
                break;
            p = next;
        }
    }
}

/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/gameplay/state/usage_stats.h"

typedef struct {
    unsigned char pad0[0x10];
    int flags;
    float f14;
    unsigned char pad18[4];
    float f1C;
    unsigned char pad20[4];
    float f24;
} S_2e89b0;
extern float D_0013CA40_2e89b0[] __asm__("D_0013CA40") __attribute__((section(".data")));
extern int FUN_001f96f8_2e89b0(int) __asm__("FUN_001f96f8");

void follow_camera_read_stick(char *o, float *a, float *b) __asm__("FUN_L00_002e89b0");
void follow_camera_read_stick(char *camera_object, float *a, float *b) {
    S_2e89b0 *turn_pitch_state = (S_2e89b0 *)(*(char **)(camera_object + 0x70) + 0x1A8);
    struct UsageStats *g;

    if (turn_pitch_state->flags & 1) {
        *a = 0.0f;
    } else {
        *a = D_0013CA40_2e89b0[0];
        if (opt_cam_invert == 0) {
            *a = -*a;
        }
    }
    if (*a == 0.0f) {
        D_L00_00161D8C = 0;
        D_L00_00161D90 = 0;
        if (turn_pitch_state->f1C != 0.0f) {
            *a = turn_pitch_state->f1C;
        }
    } else {
        turn_pitch_state->f14 = D_L00_0015EF44;
        g = &usage_stats;
        if (g->stat[8].count < 3) {
            D_L00_00161D8C++;
            if (D_L00_00161D90 == 0 && FUN_001f96f8_2e89b0(0x3C) < D_L00_00161D8C) {
                if (g->stat[8].count <= 0xFFFE) {
                    g->stat[8].count++;
                }
                if (FUN_001f96f8_2e89b0(D_0015EEA4) / 600 > g->stat[8].unk2) {
                    g->stat[8].unk2 = FUN_001f96f8_2e89b0(D_0015EEA4) / 600;
                }
                D_L00_00161D90 = 1;
                g->stat[8].level_mask = g->stat[8].level_mask | (1 << current_level_index) | 0x80000000;
            }
        }
    }
    if (turn_pitch_state->flags & 2) {
        *b = 0.0f;
        *b = turn_pitch_state->f24 / 0.6981317f;
    } else {
        float t = D_0013CA40_2e89b0[1];
        *b = -t;
        if (invert52 == 0) {
            *b = t;
        }
        if (*b == 0.0f) {
            *b = turn_pitch_state->f24 / 0.6981317f;
        }
    }
}
typedef float V_2e8bb8[4] __attribute__((aligned(16)));
extern unsigned char D_L00_00166E10_2e8bb8[] __asm__("D_L00_00166E10")
    __attribute__((section(".data")));
extern float FUN_001ebd78_2e8bb8(void *, float, float, float, float, float) __asm__("FUN_001ebd78");
extern float FUN_001f99c0_2e8bb8(float) __asm__("FUN_001f99c0");
extern void FUN_00214890_2e8bb8(void *, void *, void *, float) __asm__("FUN_00214890");
extern float FUN_001f9ab0_2e8bb8(void *, void *) __asm__("FUN_001f9ab0");
extern float FUN_001f9af0_2e8bb8(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9ad8_2e8bb8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void FUN_001f9bf8_2e8bb8(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_001f9e90_2e8bb8(float, float) __asm__("FUN_001f9e90");
extern float FUN_001fa580_2e8bb8(float, float) __asm__("FUN_001fa580");
extern float FUN_001fa5c8_2e8bb8(float, float) __asm__("FUN_001fa5c8");

void follow_camera_turn_and_pitch(unsigned char *o, float k, float in) __asm__("FUN_L00_002e8bb8");
extern float D_L00_00161CAC __attribute__((sda));
extern float D_L00_00161CB0 __attribute__((sda));
extern float D_L00_00161CBC __attribute__((sda));
extern float D_L00_00161CC0 __attribute__((sda));
extern float D_L00_00161CC4 __attribute__((sda));
void follow_camera_turn_and_pitch(unsigned char *camera_object, float k, float in) {
    V_2e8bb8 axis;
    unsigned char *camera_data;
    unsigned char *base;
    unsigned char *turn_pitch_state;
    unsigned char *c;
    float r;
    float lim;
    float sel;
    float a;
    float b;
    float ang;
    float z;

    camera_data = *(unsigned char **)(camera_object + 0x70);
    base = D_L00_00166E10_2e8bb8;
    turn_pitch_state = camera_data + 0x1A8;
    c = camera_data + 0x130;
    r = FUN_001ebd78_2e8bb8(camera_data + 0x1AC, *(float *)(camera_data + 0x1A8), k, D_L00_00161CAC,
                            D_L00_00161CB0, 0.0f);
    *(float *)(camera_data + 0x1A8) = r;
    *(float *)(c + 0x34) = *(float *)(turn_pitch_state + 0x14) * r;
    if (in < -0.3f) {
        in = (in + 0.3f) * 1.4285715f;
    } else if (0.3f < in) {
        in = (in - 0.3f) * 1.4285715f;
    } else {
        in = 0.0f;
    }
    z = 0.0f;
    if (in == z) {
        sel = D_L00_00161CC0;
    } else if (!(FUN_001f99c0_2e8bb8(in) < FUN_001f99c0_2e8bb8(*(float *)(turn_pitch_state + 8)))) {
        sel = D_L00_00161CBC;
    } else if (z < in && z < *(float *)(turn_pitch_state + 8)) {
        sel = D_L00_00161CC0;
    } else if (in < z && *(float *)(turn_pitch_state + 8) < z) {
        sel = D_L00_00161CC0;
    } else {
        sel = D_L00_00161CBC;
    }
    *(float *)(turn_pitch_state + 8) = FUN_001ebd78_2e8bb8(turn_pitch_state + 0xC, *(float *)(turn_pitch_state + 8), in, D_L00_00161CAC,
                                            D_L00_00161CB0, sel);
    lim = *(float *)(turn_pitch_state + 8) * 0.69813168f;
    if (k == 0.0f) {
        FUN_00214890_2e8bb8(c, c, base + 0x20, *(float *)(turn_pitch_state + 0x20));
    } else {
        FUN_00214890_2e8bb8(c, c, base + 0x20, *(float *)(c + 0x34));
    }
    a = FUN_001f9ab0_2e8bb8(c, base + 0x20);
    b = FUN_001f9af0_2e8bb8(c);
    FUN_001f9ad8_2e8bb8(axis, base + 0x20, c);
    FUN_001f9bf8_2e8bb8(axis, axis, 1.0f);
    ang = FUN_001f9e90_2e8bb8(b, a);
    if (FUN_001f99c0_2e8bb8(ang) < FUN_001f99c0_2e8bb8(lim) && sel == D_L00_00161CC0) {
        *(float *)(turn_pitch_state + 8) = ang / 0.69813168f;
    }
    if (ang < lim) {
        if (lim < FUN_001fa580_2e8bb8(ang, D_L00_00161CC4)) {
            *(float *)(c + 0x38) = FUN_001fa5c8_2e8bb8(lim, ang);
        } else {
            *(float *)(c + 0x38) = D_L00_00161CC4;
        }
    } else if (lim < ang) {
        if (*(short *)(*(unsigned char **)(camera_object + 0x70) + 0x208) < 0) {
            *(float *)(c + 0x38) = 0.0f;
        } else if (FUN_001fa580_2e8bb8(ang, -D_L00_00161CC4) < lim) {
            *(float *)(c + 0x38) = FUN_001fa5c8_2e8bb8(lim, ang);
        } else {
            *(float *)(c + 0x38) = -D_L00_00161CC4;
        }
    } else {
        *(float *)(c + 0x38) = 0.0f;
    }
    if (lim < 0.0f) {
        *(short *)(*(unsigned char **)(camera_object + 0x70) + 0x22) = 1;
    }
    FUN_00214890_2e8bb8(c, c, axis, *(float *)(c + 0x38));
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002EA3F0), where it is exact; names translated to the US level program. */

extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void subtract_vector_xyz(float *, float *, float *) __asm__("FUN_001f9a28");
extern void lerp_vector(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void follow_camera_auto_turn(int m) __asm__("FUN_L00_002e8f40");
void follow_camera_auto_turn(int m) {
    char *p = *(char **)(m + 0x70);
    char *r = p + 0x1D0;
    char *q = p + 0x130;
    if (*(short *)(p + 0x10) > 0) {
        float v[4];
        float vector_length;
        float lim;
        subtract_vector_xyz(v, (float *)p, (float *)(p + 0x90));
        lerp_vector(D_L00_0015EF40, q, q, v);
        vector_length = fast_vec_length(q);
        lim = *(float *)(q + 0x2C);
        if (lim < vector_length) {
            vector_length = lim;
            fast_vec_normalize(q, q, vector_length);
            *(float *)(r + 0x30) = *(float *)(q + 0x2C) - vector_length;
        } else {
            float minimum_length = lim - *(float *)(r + 0x30);
            if (vector_length < minimum_length) {
                vector_length = minimum_length;
                fast_vec_normalize(q, q, vector_length);
            }
            *(float *)(r + 0x30) = *(float *)(q + 0x2C) - vector_length;
        }
    }
}

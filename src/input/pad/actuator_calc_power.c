#include "types.h"

typedef struct {
    s16 f00;
    u8 f02;
    u8 f03;
    s16 f04;
    u16 f06;
    s16 f08;
    s16 f0a;
    s16 f0c;
    u8 f0e;
    u8 f0f;
} L00Entry;

extern L00Entry actuator_wave[8] __asm__("D_00165480");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");

s32 actuator_calc_power(s32 *p1) __asm__("FUN_001e8d08");

s32 actuator_calc_power(s32 *p1)
{
    enum {
        ACTUATOR_WAVE_INACTIVE = 0,
        ACTUATOR_WAVE_RECTANGULAR = 1,
        ACTUATOR_WAVE_TRIANGULAR = 2,
        ACTUATOR_WAVE_RISING_RAMP = 3,
        ACTUATOR_WAVE_FALLING_RAMP = 4,
        ACTUATOR_WAVE_COSINE = 5
    };
    s32 aa[3][4];
    s32 *q2;
    s32 *q1;
    s32 i;
    s32 count;

    count = 0;
    q1 = aa[1];
    q2 = aa[2];
    for (i = 0; i < 2; i++) {
        p1[i] = 0;
        aa[0][i] = 0;
        aa[1][i] = 0;
        aa[2][i] = 0;
    }
    for (i = 0; i < 8; i++) {
        L00Entry *e;
        s32 rem;
        s32 res;
        u16 v;

        if (actuator_wave[i].f00 == 0) {
            continue;
        }
        e = &actuator_wave[i];
        count++;
        v = actuator_wave[i].f06;
        actuator_wave[i].f06 = v - 1;
        if ((s16)v <= 0) {
            e->f00 = 0;
        }
        if (actuator_wave[i].f04 != 0) {
            actuator_wave[i].f04 = actuator_wave[i].f04 - 1;
            continue;
        }
        res = 0;
        actuator_wave[i].f08++;
        rem = e->f08 % (e->f0a + e->f0c);
        switch (e->f00) {
        case ACTUATOR_WAVE_INACTIVE:
            break;
        case ACTUATOR_WAVE_RECTANGULAR:
            if (rem < e->f0a) {
                res = e->f0e + e->f0f;
            } else {
                res = e->f0f;
            }
            break;
        case ACTUATOR_WAVE_TRIANGULAR:
            if (rem < e->f0a) {
                res = (e->f0e * rem) / e->f0a;
            } else {
                res = (e->f0e * (e->f0c + e->f0a - rem)) / e->f0c;
            }
            res = res + e->f0f;
            break;
        case ACTUATOR_WAVE_RISING_RAMP:
            if (rem < e->f0a) {
                res = (e->f0e * rem) / e->f0a + e->f0f;
            }
            break;
        case ACTUATOR_WAVE_FALLING_RAMP:
            if (rem >= e->f0a) {
                res = (e->f0e * (e->f0c + e->f0a - rem)) / e->f0c + e->f0f;
            }
            break;
        case ACTUATOR_WAVE_COSINE: {
            f32 f;
            f32 g;
            if (rem < e->f0c) {
                f = convert_integer_to_float(rem) * 3.1415927f / convert_integer_to_float(e->f0c);
            } else {
                f = 3.1415927f - convert_integer_to_float(rem - e->f0c) * 3.1415927f / convert_integer_to_float(e->f0a);
            }
            f = fast_cos(f);
            g = convert_integer_to_float(e->f0e);
            res = truncate_float_to_s32((f * g + g) * 0.5f) + e->f0f;
            if (res >= 256) {
                res = 255;
            }
            break;
        }
        }
        if (e->f03 != 0) {
            aa[0][e->f02] = aa[0][e->f02] + res;
            { s32 *t = q1 + e->f02; *t = *t + 1; }
        } else {
            p1[e->f02] = p1[e->f02] + res;
            { s32 *t = q2 + e->f02; *t = *t + 1; }
        }
    }
    for (i = 0; i < 2; i++) {
        if (q2[i] != 0) {
            p1[i] = p1[i] / q2[i];
        }
        if (q1[i] != 0) {
            aa[0][i] = aa[0][i] / q1[i];
            p1[i] = (p1[i] * aa[0][i]) / 255;
        }
    }
    return count;
}

extern __typeof__(actuator_calc_power) func_001E8D08 __attribute__((alias("FUN_001e8d08")));

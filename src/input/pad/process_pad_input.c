#include "types.h"

typedef struct Pad {
    u8 pad0[0x100];
    float axes[16]; /* 0x100 */
    float prev[16]; /* 0x140 */
    u8 pad180[0xE];
    s16 idx;   /* 0x18E */
    s32 count; /* 0x190 */
    u8 pad194[0xC];
    s32 held;     /* 0x1A0 */
    s32 pressed;  /* 0x1A4 */
    s32 released; /* 0x1A8 */
    s32 old;      /* 0x1AC */
    s32 raw;      /* 0x1B0 */
    s32 x1B4;     /* 0x1B4 */
    s32 x1B8;     /* 0x1B8 */
    s32 x1BC;     /* 0x1BC */
    s32 x1C0;     /* 0x1C0 */
    s32 x1C4;     /* 0x1C4 */
    s32 x1C8;     /* 0x1C8 */
    s32 mode;     /* 0x1CC */
    s32 none;     /* 0x1D0 */
    s32 nodir;    /* 0x1D4 */
    s32 moving;   /* 0x1D8 */
    u8 pad1DC[4];
    s32 hist_btn[30];   /* 0x1E0 */
    float hist_ang[30]; /* 0x258 */
    float hist_mag[30]; /* 0x2D0 */
    s32 x348;           /* 0x348 */
    u8 pad34C;
    u8 rep_delay; /* 0x34D */
    u8 rep_rate;  /* 0x34E */
    u8 rep_timer; /* 0x34F */
} Pad;

extern volatile u8 D_0015EDB4;
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern float FUN_001f9b20(float *);
extern float FUN_001f9e90(float, float);
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float func_001FA6C0(s32);

#define SWAPLR(x)                                                                                  \
    if ((x) & 0x8000) {                                                                            \
        (x) = ((x) & ~0x8000) | 0x2000;                                                            \
    } else if ((x) & 0x2000) {                                                                     \
        (x) = ((x) & ~0x2000) | 0x8000;                                                            \
    }

void process_pad_input(Pad *p, u8 *buf, s32 len) __asm__("FUN_00217328");

void process_pad_input(Pad *p, u8 *buf, s32 len) {
    float v[2];
    s32 i, j, k;
    s32 cur, old, ncur, nold;
    float mag, ang;

    p->raw = p->held = ((buf[0] << 8) | buf[1]) ^ 0xFFFF;
    for (i = 15; i >= 0; i--) {
        p->axes[i] = 0;
    }
    if (len >= 6) {
        for (i = 0; i < 4; i++) {
            s32 d = buf[i + 2] - 0x7F;
            if (d < 0)
                d = -d;
            if (d >= 0x30) {
                float f = func_001FA6C0(d - 0x30) / func_001FA6C0(0x4C);
                p->axes[i] = f;
                if (f > 1.0f) {
                    p->axes[i] = 1.0f;
                }
                if (buf[i + 2] < 0x7F) {
                    p->axes[i] = -p->axes[i];
                }
            }
        }
    }
    if (len >= 0x12) {
        for (i = 4; i < 16; i++) {
            p->axes[i] = func_001FA6C0(buf[i + 2]) * 0.003921569f;
        }
    }
    if (D_0015EDB4) {
        p->axes[2] = -p->axes[2];
        p->axes[0] = -p->axes[0];
        SWAPLR(p->held);
        p->raw = p->held;
    }
    {
        float *dst = p->prev, *src = p->axes;
        for (i = 15; i >= 0; i--) {
            *dst++ = *src++;
        }
    }
    if (((volatile float *)p->axes)[2] != 0.0f || ((volatile float *)p->axes)[3] != 0.0f) {
        p->moving = 1;
    } else {
        p->moving = 0;
    }
    if (p->axes[2] < 0.0f)
        p->held |= 0x8000;
    if (p->axes[2] > 0.0f)
        p->held |= 0x2000;
    if (p->axes[3] < 0.0f)
        p->held |= 0x1000;
    if (p->axes[3] > 0.0f)
        p->held |= 0x4000;
    p->nodir = (p->held & 0xF000) == 0;
    p->none = p->held == 0;
    p->pressed = ~p->old & p->held;
    p->x1B4 = ~p->old & p->raw;
    p->released = ~p->held & p->old;
    p->x1B8 = ~p->held & p->x1BC;
    p->x1C4 = p->pressed;
    p->x1C8 = p->released;
    p->x1C0 = p->held;
    p->x348 = p->held;
    if (D_0015EDB4) {
        p->prev[2] = -p->prev[2];
        p->prev[0] = -p->prev[0];
        SWAPLR(p->x1C0);
        SWAPLR(p->x1C4);
        SWAPLR(p->x1C8);
    }
    if (p->mode == 1) {
        p->held = *(volatile s32 *)&p->held & ~0x5030;
        p->pressed &= ~0x5030;
        p->released &= ~0x5030;
        p->axes[0] = 0.0f;
        p->axes[1] = 0.0f;
        p->mode = 0;
    }
    if (p->mode == 2) {
        p->held = *(volatile s32 *)&p->held & 0x900;
        p->pressed &= 0x900;
        p->released &= 0x900;
        p->raw = *(volatile s32 *)&p->raw & 0x900;
        p->nodir = 1;
        p->axes[2] = 0.0f;
        p->axes[3] = 0.0f;
        p->mode = 0;
    }
    v[0] = p->axes[2];
    v[1] = p->axes[3];
    mag = FUN_001f9b20(v);
    ang = FUN_001f9e90(v[0], v[1]);
    p->hist_mag[p->idx] = mag;
    p->hist_ang[p->idx] = ang;
    if (mag > 0.9f) {
        for (j = 1; j < scale_game_frames(4); j++) {
            float m = p->hist_mag[(p->idx - j + 30) % 30];
            if (m > 0.9f)
                break;
            if (m < 0.25f) {
                p->pressed |= 0x10000;
                break;
            }
        }
        if (!(p->pressed & 0x10000)) {
            for (k = 1; k < scale_game_frames(5); k++) {
                if (fast_difference_between_rotations(p->hist_ang[(p->idx - k + 30) % 30], ang) >
                    0.9599311f) {
                    p->pressed |= 0x10000;
                    break;
                }
            }
        }
    }
    p->hist_btn[p->idx] = p->pressed;
    p->idx = (p->idx + 1) % 30;
    if (++p->count > 30) {
        p->count = 30;
    }
    if (p->rep_rate) {
        cur = p->held;
        if (cur != 0 && cur == p->old) {
            u8 t = --p->rep_timer;
            if (0 == t || t == 0xFF) {
                p->rep_timer = p->rep_rate;
                p->pressed = cur;
            }
        } else {
            p->rep_timer = p->rep_delay;
        }
    }
}

extern __typeof__(process_pad_input) func_00217328 __attribute__((alias("FUN_00217328")));

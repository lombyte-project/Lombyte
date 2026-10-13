/* Airship parts update. Its own unit: it reads the spawn counter D_L09_001613C8 through gp, where FUN_L09_00300888 in 0021e538.c reads it through lui. */
#include "types.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/hero.h"
#include "asm.h"
#include "qcopy.h"

extern s32 FUN_00120478_2c26c8(f32) __asm__("FUN_00120478");
extern void FUN_001f9a10_2c26c8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28_2c26c8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_00210850_2c26c8(void *, int, int *, void *) __asm__("FUN_00210850");
extern void FUN_L00_0024f7c8_2c26c8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
typedef struct Data_2c26c8 {
    float vel[4];
    char pad10[0x10];
    float f20;
    float f24;
    short h28;
    short h2A;
    struct Moby *parent;
    float off[4];
    float f40;
    float f44;
    float f48;
    float f4C;
    float f50;
    float f54;
    float f58;
    float f5C;
    float f60;
    float f64;
    int f68;
    struct Moby *parts[9];
} Data_2c26c8;
typedef struct { char pad0[0x38]; unsigned char b38; unsigned char b39; } Flags_2c26c8;
typedef struct { int v[2]; } Pair_2c26c8;
/* one scratch slot of the frame: a joint matrix in state 0, two vectors (rows 0 and 1) elsewhere, and the effect id pair in state 4 */
typedef union { float m[4][4]; Pair_2c26c8 pair; } Buf_2c26c8;

extern int FUN_001e93b0_2c26c8() __asm__("FUN_001e93b0");
extern void FUN_L00_00262b00_2c26c8(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void FUN_L00_00262b80_2c26c8(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void FUN_001fa050_2c26c8(void *, void *) __asm__("FUN_001fa050");
extern void FUN_001f9d20_2c26c8(void *, void *, void *) __asm__("FUN_001f9d20");
extern void FUN_L00_00257470_2c26c8(void *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025f090_2c26c8(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_001f99f8_2c26c8(void *) __asm__("FUN_001f99f8");
extern char *FUN_00218888_2c26c8(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void FUN_001f9a68_2c26c8(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001fa580_2c26c8(float, float) __asm__("FUN_001fa580");
extern int FUN_001f9770_2c26c8(void *) __asm__("FUN_001f9770");
extern int FUN_001f96f8_2c26c8(int) __asm__("FUN_001f96f8");
extern int FUN_L00_00257b90_2c26c8(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_0022da68_2c26c8(int, int, int) __asm__("FUN_0022da68");
extern void FUN_001f9bf8_2c26c8(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_002132a8_2c26c8(float, float) __asm__("FUN_002132a8");
extern void FUN_0020c828_2c26c8(void *) __asm__("FUN_0020c828");
extern int FUN_00213260_2c26c8(int) __asm__("FUN_00213260");
extern void FUN_L00_0026a9f0_2c26c8(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern float FUN_00213308_2c26c8(void) __asm__("FUN_00213308");
extern float FUN_001f9dc8_2c26c8(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2c26c8(float) __asm__("FUN_001f9de0");
extern float FUN_001f96b0_2c26c8(float) __asm__("FUN_001f96b0");
extern int truncate_float_to_s32_r(float) __asm__("FUN_001fa6d0");
extern float D_0015ED6C_2c26c8 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_0015ED70_2c26c8 __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern char D_0013D388_2c26c8[] __asm__("D_0013D388");
extern char D_L09_002082A0_2c26c8[] __asm__("D_L09_002082A0");
extern Pair_2c26c8 D_L09_001613D0_2c26c8 __asm__("D_L09_001613D0") __attribute__((section(".data")));
extern int D_L09_00161368_2c26c8 __asm__("D_L09_00161368") __attribute__((sda));
extern int D_L09_0016136C_2c26c8 __asm__("D_L09_0016136C") __attribute__((sda));
extern int D_L09_00161370_2c26c8 __asm__("D_L09_00161370") __attribute__((sda));
extern float D_L09_00161374_2c26c8 __asm__("D_L09_00161374") __attribute__((sda));
extern float D_L09_00161378_2c26c8 __asm__("D_L09_00161378") __attribute__((sda));
extern float D_L09_0016137C_2c26c8 __asm__("D_L09_0016137C") __attribute__((sda));
extern float D_L09_00161380_2c26c8 __asm__("D_L09_00161380") __attribute__((sda));
extern int D_L09_00161384_2c26c8 __asm__("D_L09_00161384") __attribute__((sda));
extern int D_L09_00161388_2c26c8 __asm__("D_L09_00161388") __attribute__((sda));
extern int D_L09_0016138C_2c26c8 __asm__("D_L09_0016138C") __attribute__((sda));
extern float D_L09_00161390_2c26c8 __asm__("D_L09_00161390") __attribute__((sda));
extern float D_L09_00161394_2c26c8 __asm__("D_L09_00161394") __attribute__((sda));
extern float D_L09_00161398_2c26c8 __asm__("D_L09_00161398") __attribute__((sda));
extern float D_L09_0016139C_2c26c8 __asm__("D_L09_0016139C") __attribute__((sda));
extern float D_L09_001613A0_2c26c8 __asm__("D_L09_001613A0") __attribute__((sda));
extern float D_L09_001613A4_2c26c8 __asm__("D_L09_001613A4") __attribute__((sda));
extern float D_L09_001613A8_2c26c8 __asm__("D_L09_001613A8") __attribute__((sda));
extern float D_L09_001613AC_2c26c8 __asm__("D_L09_001613AC") __attribute__((sda));
extern float D_L09_001613B0_2c26c8 __asm__("D_L09_001613B0") __attribute__((sda));
extern float D_L09_001613B4_2c26c8 __asm__("D_L09_001613B4") __attribute__((sda));
extern float D_L09_001613B8_2c26c8 __asm__("D_L09_001613B8") __attribute__((sda));
extern float D_L09_001613BC_2c26c8 __asm__("D_L09_001613BC") __attribute__((sda));
extern int D_L09_001613C0_2c26c8 __asm__("D_L09_001613C0") __attribute__((sda));
extern int D_L09_001613C4_2c26c8 __asm__("D_L09_001613C4") __attribute__((sda));
extern int D_L09_001613C8_2c26c8 __asm__("D_L09_001613C8") __attribute__((sda));

/* Update for the airship parts (classes 0x49E-0x4A5): attach to the hull, ride it, then break off, tumble away and fall with smoke and sparks. */
void FUN_L09_002c26c8(struct Moby *moby) {
    float vec[4];
    Buf_2c26c8 buf;
    float rotm[4][4];
    float dist;
    int joint;
    Data_2c26c8 *d = (*(Data_2c26c8 **)&moby->pvars);

    D_L09_001613C8_2c26c8++;
    switch (moby->state) {
    case 0: {
        float s;

        FUN_L00_0024f7c8_2c26c8(moby, 0, vec);
        if (d->f68 != -1 && ((Flags_2c26c8 *)(D_0013D388_2c26c8 + d->f68))->b39 != 0) {
            FUN_0020c828_2c26c8(moby);
            return;
        }
        moby->draw_dist = 0x3FF;
        if (moby->oclass == 0x4A0) {
            if (d->h28 != 0) {
                int i;
                moby->draw_dist = 0xFF;
                d->parent = 0;
                for (i = 0x49E; i < 0x4A6; i++) {
                    if (i != 0x4A0) {
                        FUN_L09_002c2500((char *)moby, (char *)vec, i);
                    }
                }
            }
        } else {
            struct Moby *p;
            dist = 100000.0f;
            p = (struct Moby *)FUN_L09_002c2610(((f32 *)&moby->pos), 0x4A0, &dist);
            if (dist < 20.0f) {
                d->parent = p;
                (*(Data_2c26c8 **)&p->pvars)->parts[moby->oclass - 0x49E] = moby;
            } else {
                FUN_001e93b0_2c26c8(D_L09_002082A0_2c26c8, (*(short *)&moby->save_id), moby->oclass, FUN_00120478_2c26c8(dist));
            }
        }
        joint = 0;
        s = moby->scale * 0.0009765625f;
        FUN_00210850_2c26c8(moby, 1, &joint, buf.m);
        FUN_001f9a68_2c26c8(d->off, buf.m[3], s);
        moby->state = 1;
        break;
    }
    case 1:
    case 2: {
        if (moby->oclass == 0x4A0) {
            FUN_L00_00262b00_2c26c8(d->f48, d->f4C * 0.017453292f * D_0015ED6C_2c26c8, (char *)moby, &d->f40, &d->f44);
            FUN_L00_00262b80_2c26c8(moby, (char *)&d->f50, (char *)&d->f54, d->f58 * 0.017453292f, d->f5C * 0.017453292f * D_0015ED6C_2c26c8, d->f60 * 0.017453292f * D_0015ED6C_2c26c8);
        }
        if (moby->oclass != 0x4A0 && d->parent != 0 && d->parent->state != 3) {
            struct Moby *p = d->parent;
            qcopy(((f32 *)&moby->rot), ((f32 *)&p->rot));
            FUN_001fa050_2c26c8(rotm, ((f32 *)&p->rot));
            FUN_001f9d20_2c26c8(vec, d->off, rotm);
            FUN_L00_0024f7c8_2c26c8(d->parent, 0, buf.m[0]);
            FUN_001f9a28_2c26c8(((f32 *)&moby->pos), buf.m[0], vec);
        }
        if (d->parent != 0) {
            moby->flags &= ~1;
            moby->flags |= d->parent->flags & 1;
            moby->command = d->parent->command;
        }
        if (moby->state == 2) {
            if (moby->oclass == 0x4A0) {
                moby->rot.x = FUN_001fa580_2c26c8(moby->rot.x, D_L09_001613BC_2c26c8 * 0.017453292f * D_0015ED6C_2c26c8);
            }
            if (FUN_001f9770_2c26c8(&d->h2A) != 0) {
                moby->state = 3;
            }
        } else if (moby->command == 1) {
            int i;
            moby->update_dist = 0xFF;
            d->h2A = FUN_L00_00257b90_2c26c8(FUN_001f96f8_2c26c8(D_L09_001613C0_2c26c8), FUN_001f96f8_2c26c8(D_L09_001613C4_2c26c8));
            for (i = 0; i < 9; i++) {
                if (d->parts[i] != 0) {
                    d->parts[i]->update_dist = 0xFF;
                }
            }
            moby->state = 2;
        }
        break;
    }
    case 3: {
        FUN_001f9770_2c26c8(&d->h2A);
        ((Flags_2c26c8 *)(D_0013D388_2c26c8))->b38 = 1;
        if (d->f68 != -1) {
            ((Flags_2c26c8 *)(D_0013D388_2c26c8 + d->f68))->b39 = 1;
        }
        FUN_L00_00257470_2c26c8(moby, 0x200, -1);
        FUN_L00_0025f090_2c26c8(moby, ((f32 *)&moby->pos), -1, 4.0f, 13.0f);
        if (moby->oclass == 0x4A0) {
            FUN_0022da68_2c26c8(2, 0, (int)moby);
        }
        FUN_L00_0024f7c8_2c26c8(moby, 0, vec);
        FUN_001f9a28_2c26c8(d->vel, ((f32 *)&moby->pos), vec);
        FUN_001f9a68_2c26c8(d->vel, d->vel, D_L09_001613A8_2c26c8 * D_0015ED6C_2c26c8 / 20.0f);
        FUN_001f9a28_2c26c8(buf.m[0], vec, &hero.motion.pos);
        if (moby->oclass >= 0x4A1 && moby->oclass <= 0x4A2) {
            FUN_001f9bf8_2c26c8(buf.m[0], buf.m[0], D_L09_001613B8_2c26c8 * D_0015ED6C_2c26c8);
        } else {
            FUN_001f9bf8_2c26c8(buf.m[0], buf.m[0], D_L09_001613AC_2c26c8 * D_0015ED6C_2c26c8);
        }
        FUN_001f9a10_2c26c8(d->vel, d->vel, buf.m[0]);
        d->vel[2] = 0.0f;
        d->f20 = FUN_002132a8_2c26c8(-(D_L09_001613B0_2c26c8 * 0.017453292f * D_0015ED6C_2c26c8), D_L09_001613B0_2c26c8 * 0.017453292f * D_0015ED6C_2c26c8);
        d->f24 = FUN_002132a8_2c26c8(-(D_L09_001613B4_2c26c8 * 0.017453292f * D_0015ED6C_2c26c8), D_L09_001613B4_2c26c8 * 0.017453292f * D_0015ED6C_2c26c8);
        d->h2A = FUN_002132a8_2c26c8(FUN_001f96f8_2c26c8(0x5A), FUN_001f96f8_2c26c8(0x96));
        d->f64 = FUN_002132a8_2c26c8(D_L09_00161398_2c26c8 * D_0015ED70_2c26c8, D_L09_0016139C_2c26c8 * D_0015ED70_2c26c8);
        if (moby->oclass >= 0x4A1 && moby->oclass <= 0x4A2) {
            d->f20 = d->f20 / d->f20 * (1.5707964f / d->h2A);
            d->f64 = FUN_002132a8_2c26c8(D_L09_001613A0_2c26c8 * D_0015ED70_2c26c8, D_L09_001613A4_2c26c8 * D_0015ED70_2c26c8);
        }
        moby->collision = 0;
        moby->state = 4;
        break;
    }
    case 4:
        d->vel[2] -= d->f64;
        FUN_001f9a10_2c26c8(((f32 *)&moby->pos), ((f32 *)&moby->pos), d->vel);
        moby->rot.x = FUN_001fa580_2c26c8(moby->rot.x, d->f20);
        moby->rot.y = FUN_001fa580_2c26c8(moby->rot.y, d->f24);
        if (moby->pos.x < 12.0f || moby->pos.x > 1011.0f || moby->pos.y < 12.0f || moby->pos.y > 1011.0f || moby->pos.z < 12.0f || moby->pos.z > 1011.0f) {
            FUN_0020c828_2c26c8(moby);
            return;
        }
        if (FUN_00213260_2c26c8(3) == 0) {
            FUN_001f99f8_2c26c8(vec);
            buf.pair = D_L09_001613D0_2c26c8;
            FUN_L00_0026a9f0_2c26c8(((f32 *)&moby->pos), vec, 0x2F3F3F7F, buf.pair.v[FUN_00213260_2c26c8(2)], FUN_L00_00257b90_2c26c8(FUN_001f96f8_2c26c8(0xF), FUN_001f96f8_2c26c8(0x14)), FUN_L00_00257b90_2c26c8(FUN_001f96f8_2c26c8(0x19), FUN_001f96f8_2c26c8(0x1E)), 0, 0, 400000.0f, FUN_002132a8_2c26c8(8.0f, 16.0f) * D_0015ED6C_2c26c8);
        }
        if (moby->pos.z < 25.0f) {
            int i;
            for (i = 0; i < D_L09_00161368_2c26c8; i++) {
                float a = FUN_00213308_2c26c8();
                qcopy(buf.m[1], ((f32 *)&moby->pos));
                buf.m[1][0] += FUN_002132a8_2c26c8(-3.0f, 3.0f);
                buf.m[1][1] += FUN_002132a8_2c26c8(-3.0f, 3.0f);
                vec[2] = D_L09_00161374_2c26c8 * D_0015ED6C_2c26c8;
                vec[0] += FUN_001f9dc8_2c26c8(a) * D_L09_0016137C_2c26c8;
                vec[1] += FUN_001f9de0_2c26c8(a) * D_L09_0016137C_2c26c8;
                buf.m[0][2] = D_L09_00161378_2c26c8 * D_0015ED6C_2c26c8;
                buf.m[0][0] += FUN_001f9dc8_2c26c8(a) * D_L09_00161380_2c26c8;
                buf.m[0][1] += FUN_001f9de0_2c26c8(a) * D_L09_00161380_2c26c8;
                vec[3] = D_L09_00161390_2c26c8;
                buf.m[0][3] = D_L09_00161394_2c26c8;
                FUN_00218888_2c26c8(buf.m[1], vec, buf.m[0], D_L09_0016136C_2c26c8, D_L09_00161370_2c26c8,
                    truncate_float_to_s32_r(FUN_001f96b0_2c26c8(FUN_002132a8_2c26c8(D_L09_00161384_2c26c8, D_L09_00161384_2c26c8 * 2))),
                    truncate_float_to_s32_r(FUN_001f96b0_2c26c8(FUN_002132a8_2c26c8(D_L09_00161388_2c26c8, D_L09_00161388_2c26c8 * 2))),
                    truncate_float_to_s32_r(FUN_001f96b0_2c26c8(FUN_002132a8_2c26c8(D_L09_0016138C_2c26c8, D_L09_0016138C_2c26c8 * 2))),
                    -1);
            }
            FUN_001f9a68_2c26c8(d->vel, d->vel, 0.45f);
            d->f20 *= 0.9f;
            d->f24 *= 0.9f;
            FUN_0022da68_2c26c8(0, 0, (int)moby);
            moby->state = 5;
        }
        break;
    case 5:
        d->vel[2] -= D_0015ED70_2c26c8 * 10.8f / 3.0f;
        FUN_001f9a10_2c26c8(((f32 *)&moby->pos), ((f32 *)&moby->pos), d->vel);
        moby->rot.x = FUN_001fa580_2c26c8(moby->rot.x, d->f20);
        moby->rot.y = FUN_001fa580_2c26c8(moby->rot.y, d->f24);
        if (moby->pos.x < 12.0f || moby->pos.x > 1011.0f || moby->pos.y < 12.0f || moby->pos.y > 1011.0f || moby->pos.z < 12.0f || moby->pos.z > 1011.0f) {
            FUN_0020c828_2c26c8(moby);
        }
        break;
    }
}

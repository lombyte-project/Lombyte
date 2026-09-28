/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_00214550). */
#include "sda.h"
#include "qcopy.h"
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001f9a10(void *, void *, void *);
extern float D_001CAA80[] NOT_SDA;
extern int FUN_001efa68(void *, void *, int, int, int);
extern char D_00194100[];
extern void FUN_001f9c48(float *, float *, float);
extern void FUN_001f9bf8(void *, void *, float);
extern float FUN_001f99d0(float, float);
extern float FUN_001f99c8(float, float);
/* Shadow range probe: cast a ray down (8 units) from the moby position
   shifted against the light direction D_001CAA80 by its size, then a
   second one along the light from just above; +0x84/+0x88 get the lower
   and upper hit height (at most 4 apart), or 0 when nothing is below. */
void update_moby_shadow_range(char *m) __asm__("FUN_00213700");

void update_moby_shadow_range(char *m) {
    float dir[4];
    float p[4];
    float a[4];
    float b[4];
    float h2;
    float h1;
    float t;
    char *hit;

    qcopy(dir, D_001CAA80);
    FUN_001f9c48(dir, dir, 1.0f);
    FUN_001f9a68(p, m, 0.0009765625f);
    qcopy(a, p);
    a[0] -= dir[0] * *(float *)(m + 0xC) * 0.000732421875f;
    a[1] -= dir[1] * *(float *)(m + 0xC) * 0.000732421875f;
    qcopy(b, a);
    b[2] -= 8.0f;
    if (FUN_001efa68(a, b, 0x22, 0, 0) != 0) {
        qcopy(a, p);
        hit = D_00194100;
        h1 = *(float *)(hit + 0x28);
        a[2] = a[2] + *(float *)(m + 0xC) * 0.00048828125f;
        a[0] = a[0] + dir[0] * *(float *)(m + 0xC) * 0.000732421875f;
        a[1] = a[1] + dir[1] * *(float *)(m + 0xC) * 0.000732421875f;
        FUN_001f9bf8(dir, dir, (a[2] - h1) / -dir[2]);
        FUN_001f9a10(b, a, dir);
        h2 = h1;
        if (FUN_001efa68(a, b, 0x22, 0, 0) != 0) {
            h2 = *(float *)(hit + 0x28);
        }
        *(float *)(m + 0x84) = FUN_001f99d0(h1, h2) - 0.25f;
        t = FUN_001f99c8(h1, h2) + 0.25f;
        *(float *)(m + 0x88) = t;
        if (*(float *)(m + 0x84) + 4.0f < t) {
            *(float *)(m + 0x88) = *(float *)(m + 0x84) + 4.0f;
        }
    } else {
        *(float *)(m + 0x84) = 0.0f;
        *(float *)(m + 0x88) = 0.0f;
    }
}

extern __typeof__(update_moby_shadow_range) func_00213700 __attribute__((alias("FUN_00213700")));

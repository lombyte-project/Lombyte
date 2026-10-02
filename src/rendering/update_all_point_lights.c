/* Ported from rac1-decomp, the PAL decompilation (src/game/lights.c, func_00202260). */

#include "qcopy.h"

extern char D_00187098[];
extern char D_0019BDC0[];
extern char D_0019C1C0[];
extern char D_0019C3C0[];
extern float func_001FA580(float, float);
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float FUN_001f9b48(void *, void *);
extern void FUN_00201ba8(int);
extern void FUN_00201f58(int);
void update_all_point_lights(void) __asm__("FUN_00201a28");

void update_all_point_lights(void) {
    char *l = D_0019BDC0;
    float ang;
    int i;

    *(float *)(l + 0x34C) = -0.3f;
    *(float *)(l + 0x340) = 0.8f;
    *(float *)(l + 0x344) = 0.8f;
    *(float *)(l + 0x348) = 0.8f;
    ang = func_001FA580(*(float *)D_00187098, -0.8f);
    *(float *)(l + 0x350) = fast_cos(ang) * 0.866f;
    *(float *)(l + 0x354) = fast_sin(ang) * 0.866f;
    *(float *)(l + 0x358) = -0.5f;
    *(int *)(l + 0x35C) = 0;
    for (i = 0; i < 8; i++) {
        char *src = D_0019C1C0 + i * 0x20;
        char *dst = D_0019C3C0 + i * 0x30;

        if (*(int *)(dst + 0x10) != 0 && FUN_001f9b48(src + 0x10, dst + 0x20) > 8.0f) {
            qcopy(dst + 0x20, src + 0x10);
            if (*(int *)(dst + 0x10) == 1) {
                FUN_00201ba8(i);
                *(int *)(dst + 0x10) = 2;
            } else if (*(int *)(dst + 0x10) == 2) {
                FUN_00201f58(i);
            }
        }
    }
}

extern __typeof__(update_all_point_lights) func_00201A28 __attribute__((alias("FUN_00201a28")));

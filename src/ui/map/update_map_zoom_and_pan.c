#include "types.h"

extern void func_00208810(void);
extern u8 D_0013C940[];
extern u8 D_001A00F0[];

int update_map_zoom_and_pan(void) __asm__("FUN_00205440");

int update_map_zoom_and_pan(void) {
    char *pad;
    char *m;
    float *zoom;
    int *xs;
    int *ys;

    func_00208810();
    pad = D_0013C940;
    if (*(int *)(pad + 0x1A4) & 0x500) {
        return 1;
    }
    m = (char *)D_001A00F0;
    if (*(int *)(m + 0x24) == 0) {
        return 0;
    }
    if (*(int *)(m + 0x228) < 0) {
        return 0;
    }
    zoom = (float *)(m + 0xB4);
    zoom[*(int *)(m + 0x228)] *= 1.0f - *(float *)(pad + 0x104) * 0.02f;
    if (zoom[*(int *)(m + 0x228)] > 4.0f) {
        zoom[*(int *)(m + 0x228)] = 4.0f;
    }
    if (zoom[*(int *)(m + 0x228)] < 0.65f) {
        zoom[*(int *)(m + 0x228)] = 0.65f;
    }
    xs = (int *)(m + 0x104);
    ys = (int *)(m + 0x154);
    {
        float scale = 3000000.0f / zoom[*(int *)(m + 0x228)];
        xs[*(int *)(m + 0x228)] += (int)(*(float *)(pad + 0x108) * scale);
        ys[*(int *)(m + 0x228)] += (int)(*(float *)(pad + 0x10C) * scale);
    }
    {
        float z = zoom[*(int *)(m + 0x228)];
        int xlo = (int)(0.0f / z) << 15;
        int ylo = (int)(1280.0f / z) << 15;
        int xhi = 0x10000000 - xlo;
        int yhi = 0x10000000 - ylo;

        if (xs[*(int *)(m + 0x228)] < xlo) {
            xs[*(int *)(m + 0x228)] = xlo;
        }
        if (xs[*(int *)(m + 0x228)] > xhi) {
            xs[*(int *)(m + 0x228)] = xhi;
        }
        if (ys[*(int *)(m + 0x228)] < ylo) {
            ys[*(int *)(m + 0x228)] = ylo;
        }
        if (ys[*(int *)(m + 0x228)] > yhi) {
            ys[*(int *)(m + 0x228)] = yhi;
        }
    }
    return 0;
}

extern __typeof__(update_map_zoom_and_pan) func_00205440 __attribute__((alias("FUN_00205440")));

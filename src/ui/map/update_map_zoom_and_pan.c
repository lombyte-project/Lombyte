#include "types.h"

extern void func_00208810(void);
extern u8 D_0013C940[];
#include "rnc/ui/map/map_state.h"

int update_map_zoom_and_pan(void) __asm__("FUN_00205440");

int update_map_zoom_and_pan(void) {
    char *pad;
    float *zoom;
    int *xs;
    int *ys;

    func_00208810();
    pad = D_0013C940;
    if (*(int *)(pad + 0x1A4) & 0x500) {
        return 1;
    }
    if (D_001A00F0.unk24 == 0) {
        return 0;
    }
    if (D_001A00F0.loaded < 0) {
        return 0;
    }
    zoom = D_001A00F0.zoom;
    zoom[D_001A00F0.loaded] *= 1.0f - *(float *)(pad + 0x104) * 0.02f;
    if (zoom[D_001A00F0.loaded] > 4.0f) {
        zoom[D_001A00F0.loaded] = 4.0f;
    }
    if (zoom[D_001A00F0.loaded] < 0.65f) {
        zoom[D_001A00F0.loaded] = 0.65f;
    }
    xs = D_001A00F0.pan_x;
    ys = D_001A00F0.pan_y;
    {
        float scale = 3000000.0f / zoom[D_001A00F0.loaded];
        xs[D_001A00F0.loaded] += (int)(*(float *)(pad + 0x108) * scale);
        ys[D_001A00F0.loaded] += (int)(*(float *)(pad + 0x10C) * scale);
    }
    {
        float z = zoom[D_001A00F0.loaded];
        int xlo = (int)(0.0f / z) << 15;
        int ylo = (int)(1280.0f / z) << 15;
        int xhi = 0x10000000 - xlo;
        int yhi = 0x10000000 - ylo;

        if (xs[D_001A00F0.loaded] < xlo) {
            xs[D_001A00F0.loaded] = xlo;
        }
        if (xs[D_001A00F0.loaded] > xhi) {
            xs[D_001A00F0.loaded] = xhi;
        }
        if (ys[D_001A00F0.loaded] < ylo) {
            ys[D_001A00F0.loaded] = ylo;
        }
        if (ys[D_001A00F0.loaded] > yhi) {
            ys[D_001A00F0.loaded] = yhi;
        }
    }
    return 0;
}

extern __typeof__(update_map_zoom_and_pan) func_00205440 __attribute__((alias("FUN_00205440")));

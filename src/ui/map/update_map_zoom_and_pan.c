#include "types.h"
#include "rnc/input/pad_state.h"

extern void func_00208810(void);
#include "rnc/ui/map/map_state.h"

int update_map_zoom_and_pan(void) __asm__("FUN_00205440");

int update_map_zoom_and_pan(void) {
    float *zoom;
    int *xs;
    int *ys;

    func_00208810();
    if (controller_state.pressed & 0x500) {
        return 1;
    }
    if (level_map_selection.unk24 == 0) {
        return 0;
    }
    if (level_map_selection.loaded < 0) {
        return 0;
    }
    zoom = level_map_selection.zoom;
    zoom[level_map_selection.loaded] *= 1.0f - controller_state.analog[1] * 0.02f;
    if (zoom[level_map_selection.loaded] > 4.0f) {
        zoom[level_map_selection.loaded] = 4.0f;
    }
    if (zoom[level_map_selection.loaded] < 0.65f) {
        zoom[level_map_selection.loaded] = 0.65f;
    }
    xs = level_map_selection.pan_x;
    ys = level_map_selection.pan_y;
    {
        float scale = 3000000.0f / zoom[level_map_selection.loaded];
        xs[level_map_selection.loaded] += (int)(controller_state.analog[2] * scale);
        ys[level_map_selection.loaded] += (int)(controller_state.analog[3] * scale);
    }
    {
        float z = zoom[level_map_selection.loaded];
        int xlo = (int)(0.0f / z) << 15;
        int ylo = (int)(1280.0f / z) << 15;
        int xhi = 0x10000000 - xlo;
        int yhi = 0x10000000 - ylo;

        if (xs[level_map_selection.loaded] < xlo) {
            xs[level_map_selection.loaded] = xlo;
        }
        if (xs[level_map_selection.loaded] > xhi) {
            xs[level_map_selection.loaded] = xhi;
        }
        if (ys[level_map_selection.loaded] < ylo) {
            ys[level_map_selection.loaded] = ylo;
        }
        if (ys[level_map_selection.loaded] > yhi) {
            ys[level_map_selection.loaded] = yhi;
        }
    }
    return 0;
}

extern __typeof__(update_map_zoom_and_pan) func_00205440 __attribute__((alias("FUN_00205440")));

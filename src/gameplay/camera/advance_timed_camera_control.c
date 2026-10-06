#include "types.h"

extern float D_00186F40[];
extern float D_0015F43C;

void advance_timed_camera_control(void) __asm__("FUN_001eda60");

void advance_timed_camera_control(void) {
    float decrement = D_00186F40[150];
    float remaining;

    if (decrement != 0.0f) {
        remaining = D_0015F43C - decrement;
        D_0015F43C = remaining;
        if (remaining <= 0.0f) {
            D_00186F40[150] = 0.0f;
            *(volatile float *)&D_0015F43C = 0.0f;
        }
    }
}

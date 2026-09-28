#include "types.h"

extern float D_00186F40[];
extern float D_0015F43C;

void FUN_001eda60(void) {
    float cur = D_00186F40[150];
    float next;

    if (cur != 0.0f) {
        next = D_0015F43C - cur;
        D_0015F43C = next;
        if (next <= 0.0f) {
            D_00186F40[150] = 0.0f;
            *(volatile float *)&D_0015F43C = 0.0f;
        }
    }
}

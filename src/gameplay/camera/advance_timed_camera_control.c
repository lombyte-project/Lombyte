#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"

extern float D_00186F40[];

void advance_timed_camera_control(void) __asm__("FUN_001eda60");

void advance_timed_camera_control(void) {
    float decrement = D_00186F40[150];
    float remaining;

    if (decrement != 0.0f) {
        remaining = sequence - decrement;
        sequence = remaining;
        if (remaining <= 0.0f) {
            D_00186F40[150] = 0.0f;
            *(volatile float *)&sequence = 0.0f;
        }
    }
}

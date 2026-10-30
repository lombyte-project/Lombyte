#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

#include "rnc/ui/menus/menu_globals.h"

void advance_timed_camera_control(void) __asm__("FUN_001eda60");

void advance_timed_camera_control(void) {
    float decrement = preview_camera.timed_control_step;
    float remaining;

    if (decrement != 0.0f) {
        remaining = sequence - decrement;
        sequence = remaining;
        if (remaining <= 0.0f) {
            preview_camera.timed_control_step = 0.0f;
            *(volatile float *)&sequence = 0.0f;
        }
    }
}

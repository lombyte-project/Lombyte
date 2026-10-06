#include "rnc/gameplay/camera/refresh_camera_control_flags.h"
#include "types.h"
extern struct CameraControlConditions D_0013F350;
extern s32 D_0015EF98;
extern s32 D_0015EF9C;
extern s32 D_0015EFA0;
extern struct CameraPosition D_00187080;
extern struct CameraTrackingControl D_001870D0;
/* retail small-data globals, declared to GAS before the body */

void refresh_camera_control_flags(void) __asm__("FUN_001ed940");

void refresh_camera_control_flags(void) {
    struct CameraTrackingControl *tracking;

    tracking = &D_001870D0;
    D_0015EF9C = 0x14;
    if ((u32)(D_0013F350.control_mode - 0x11) < 2U || D_0013F350.secondary_mode == 0x73) {
        D_0015EF9C = 0x34;
    }
    if (D_0013F350.control_mode != 0x11 && D_0013F350.height_threshold < D_00187080.z) {
        D_0015EF9C = 0x14;
    }
    D_0015EFA0 = D_0015EF9C;
    D_0015EF9C |= 0x80;
    D_0015EF98 = 0xB4;
    if (D_0013F350.selector_1 != 0) {
        tracking->control_selector = 0x100;
        D_0015EF98 = 0x1B4;
    } else if (D_0013F350.selector_11 != 0) {
        tracking->control_selector = 0xB00;
        D_0015EF98 = 0xBB4;
    } else if (D_0013F350.selector_3 != 0) {
        tracking->control_selector = 0x300;
        D_0015EF98 = 0x3B4;
    } else if (D_0013F350.selector_13 != 0) {
        tracking->control_selector = 0xD00;
        D_0015EF98 = 0xDB4;
    } else if (D_0013F350.base_condition != 0) {
        tracking->control_selector = 0;
        D_0015EF98 = 0xB4;
    } else {
        D_0015EF98 = tracking->control_selector | 0xB4;
    }
}

extern __typeof__(refresh_camera_control_flags) func_001ED940
    __attribute__((alias("FUN_001ed940")));

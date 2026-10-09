#include "types.h"
#include "sda.h"

struct CameraPosition {
    u8 pad_0[0x8];
    f32 z;
};

struct CameraTrackingControl {
    u8 pad_0[0xC0];
    s32 control_selector;
};
#include "rnc/gameplay/hero.h"
extern s32 D_0015EF98 MACRO_ADDR;
extern s32 D_0015EF9C MACRO_ADDR;
extern s32 D_0015EFA0 MACRO_ADDR;
extern struct CameraPosition D_00187080;
extern struct CameraTrackingControl D_001870D0;
/* retail small-data globals, declared to GAS before the body */

void refresh_camera_control_flags(void) __asm__("FUN_001ed940");

void refresh_camera_control_flags(void) {
    struct CameraTrackingControl *tracking;

    tracking = &D_001870D0;
    D_0015EF9C = 0x14;
    if ((u32)(hero.state.control_mode - 0x11) < 2U || hero.state.current == 0x73) {
        D_0015EF9C = 0x34;
    }
    if (hero.state.control_mode != 0x11 && hero.height_threshold < D_00187080.z) {
        D_0015EF9C = 0x14;
    }
    D_0015EFA0 = D_0015EF9C;
    D_0015EF9C |= 0x80;
    D_0015EF98 = 0xB4;
    if (hero.selector_1 != 0) {
        tracking->control_selector = 0x100;
        D_0015EF98 = 0x1B4;
    } else if (hero.selector_11 != 0) {
        tracking->control_selector = 0xB00;
        D_0015EF98 = 0xBB4;
    } else if (hero.selector_3 != 0) {
        tracking->control_selector = 0x300;
        D_0015EF98 = 0x3B4;
    } else if (hero.selector_13 != 0) {
        tracking->control_selector = 0xD00;
        D_0015EF98 = 0xDB4;
    } else if (hero.base_condition != 0) {
        tracking->control_selector = 0;
        D_0015EF98 = 0xB4;
    } else {
        D_0015EF98 = tracking->control_selector | 0xB4;
    }
}

extern __typeof__(refresh_camera_control_flags) func_001ED940
    __attribute__((alias("FUN_001ed940")));

/* Defined below their only users, so retail reaches them with lui. */
s32 D_0015EF98 MACRO_ADDR = 0;
s32 D_0015EF9C MACRO_ADDR = 0;
s32 D_0015EFA0 MACRO_ADDR = 0;

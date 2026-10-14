#ifndef LOMBYTE_RNC_GAMEPLAY_CAMERA_CAMERA_GLOBALS_H
#define LOMBYTE_RNC_GAMEPLAY_CAMERA_CAMERA_GLOBALS_H

#include "types.h"

extern s32 camera_secondary_mode[] __asm__("D_001413D4");
extern u8 camera_position[] __asm__("D_00187080");
extern char cam_trans_state[] __asm__("D_001871B0");
extern char camera_rows[] __asm__("D_00187290");
extern char occl_cam_state[] __asm__("D_0018C318");

#endif /* LOMBYTE_RNC_GAMEPLAY_CAMERA_CAMERA_GLOBALS_H */

#include "types.h"
#include "sda.h"
#include "rnc/globals.h"

extern s32 D_0015ED78 MACRO_ADDR;
extern f32            frame_delta_time __asm__("D_0015ED7C") MACRO_ADDR;

void set_video_timing(s32 pal_mode_enabled) __asm__("FUN_00214970");

extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern f32 frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern f32 frame_scale_inv_macro __asm__("D_0015ED68") MACRO_ADDR;
extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern f32 frame_time_cube_macro __asm__("D_0015ED74") MACRO_ADDR;

void set_video_timing(s32 pal_mode_enabled) {
    if (pal_mode_enabled == 0) {
        pal_mode = 0;
        frame_scale_macro = 1.0f;
        frame_scale_sq_macro = 1.0f;
        frame_scale_inv_macro = 1.0f;
        frame_time_macro = 0.016666668f;
        frame_time_sq_macro = 0.00027777778f;
        frame_time_cube_macro = 0.0000046296295f;
        D_0015ED78 = 5;
        frame_delta_time = 0.016666668f;
        return;
    }
    pal_mode = 1;
    frame_scale_macro = 1.2f;
    frame_scale_sq_macro = 1.44f;
    frame_scale_inv_macro = 0.8333333f;
    frame_time_macro = 0.020000001f;
    frame_time_sq_macro = 0.00040000002f;
    frame_time_cube_macro = 0.000008f;
    D_0015ED78 = 6;
    frame_delta_time = 0.02f;
}

/* Defined below their only writer, so retail reaches them with lui. */
f32            frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR = 1.0f;
f32            frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR = 1.0f;
f32            frame_scale_inv_macro __asm__("D_0015ED68") MACRO_ADDR = 1.0f;
f32            frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR = 0.016666668f;
f32            frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR = 0.00027777778f;
f32            frame_time_cube_macro __asm__("D_0015ED74") MACRO_ADDR = 0.0000046296295f;
s32 D_0015ED78 MACRO_ADDR = 5;
f32            frame_delta_time __asm__("D_0015ED7C") MACRO_ADDR = 0.016666668f;

extern __typeof__(set_video_timing) func_00214970 __attribute__((alias("FUN_00214970")));

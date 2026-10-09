#include "types.h"
#include "sda.h"
#include "rnc/globals.h"

extern f32 D_0015ED60 MACRO_ADDR;
extern f32 D_0015ED64 MACRO_ADDR;
extern f32 D_0015ED68 MACRO_ADDR;
extern f32 D_0015ED6C MACRO_ADDR;
extern f32 D_0015ED70 MACRO_ADDR;
extern f32 D_0015ED74 MACRO_ADDR;
extern s32 D_0015ED78 MACRO_ADDR;
extern f32 D_0015ED7C MACRO_ADDR;

void set_video_timing(s32 arg0) __asm__("FUN_00214970");

void set_video_timing(s32 arg0) {
    if (arg0 == 0) {
        pal_mode = 0;
        D_0015ED60 = 1.0f;
        D_0015ED64 = 1.0f;
        D_0015ED68 = 1.0f;
        D_0015ED6C = 0.016666668f;
        D_0015ED70 = 0.00027777778f;
        D_0015ED74 = 0.0000046296295f;
        D_0015ED78 = 5;
        D_0015ED7C = 0.016666668f;
        return;
    }
    pal_mode = 1;
    D_0015ED60 = 1.2f;
    D_0015ED64 = 1.44f;
    D_0015ED68 = 0.8333333f;
    D_0015ED6C = 0.020000001f;
    D_0015ED70 = 0.00040000002f;
    D_0015ED74 = 0.000008f;
    D_0015ED78 = 6;
    D_0015ED7C = 0.02f;
}

/* Defined below their only writer, so retail reaches them with lui. */
f32 D_0015ED60 MACRO_ADDR = 1.0f;
f32 D_0015ED64 MACRO_ADDR = 1.0f;
f32 D_0015ED68 MACRO_ADDR = 1.0f;
f32 D_0015ED6C MACRO_ADDR = 0.016666668f;
f32 D_0015ED70 MACRO_ADDR = 0.00027777778f;
f32 D_0015ED74 MACRO_ADDR = 0.0000046296295f;
s32 D_0015ED78 MACRO_ADDR = 5;
f32 D_0015ED7C MACRO_ADDR = 0.016666668f;

extern __typeof__(set_video_timing) func_00214970 __attribute__((alias("FUN_00214970")));

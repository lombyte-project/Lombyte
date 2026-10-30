#include "types.h"
#include "sda.h"
extern s32 cam_post_upd_routine_cnt __asm__("D_0015EF8C") MACRO_ADDR;
extern void (*cam_post_upd_routines[])(void) __asm__("D_001892B0");
void execute_camera_post_update_callbacks(void) __asm__("FUN_001ebcf0");

void execute_camera_post_update_callbacks(void) {
    s32 i;

    for (i = 0; i < cam_post_upd_routine_cnt; i++) {
        cam_post_upd_routines[i]();
    }
    cam_post_upd_routine_cnt = 0;
}

extern __typeof__(execute_camera_post_update_callbacks) func_001EBCF0
    __attribute__((alias("FUN_001ebcf0")));

/* Defined below their only users, so retail reaches them with lui. */
s32 cam_post_upd_routine_cnt __asm__("D_0015EF8C") MACRO_ADDR = 0;

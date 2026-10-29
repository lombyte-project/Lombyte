#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
/* Ported from rac1-decomp (src/overlays/shared/pause_00277208.c, func_L00_00284620). */
extern void *func_00226720_a(int) __asm__("func_00225490");
extern int acquire_menu_stream_buffer(int) __asm__("FUN_00225c18");
extern void func_00225AB8(void *);
/* sets up a pause-menu helper moby in front of the camera */
int enter_streamed_moby_screen(char *h) __asm__("FUN_00225588");

int enter_streamed_moby_screen(char *screen_state) {
    char *preview_moby;
    int stream_buffer_address;
    preview_moby = func_00226720_a(0x7A5);
    *(int *)(screen_state + 0x34) = 0;
    if (preview_moby != 0) {
        char *moby_sidecar;
        char *preview_camera_state = preview_camera;
        *(char **)(screen_state + 0x44) = preview_moby;
        *(short *)(preview_moby + 0x34) = 0;
        *(float *)(preview_moby + 0x10) = *(float *)(preview_camera_state + 0x140) + 2.2f;
        *(float *)(preview_moby + 0x14) = *(float *)(preview_camera_state + 0x144) + 0.0f;
        *(float *)(preview_moby + 0x18) = *(float *)(preview_camera_state + 0x148) + -1.6f;
        moby_sidecar = *(char **)(preview_moby + 0x78);
        *(void **)(preview_moby + 0x74) = func_00225AB8;
        *(float *)(preview_moby + 0x48) = 3.1415927f;
        *(char **)moby_sidecar = screen_state;
        *(int *)(moby_sidecar + 4) = 0;
        *(int *)(moby_sidecar + 8) = 0;
    } else {
        *(int *)(screen_state + 0x34) = 3;
    }
    stream_buffer_address = acquire_menu_stream_buffer(1);
    *(int *)(screen_state + 0x3C) = stream_buffer_address;
    if (stream_buffer_address == 0) {
        *(int *)(screen_state + 0x34) = 3;
    }
    return 0;
}

extern __typeof__(enter_streamed_moby_screen) func_00225588 __attribute__((alias("FUN_00225588")));

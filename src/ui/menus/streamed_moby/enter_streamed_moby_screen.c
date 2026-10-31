#include "rnc/data_labels.h"
#include "rnc/mobys/moby.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/stream_buffers/acquire_menu_stream_buffer.h"

/* Ported from rac1-decomp (src/overlays/shared/pause_00277208.c, func_L00_00284620). */
extern struct Moby *create_menu_preview_moby(int) __asm__("func_00225490");
extern void         helper_moby_update_noop(struct Moby *) __asm__("func_00225AB8");
/* sets up a pause-menu helper moby in front of the camera */
int enter_streamed_moby_screen(char *h) __asm__("FUN_00225588");

int enter_streamed_moby_screen(char *screen_state) {
    struct Moby *preview_moby;
    int          stream_buffer_address;
    preview_moby = create_menu_preview_moby(0x7A5);
    *(int *)(screen_state + 0x34) = 0;
    if (preview_moby != 0) {
        char                 *moby_sidecar;
        struct PreviewCamera *camera = &preview_camera;
        *(struct Moby **)(screen_state + 0x44) = preview_moby;
        preview_moby->flags = 0;
        preview_moby->pos.x = camera->position.f[0] + 2.2f;
        preview_moby->pos.y = camera->position.f[1] + 0.0f;
        preview_moby->pos.z = camera->position.f[2] + -1.6f;
        moby_sidecar = (char *)preview_moby->pvars;
        preview_moby->update = helper_moby_update_noop;
        preview_moby->rot.z = 3.1415927f;
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

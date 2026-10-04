#include "types.h"
extern s32 init_view_context() __asm__("FUN_001f2c60");
extern s32 update_view_context() __asm__("FUN_001f2d98");
extern s32 append_draw_environment_packet() __asm__("FUN_001fb2d0");
void setup_view_context(void) __asm__("FUN_00239750");

void setup_view_context(void) {
    append_draw_environment_packet();
    init_view_context();
    update_view_context();
}
/* Recovered original symbol name. */
extern __typeof__(setup_view_context) setupViewContext __attribute__((alias("FUN_00239750")));

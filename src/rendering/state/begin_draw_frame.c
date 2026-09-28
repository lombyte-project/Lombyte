#include "types.h"
extern s32 init_view_context() __asm__("FUN_001f2c60");
extern s32 update_view_context() __asm__("FUN_001f2d98");
extern s32 append_draw_environment_packet() __asm__("FUN_001fb2d0");
void begin_draw_frame(void) __asm__("FUN_001f7978");

void begin_draw_frame(void) {
    append_draw_environment_packet();
    init_view_context();
    update_view_context();
}

#include "types.h"
#include "rnc/rendering/packets/put_draw_buffer_large.h"
#include "rnc/rendering/view/init_view_context.h"
#include "rnc/rendering/view/update_view_context.h"

void begin_draw_frame(void) __asm__("FUN_001f7978");

void begin_draw_frame(void) {
    put_draw_buffer_large();
    init_view_context();
    update_view_context();
}

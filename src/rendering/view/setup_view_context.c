#include "types.h"
#include "rnc/rendering/packets/put_draw_buffer_large.h"
#include "rnc/rendering/view/init_view_context.h"
#include "rnc/rendering/view/update_view_context.h"

void setup_view_context(void) __asm__("FUN_00239750");

void setup_view_context(void) {
    put_draw_buffer_large();
    init_view_context();
    update_view_context();
}

/* Recovered original symbol name. */
extern __typeof__(setup_view_context) setupViewContext __attribute__((alias("FUN_00239750")));

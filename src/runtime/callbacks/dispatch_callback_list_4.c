#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

void dispatch_callback_list_4(void) __asm__("FUN_001f4808");

void dispatch_callback_list_4(void) {
    s32 i;

    for (i = 0; i < draw_callback2_count; i++) {
        draw_callback2_funcs[i](draw_callback2_args[i]);
    }
}

extern __typeof__(dispatch_callback_list_4) func_001F4808 __attribute__((alias("FUN_001f4808")));

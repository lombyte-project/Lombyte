#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
void dispatch_callback_list_1(void) __asm__("FUN_001f4650");

void dispatch_callback_list_1(void) {
    s32 i;

    for (i = 0; i < draw_callback_count; i++) {
        draw_callback_funcs[i](draw_callback_args[i]);
    }
}

extern __typeof__(dispatch_callback_list_1) func_001F4650 __attribute__((alias("FUN_001f4650")));

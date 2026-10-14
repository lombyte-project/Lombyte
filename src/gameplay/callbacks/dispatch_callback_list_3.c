#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
extern void (*draw_callback4_funcs[])(s32) __asm__("D_0018E140");
extern s32 draw_callback4_args[] __asm__("D_0018E240");
void dispatch_callback_list_3(void) __asm__("FUN_001f4740");

void dispatch_callback_list_3(void) {
    s32 i;

    for (i = 0; i < draw_callback4_count; i++) {
        draw_callback4_funcs[i](draw_callback4_args[i]);
    }
}

extern __typeof__(dispatch_callback_list_3) func_001F4740 __attribute__((alias("FUN_001f4740")));

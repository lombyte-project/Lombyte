#include "types.h"

struct Handler;
struct Handler { s32 id; void (*fn)(struct Handler *); u8 pad8[0x88]; };
struct Table { u8 pad0[0xD90]; s32 count; struct Handler *handlers; };
extern struct Table D_0013E550;

void enqueue_voice_request(void) __asm__("FUN_0022dc50");

void enqueue_voice_request(void) {
    s32 i;

    for (i = 0; i < D_0013E550.count; i++) {
        struct Handler *h = (struct Handler *)(i * 0x90 + (s32)D_0013E550.handlers);

        if (h->fn != 0) {
            h->fn(h);
        }
    }
}

extern __typeof__(enqueue_voice_request) func_0022DC50 __attribute__((alias("FUN_0022dc50")));

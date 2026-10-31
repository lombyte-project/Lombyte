#include "types.h"

extern void scePrintf(const char *format, ...);

void print_mpeg_error(void *image) __asm__("JumpToImageSetup");

void print_mpeg_error(void *image) {
    scePrintf("[MPEG ERROR]%s\n", image);
}

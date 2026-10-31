#include "types.h"
#include "rnc/sdk/deci2/sce_printf.h"

void print_mpeg_error(void *image) __asm__("JumpToImageSetup");

void print_mpeg_error(void *image) {
    scePrintf("[MPEG ERROR]%s\n", image);
}

#include "types.h"

extern void scePrintf(const char *format, ...);

void JumpToImageSetup(void *image) {
    scePrintf("[MPEG ERROR]%s\n", image);
}

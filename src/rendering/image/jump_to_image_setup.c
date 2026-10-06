#include "types.h"

extern const char ImageSetupMessage[] __asm__("D_00153AD8");
extern void scePrintf(const char *format, ...);

void JumpToImageSetup(void *image) {
    scePrintf(ImageSetupMessage, image);
}

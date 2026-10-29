/* Reset the filesystem status word and clear its shared reset buffer. */

#include "rnc/sdk/library/sdk_state.h"

extern int FsResetBuffer __asm__("D_00157FA8") __attribute__((section(".data")));

extern void *Memset(void *, int, unsigned int) __asm__("memset");

int SceFsReset(void) __asm__("sceFsReset");

int SceFsReset(void) {
    FsResetState = 0;
    Memset(&FsResetBuffer, 0, 4);
    return 0;
}

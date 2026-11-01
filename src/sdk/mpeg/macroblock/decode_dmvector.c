#include "types.h"
#include "rnc/sdk/ipu.h"

s32 decode_dmvector(s32 command, s32 requested_mode) __asm__("InitializeMemoryCardDirectory");

s32 decode_dmvector(s32 command, s32 requested_mode) {
    (void)requested_mode;
    return _ipuVdec(command, 3);
}

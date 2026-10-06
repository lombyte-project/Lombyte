#include "types.h"

extern s32 InitializeMpegDecoder(s32 command, s32 mode) __asm__("_ipuVdec");

s32 InitializeMemoryCardDirectory(s32 command,
                                  s32 requested_mode) __asm__("InitializeMemoryCardDirectory");

s32 InitializeMemoryCardDirectory(s32 command, s32 requested_mode) {
    (void)requested_mode;
    return InitializeMpegDecoder(command, 3);
}

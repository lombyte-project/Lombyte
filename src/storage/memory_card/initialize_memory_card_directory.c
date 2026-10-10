#include "types.h"

extern s32 initialize_mpeg_decoder(s32 command, s32 mode) __asm__("_ipuVdec");

s32 InitializeMemoryCardDirectory(s32 command,
                                  s32 requested_mode) __asm__("InitializeMemoryCardDirectory");

s32 InitializeMemoryCardDirectory(s32 command, s32 requested_mode) {
    (void)requested_mode;
    return initialize_mpeg_decoder(command, 3);
}

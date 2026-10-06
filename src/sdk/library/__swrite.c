#include "types.h"
struct StdioFile {
    u8 pad_0[0xC];
    u16 unkC;
    s16 unkE;
    u8 pad_12[0x44];
    s32 unk54;
};

extern s32 reentrant_syscall_with_three_arguments() __asm__("func_00114518");
extern s32 reentrant_write() __asm__("func_001185D0");
s64 __swrite(struct StdioFile *file, s32 buf, s32 len) {
    s64 written;

    if (file->unkC & 0x100) {
        reentrant_syscall_with_three_arguments(file->unk54, file->unkE, 0, 2);
    }
    file->unkC = (u16)(file->unkC & 0xEFFF);
    written = reentrant_write(file->unk54, file->unkE, buf, len);
    return (s64)(s32)(u32)written;
}

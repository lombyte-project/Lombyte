/* FUN_0023d080 (mpeg_error) exact recovery: one DebugPrint of the record field and return 1; the patched profile (lq/sq saves) reproduces all 10 instructions. Caller-facing func_0023D080 alias required for the oracle link. */

#include "types.h"
struct MpegErrorData {
    u8 pad_0[0x4];
    s32 unk4;
};

extern u8 D_00161220[];
extern s32 DebugPrint();
s32 mpeg_error(s32 arg0, struct MpegErrorData *cb_data) __asm__("FUN_0023d080");

s32 mpeg_error(s32 arg0, struct MpegErrorData *cb_data) {
    DebugPrint(D_00161220, cb_data->unk4);
    return 1;
}

extern s32 func_0023D080(s32 arg0, struct MpegErrorData *cb_data) __attribute__((alias("FUN_0023d080")));
extern __typeof__(mpeg_error) D_0023D080 __attribute__((alias("FUN_0023d080")));

/* Recovered original symbol name. */
extern __typeof__(mpeg_error) mpegError__FP7sceMpegP18sceMpegCbDataErrorPv
    __attribute__((alias("FUN_0023d080")));

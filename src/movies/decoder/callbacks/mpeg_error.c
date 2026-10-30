#include "types.h"

struct MpegErrorData {
    u8  pad_0[0x4];
    s32 unk4;
};

extern u8  video_error_message[] __asm__("D_00161220");
extern s32 DebugPrint();
s32        mpeg_error(s32 unused_mpeg, struct MpegErrorData *cb_data) __asm__("FUN_0023d080");

s32 mpeg_error(s32 unused_mpeg, struct MpegErrorData *cb_data) {
    DebugPrint(video_error_message, cb_data->unk4);
    return 1;
}

extern s32 func_0023D080(s32 arg0, struct MpegErrorData *cb_data)
    __attribute__((alias("FUN_0023d080")));
extern __typeof__(mpeg_error) D_0023D080 __attribute__((alias("FUN_0023d080")));

/* Recovered original symbol name. */
extern __typeof__(mpeg_error) mpegError__FP7sceMpegP18sceMpegCbDataErrorPv
    __attribute__((alias("FUN_0023d080")));
